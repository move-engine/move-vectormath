cmake_minimum_required(VERSION 3.14)

include(cmake/cpm.cmake)
include(cmake/prelude.cmake)

project(
    move-vectormath
    VERSION 0.1.0
    DESCRIPTION "The vectormath module of the Move engine"
    HOMEPAGE_URL "https://github.com/move-engine/move-vectormath"
    LANGUAGES NONE
)

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED on)

include(cmake/project-is-top-level.cmake)
include(cmake/variables.cmake)

# ---- Declare library ----
add_subdirectory(lib)

# # ---- Install rules ----

# if(NOT CMAKE_SKIP_INSTALL_RULES)
#   include(cmake/install-rules.cmake)
# endif()

# ---- Developer mode ----

if(NOT MOVE_VECTORMATH_DEVELOPER_MODE)
  return()
elseif(NOT PROJECT_IS_TOP_LEVEL)
  message(
      AUTHOR_WARNING
      "Developer mode is intended for developers of move-vectormath"
  )
endif()

include(cmake/dev-mode.cmake)
