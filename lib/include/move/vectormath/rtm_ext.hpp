#pragma once
#include <cassert>

#include <rtm/macros.h>

#include <rtm/matrix4x4d.h>
#include <rtm/matrix4x4f.h>
#include <rtm/qvvd.h>
#include <rtm/qvvf.h>

namespace rtm
{
    namespace ext
    {
        template <typename mat_type = matrix4x4f, typename vec_type = vector4f>
        RTM_DISABLE_SECURITY_COOKIE_CHECK RTM_FORCE_INLINE constexpr mat_type
        look_at_rh(
            const vec_type& eye, const vec_type& center, const vec_type& up)
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
        RTM_DISABLE_SECURITY_COOKIE_CHECK RTM_FORCE_INLINE constexpr mat_type
        look_at_lh(
            const vec_type& eye, const vec_type& center, const vec_type& up)
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
        RTM_FORCE_INLINE matrix4x4f perspective_fov_lh(
            float fovY, float aspectRatio, float near, float far)
        {
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
        RTM_FORCE_INLINE matrix4x4d perspective_fov_lh(
            double fovY, double aspectRatio, double near, double far)
        {
            using mat_type = matrix4x4d;
            using value_type = double;

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
            result.x_axis = vector_set(width, 0.0, 0.0, 0.0);
            result.y_axis = vector_set(0.0, height, 0.0, 0.0);
            result.z_axis = vector_set(0.0, 0.0, range, 1.0);
            result.w_axis = vector_set(0.0, 0.0, -range * near, 0.0);
            return result;
        }

        RTM_DISABLE_SECURITY_COOKIE_CHECK
        RTM_FORCE_INLINE matrix4x4f perspective_fov_rh(
            float fovY, float aspectRatio, float near, float far)
        {
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
        RTM_FORCE_INLINE matrix4x4d perspective_fov_rh(
            double fovY, double aspectRatio, double near, double far)
        {
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
        RTM_FORCE_INLINE matrix4x4f ortho_lh(
            float width, float height, float near, float far)
        {
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
        RTM_FORCE_INLINE matrix4x4d ortho_lh(
            double width, double height, double near, double far)
        {
            using mat_type = matrix4x4d;
            using value_type = double;

            assert(!scalar_near_equal(width, 0.0f, 0.00001f));
            assert(!scalar_near_equal(height, 0.0f, 0.00001f));
            assert(!scalar_near_equal(far, near, 0.00001f));

            value_type range = 1.0f / (far - near);
            mat_type result;
            result.x_axis = vector_set(2.0 / width, 0.0, 0.0, 0.0);
            result.y_axis = vector_set(0.0, 2.0 / height, 0.0, 0.0);
            result.z_axis = vector_set(0.0, 0.0, range, 0.0);
            result.w_axis = vector_set(0.0, 0.0, -range * near, 1.0);
            return result;
        }

        RTM_DISABLE_SECURITY_COOKIE_CHECK
        RTM_FORCE_INLINE matrix4x4f ortho_rh(
            float width, float height, float near, float far)
        {
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
        RTM_FORCE_INLINE matrix4x4d ortho_rh(
            double width, double height, double near, double far)
        {
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
        RTM_FORCE_INLINE matrix4x4f ortho_off_center_lh(float left, float right,
            float bottom, float top, float near, float far)
        {
            using mat_type = matrix4x4f;
            using value_type = float;

            assert(!scalar_near_equal(ViewRight, ViewLeft, 0.00001f));
            assert(!scalar_near_equal(ViewTop, ViewBottom, 0.00001f));
            assert(!scalar_near_equal(FarZ, NearZ, 0.00001f));

            value_type reciprocal_width = 1.0f / (right - left);
            value_type reciprocal_height = 1.0f / (top - bottom);
            value_type range = 1.0f / (far - near);

            mat_type result;
            result.x_axis = vector_set(
                reciprocal_width + reciprocal_width, 0.0f, 0.0f, 0.0f);
            result.y_axis = vector_set(
                0.0f, reciprocal_height + reciprocal_height, 0.0f, 0.0f);
            result.z_axis = vector_set(0.0f, 0.0f, range, 0.0f);
            result.w_axis = vector_set(-(left + right) * reciprocal_width,
                -(top + bottom) * reciprocal_height, -range * near, 1.0f);
            return result;
        }

        RTM_DISABLE_SECURITY_COOKIE_CHECK
        RTM_FORCE_INLINE matrix4x4f ortho_off_center_rh(float left, float right,
            float bottom, float top, float near, float far)
        {
            using mat_type = matrix4x4f;
            using value_type = float;

            assert(!scalar_near_equal(ViewRight, ViewLeft, 0.00001f));
            assert(!scalar_near_equal(ViewTop, ViewBottom, 0.00001f));
            assert(!scalar_near_equal(FarZ, NearZ, 0.00001f));

            value_type reciprocal_width = 1.0f / (right - left);
            value_type reciprocal_height = 1.0f / (top - bottom);
            value_type range = 1.0f / (near - far);

            mat_type result;
            result.x_axis = vector_set(
                reciprocal_width + reciprocal_width, 0.0f, 0.0f, 0.0f);
            result.y_axis = vector_set(
                0.0f, reciprocal_height + reciprocal_height, 0.0f, 0.0f);
            result.z_axis = vector_set(0.0f, 0.0f, range, 0.0f);
            result.w_axis = vector_set(-(left + right) * reciprocal_width,
                -(top + bottom) * reciprocal_height, range * near, 1.0f);
            return result;
        }

        RTM_DISABLE_SECURITY_COOKIE_CHECK
        RTM_FORCE_INLINE matrix4x4d ortho_off_center_lh(double left,
            double right, double bottom, double top, double near, double far)
        {
            using mat_type = matrix4x4d;
            using value_type = double;

            assert(!scalar_near_equal(ViewRight, ViewLeft, 0.00001));
            assert(!scalar_near_equal(ViewTop, ViewBottom, 0.00001));
            assert(!scalar_near_equal(FarZ, NearZ, 0.00001));

            value_type reciprocal_width = 1.0 / (right - left);
            value_type reciprocal_height = 1.0 / (top - bottom);
            value_type range = 1.0 / (far - near);

            mat_type result;
            result.x_axis =
                vector_set(reciprocal_width + reciprocal_width, 0.0, 0.0, 0.0);
            result.y_axis = vector_set(
                0.0, reciprocal_height + reciprocal_height, 0.0, 0.0);
            result.z_axis = vector_set(0.0, 0.0, range, 0.0);
            result.w_axis = vector_set(-(left + right) * reciprocal_width,
                -(top + bottom) * reciprocal_height, -range * near, 1.0);
            return result;
        }

        RTM_DISABLE_SECURITY_COOKIE_CHECK
        RTM_FORCE_INLINE matrix4x4d ortho_off_center_rh(double left,
            double right, double bottom, double top, double near, double far)
        {
            using mat_type = matrix4x4d;
            using value_type = double;

            assert(!scalar_near_equal(ViewRight, ViewLeft, 0.00001));
            assert(!scalar_near_equal(ViewTop, ViewBottom, 0.00001));
            assert(!scalar_near_equal(FarZ, NearZ, 0.00001));

            value_type reciprocal_width = 1.0 / (right - left);
            value_type reciprocal_height = 1.0 / (top - bottom);
            value_type range = 1.0 / (near - far);

            mat_type result;
            result.x_axis =
                vector_set(reciprocal_width + reciprocal_width, 0.0, 0.0, 0.0);
            result.y_axis = vector_set(
                0.0, reciprocal_height + reciprocal_height, 0.0, 0.0);
            result.z_axis = vector_set(0.0, 0.0, range, 0.0);
            result.w_axis = vector_set(-(left + right) * reciprocal_width,
                -(top + bottom) * reciprocal_height, range * near, 1.0);
            return result;
        }

        RTM_DISABLE_SECURITY_COOKIE_CHECK
        RTM_FORCE_INLINE matrix3x4f transform_3x4(
            const rtm::vector4f& translation, const rtm::quatf& rotation,
            const rtm::vector4f& scale)
        {
            auto qvv = rtm::qvv_set(rotation, translation, scale);
            return rtm::matrix_from_qvv(qvv);
        }

        RTM_DISABLE_SECURITY_COOKIE_CHECK
        RTM_FORCE_INLINE matrix3x4d transform_3x4(
            const rtm::vector4d& translation, const rtm::quatd& rotation,
            const rtm::vector4d& scale)
        {
            auto qvv = rtm::qvv_set(rotation, translation, scale);
            return rtm::matrix_from_qvv(qvv);
        }

        RTM_DISABLE_SECURITY_COOKIE_CHECK
        RTM_FORCE_INLINE matrix4x4f transform_4x4(
            const rtm::vector4f& translation, const rtm::quatf& rotation,
            const rtm::vector4f& scale)
        {
            return rtm::matrix_cast(
                transform_3x4(translation, rotation, scale));
        }

        RTM_DISABLE_SECURITY_COOKIE_CHECK
        RTM_FORCE_INLINE matrix4x4d transform_4x4(
            const rtm::vector4d& translation, const rtm::quatd& rotation,
            const rtm::vector4d& scale)
        {
            return rtm::matrix_cast(
                transform_3x4(translation, rotation, scale));
        }
    }  // namespace ext
}  // namespace rtm