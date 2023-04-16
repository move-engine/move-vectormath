
# ---- Dependencies ----
CPMAddPackage("gh:microsoft/DirectXMath#dec2022")

# ---- Declare library ----

add_library(move-vectormath INTERFACE)
add_library(move::vectormath ALIAS move-vectormath)

target_link_libraries(move-vectormath INTERFACE DirectXMath)

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