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

#include "mvm_test_utils.hpp"

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