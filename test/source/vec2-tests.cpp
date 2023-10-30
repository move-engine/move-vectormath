#include <math.h>

#include <catch2/catch_all.hpp>
#include <cereal/archives/binary.hpp>
#include "catch2/catch_approx.hpp"
#include "catch2/catch_test_macros.hpp"
#include "nlohmann/json_fwd.hpp"

// #include <move/vectormath.hpp>
#include <move/vectormath/vec2.hpp>

#include "mvm_test_utils.hpp"

#define MVM_IS_VEC2_TEST_CPP

#define MVM_VEC2_TEST_VEC_TYPE vec2f32
#include "vec2-tests.ixx"
#define MVM_VEC2_TEST_VEC_TYPE vec2f64
#include "vec2-tests.ixx"
#define MVM_VEC2_TEST_VEC_TYPE vec2i8
#include "vec2-tests.ixx"
#define MVM_VEC2_TEST_VEC_TYPE vec2i16
#include "vec2-tests.ixx"
#define MVM_VEC2_TEST_VEC_TYPE vec2i32
#include "vec2-tests.ixx"
#define MVM_VEC2_TEST_VEC_TYPE vec2i64
#include "vec2-tests.ixx"
#define MVM_VEC2_TEST_VEC_TYPE vec2u8
#include "vec2-tests.ixx"
#define MVM_VEC2_TEST_VEC_TYPE vec2u16
#include "vec2-tests.ixx"
#define MVM_VEC2_TEST_VEC_TYPE vec2u32
#include "vec2-tests.ixx"
#define MVM_VEC2_TEST_VEC_TYPE vec2u64
#include "vec2-tests.ixx"

#undef MVM_IS_VEC2_TEST_CPP
