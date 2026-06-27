export module rt.camera;

import std;
import rt.tuple;
import rt.matrix;
import rt.ray;
import rt.canvas;
import rt.world;
import rt.transformations;

export namespace rt {

    /**
     * @brief Represents a virtual camera used to capture a 3D scene from a specific perspective.
     *
     * Conceptually, the camera sits at the origin looking down the negative Z-axis,
     * projecting onto a virtual canvas at Z = -1. A transformation matrix is used to
     * place and orient the camera anywhere in the 3D world.
     * Math reference: documentation/explanation/Camera.md
     */
    struct Camera {
        std::size_t hsize;                  ///< Horizontal size of the canvas in pixels
        std::size_t vsize;                  ///< Vertical size of the canvas in pixels
        float field_of_view;                ///< Field of view angle in radians

        Matrix<4> transform{identity()};    ///< Camera-to-World/World-to-Camera transformation matrix
        Matrix<4> transform_inverse{identity()}; ///< Inverse transformation matrix used for ray casting

        float half_width;                   ///< Half width of the viewport at Z = -1
        float half_height;                  ///< Half height of the viewport at Z = -1
        float pixel_size;                   ///< World-space size of a single canvas pixel

        /**
         * @brief Constructs a new Camera with a specific canvas size and field of view.
         *
         * Automatically computes viewport properties (half_width, half_height, and pixel_size)
         * to map 2D pixels to 3D world coordinates.
         *
         * @param _hsize The width of the screen in pixels.
         * @param _vsize The height of the screen in pixels.
         * @param _field_of_view The field of view in radians.
         */
        Camera(std::size_t _hsize, std::size_t _vsize, float _field_of_view);

        /**
         * @brief Sets the camera's transformation matrix and precomputes its inverse.
         *
         * @param t The new transformation matrix.
         */
        void set_transform(const Matrix<4>& t);
    };

    /**
     * @brief Computes a Ray originating from the Camera and passing through the center of a given pixel.
     *
     * Map the 2D pixel index to camera space and then transform it to world space
     * using the camera's inverse transform.
     * Math reference: documentation/explanation/Camera.md
     *
     * @param c The Camera object.
     * @param px The pixel's column coordinate (0-indexed).
     * @param py The pixel's row coordinate (0-indexed).
     * @return Ray The generated perspective ray in world space.
     */
    [[nodiscard]] Ray ray_for_pixel(const Camera& c, std::size_t px, std::size_t py);

    /**
     * @brief Renders the given World to a Canvas from the perspective of the Camera.
     *
     * Iterates over all canvas pixels, generates corresponding rays, calculates the
     * shaded colors at intersections, and outputs the final color matrix.
     * This loop is parallelized using OpenMP.
     *
     * @param c The Camera object defining viewpoint and screen dimensions.
     * @param w The World object representing the scene to render.
     * @return Canvas The rendered 2D image.
     */
    [[nodiscard]] Canvas render(const Camera& c, const World& w);

} // namespace rt
