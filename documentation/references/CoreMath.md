# Reference: Core Mathematics

This section provides technical details regarding the foundational mathematical structures used in the Ray tracer. These structures are essential for representing points, vectors, colors, and transformations in 3D space.

## Tuples

A `Tuple` is the fundamental data structure used to represent coordinates and directions in 3D space. It is composed of four single-precision floating-point numbers (`float`): `x`, `y`, `z`, and `w`.

- **`x, y, z`**: Represent the position or magnitude along the three axes.
- **`w`**: The homogeneous coordinate. It is crucial for distinguishing between points and vectors and for allowing Matrix translations.

### Points vs. Vectors

- **Point (`w = 1.0f`)**: Represents a specific location in space. When `w=1`, the Point can be translated (moved) by Matrix multiplication.
- **Vector (`w = 0.0f`)**: Represents a direction and magnitude, but no specific location. When `w=0`, translation matrices have no effect on the Vector, which is mathematically correct (a direction doesn't change when you move it).

*Reference code: `src/math/Tuple.cppm`*

## Colors

Colors in the Ray tracer are implemented similarly to Tuples, but they represent the Red, Green, Blue, and Alpha (RGBA) components of light.

- **`r, g, b, a`**: Single-precision values representing Red, Green, Blue, and Alpha (transparency/opacity). Channels typically range from `0.0f` (no intensity / fully transparent) to `1.0f` (full intensity / fully opaque).
- **Default value**: Default constructed color represents opaque white (`1.0f, 1.0f, 1.0f, 1.0f`). If alpha is omitted in custom instantiation, it defaults to `1.0f` (opaque).
- **Operations**: Operations on colors (`+`, `-`, scalar `*`, Hadamard/blend `*`) perform arithmetic on the RGB components and return a color with the default alpha of `1.0f`. The equality operator `==` compares the RGB channels.

*Reference code: `src/core/Colors.cppm`*

## Matrices

A `Matrix` is a grid of numbers used to manipulate Tuples. In 3D graphics, a 4x4 Matrix is standard because it allows for homogeneous coordinates (the `w` component in our Tuples).

### Transformations

Transformations are specialized matrices that alter a Tuple when multiplied together (`Matrix * Tuple`). 

The core transformations are:
- **Translation**: Moves a Point in space. Does not affect vectors.
- **Scaling**: Changes the size of an object. Affects both points and vectors.
- **Rotation**: Rotates a Point or Vector around the X, Y, or Z axis.
- **Shearing**: Slants the object by shifting points along one axis based on their coordinate on another axis.
- **View Transform**: Positions and orients the virtual camera in 3D space by shifting and rotating the entire world in the opposite direction. See the **[View Transformation Explanation](../explanation/ViewTransformation.md)** for mathematical details.

Transformations can be chained together via Matrix multiplication. The order of multiplication is crucial: `Transformation_3 * Transformation_2 * Transformation_1 * Point` applies Transformation 1 first, then 2, then 3.

### Invertibility

A matrix can only be inverted if its determinant is not zero. In floating point, "zero" needs a tolerance, but a fixed one does not work: the determinant scales with the matrix (`scale(s, s, s)` has determinant $s^3$), so a valid transform for a small object such as `scale(0.04, 0.04, 0.04)` (determinant $6.4 \times 10^{-5}$) would look singular next to `EPSILON`.

`Matrix::isInvertible()` instead compares the determinant with the largest value it could have. By **Hadamard's inequality**, $|\det M|$ is at most the product of the row lengths, and also at most the product of the column lengths:

$$\text{invertible} \iff |\det M| > 10^{-6} \cdot \min\Big(\prod_i \lVert \text{row}_i \rVert,\; \prod_j \lVert \text{col}_j \rVert\Big)$$

The ratio does not change when the matrix is scaled, so tiny and huge transforms are treated alike, while matrices whose rows are (numerically) dependent are still rejected. The smaller of the two bounds is used because a translation lengthens one column (or, once transposed, one row) without affecting the determinant.

*Reference code: `src/math/Matrix.cppm`, `src/math/Transformations.cppm`*

## Comparing Floating-Point Numbers

All math in the engine uses single-precision `float`, which carries about 7 significant digits. Rounding means two computations of "the same" value rarely produce identical bits, so values are compared with `equal(a, b)` instead of `==`.

### Machine Epsilon and ULPs

**Machine epsilon** (`std::numeric_limits<float>::epsilon()`, $\approx 1.19 \times 10^{-7}$) is the gap between `1.0f` and the next larger float. The gap, called one **ULP** (unit in the last place), grows with the magnitude of the number:

| Value | Gap to the next float (1 ULP) |
| :--- | :--- |
| 1 | $1.2 \times 10^{-7}$ |
| 1,000 | $6.1 \times 10^{-5}$ |
| 100,000 | $7.8 \times 10^{-3}$ |

### Absolute vs. Relative Tolerance

- An **absolute** tolerance, $|a - b| < \varepsilon_{\text{abs}}$, suits values near zero, but large values can never meet it: at 100,000 even neighbouring floats are 0.008 apart.
- A **relative** tolerance, $|a - b| \le \varepsilon_{\text{rel}} \cdot \max(|a|, |b|)$, scales with the values, but shrinks to nothing near zero: a computed `1e-9` would never equal `0`.

`equal()` combines both, so each covers the other's weakness:

$$\text{equal}(a, b) \iff a = b \;\;\lor\;\; |a - b| < \underbrace{10^{-4}}_{\texttt{EPSILON}} \;\;\lor\;\; |a - b| \le \underbrace{10^{-5}}_{\texttt{RELATIVE\_EPSILON}} \cdot \max(|a|, |b|)$$

- `RELATIVE_EPSILON` ($10^{-5}$, about 84 ULPs) leaves room for the rounding that accumulates over a chain of matrix operations.
- The two tolerances meet at magnitude 10. Below it the absolute tolerance governs, exactly as before; above it the comparison scales with the values.
- The $a = b$ case lets an infinity equal the same infinity (where $a - b$ would be NaN). An infinity never equals a finite value, and NaN never equals anything.

> [!NOTE]
> The core library is compiled with `-ffast-math`, which normally lets the compiler assume infinities and NaN never occur and remove checks like `std::isinf`. `-fno-finite-math-only` restores those semantics (see [Data-Oriented Design](../explanation/DataOrientedDesign.md)).

### Surface Offsets Far from the Origin

`over_point` and `under_point` lift a hit slightly off its surface to avoid shadow acne (see [Shadows](../explanation/Shadows.md)). A fixed offset of `EPSILON` only works while it is larger than the float spacing at the hit: at 10,000 units from the origin floats are about 0.001 apart, ten times `EPSILON`, and roughly one hit in ten would shadow itself.

The offset therefore grows with the hit point's largest coordinate:

$$\text{offset} = \max\big(\texttt{EPSILON},\; \underbrace{10^{-6}}_{\texttt{SURFACE\_OFFSET\_SCALE}} \cdot \max(|p_x|, |p_y|, |p_z|)\big)$$

- Within 100 units of the origin, where all of the book's tests and scenes live, the offset is exactly `EPSILON`, so the book's expected values are unchanged.
- Beyond that it scales with the coordinates and always spans several ULPs: no acne at 10,000 or even 100,000 units (`MagnitudeStability.OverPointClearsTheSurfaceAtAnyDistance`).

### Stability Tests

[`tests/FloatStability.t.cpp`](../../tests/FloatStability.t.cpp) checks properties that must hold for every input, over hundreds of deterministic random cases (a seeded `std::mt19937`):

- a matrix times its inverse is the identity (worst error measured: $3 \times 10^{-5}$);
- hit points lie on the sphere's surface, and normals have unit length;
- reflection preserves a vector's length, and refraction between equal refractive indices does not bend light;
- Schlick's reflectance stays within $[0, 1]$, and rendered colors are never NaN or infinite;
- tiny and huge scales stay invertible, while numerically singular matrices are rejected;
- grazing rays hit planes scaled anywhere from 0.001 to 1000 (the plane's parallel-ray test is relative to the ray direction's length);
- no hit shadows itself, from the origin out to 100,000 units.

*Reference code: `src/math/Utils.cppm`, `src/math/Matrix.cppm`*

## Canvas

A `Canvas` is a rectangular grid of pixels, where each pixel represents a `Color`. To optimize memory footprint and CPU cache usage, the grid stores pixels using a packed 8-bit `PixelRGBA8` format internally, while presenting a float-precision HDR `Color` interface to the rest of the application. For detailed rationale, see the **[Data-Oriented Design Explanation](../explanation/DataOrientedDesign.md)**.

*Reference code: `src/core/Canvas.cppm`*

### Standard Canvas Resolutions

Below is a reference of common target resolutions when configuring a `Canvas(width, height)`:

| Format Name | Width | Height | Total Pixels | Aspect Ratio | Use Case |
| :--- | :--- | :--- | :--- | :--- | :--- |
| **1080p (Full HD)** | 1920 | 1080 | 2,073,600 | 16:9 | Standard high-definition display. |
| **2K (DCI Cinema)** | 2048 | 1080 | 2,211,840 | 1.89:1 | Digital cinema projection standard. |
| **2K (Consumer QHD)** | 2560 | 1440 | 3,686,400 | 16:9 | High-res monitors/screens. |
| **4K UHD (Consumer TV)** | 3840 | 2160 | 8,294,400 | 16:9 | Standard 4K monitors and televisions. |
| **4K (DCI Cinema)** | 4096 | 2160 | 8,847,360 | 1.89:1 | Professional cinema 4K projection. |

> [!NOTE]
> All reference visualizers under `visualizers/` are configured to render at the **2K (Consumer QHD)** resolution of $2560 \times 1440$ pixels, applying uniform pixel size mapping to maintain a correct 16:9 aspect ratio without shape distortion.

## Camera & Projection

A `Camera` represents the observer's viewpoint, mapping the 3D world to a 2D `Canvas`.

- **Field of View (FOV)**: The angular span of the camera's view.
- **Viewport Properties**: Automatically computed values (`half_width`, `half_height`, and `pixel_size`) that map pixel coordinates to the image plane at $Z = -1$.
- **Ray Generation**: Generates a ray from the camera origin through a specific pixel, transformed into world space using the inverse of the camera's transform matrix. See the **[Camera Explanation](../explanation/Camera.md)** for viewport and ray casting math.

*Reference code: `src/scene/Camera.cppm`*
