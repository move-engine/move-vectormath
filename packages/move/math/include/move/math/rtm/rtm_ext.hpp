#pragma once
#include <cassert>
#include <limits>
#include <move/math/macros.hpp>

#include <rtm/macros.h>

#include <rtm/impl/compiler_utils.h>
#include <rtm/mask4d.h>
#include <rtm/mask4f.h>
#include <rtm/matrix4x4d.h>
#include <rtm/matrix4x4f.h>
#include <rtm/quatd.h>
#include <rtm/quatf.h>
#include <rtm/qvvd.h>
#include <rtm/qvvf.h>
#include <rtm/vector4d.h>
#include <rtm/vector4f.h>

namespace rtm::ext
{
    template <typename mat_type, typename vec_type = vector4f>
    RTM_DISABLE_SECURITY_COOKIE_CHECK MVM_INLINE_NODISCARD constexpr mat_type
    look_at_rh(const vec_type& eye, const vec_type& center, const vec_type& up)
    {
        // Largely taken from GLM's implementation
        using namespace rtm;
        const vec_type look_dir(vector_normalize3(vector_sub(eye, center)));
        const vec_type right_dir(
            vector_normalize3(vector_mul(vector_cross3(up, look_dir), 1)));
        const vec_type actual_up_dir(vector_cross3(look_dir, right_dir));

        mat_type result = matrix_identity();
        result.x_axis = vector_set_w(right_dir, 0);
        result.y_axis = vector_set_w(actual_up_dir, 0);
        result.z_axis = vector_set_w(look_dir, 0);
        result = matrix_transpose(result);

        if constexpr (std::is_same_v<mat_type, matrix4x4d>)
        {
            result.w_axis = vector_set_w(
                vector_set(-double(vector_dot3(right_dir, eye)),
                           -double(vector_dot3(actual_up_dir, eye)),
                           -double(vector_dot3(look_dir, eye))),
                1);
        }
        else
        {
            result.w_axis =
                vector_set_w(vector_set(-float(vector_dot3(right_dir, eye)),
                                        -float(vector_dot3(actual_up_dir, eye)),
                                        -float(vector_dot3(look_dir, eye))),
                             1);
        }
        return result;
    }

    template <typename mat_type = matrix4x4f, typename vec_type = vector4f>
    RTM_DISABLE_SECURITY_COOKIE_CHECK MVM_INLINE_NODISCARD constexpr mat_type
    look_at_lh(const vec_type& eye, const vec_type& center, const vec_type& up)
    {
        // Largely taken from GLM's implementation
        using namespace rtm;
        const vec_type look_dir(vector_normalize3(vector_sub(center, eye)));
        const vec_type right_dir(
            vector_normalize3(vector_mul(vector_cross3(up, look_dir), 1)));
        const vec_type actual_up_dir(vector_cross3(look_dir, right_dir));

        mat_type result = matrix_identity();
        result.x_axis = vector_set_w(right_dir, 0);
        result.y_axis = vector_set_w(actual_up_dir, 0);
        result.z_axis = vector_set_w(look_dir, 0);
        result = matrix_transpose(result);

        if constexpr (std::is_same_v<mat_type, matrix4x4d>)
        {
            result.w_axis = vector_set_w(
                vector_set(-double(vector_dot3(right_dir, eye)),
                           -double(vector_dot3(actual_up_dir, eye)),
                           -double(vector_dot3(look_dir, eye))),
                1);
        }
        else
        {
            result.w_axis =
                vector_set_w(vector_set(-float(vector_dot3(right_dir, eye)),
                                        -float(vector_dot3(actual_up_dir, eye)),
                                        -float(vector_dot3(look_dir, eye))),
                             1);
        }
        return result;
    }

    RTM_DISABLE_SECURITY_COOKIE_CHECK
    MVM_INLINE_NODISCARD matrix4x4f perspective_fov_lh(float fovY,
                                                       float aspectRatio,
                                                       float near,
                                                       float far)
    {
        // Based on DXM's implementation
        using mat_type = matrix4x4f;
        using value_type = float;

        assert(near > 0.f && far > 0.f);
        assert(!scalar_near_equal(fovY, 0.0f, 0.00001f * 2.0f));
        assert(!scalar_near_equal(aspectRatio, 0.0f, 0.00001f));
        assert(!scalar_near_equal(far, near, 0.00001f));

        value_type sin_fov;
        value_type cos_fov;
        scalar_sincos(0.5f * fovY, sin_fov, cos_fov);

        value_type height = cos_fov / sin_fov;
        value_type width = height / aspectRatio;
        value_type range = far / (far - near);

        mat_type result;
        result.x_axis = vector_set(width, 0.0f, 0.0f, 0.0f);
        result.y_axis = vector_set(0.0f, height, 0.0f, 0.0f);
        result.z_axis = vector_set(0.0f, 0.0f, range, 1.0f);
        result.w_axis = vector_set(0.0f, 0.0f, -range * near, 0.0f);
        return result;
    }

    RTM_DISABLE_SECURITY_COOKIE_CHECK
    MVM_INLINE_NODISCARD matrix4x4d perspective_fov_lh(double fovY,
                                                       double aspectRatio,
                                                       double near,
                                                       double far)
    {
        // Based on DXM's implementation
        using mat_type = matrix4x4d;
        using value_type = double;

        assert(near > 0.f && far > 0.);
        assert(!scalar_near_equal(fovY, 0.0, 0.00001 * 2.0));
        assert(!scalar_near_equal(aspectRatio, 0.0, 0.00001));
        assert(!scalar_near_equal(far, near, 0.00001));

        value_type sin_fov;
        value_type cos_fov;
        scalar_sincos(0.5 * fovY, sin_fov, cos_fov);

        value_type height = cos_fov / sin_fov;
        value_type width = height / aspectRatio;
        value_type range = far / (far - near);

        mat_type result;
        result.x_axis = vector_set(width, 0.0, 0.0, 0.0);
        result.y_axis = vector_set(0.0, height, 0.0, 0.0);
        result.z_axis = vector_set(0.0, 0.0, range, 1.0);
        result.w_axis = vector_set(0.0, 0.0, -range * near, 0.0);
        return result;
    }

    RTM_DISABLE_SECURITY_COOKIE_CHECK
    MVM_INLINE_NODISCARD matrix4x4f perspective_fov_rh(float fovY,
                                                       float aspectRatio,
                                                       float near,
                                                       float far)
    {
        // Based on DXM's implementation
        using mat_type = matrix4x4f;
        using value_type = float;

        assert(near > 0.f && far > 0.f);
        assert(!scalar_near_equal(fovY, 0.0f, 0.00001f * 2.0f));
        assert(!scalar_near_equal(aspectRatio, 0.0f, 0.00001f));
        assert(!scalar_near_equal(far, near, 0.00001f));

        value_type sin_fov;
        value_type cos_fov;
        scalar_sincos(0.5f * fovY, sin_fov, cos_fov);

        value_type height = cos_fov / sin_fov;
        value_type width = height / aspectRatio;
        value_type range = far / (near - far);

        mat_type result;
        result.x_axis = vector_set(width, 0.0f, 0.0f, 0.0f);
        result.y_axis = vector_set(0.0f, height, 0.0f, 0.0f);
        result.z_axis = vector_set(0.0f, 0.0f, range, -1.0f);
        result.w_axis = vector_set(0.0f, 0.0f, range * near, 0.0f);
        return result;
    }

    RTM_DISABLE_SECURITY_COOKIE_CHECK
    MVM_INLINE_NODISCARD matrix4x4d perspective_fov_rh(double fovY,
                                                       double aspectRatio,
                                                       double near,
                                                       double far)
    {
        // Based on DXM's implementation
        using mat_type = matrix4x4d;
        using value_type = double;

        assert(near > 0. && far > 0.);
        assert(!scalar_near_equal(fovY, 0.0, 0.00001 * 2.0));
        assert(!scalar_near_equal(aspectRatio, 0.0, 0.00001));
        assert(!scalar_near_equal(far, near, 0.00001));

        value_type sin_fov;
        value_type cos_fov;
        scalar_sincos(0.5f * fovY, sin_fov, cos_fov);

        value_type height = cos_fov / sin_fov;
        value_type width = height / aspectRatio;
        value_type range = far / (near - far);

        mat_type result;
        result.x_axis = vector_set(width, 0.0, 0.0, 0.0);
        result.y_axis = vector_set(0.0, height, 0.0, 0.0);
        result.z_axis = vector_set(0.0, 0.0, range, -1.0);
        result.w_axis = vector_set(0.0, 0.0, range * near, 0.0);
        return result;
    }

    RTM_DISABLE_SECURITY_COOKIE_CHECK
    MVM_INLINE_NODISCARD matrix4x4f ortho_lh(float width,
                                             float height,
                                             float near,
                                             float far)
    {
        // Based on DXM's implementation
        using mat_type = matrix4x4f;
        using value_type = float;

        assert(!scalar_near_equal(width, 0.0f, 0.00001f));
        assert(!scalar_near_equal(height, 0.0f, 0.00001f));
        assert(!scalar_near_equal(far, near, 0.00001f));

        value_type range = 1.0f / (far - near);
        mat_type result;
        result.x_axis = vector_set(2.0f / width, 0.0f, 0.0f, 0.0f);
        result.y_axis = vector_set(0.0f, 2.0f / height, 0.0f, 0.0f);
        result.z_axis = vector_set(0.0f, 0.0f, range, 0.0f);
        result.w_axis = vector_set(0.0f, 0.0f, -range * near, 1.0f);
        return result;
    }

    RTM_DISABLE_SECURITY_COOKIE_CHECK
    MVM_INLINE_NODISCARD matrix4x4d ortho_lh(double width,
                                             double height,
                                             double near,
                                             double far)
    {
        // Based on DXM's implementation
        using mat_type = matrix4x4d;
        using value_type = double;

        assert(!scalar_near_equal(width, 0.0, 0.00001));
        assert(!scalar_near_equal(height, 0.0, 0.00001));
        assert(!scalar_near_equal(far, near, 0.00001));

        value_type range = 1.0f / (far - near);
        mat_type result;
        result.x_axis = vector_set(2.0 / width, 0.0, 0.0, 0.0);
        result.y_axis = vector_set(0.0, 2.0 / height, 0.0, 0.0);
        result.z_axis = vector_set(0.0, 0.0, range, 0.0);
        result.w_axis = vector_set(0.0, 0.0, -range * near, 1.0);
        return result;
    }

    RTM_DISABLE_SECURITY_COOKIE_CHECK
    MVM_INLINE_NODISCARD matrix4x4f ortho_rh(float width,
                                             float height,
                                             float near,
                                             float far)
    {
        // Based on DXM's implementation
        using mat_type = matrix4x4f;
        using value_type = float;

        assert(!scalar_near_equal(width, 0.0f, 0.00001f));
        assert(!scalar_near_equal(height, 0.0f, 0.00001f));
        assert(!scalar_near_equal(far, near, 0.00001f));

        value_type range = 1.0f / (near - far);
        mat_type result;
        result.x_axis = vector_set(2.0f / width, 0.0f, 0.0f, 0.0f);
        result.y_axis = vector_set(0.0f, 2.0f / height, 0.0f, 0.0f);
        result.z_axis = vector_set(0.0f, 0.0f, range, 0.0f);
        result.w_axis = vector_set(0.0f, 0.0f, range * near, 1.0f);
        return result;
    }

    RTM_DISABLE_SECURITY_COOKIE_CHECK
    MVM_INLINE_NODISCARD matrix4x4d ortho_rh(double width,
                                             double height,
                                             double near,
                                             double far)
    {
        // Based on DXM's implementation
        using mat_type = matrix4x4d;
        using value_type = double;

        assert(!scalar_near_equal(width, 0.0, 0.00001));
        assert(!scalar_near_equal(height, 0.0, 0.00001));
        assert(!scalar_near_equal(far, near, 0.00001));

        value_type range = 1.0f / (near - far);
        mat_type result;
        result.x_axis = vector_set(2.0 / width, 0.0, 0.0, 0.0);
        result.y_axis = vector_set(0.0, 2.0 / height, 0.0, 0.0);
        result.z_axis = vector_set(0.0, 0.0, range, 0.0);
        result.w_axis = vector_set(0.0, 0.0, range * near, 1.0);
        return result;
    }

    RTM_DISABLE_SECURITY_COOKIE_CHECK
    MVM_INLINE_NODISCARD matrix4x4f ortho_off_center_lh(
        float left, float right, float bottom, float top, float near, float far)
    {
        // Based on DXM's implementation
        using mat_type = matrix4x4f;
        using value_type = float;

        assert(!scalar_near_equal(right, left, 0.00001f));
        assert(!scalar_near_equal(top, bottom, 0.00001f));
        assert(!scalar_near_equal(far, near, 0.00001f));

        value_type reciprocal_width = 1.0f / (right - left);
        value_type reciprocal_height = 1.0f / (top - bottom);
        value_type range = 1.0f / (far - near);

        mat_type result;
        result.x_axis =
            vector_set(reciprocal_width + reciprocal_width, 0.0f, 0.0f, 0.0f);
        result.y_axis =
            vector_set(0.0f, reciprocal_height + reciprocal_height, 0.0f, 0.0f);
        result.z_axis = vector_set(0.0f, 0.0f, range, 0.0f);
        result.w_axis = vector_set(-(left + right) * reciprocal_width,
                                   -(top + bottom) * reciprocal_height,
                                   -range * near, 1.0f);
        return result;
    }

    RTM_DISABLE_SECURITY_COOKIE_CHECK
    MVM_INLINE_NODISCARD matrix4x4f ortho_off_center_rh(
        float left, float right, float bottom, float top, float near, float far)
    {
        // Based on DXM's implementation
        using mat_type = matrix4x4f;
        using value_type = float;

        assert(!scalar_near_equal(right, left, 0.00001f));
        assert(!scalar_near_equal(top, bottom, 0.00001f));
        assert(!scalar_near_equal(far, near, 0.00001f));

        value_type reciprocal_width = 1.0f / (right - left);
        value_type reciprocal_height = 1.0f / (top - bottom);
        value_type range = 1.0f / (near - far);

        mat_type result;
        result.x_axis =
            vector_set(reciprocal_width + reciprocal_width, 0.0f, 0.0f, 0.0f);
        result.y_axis =
            vector_set(0.0f, reciprocal_height + reciprocal_height, 0.0f, 0.0f);
        result.z_axis = vector_set(0.0f, 0.0f, range, 0.0f);
        result.w_axis =
            vector_set(-(left + right) * reciprocal_width,
                       -(top + bottom) * reciprocal_height, range * near, 1.0f);
        return result;
    }

    RTM_DISABLE_SECURITY_COOKIE_CHECK
    MVM_INLINE_NODISCARD matrix4x4d ortho_off_center_lh(double left,
                                                        double right,
                                                        double bottom,
                                                        double top,
                                                        double near,
                                                        double far)
    {
        // Based on DXM's implementation
        using mat_type = matrix4x4d;
        using value_type = double;

        assert(!scalar_near_equal(right, left, 0.00001));
        assert(!scalar_near_equal(top, bottom, 0.00001));
        assert(!scalar_near_equal(far, near, 0.00001));

        value_type reciprocal_width = 1.0 / (right - left);
        value_type reciprocal_height = 1.0 / (top - bottom);
        value_type range = 1.0 / (far - near);

        mat_type result;
        result.x_axis =
            vector_set(reciprocal_width + reciprocal_width, 0.0, 0.0, 0.0);
        result.y_axis =
            vector_set(0.0, reciprocal_height + reciprocal_height, 0.0, 0.0);
        result.z_axis = vector_set(0.0, 0.0, range, 0.0);
        result.w_axis =
            vector_set(-(left + right) * reciprocal_width,
                       -(top + bottom) * reciprocal_height, -range * near, 1.0);
        return result;
    }

    RTM_DISABLE_SECURITY_COOKIE_CHECK
    MVM_INLINE_NODISCARD matrix4x4d ortho_off_center_rh(double left,
                                                        double right,
                                                        double bottom,
                                                        double top,
                                                        double near,
                                                        double far)
    {
        // Based on DXM's implementation
        using mat_type = matrix4x4d;
        using value_type = double;

        assert(!scalar_near_equal(right, left, 0.00001));
        assert(!scalar_near_equal(top, bottom, 0.00001));
        assert(!scalar_near_equal(far, near, 0.00001));

        value_type reciprocal_width = 1.0 / (right - left);
        value_type reciprocal_height = 1.0 / (top - bottom);
        value_type range = 1.0 / (near - far);

        mat_type result;
        result.x_axis =
            vector_set(reciprocal_width + reciprocal_width, 0.0, 0.0, 0.0);
        result.y_axis =
            vector_set(0.0, reciprocal_height + reciprocal_height, 0.0, 0.0);
        result.z_axis = vector_set(0.0, 0.0, range, 0.0);
        result.w_axis =
            vector_set(-(left + right) * reciprocal_width,
                       -(top + bottom) * reciprocal_height, range * near, 1.0);
        return result;
    }

    RTM_DISABLE_SECURITY_COOKIE_CHECK
    MVM_INLINE_NODISCARD matrix3x4f
    transform_3x4(const rtm::vector4f& translation,
                  const rtm::quatf& rotation,
                  const rtm::vector4f& scale)
    {
        auto qvv = rtm::qvv_set(rotation, translation, scale);
        return rtm::matrix_from_qvv(qvv);
    }

    RTM_DISABLE_SECURITY_COOKIE_CHECK
    MVM_INLINE_NODISCARD matrix3x4d
    transform_3x4(const rtm::vector4d& translation,
                  const rtm::quatd& rotation,
                  const rtm::vector4d& scale)
    {
        auto qvv = rtm::qvv_set(rotation, translation, scale);
        return rtm::matrix_from_qvv(qvv);
    }

    RTM_DISABLE_SECURITY_COOKIE_CHECK
    MVM_INLINE_NODISCARD matrix4x4f
    transform_4x4(const rtm::vector4f& translation,
                  const rtm::quatf& rotation,
                  const rtm::vector4f& scale)
    {
        return rtm::matrix_cast(transform_3x4(translation, rotation, scale));
    }

    RTM_DISABLE_SECURITY_COOKIE_CHECK
    MVM_INLINE_NODISCARD matrix4x4d
    transform_4x4(const rtm::vector4d& translation,
                  const rtm::quatd& rotation,
                  const rtm::vector4d& scale)
    {
        return rtm::matrix_cast(transform_3x4(translation, rotation, scale));
    }

    RTM_DISABLE_SECURITY_COOKIE_CHECK
    MVM_INLINE_NODISCARD quatf quat_inverse(const quatf& input)
    {
        // Based on DXM's implementation
        using quat_t = quatf;
        using vector_t = vector4f;
        using value_t = float;
        using mask_t = mask4f;

        vector_t conj = quat_to_vector(quat_conjugate(input));
        value_t lsq = quat_length_squared(input);
        vector_t lsqv = vector_set(lsq);
        if (lsq <= std::numeric_limits<value_t>::epsilon())
        {
            return quat_set(value_t(0), value_t(0), value_t(0), value_t(0));
        }

        return vector_to_quat(vector_div(conj, lsqv));
    }

    RTM_DISABLE_SECURITY_COOKIE_CHECK
    MVM_INLINE_NODISCARD quatd quat_inverse(const quatd& input)
    {
        // Based on DXM's implementation
        using quat_t = quatd;
        using vector_t = vector4d;
        using value_t = double;
        using mask_t = mask4d;

        vector_t conj = quat_to_vector(quat_conjugate(input));
        value_t lsq = quat_length_squared(input);
        vector_t lsqv = vector_set(lsq);
        if (lsq <= std::numeric_limits<value_t>::epsilon())
        {
            return quat_set(value_t(0), value_t(0), value_t(0), value_t(0));
        }

        return vector_to_quat(vector_div(conj, lsqv));
    }

    RTM_DISABLE_SECURITY_COOKIE_CHECK
    MVM_INLINE_NODISCARD mask4f vector_in_bounds(const vector4f& input,
                                                 const vector4f& bounds_min,
                                                 const vector4f& bounds_max)
    {
        return mask_and(vector_less_equal(input, bounds_max),
                        vector_greater_equal(input, bounds_min));
    }

    RTM_DISABLE_SECURITY_COOKIE_CHECK
    MVM_INLINE_NODISCARD mask4d vector_in_bounds(const vector4d& input,
                                                 const vector4d& bounds_min,
                                                 const vector4d& bounds_max)
    {
        return mask_and(vector_less_equal(input, bounds_max),
                        vector_greater_equal(input, bounds_min));
    }

    RTM_DISABLE_SECURITY_COOKIE_CHECK
    MVM_INLINE_NODISCARD mask4f vector_in_bounds(const vector4f& input,
                                                 const vector4f& bounds)
    {
        return vector_in_bounds(input, vector_neg(bounds), bounds);
    }

    RTM_DISABLE_SECURITY_COOKIE_CHECK
    MVM_INLINE_NODISCARD mask4d vector_in_bounds(const vector4d& input,
                                                 const vector4d& bounds)
    {
        return vector_in_bounds(input, vector_neg(bounds), bounds);
    }

    RTM_DISABLE_SECURITY_COOKIE_CHECK MVM_INLINE_NODISCARD vector4f
    vector_splat_x(const vector4f& input)
    {
        return vector_set(float(vector_get_x(input)));
    }

    RTM_DISABLE_SECURITY_COOKIE_CHECK MVM_INLINE_NODISCARD vector4f
    vector_splat_y(const vector4f& input)
    {
        return vector_set(float(vector_get_y(input)));
    }

    RTM_DISABLE_SECURITY_COOKIE_CHECK MVM_INLINE_NODISCARD vector4f
    vector_splat_w(const vector4f& input)
    {
        return vector_set(float(vector_get_w(input)));
    }

    RTM_DISABLE_SECURITY_COOKIE_CHECK MVM_INLINE_NODISCARD vector4f
    vector_splat_z(const vector4f& input)
    {
        return vector_set(float(vector_get_z(input)));
    }

    RTM_DISABLE_SECURITY_COOKIE_CHECK MVM_INLINE_NODISCARD vector4d
    vector_splat_x(const vector4d& input)
    {
        return vector_set(double(vector_get_x(input)));
    }

    RTM_DISABLE_SECURITY_COOKIE_CHECK MVM_INLINE_NODISCARD vector4d
    vector_splat_y(const vector4d& input)
    {
        return vector_set(double(vector_get_y(input)));
    }

    RTM_DISABLE_SECURITY_COOKIE_CHECK MVM_INLINE_NODISCARD vector4d
    vector_splat_w(const vector4d& input)
    {
        return vector_set(double(vector_get_w(input)));
    }

    RTM_DISABLE_SECURITY_COOKIE_CHECK MVM_INLINE_NODISCARD vector4d
    vector_splat_z(const vector4d& input)
    {
        return vector_set(double(vector_get_z(input)));
    }

    // TODO: This and exp
    // RTM_DISABLE_SECURITY_COOKIE_CHECK
    // MVM_INLINE_NODISCARD quatf quat_ln(const quatf& input)
    // {
    //     // static const XMVECTORF32 OneMinusEpsilon = {{{1.0f - 0.00001f,
    //     //     1.0f - 0.00001f, 1.0f - 0.00001f, 1.0f - 0.00001f}}};

    //     // XMVECTOR QW = XMVectorSplatW(Q);
    //     // XMVECTOR Q0 = XMVectorSelect(g_XMSelect1110.v, Q,
    //     // g_XMSelect1110.v);

    //     // XMVECTOR ControlW = XMVectorInBounds(QW, OneMinusEpsilon.v);

    //     // XMVECTOR Theta = XMVectorACos(QW);
    //     // XMVECTOR SinTheta = XMVectorSin(Theta);

    //     // XMVECTOR S = XMVectorDivide(Theta, SinTheta);

    //     // XMVECTOR Result = XMVectorMultiply(Q0, S);
    //     // Result = XMVectorSelect(Q0, Result, ControlW);

    //     // return Result;

    //     using quat_t = quatf;
    //     using vector_t = vector4f;
    //     using value_t = float;
    //     using mask_t = mask4f;

    //     static constexpr value_t epsilon =
    //         std::numeric_limits<value_t>::epsilon();

    //     static const vector_t one_minus_epsilon =
    //         vector_set(value_t(1.0) - epsilon);

    //     static const mask_t select1110 =
    //         vector_set(value_t(1), value_t(1), value_t(1), value_t(0));

    //     vector_t qw = vector_splat_w(quat_to_vector(input));
    //     vector_t q0 = vector_select(select1110, select1110, qw);

    //     mask_t control_w = vector_in_bounds(qw, one_minus_epsilon);

    //     vector_t theta = vector_acos(qw);
    //     vector_t sin_theta = vector_sin(theta);

    //     vector_t s = vector_div(theta, sin_theta);

    //     vector_t result = vector_mul(q0, s);
    //     result = vector_select(control_w, q0, result);

    //     return vector_to_quat(result);
    // }
}  // namespace rtm::ext