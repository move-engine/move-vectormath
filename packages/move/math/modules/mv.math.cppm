module;

#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <memory>
#include <numbers>
#include <optional>
#include <span>
#include <type_traits>
#include <utility>

#include <rtm/matrix3x3f.h>
#include <rtm/matrix4x4f.h>
#include <rtm/vector4f.h>

export module mv.math;

export extern "C++"
{
#include <mv/math/Math.hpp>
}
