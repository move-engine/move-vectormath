#include <math.h>

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4244)
#pragma warning(disable : 4305)
#endif

#include <catch2/catch_all.hpp>
#include <cereal/archives/binary.hpp>
#include "catch2/catch_approx.hpp"
#include "catch2/catch_test_macros.hpp"
#include "nlohmann/json_fwd.hpp"

// #include <move/vectormath.hpp>
#include <move/vectormath/vec4.hpp>

#include "mvm_test_utils.hpp"

#define MVM_IS_TEST_INCLUDE

#define MVM_TEST_TYPE vec4f32_rtm
#include "vec4-tests.ixx"

#define MVM_TEST_TYPE vec4f64_rtm
#include "vec4-tests.ixx"

#define MVM_TEST_TYPE vec4f32_scalar
#include "vec4-tests.ixx"

#define MVM_TEST_TYPE vec4f64_scalar
#include "vec4-tests.ixx"

#define MVM_TEST_TYPE vec4i8_scalar
#include "vec4-tests.ixx"

#define MVM_TEST_TYPE vec4i16_scalar
#include "vec4-tests.ixx"

#define MVM_TEST_TYPE vec4i32_scalar
#include "vec4-tests.ixx"

#define MVM_TEST_TYPE vec4i64_scalar
#include "vec4-tests.ixx"

#undef MVM_IS_TEST_INCLUDE

#ifdef _MSC_VER
#pragma warning(pop)
#endif