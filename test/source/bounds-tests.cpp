#include <math.h>
#include <cmath>
#include <stdexcept>

#include <catch2/catch_all.hpp>
#include <cereal/archives/binary.hpp>
#include "DirectXMath.h"
#include "catch2/catch_approx.hpp"
#include "catch2/catch_message.hpp"
#include "catch2/catch_test_macros.hpp"
#include "catch2/internal/catch_source_line_info.hpp"
#include "move/vectormath/common.hpp"
#include "nlohmann/json_fwd.hpp"

#include <move/vectormath/bounds.hpp>

#include <DirectXCollision.h>
#include <DirectXMath.h>
#include "mvm_test_utils.hpp"

SCENARIO("AABBf tests against DXM", "[move::vectormath::aabbf]")
{
    using Catch::Approx;
    using namespace move::vectormath;
    using namespace DirectX;

    using bounds_type = aabbf;
    using vec_type = bounds_type::vec_type;

    GIVEN(
        "A bounding box from (10, 10, 10) to (20, 20, 20), another from (15, "
        "15, 15) to (25, 25, 25), and another from (0, 0, 0) to (1, 1, 1)")
    {
        bounds_type bounds1(vec_type(10, 10, 10), vec_type(20, 20, 20));
        BoundingBox dxm_bounds1(XMFLOAT3(10, 10, 10), XMFLOAT3(20, 20, 20));

        bounds_type bounds2(vec_type(15, 15, 15), vec_type(25, 25, 25));
        BoundingBox dxm_bounds2(XMFLOAT3(15, 15, 15), XMFLOAT3(25, 25, 25));

        bounds_type bounds3(vec_type(0, 0, 0), vec_type(1, 1, 1));
        BoundingBox dxm_bounds3(XMFLOAT3(0, 0, 0), XMFLOAT3(1, 1, 1));

        // TODO: Verify ray/bounds intersection against DXM
    }
}

#define MVM_IS_BOUNDS_TEST_CPP
#define MVM_BOUNDS_TEST_BOUNDS_TYPE bounds2f32
#include "bounds-tests.ixx"
#define MVM_BOUNDS_TEST_BOUNDS_TYPE bounds2f64
#include "bounds-tests.ixx"
#define MVM_BOUNDS_TEST_BOUNDS_TYPE bounds2i8
#include "bounds-tests.ixx"
#define MVM_BOUNDS_TEST_BOUNDS_TYPE bounds2i16
#include "bounds-tests.ixx"
#define MVM_BOUNDS_TEST_BOUNDS_TYPE bounds2i32
#include "bounds-tests.ixx"
#define MVM_BOUNDS_TEST_BOUNDS_TYPE bounds2i64
#include "bounds-tests.ixx"
#define MVM_BOUNDS_TEST_BOUNDS_TYPE bounds2u8
#include "bounds-tests.ixx"
#define MVM_BOUNDS_TEST_BOUNDS_TYPE bounds2u16
#include "bounds-tests.ixx"
#define MVM_BOUNDS_TEST_BOUNDS_TYPE bounds2u32
#include "bounds-tests.ixx"
#define MVM_BOUNDS_TEST_BOUNDS_TYPE bounds2u64
#include "bounds-tests.ixx"
#define MVM_BOUNDS_TEST_BOUNDS_TYPE bounds2f
#include "bounds-tests.ixx"
#define MVM_BOUNDS_TEST_BOUNDS_TYPE bounds2d
#include "bounds-tests.ixx"
#define MVM_BOUNDS_TEST_BOUNDS_TYPE bounds2i
#include "bounds-tests.ixx"
#define MVM_BOUNDS_TEST_BOUNDS_TYPE bounds2u
#include "bounds-tests.ixx"
#define MVM_BOUNDS_TEST_BOUNDS_TYPE bounds3f
#include "bounds-tests.ixx"
#define MVM_BOUNDS_TEST_BOUNDS_TYPE bounds3d
#include "bounds-tests.ixx"
#define MVM_BOUNDS_TEST_BOUNDS_TYPE bounds4f
#include "bounds-tests.ixx"
#define MVM_BOUNDS_TEST_BOUNDS_TYPE bounds4d
#include "bounds-tests.ixx"
#undef MVM_IS_BOUNDS_TEST_CPP