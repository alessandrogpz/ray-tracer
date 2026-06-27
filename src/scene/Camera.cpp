module rt.camera;

import rt.tuple;
import rt.matrix;
import rt.ray;
import rt.canvas;
import rt.world;
import rt.colors;
import rt.transformations;

namespace rt {

    Camera::Camera(std::size_t _hsize, std::size_t _vsize, float _field_of_view)
        : hsize(_hsize), vsize(_vsize), field_of_view(_field_of_view)
    {
        float half_view = std::tan(field_of_view / 2.0f);
        float aspect = static_cast<float>(hsize) / static_cast<float>(vsize);

        if (aspect >= 1.0f) {
            half_width = half_view;
            half_height = half_view / aspect;
        } else {
            half_width = half_view * aspect;
            half_height = half_view;
        }

        pixel_size = (half_width * 2.0f) / static_cast<float>(hsize);
    }

    void Camera::set_transform(const Matrix<4>& t) {
        transform = t;
        transform_inverse = t.inverse();
    }

    Ray ray_for_pixel(const Camera& c, std::size_t px, std::size_t py)
    {
        // Offset from edge of viewport to pixel's center
        float x_offset = (static_cast<float>(px) + 0.5f) * c.pixel_size;
        float y_offset = (static_cast<float>(py) + 0.5f) * c.pixel_size;

        // Untransformed coordinates of pixel in camera space
        float world_x = c.half_width - x_offset;
        float world_y = c.half_height - y_offset;

        // Transform screen point and camera origin to world space
        Point pixel = c.transform_inverse * Point(world_x, world_y, -1.0f);
        Point origin = c.transform_inverse * Point(0.0f, 0.0f, 0.0f);
        Vector direction = normalizeVector(pixel - origin);

        return Ray(origin, direction);
    }

    Canvas render(const Camera& c, const World& w)
    {
        Canvas image(c.hsize, c.vsize);

        // Multithreaded loop rendering pixels concurrently across CPU cores
        #ifdef _OPENMP
        #pragma omp parallel for collapse(2) schedule(dynamic, 16)
        #endif
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

} // namespace rt
