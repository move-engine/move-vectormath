#include <math.h>

#include <catch2/catch_all.hpp>
#include <cereal/archives/binary.hpp>
#include "catch2/catch_approx.hpp"
#include "catch2/catch_test_macros.hpp"
#include "nlohmann/json_fwd.hpp"

// #include <move/vectormath.hpp>
#include <move/vectormath/vec3.hpp>

#include "mvm_test_utils.hpp"

#define MVM_IS_TEST_INCLUDE

#define MVM_TEST_TYPE vec3f32_rtm
#include "vec3-tests.ixx"

#define MVM_TEST_TYPE vec3f64_rtm
#include "vec3-tests.ixx"

#define MVM_TEST_TYPE vec3f32_scalar
#include "vec3-tests.ixx"

#define MVM_TEST_TYPE vec3f64_scalar
#include "vec3-tests.ixx"

#define MVM_TEST_TYPE vec3i8_scalar
#include "vec3-tests.ixx"

#define MVM_TEST_TYPE vec3i16_scalar
#include "vec3-tests.ixx"

#define MVM_TEST_TYPE vec3i32_scalar
#include "vec3-tests.ixx"

#define MVM_TEST_TYPE vec3i64_scalar
#include "vec3-tests.ixx"

#undef MVM_IS_TEST_INCLUDE