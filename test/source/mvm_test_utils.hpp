#pragma once

#include <DirectXMath.h>
#include <sstream>

template <typename vec_type>
inline vec_type vec2_from_dxm(const DirectX::XMVECTOR& vec)
{
    return vec_type(DirectX::XMVectorGetX(vec), DirectX::XMVectorGetY(vec));
}

template <typename vec_type>
inline DirectX::XMVECTOR vec2_to_dxm(const vec_type& vec)
{
    return DirectX::XMVectorSet(vec.x(), vec.y(), 0.0f, 0.0f);
}

template <typename vec_type>
inline vec_type vec3_from_dxm(const DirectX::XMVECTOR& vec)
{
    return vec_type(DirectX::XMVectorGetX(vec), DirectX::XMVectorGetY(vec),
        DirectX::XMVectorGetZ(vec));
}

template <typename vec_type>
inline DirectX::XMVECTOR vec3_to_dxm(const vec_type& vec)
{
    return DirectX::XMVectorSet(vec.x(), vec.y(), vec.z(), 0.0f);
}

template <typename vec_type>
inline vec_type vec4_from_dxm(const DirectX::XMVECTOR& vec)
{
    return vec_type(DirectX::XMVectorGetX(vec), DirectX::XMVectorGetY(vec),
        DirectX::XMVectorGetZ(vec), DirectX::XMVectorGetW(vec));
}

template <typename vec_type>
inline DirectX::XMVECTOR vec4_to_dxm(const vec_type& vec)
{
    return DirectX::XMVectorSet(vec.x(), vec.y(), vec.z(), vec.w());
}

template <typename mat_type>
inline mat_type mat4_from_dxm(const DirectX::XMMATRIX& vec)
{
    using namespace DirectX;
    using value_type = typename mat_type::component_type;
    using vector_type = typename mat_type::vector_type;
    using vector_type_raw = typename vector_type::underlying_type;
    using vector_type_raw_type = typename vector_type_raw::type;

    auto row0 = XMVectorGetX(vec.r[0]);
    auto row1 = XMVectorGetX(vec.r[1]);
    auto row2 = XMVectorGetX(vec.r[2]);
    auto row3 = XMVectorGetX(vec.r[3]);

    return mat_type(vector_type_raw_type(row0), vector_type_raw_type(row1),
        vector_type_raw_type(row2), vector_type_raw_type(row3));
}

template <typename mat_type>
inline DirectX::XMMATRIX mat4_to_dxm(const mat_type& mat)
{
    using namespace DirectX;
    using value_type = typename mat_type::component_type;

    auto row0 =
        XMVectorSet(value_type(mat.get(0, 0)), value_type(mat.get(0, 1)),
            value_type(mat.get(0, 2)), value_type(mat.get(0, 3)));
    auto row1 =
        XMVectorSet(value_type(mat.get(1, 0)), value_type(mat.get(1, 1)),
            value_type(mat.get(1, 2)), value_type(mat.get(1, 3)));
    auto row2 =
        XMVectorSet(value_type(mat.get(2, 0)), value_type(mat.get(2, 1)),
            value_type(mat.get(2, 2)), value_type(mat.get(2, 3)));
    auto row3 =
        XMVectorSet(value_type(mat.get(3, 0)), value_type(mat.get(3, 1)),
            value_type(mat.get(3, 2)), value_type(mat.get(3, 3)));

    return XMMATRIX(row0, row1, row2, row3);
}

template <typename mat_type>
inline bool equals(const mat_type& mvm, const DirectX::XMMATRIX& dxm)
{
    using Catch::Approx;
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            auto mvm_val = mvm.get(i, j);
            auto dxm_val = DirectX::XMVectorGetByIndex(dxm.r[i], j);
            if (mvm_val != Approx(dxm_val))
            {
                // Report to catch that there was an error
                std::stringstream ss;
                ss << "mvm[" << i << "][" << j << "] != dxm[" << i << "][" << j
                   << "] (" << mvm_val << " != " << dxm_val << ")";

                throw std::runtime_error(ss.str());

                return false;
            }
        }
    }
    return true;
}

template <typename vec_type>
inline bool equals(const vec_type& mvm, const DirectX::XMVECTOR& dxm)
{
    using Catch::Approx;
    for (int i = 0; i < vec_type::num_components; ++i)
    {
        auto mvm_val = mvm.get_component(i);
        auto dxm_val = DirectX::XMVectorGetByIndex(dxm, i);
        if (mvm_val != Approx(dxm_val))
        {
            // Report to catch that there was an error
            std::stringstream ss;
            ss << "mvm[" << i << "] != dxm[" << i << "] (" << mvm_val
               << " != " << dxm_val << ")";

            throw std::runtime_error(ss.str());
            return false;
        }
    }
    return true;
}