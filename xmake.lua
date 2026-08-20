set_project("move-vectormath")
set_version("0.2.0")
set_languages("c++20")

add_rules("mode.debug", "mode.release")
add_requires("rtm v2.3.1")

option("force_scalar")
    set_default(false)
    set_showmenu(true)
    set_description("Use the portable scalar backend instead of RTM SIMD")
option_end()

option("build_tests")
    set_default(false)
    set_showmenu(true)
    set_description("Build XMake public-header consumer smoke tests")
option_end()

target("move-vectormath")
    set_kind("headeronly")
    add_headerfiles("packages/move/math/include/(**.hpp)")
    add_includedirs("packages/move/math/include", {public = true})
    add_packages("rtm", {public = true})
    if has_config("force_scalar") then
        add_defines("MV_MATH_FORCE_SCALAR=1", {public = true})
    end
target_end()

if has_config("build_tests") then
    target("move-vectormath-xmake-header-test")
        set_kind("binary")
        add_files("tests/xmake/header_consumer.cpp")
        add_deps("move-vectormath")
        set_warnings("all", "error")
        add_tests("header-consumer")
    target_end()

    target("move-vectormath-xmake-scalar-header-test")
        set_kind("binary")
        add_files("tests/xmake/header_consumer.cpp")
        add_deps("move-vectormath")
        add_defines("MV_MATH_FORCE_SCALAR=1")
        set_warnings("all", "error")
        add_tests("scalar-header-consumer")
    target_end()
end
