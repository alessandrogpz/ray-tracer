# Explanation: Procedural Patterns

A **pattern** is a function that maps a point in space to a color. Instead of painting a whole shape with one flat `Material::color`, a material can carry an optional `Pattern` (see [`Patterns.cppm`](../../src/scene/Patterns.cppm)) whose color is evaluated at every shaded point.

Like shapes, patterns follow the project's [Data-Oriented Design](DataOrientedDesign.md): there is no abstract `Pattern` base class. A single flat `Pattern` struct holds a `PatternType` enum, two colors `a` and `b`, and a transformation matrix (plus its precomputed inverse). `pattern_at()` switches on the enum to pick the color function, just as `ShapeType` selects a shape's intersection code.

---

## 1. The Color Functions

All functions receive a point that is already in **pattern space** (see section 2).

| Pattern | Factory | Rule | Result |
| :--- | :--- | :--- | :--- |
| **Stripe** | `stripe_pattern(a, b)` | $\lfloor p_x \rfloor \bmod 2 = 0 \Rightarrow a$, otherwise $b$ | Alternating slabs along $x$, constant in $y$ and $z$ |
| **Gradient** | `gradient_pattern(a, b)` | $a + (b - a)\,(p_x - \lfloor p_x \rfloor)$ | Linear blend from $a$ to $b$, repeating every unit of $x$ |
| **Ring** | `ring_pattern(a, b)` | $\lfloor \sqrt{p_x^2 + p_z^2} \rfloor \bmod 2 = 0 \Rightarrow a$ | Concentric rings around the $y$ axis |
| **Checkers** | `checkers_pattern(a, b)` | $(\lfloor p_x \rfloor + \lfloor p_y \rfloor + \lfloor p_z \rfloor) \bmod 2 = 0 \Rightarrow a$ | Alternating unit cubes (3D checkers) |
| **Solid** | `solid_pattern(c)` | always $c$ | A single color; the building block of nested patterns |
| **Test** | `test_pattern()` | $(p_x, p_y, p_z)$ as $(r, g, b)$ | Debug pattern that exposes the transformed point in unit tests |

> [!NOTE]
> `std::floor` (not truncation) is essential for negative coordinates: $\lfloor -0.1 \rfloor = -1$ (odd), so the stripe just left of the origin uses color $b$, exactly as the book's tests expect. A plain `static_cast<int>(-0.1f)` gives `0` (even), which would merge the stripes on both sides of zero into one double-width stripe.

Each rule only reads one, two, or three axes. To change a pattern's direction or size, transform it rather than writing a new rule: for example, `rotation_z(π/2)` turns stripes that vary along $x$ into horizontal bands that vary along $y$, and `scale(0.2, 0.2, 0.2)` makes them five times narrower.

---

## 2. Transforming Patterns: World → Object → Pattern Space

If patterns were evaluated in world space, every object would show identical stripes frozen in place, as if light were shining through a striped stencil. Instead, `pattern_at_shape()` performs two changes of basis before evaluating the rule:

$$P_{\text{object}} = M_{\text{object}}^{-1} \cdot P_{\text{world}}$$
$$P_{\text{pattern}} = M_{\text{pattern}}^{-1} \cdot P_{\text{object}}$$

1. The **object's** inverse transform makes the pattern move, scale, and rotate together with its shape.
2. The **pattern's** own inverse transform (set with `Pattern::set_transform`) adjusts the pattern independently: stripe width, ring orientation, phase.

The book passes the shape itself to `lighting()`. In this project a shape is an index into the World's Structure-of-Arrays vectors, and the only part of it a pattern needs is the world-to-object matrix, so `lighting()` takes that matrix directly:

```cpp
const Color surface_color = material.pattern
    ? pattern_at_shape(*material.pattern, object_inverse, point)
    : material.color;
```

`shade_hit()` looks the matrix up for the hit shape (`sphere_transforms_inverse`, `plane_transforms_inverse`, …) the same way it looks up the material.

---

## 3. Why Checkered Spheres Look "Carved"

Patterns are *solid* (3D) textures: a checkered sphere looks as if it were carved out of a checkered block rather than wrapped in checkered paper. Wrapping a 2D texture around a surface requires **UV mapping** (converting a 3D point to 2D surface coordinates), which is beyond the scope of the book.

---

## 4. Shading Point and Acne

`shade_hit()` evaluates lighting (and therefore patterns) at `over_point`, the hit nudged slightly along the normal, rather than at the exact hit `point`. On a checkered floor at $y = 0$, rounding can place `point.y` at $-0.000001$, where $\lfloor y \rfloor = -1$ flips the checker color in random pixels. `over_point` keeps the sample consistently on the visible side of the surface (the same idea that prevents shadow acne, see [Shadows](Shadows.md)).

---

## 5. Nested Patterns

Instead of two colors, a pattern can choose between two **sub-patterns**. Each factory has an overload that takes patterns instead of colors:

```cpp
Pattern along_x = stripe_pattern(white, grey);
Pattern along_z = stripe_pattern(navy, slate);
along_z.set_transform(rotation_y(PI / 2));        // these stripes run the other way

Pattern floor = checkers_pattern(along_x, along_z); // checkers whose squares hold stripes
```

The two sub-patterns are stored in `Pattern::children`. When a rule picks "color $a$" or "color $b$", it evaluates the corresponding child instead. Each child keeps its **own transform**, applied on top of its parent's pattern space:

$$P_{\text{child}} = M_{\text{child}}^{-1} \cdot P_{\text{parent}}$$

Nesting can go as deep as you like (stripes of checkers of stripes, …); every chain ends in `solid_pattern(color)` or a pattern with plain colors. A gradient of two sub-patterns blends their colors, which also makes it a simple *blended* pattern.

Because `children` is a `std::vector<Pattern>`, a nested pattern owns its sub-patterns by value: copying a `Material` copies the whole tree, and two patterns compare equal when their whole trees match.

---

## 6. Perturbed Patterns (Perlin Noise)

`perturbed_pattern(inner, amount)` wraps another pattern and **jitters each point** before the inner pattern sees it. The jitter comes from smooth 3D noise, so neighbouring points move by similar amounts and straight edges become organic, wavy curves, as if someone had swirled wet paint.

```cpp
Pattern veins = stripe_pattern(white, blue_grey);
veins.set_transform(scale(0.35, 0.35, 0.35));
Pattern marble = perturbed_pattern(veins, 0.45f);   // jitter up to 0.45 units
```

For a point $p$, three independent noise samples displace each axis:

$$p' = p + a \cdot \big(\,n(p),\; n(p + o_y),\; n(p + o_z)\,\big)$$

where $n$ is Perlin noise in $[-1, 1]$, $a$ is the perturbation amount, and $o_y$, $o_z$ are fixed offsets that keep the three samples uncorrelated. The inner pattern is then evaluated at $p'$ (in its own transformed space).

- **Amount:** how far points move, in the perturbed pattern's units. Around 0.1–0.3 gives subtle waves; larger values give strong swirls.
- **Frequency:** noise changes roughly once per unit. To get more (smaller) waves, scale the perturbed pattern down with `set_transform`; to get fewer, scale it up.

### Perlin Noise (`rt.noise`)

[`perlin_noise(x, y, z)`](../../src/math/Noise.cppm) implements Ken Perlin's 2002 *improved noise*:

1. Find the unit cube containing the point, and the point's position $(x, y, z)$ inside it.
2. Hash each of the cube's 8 corners through a permutation table to pick one of 12 gradient directions.
3. Take the dot product of each corner's gradient with the vector from that corner to the point.
4. Blend the 8 values with the easing curve $6t^5 - 15t^4 + 10t^3$, which has zero first and second derivatives at the cube faces, so the noise is smooth everywhere.

The result is deterministic, continuous, exactly $0$ at every integer lattice point, and stays within $[-1, 1]$. The permutation table is the numbers 0–255 shuffled once with a fixed seed (computed at compile time), so every render is identical.
