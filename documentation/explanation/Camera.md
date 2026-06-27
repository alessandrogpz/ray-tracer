# Explanation: Virtual Camera & Perspective Projection

The **Camera** system is the bridge between a 3D scene (defined by worlds, lights, and spheres) and a 2D canvas of pixels. It models a pinhole perspective camera, generating light rays that traverse the virtual world to capture a 2D projection.

---

## 1. Camera Space coordinate System

By default, the camera is positioned at the origin $(0, 0, 0)$ looking straight down the negative $Z$-axis.
*   **Up** is along the positive $Y$-axis.
*   **Right** is along the positive $X$-axis.
*   The virtual **screen (canvas)** sits exactly at distance $Z = -1$.

```
           +Y (Up)
             |
             |   / (Screen at Z = -1)
             |  /
  -X <-------+-------> +X (Right)
            /|
           / |
          v  |
     -Z (Look) -Y (Down)
```

---

## 2. Viewport Geometry Math

To construct the projection viewport, we use the camera's canvas size (`hsize` and `vsize`) and the **Field of View** angle $\theta$ (in radians):

### 1. View Angle
The width of the viewport at $Z = -1$ is derived using basic trigonometry:
$$ \text{halfView} = \tan\left(\frac{\theta}{2}\right) $$

### 2. Aspect Ratio & Fit
The aspect ratio defines the screen shape:
$$ \text{aspect} = \frac{\text{hsize}}{\text{vsize}} $$

To keep the field of view consistent when fitting the canvas:
*   **Horizontal Viewport (width $\ge$ height, $\text{aspect} \ge 1.0$):**
    $$ \text{halfWidth} = \text{halfView} $$
    $$ \text{halfHeight} = \frac{\text{halfView}}{\text{aspect}} $$
*   **Vertical Viewport (width $<$ height, $\text{aspect} < 1.0$):**
    $$ \text{halfWidth} = \text{halfView} \times \text{aspect} $$
    $$ \text{halfHeight} = \text{halfView} $$

### 3. Pixel Size
The physical width of a single pixel in world coordinates is:
$$ \text{pixelSize} = \frac{\text{halfWidth} \times 2}{\text{hsize}} $$

---

## 3. Ray Generation (`ray_for_pixel`)

To sample a pixel $(px, py)$ on the canvas, we construct a 3D Ray in world coordinates:

1.  **Pixel Center Coordinates:** Add $0.5$ to the pixel index to locate its center, then scale by `pixelSize`:
    $$ \text{xOffset} = (px + 0.5) \times \text{pixelSize} $$
    $$ \text{yOffset} = (py + 0.5) \times \text{pixelSize} $$
2.  **Camera-space Screen Coordinates:**
    $$ \text{worldX} = \text{halfWidth} - \text{xOffset} $$
    $$ \text{worldY} = \text{halfHeight} - \text{yOffset} $$
3.  **Coordinate Transformation:** Using the **inverse** of the camera's transform matrix ($\mathbf{T}^{-1}$), we map the camera-space pixel point and origin back to world space:
    $$ P_{\text{world}} = \mathbf{T}^{-1} \times \text{Point}(\text{worldX}, \text{worldY}, -1.0) $$
    $$ O_{\text{world}} = \mathbf{T}^{-1} \times \text{Point}(0.0, 0.0, 0.0) $$
4.  **Direction:** The ray direction is the normalized vector between the two:
    $$ \vec{D} = \text{normalize}(P_{\text{world}} - O_{\text{world}}) $$

---

## 4. Multi-Core Render Loop

Because the rays generated for each pixel are completely independent, the rendering loop is a prime candidate for parallelism. We use **OpenMP** to split the loop iteration space across multiple CPU cores, achieving optimal CPU occupancy.

```cpp
Canvas render(const Camera& c, const World& w)
{
    Canvas image(c.hsize, c.vsize);

    #pragma omp parallel for collapse(2) schedule(dynamic, 16)
    for (std::size_t y = 0; y < c.vsize; ++y)
    {
        for (std::size_t x = 0; x < c.hsize; ++x)
        {
            Ray r = ray_for_pixel(c, x, y);
            Color col = color_at(w, r);
            writePixel(image, x, y, col);
        }
    }

    return image;
}
```
