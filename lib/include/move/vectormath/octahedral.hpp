#pragma once
#include "./macros.hpp"
#include "./vec2.hpp"
#include "./vec3.hpp"

namespace move::vectormath
{
    namespace octahedral_mapping
    {
        template <typename vec2_t = move::vectormath::vec2f>
        MVM_INLINE_NODISCARD static vec2_t oct_wrap(const vec2_t& v) noexcept
        {
            vec2_t xy = vec2_t(v.x(), v.y());
            vec2_t yx = vec2_t(v.y(), v.x());
            vec2_t abs_yx = yx.abs();
            vec2_t one = vec2_t::one();
            vec2_t zero = vec2_t::zero();
            return (one - abs_yx) * (xy >= zero ? one : -one);
        }

        template <typename vec2_t = move::vectormath::vec2f,
            typename vec3_t = move::vectormath::vec3f>
        MVM_INLINE_NODISCARD static vec2_t encode(const vec3_t& n) noexcept
        {
            // This will be inefficient with the vectorized types, but it is the
            // most straightforward way to implement this function.  If we find
            // that we're using this frequently for the vectorized types, we can
            // optimize it then.
            n /= (scalar::abs(n.x()) + scalar::abs(n.y()) + scalar::abs(n.z()));
            if (n.z >= 0)
            {
                n.set_x(n.x() * typename vec2_t::component_type(0.5) +
                        typename vec2_t::component_type(0.5));
                n.set_y(n.y() * typename vec2_t::component_type(0.5) +
                        typename vec2_t::component_type(0.5));
            }
            else
            {
                vec2_t wrapped = oct_wrap(vec2_t(n.x(), n.y()));
                n.set_x(typename vec2_t::component_type(wrapped.x()) *
                            typename vec2_t::component_type(0.5) +
                        typename vec2_t::component_type(0.5));
                n.set_y(typename vec2_t::component_type(wrapped.y()) *
                            typename vec2_t::component_type(0.5) +
                        typename vec2_t::component_type(0.5));
            }
            return vec2_t(n.x(), n.y());
        }

        template <typename vec2_t = move::vectormath::vec2f,
            typename vec3_t = move::vectormath::vec3f>
        MVM_INLINE_NODISCARD static vec3_t decode(const vec2_t& v) noexcept
        {
            vec2_t f = v * typename vec2_t::component_type(2.0) -
                       typename vec2_t::component_type(1.0);
            vec3_t n = vec3_t(f.x(), f.y(),
                typename vec2_t::component_type(1.0) - scalar::abs(f.x()) -
                    scalar::abs(f.y()));
            typename vec2_t::component_type t = scalar::saturate(-n.z());
            n.set_x(n.x() +
                    (n.x() >= typename vec2_t::component_type(0.0) ? -t : t));
            n.set_y(n.y() +
                    (n.y() >= typename vec2_t::component_type(0.0) ? -t : t));
            return n.normalized();
        }
    }  // namespace octahedral_mapping
}  // namespace move::vectormath