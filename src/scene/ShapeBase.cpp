module rt.shape_base;

import rt.matrix;

namespace rt {

    Shape::Shape() {
        static int next_id = 0;
        id = ++next_id;
    }

    void Shape::set_transform(const Matrix<4>& t) {
        transform = t;
        transform_inverse = t.inverse();
        transform_inverse_transpose = transform_inverse.transpose();
    }

    const Matrix<4>& Shape::get_transform() const {
        return transform;
    }

    const Matrix<4>& Shape::get_transform_inverse() const {
        return transform_inverse;
    }

    const Matrix<4>& Shape::get_transform_inverse_transpose() const {
        return transform_inverse_transpose;
    }

} // namespace rt
