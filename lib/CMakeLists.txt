
# ---- Dependencies ----
CPMAddPackage("gh:microsoft/DirectXMath#dec2022")

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

target_link_libraries(move-vectormath INTERFACE DirectXMath njson cereal fmt::fmt)

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