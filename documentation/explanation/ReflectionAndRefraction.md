# Explanation: Reflection, Refraction & the Fresnel Effect

Mirrors and glass are what ray tracers are known for. Both effects work the same way: at the hit point, spawn a **secondary ray**, trace it recursively through the World with `color_at()`, and blend its color into the surface color.

The implementation lives in [`World.cpp`](../../src/scene/World.cpp): `reflected_color()`, `refracted_color()`, `schlick()`, and the `n1`/`n2` computation in `prepare_computation()`.

---

## 1. Material Attributes

| Attribute | Default | Meaning |
| :--- | :--- | :--- |
| `reflective` | `0.0` | 0 = matte, 1 = perfect mirror |
| `transparency` | `0.0` | 0 = opaque, 1 = fully transparent |
| `refractive_index` | `1.0` | Vacuum 1.0, air 1.00029, water 1.333, glass 1.52, diamond 2.417 |
| `casts_shadow` | `true` | `false` lets light pass through, e.g. a pond's water surface (see [Shadows](Shadows.md)) |

`glass_sphere()` returns a unit sphere with `transparency = 1` and `refractive_index = 1.5`.

---

## 2. Reflection

`prepare_computation()` reflects the incoming ray direction around the (possibly flipped) surface normal:

$$\vec{R} = \vec{D} - 2(\vec{D} \cdot \vec{N})\,\vec{N}$$

`reflected_color()` then traces a ray from `over_point` (just *above* the surface, so the ray cannot re-hit its own surface) along $\vec{R}$, and scales the result by `reflective`.

### Bounding the Recursion

`shade_hit → reflected_color → color_at → shade_hit → …` is recursive. Two parallel mirrors would bounce a ray forever, so every function in the chain takes a `remaining` budget (default `MAX_RECURSION_DEPTH = 5`). Each secondary ray is traced with `remaining - 1`; at `0`, the reflected and refracted colors are black.

---

## 3. Refraction

### Finding $n_1$ and $n_2$

At every hit we need the refractive index of the material being **exited** ($n_1$) and of the one being **entered** ($n_2$). Overlapping transparent objects make this non-trivial, so `prepare_computation()` walks the sorted list of *all* intersections along the ray, keeping a `containers` list of the objects the ray is currently inside:

1. At the hit, $n_1$ = index of the last object in `containers` (1.0 if it is empty).
2. If the intersected object is already in `containers`, the ray is leaving it: remove it. Otherwise append it.
3. At the hit, $n_2$ = index of the last object in `containers` (1.0 if it is empty), then stop.

Objects are identified by `(shape_type, shape_index)`, the Data-Oriented equivalent of comparing object pointers. This is why `prepare_computation()` takes the full intersection list `xs`; the three-argument overload treats the hit as the only intersection.

### Snell's Law

$$\frac{\sin\theta_i}{\sin\theta_t} = \frac{n_2}{n_1}$$

With $n_{\text{ratio}} = n_1 / n_2$ and $\cos\theta_i = \vec{E}\cdot\vec{N}$ (eye vector and normal):

$$\sin^2\theta_t = n_{\text{ratio}}^2\,(1 - \cos^2\theta_i)$$

- If $\sin^2\theta_t > 1$, light cannot cross the boundary: **total internal reflection**. The refracted color is black.
- Otherwise $\cos\theta_t = \sqrt{1 - \sin^2\theta_t}$ and the refracted direction is

$$\vec{T} = \vec{N}\,(n_{\text{ratio}}\cos\theta_i - \cos\theta_t) - \vec{E}\,n_{\text{ratio}}$$

The refracted ray starts at `under_point`, just *below* the surface, so it does not immediately hit the surface it is entering. Its color is scaled by `transparency`.

---

## 4. The Fresnel Effect (Schlick Approximation)

Simply adding reflection and refraction produces a black band wherever total internal reflection occurs (for example around an air bubble inside glass). In reality, a transparent surface reflects more light at grazing angles and transmits more when viewed head-on, so the two contributions complement each other.

Christophe Schlick's approximation of Fresnel's equations computes the **reflectance** $R$, the fraction of light that is reflected:

1. Start with $\cos = \vec{E}\cdot\vec{N}$.
2. If $n_1 > n_2$: compute $\sin^2\theta_t$; if it exceeds 1, return $R = 1$ (total internal reflection). Otherwise use $\cos = \cos\theta_t$.
3. $R_0 = \left(\dfrac{n_1 - n_2}{n_1 + n_2}\right)^2$
4. $R = R_0 + (1 - R_0)(1 - \cos)^5$

`shade_hit()` uses it whenever a material is **both** reflective and transparent:

$$C = C_{\text{surface}} + C_{\text{reflected}}\cdot R + C_{\text{refracted}}\cdot (1 - R)$$

Otherwise it simply adds the three colors.

---

## 5. Material Tips (from the book)

- For glass, set both `transparency` and `reflective` high (0.9–1.0) so the Fresnel blend kicks in.
- Lower `diffuse` and `ambient` on very reflective or transparent surfaces; most of their color comes from the secondary rays.
- For tinted glass or a tinted mirror, use a **very dark** color: dark red reads as red glass.
- Pair reflective and transparent surfaces with tight highlights: `specular = 1`, `shininess` of 300 or more.
- Transparent objects still cast shadows by default. Set `casts_shadow = false` on surfaces like water so what lies beneath them stays lit.

---

## 6. Precision Note

The engine uses single-precision `float`, while the book's expected values come from double precision. The book's test colors still match within the project's `EPSILON` (0.0001); only the Schlick small-angle test compares with a slightly looser tolerance (0.0005).
