
# ---- Dependencies ----
CPMAddPackage(
  NAME rtm
  GITHUB_REPOSITORY nfrechette/rtm
  #GIT_TAG v2.1.5
  GIT_TAG 8a0012f374738ca1ecb98b917e3251d273019371
  DOWNLOAD_ONLY YES
)

add_library(rtm INTERFACE)
target_include_directories(rtm INTERFACE ${rtm_SOURCE_DIR}/includes)

# Disabled until it's needed; at some point, we may implement vec2 math with this
# CPMAddPackage(
#   name simde
#   GITHUB_REPOSITORY simd-everywhere/simde
#   VERSION 0.7.6
#   DOWNLOAD_ONLY YES
# )

# if (simde_ADDED)
#   add_library(simde INTERFACE)
#   target_include_directories(simde INTERFACE "${simde_SOURCE_DIR}")
# else()
#   message(FATAL_ERROR "Failed to acquire simde")
# endif()

CPMAddPackage(
  NAME njson
  GIT_REPOSITORY "https://github.com/nlohmann/json.git"
  VERSION "3.10.3"
  DOWNLOAD_ONLY YES
)

if (njson_ADDED)
  add_library(njson INTERFACE)
  target_include_directories(njson INTERFACE "${njson_SOURCE_DIR}/include")
else()
  # This is a hack
  # message(FATAL_ERROR "Failed to acquire nlohmann json")
endif()

CPMAddPackage(
  NAME cereal
  GIT_REPOSITORY "https://github.com/USCiLab/cereal.git"
  GIT_TAG "ddd467244713ea4fe63733628992efcdd6a9187d"
  DOWNLOAD_ONLY YES
)

if (cereal_ADDED)
  add_library(cereal INTERFACE)
  target_include_directories(cereal INTERFACE "${cereal_SOURCE_DIR}/include")
else()
  # This is a hack
  # message(FATAL_ERROR "Failed to acquire libcereal")
endif()


CPMAddPackage("gh:fmtlib/fmt#9.1.0@9.1.0")

# ---- Declare library ----

add_library(move-vectormath INTERFACE)
add_library(move::vectormath ALIAS move-vectormath)

target_link_libraries(move-vectormath INTERFACE rtm njson cereal fmt::fmt)

set_property(
    TARGET move-vectormath PROPERTY
    EXPORT_NAME move-vectormath
)

target_include_directories(
    move-vectormath ${warning_guard}
    INTERFACE
    "include"
)

if (NOT WIN32)
  target_include_directories(
      move-vectormath
      INTERFACE
      "non_win32_headers"
  )
endif()

target_compile_features(move-vectormath INTERFACE cxx_std_17)