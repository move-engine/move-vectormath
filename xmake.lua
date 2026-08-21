set_project("move-vectormath")
set_version("0.5.0")
set_languages("c++20")
set_policy("build.c++.modules.std", false)

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

option("build_modules")
    set_default(false)
    set_showmenu(true)
    set_description("Build the mv.math C++20 named module")
option_end()

option("build_compile_benchmarks")
    set_default(false)
    set_showmenu(true)
    set_description("Build header and module compile-time fan-out fixtures")
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

if has_config("build_modules") then
    target("move-vectormath-module")
        set_kind("static")
        add_files("packages/move/math/modules/mv.math.cppm", {public = true})
        add_deps("move-vectormath")
        on_load(function(target)
            local headers =
                os.files("packages/move/math/include/mv/**.hpp")
            table.sort(headers)
            local header_hashes = {}
            for _, header in ipairs(headers) do
                table.insert(header_hashes, hash.sha256(header))
            end
            local revision = hash.strhash64(table.concat(header_hashes))
            target:add(
                "defines",
                "MV_MATH_DETAIL_MODULE_REVISION_" .. revision .. "=1",
                {public = true})
        end)
        if has_config("build_compile_benchmarks") then
            set_policy("build.ccache", false)
        end
    target_end()
end

if has_config("build_compile_benchmarks") then
    target("move-vectormath-compile-header")
        set_kind("object")
        add_files("tests/compile_time/header/*.cpp")
        add_deps("move-vectormath")
        set_policy("build.ccache", false)
    target_end()

    if has_config("build_modules") then
        target("move-vectormath-compile-module")
            set_kind("object")
            add_files("tests/compile_time/module/*.cpp")
            add_deps("move-vectormath-module")
            set_policy("build.ccache", false)
        target_end()
    end
end

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

    if has_config("build_modules") then
        target("move-vectormath-xmake-module-test")
            set_kind("binary")
            add_files("tests/xmake/module_consumer.cpp")
            add_deps("move-vectormath-module")
            set_warnings("all", "error")
            add_tests("module-consumer")
        target_end()

        target("move-vectormath-xmake-module-bridge-test")
            set_kind("binary")
            add_files("tests/xmake/module_bridge.cppm")
            add_files("tests/xmake/module_bridge_consumer.cpp")
            add_deps("move-vectormath-module")
            set_warnings("all", "error")
            add_tests("module-bridge-consumer")
        target_end()
    end
end
