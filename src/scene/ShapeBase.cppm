module;

#include <vector>

export module rt.shape_base;

import rt.tuple;
import rt.matrix;
import rt.transformations;
import rt.materials;

export namespace rt {
 
     struct Shape {
         int id;
         Material material{};
 
         virtual ~Shape() = default;
 
         void set_transform(const Matrix<4>& t);
         [[nodiscard]] const Matrix<4>& get_transform() const;
         [[nodiscard]] const Matrix<4>& get_transform_inverse() const;
         [[nodiscard]] const Matrix<4>& get_transform_inverse_transpose() const;
 
         [[nodiscard]] virtual Vector local_normal_at(Point local_point) const = 0;
         [[nodiscard]] virtual std::vector<double> local_intersect(Point local_origin, Vector local_direction) const = 0;

    protected:
        Shape();

    private:
        Matrix<4> transform {identity()};
        Matrix<4> transform_inverse {identity()};
        Matrix<4> transform_inverse_transpose {identity()};
    };
}
