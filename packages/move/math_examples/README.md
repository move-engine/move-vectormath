# move/math_examples

This package contains small examples built on top of `move-vectormath`.
The package itself is configured as an application package whose entry point is
the ray tracer in `src/simple_ray_tracer.cpp`.

Files:

- `src/transform_component.hpp`: hierarchical transform component
- `src/camera_component.hpp`: camera component built on the transform component
- `src/character_controller.hpp`: collide-and-slide character controller example
- `src/simple_ray_tracer.cpp`: small ray tracer that writes a PPM image

Standalone compile example:

```bash
g++ -std=c++20 -I. -Ipackages/move/math/include \
  -I"$HOME/.cpm/rtm/ab29fa1abeaacf87a80a16348f5bfc5d266cef54/includes" \
  packages/move/math_examples/src/simple_ray_tracer.cpp -o simple_ray_tracer
```

Then run:

```bash
./simple_ray_tracer output.ppm
```
