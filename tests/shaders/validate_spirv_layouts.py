#!/usr/bin/env python3

import argparse
import pathlib
import re
import subprocess
import sys


def run(command: list[str]) -> None:
    completed = subprocess.run(command, check=False, text=True)
    if completed.returncode != 0:
        raise RuntimeError(
            f"command failed with exit code {completed.returncode}: "
            + " ".join(command)
        )


class SpirvLayout:
    def __init__(self, assembly: str) -> None:
        self.names: dict[str, str] = {}
        self.member_names: dict[tuple[str, int], str] = {}
        self.member_offsets: dict[tuple[str, int], int] = {}
        self.member_matrix_strides: dict[tuple[str, int], int] = {}
        self.array_strides: dict[str, int] = {}
        self.runtime_arrays: dict[str, str] = {}

        for line in assembly.splitlines():
            if match := re.search(r'OpName (%\S+) "([^"]*)"', line):
                self.names[match.group(1)] = match.group(2)
            elif match := re.search(
                r'OpMemberName (%\S+) (\d+) "([^"]*)"', line
            ):
                self.member_names[(match.group(1), int(match.group(2)))] = (
                    match.group(3)
                )
            elif match := re.search(
                r"OpMemberDecorate (%\S+) (\d+) Offset (\d+)", line
            ):
                self.member_offsets[(match.group(1), int(match.group(2)))] = (
                    int(match.group(3))
                )
            elif match := re.search(
                r"OpMemberDecorate (%\S+) (\d+) MatrixStride (\d+)", line
            ):
                self.member_matrix_strides[
                    (match.group(1), int(match.group(2)))
                ] = int(match.group(3))
            elif match := re.search(
                r"OpDecorate (%\S+) ArrayStride (\d+)", line
            ):
                self.array_strides[match.group(1)] = int(match.group(2))
            elif match := re.search(
                r"(%\S+) = OpTypeRuntimeArray (%\S+)", line
            ):
                self.runtime_arrays[match.group(1)] = match.group(2)

    def id_for_name(self, name: str) -> str:
        matches = [
            identifier
            for identifier, candidate in self.names.items()
            if candidate == name
        ]
        if len(matches) != 1:
            raise AssertionError(
                f"expected one SPIR-V type named {name}, found {len(matches)}"
            )
        return matches[0]

    def require_struct(
        self, name: str, expected_members: dict[str, int]
    ) -> str:
        identifier = self.id_for_name(name)
        actual = {
            member_name: self.member_offsets[(identifier, member_index)]
            for (member_id, member_index), member_name in self.member_names.items()
            if member_id == identifier
        }
        if actual != expected_members:
            raise AssertionError(
                f"{name} member offsets differ: "
                f"expected {expected_members}, found {actual}"
            )
        return identifier

    def require_runtime_array_stride(
        self, element_identifier: str, expected_stride: int
    ) -> None:
        matches = [
            array_identifier
            for array_identifier, element in self.runtime_arrays.items()
            if element == element_identifier
        ]
        if not matches:
            raise AssertionError(
                f"no runtime array found for {element_identifier}"
            )
        for array_identifier in matches:
            actual_stride = self.array_strides.get(array_identifier)
            if actual_stride != expected_stride:
                raise AssertionError(
                    f"{array_identifier} stride differs: "
                    f"expected {expected_stride}, found {actual_stride}"
                )


def validate_common_layouts(layout: SpirvLayout) -> None:
    position = layout.require_struct(
        "PositionRadius16", {"Position": 0, "Radius": 12}
    )
    bounds = layout.require_struct(
        "Aabb3fSlots16",
        {
            "Minimum": 0,
            "MinimumPadding": 12,
            "Maximum": 16,
            "MaximumPadding": 28,
        },
    )
    layout.require_runtime_array_stride(position, 16)
    layout.require_runtime_array_stride(bounds, 32)


def validate_hlsl_constants(layout: SpirvLayout) -> None:
    identifier = layout.id_for_name("PhaseAConstants")
    members = {
        member_name: member_index
        for (member_id, member_index), member_name in layout.member_names.items()
        if member_id == identifier
    }
    expected_offsets = {
        "CameraPosition": 0,
        "Exposure": 12,
        "NormalMatrix": 16,
    }
    actual_offsets = {
        member_name: layout.member_offsets[(identifier, member_index)]
        for member_name, member_index in members.items()
    }
    if actual_offsets != expected_offsets:
        raise AssertionError(
            "PhaseAConstants offsets differ: "
            f"expected {expected_offsets}, found {actual_offsets}"
        )
    matrix_index = members["NormalMatrix"]
    actual_stride = layout.member_matrix_strides.get(
        (identifier, matrix_index)
    )
    if actual_stride != 16:
        raise AssertionError(
            f"NormalMatrix stride differs: expected 16, found {actual_stride}"
        )


def compile_layout(
    glslang: pathlib.Path,
    spirv_val: pathlib.Path,
    spirv_dis: pathlib.Path,
    source: pathlib.Path,
    output: pathlib.Path,
    hlsl: bool,
) -> SpirvLayout:
    command = [str(glslang), "-V"]
    if hlsl:
        command.extend(["-D", "-S", "vert", "-e", "main"])
    command.extend([str(source), "-o", str(output)])
    run(command)
    run([str(spirv_val), str(output)])

    assembly = output.with_suffix(".spvasm")
    run([str(spirv_dis), str(output), "-o", str(assembly)])
    return SpirvLayout(assembly.read_text(encoding="utf-8"))


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--glslang", required=True, type=pathlib.Path)
    parser.add_argument("--spirv-val", required=True, type=pathlib.Path)
    parser.add_argument("--spirv-dis", required=True, type=pathlib.Path)
    parser.add_argument("--source-dir", required=True, type=pathlib.Path)
    parser.add_argument("--output-dir", required=True, type=pathlib.Path)
    arguments = parser.parse_args()

    arguments.output_dir.mkdir(parents=True, exist_ok=True)
    glsl = compile_layout(
        arguments.glslang,
        arguments.spirv_val,
        arguments.spirv_dis,
        arguments.source_dir / "phase_a_layouts.comp.glsl",
        arguments.output_dir / "phase_a_layouts_glsl.spv",
        hlsl=False,
    )
    hlsl = compile_layout(
        arguments.glslang,
        arguments.spirv_val,
        arguments.spirv_dis,
        arguments.source_dir / "phase_a_layouts.hlsl",
        arguments.output_dir / "phase_a_layouts_hlsl.spv",
        hlsl=True,
    )

    validate_common_layouts(glsl)
    validate_common_layouts(hlsl)
    validate_hlsl_constants(hlsl)
    print("HLSL and GLSL SPIR-V layout reflection matches host contracts.")
    return 0


if __name__ == "__main__":
    try:
        sys.exit(main())
    except (AssertionError, RuntimeError) as error:
        print(f"shader layout validation failed: {error}", file=sys.stderr)
        sys.exit(1)
