# Sources

This file records the primary sources used by the comparative prepass. External
sources will be added with the associated review so that every report remains
auditable.

## Move Vectormath

- Repository baseline:
  `d68818dd806bb5cbb5d23dbd5c4d0ef0a8570597`
- Public headers:
  `packages/move/math/include/move/math/`
- Tests:
  `packages/move/math/tests/`

## Benchmark suite

- Repository baseline:
  `d51e173`
- Move dependency:
  `d68818dd806bb5cbb5d23dbd5c4d0ef0a8570597`
- RTM dependency:
  `745bd25673d93b46941eda55e0993327dbc12b53` (RTM 2.3.1)

## External APIs

All external sources were accessed on 2026-07-26. Reviews use official
documentation and first-party source where available.

### Unity

- [Unity 6.5 Vector3](https://docs.unity3d.com/ScriptReference/Vector3.html)
- [Unity 6.5 normalized-vector behavior](https://docs.unity3d.com/ScriptReference/Vector3-normalized.html)
- [Unity 6.5 Vector3 approximate equality operator](https://docs.unity3d.com/ScriptReference/Vector3-operator_eq.html)
- [Unity Vector3 exact Equals method](https://docs.unity3d.com/ScriptReference/Vector3.Equals.html)
- [Unity 6.5 clamped Vector3.Lerp](https://docs.unity3d.com/ScriptReference/Vector3.Lerp.html)
- [Unity 6.5 Bounds](https://docs.unity3d.com/ScriptReference/Bounds.html)
- [Unity 6.5 Bounds.Expand](https://docs.unity3d.com/ScriptReference/Bounds.Expand.html)
- [Unity 6.5 Ray](https://docs.unity3d.com/ScriptReference/Ray.html)
- [Unity 6.5 Plane](https://docs.unity3d.com/ScriptReference/Plane.html)
- [Unity 6.5 Transform.TransformPoint](https://docs.unity3d.com/ScriptReference/Transform.TransformPoint.html)
- [Unity 6.5 Transform.TransformVector](https://docs.unity3d.com/ScriptReference/Transform.TransformVector.html)
- [Unity 6.5 Transform.TransformDirection](https://docs.unity3d.com/ScriptReference/Transform.TransformDirection.html)
- [Unity 6.5 Transform.lossyScale](https://docs.unity3d.com/ScriptReference/Transform-lossyScale.html)
- [Unity 6.5 Transform.SetParent](https://docs.unity3d.com/ScriptReference/Transform.SetParent.html)
- [Unity 6.5 Quaternion.eulerAngles](https://docs.unity3d.com/ScriptReference/Quaternion-eulerAngles.html)
- [Unity 6.5 Quaternion.Euler](https://docs.unity3d.com/ScriptReference/Quaternion.Euler.html)
- [Unity 6.5 Matrix4x4.MultiplyPoint](https://docs.unity3d.com/ScriptReference/Matrix4x4.MultiplyPoint.html)
- [Unity 6.5 Matrix4x4.MultiplyPoint3x4](https://docs.unity3d.com/ScriptReference/Matrix4x4.MultiplyPoint3x4.html)
- [Unity 6.5 Matrix4x4.MultiplyVector](https://docs.unity3d.com/ScriptReference/Matrix4x4.MultiplyVector.html)
- [Unity 6.5 Matrix4x4.TRS](https://docs.unity3d.com/ScriptReference/Matrix4x4.TRS.html)
- [Unity 6.5 GeometryUtility](https://docs.unity3d.com/ScriptReference/GeometryUtility.html)
- [Unity 6.5 frustum-plane extraction](https://docs.unity3d.com/ScriptReference/GeometryUtility.CalculateFrustumPlanes.html)
- [Unity Mathematics 1.3.1 manual](https://docs.unity.cn/Packages/com.unity.mathematics%401.3/manual/index.html)
- [Unity Mathematics 1.3.1 RigidTransform](https://docs.unity.cn/Packages/com.unity.mathematics%401.3/api/Unity.Mathematics.RigidTransform.html)
- [Unity Mathematics EulerZXY](https://docs.unity.cn/Packages/com.unity.mathematics%401.3/api/Unity.Mathematics.quaternion.EulerZXY.html)
- [Unity Mathematics 1.3 math.lerp](https://docs.unity3d.com/Packages/com.unity.mathematics%401.3/api/Unity.Mathematics.math.lerp.html)

### Unreal Engine

- [Unreal Engine 5.8 TVector](https://dev.epicgames.com/documentation/en-us/unreal-engine/API/Runtime/Core/TVector)
- [Unreal Engine 5.8 TTransform](https://dev.epicgames.com/documentation/en-us/unreal-engine/API/Runtime/Core/TTransform)
- [Unreal Engine 5.8 FMath](https://dev.epicgames.com/documentation/en-us/unreal-engine/API/Runtime/Core/FMath)
- [Unreal Engine 5.8 TBoxSphereBounds](https://dev.epicgames.com/documentation/en-us/unreal-engine/API/Runtime/Core/TBoxSphereBounds)
- [Unreal Engine 5.8 TOrientedBox3](https://dev.epicgames.com/documentation/en-us/unreal-engine/API/Runtime/GeometryCore/TOrientedBox3)
- [Unreal Engine line/box intersection](https://dev.epicgames.com/documentation/en-us/unreal-engine/API/Runtime/Core/FMath/LineBoxIntersection)
- [Unreal Engine ray/plane intersection](https://dev.epicgames.com/documentation/en-us/unreal-engine/API/Runtime/Core/FMath/RayPlaneIntersection)
- [Unreal Engine convex-volume box test](https://dev.epicgames.com/documentation/en-us/unreal-engine/API/Runtime/Engine/FConvexVolume/IntersectBox)
- [Unreal Engine view-frustum construction](https://dev.epicgames.com/documentation/en-us/unreal-engine/API/Runtime/Engine/GetViewFrustumBounds)

### Source SDK 2013

- [Valve source-sdk-2013 repository](https://github.com/ValveSoftware/source-sdk-2013)
- [Vector source](https://github.com/ValveSoftware/source-sdk-2013/blob/master/src/public/mathlib/vector.h)
- [Mathlib source](https://github.com/ValveSoftware/source-sdk-2013/blob/master/src/public/mathlib/mathlib.h)
- [VMatrix source](https://github.com/ValveSoftware/source-sdk-2013/blob/master/src/public/mathlib/vmatrix.h)
- [Ray_t source](https://github.com/ValveSoftware/source-sdk-2013/blob/master/src/public/cmodel.h)

### Garry's Mod

- [Facepunch Garry's Mod wiki](https://wiki.facepunch.com/gmod)
- [Vector](https://wiki.facepunch.com/gmod/Vector)
- [Angle](https://wiki.facepunch.com/gmod/Angle)
- [Ray/OBB intersection](https://wiki.facepunch.com/gmod/util.IntersectRayWithOBB)
- [Ray/plane intersection](https://wiki.facepunch.com/gmod/util.IntersectRayWithPlane)
- [Ray/sphere intersection](https://wiki.facepunch.com/gmod/util.IntersectRayWithSphere)
- [Line trace](https://wiki.facepunch.com/gmod/util.TraceLine)
- [Trace result](https://wiki.facepunch.com/gmod/Structures/TraceResult)

### Godot

- [Godot stable Vector3](https://docs.godotengine.org/en/stable/classes/class_vector3.html)
- [Godot stable Quaternion](https://docs.godotengine.org/en/stable/classes/class_quaternion.html)
- [Godot stable Basis](https://docs.godotengine.org/en/stable/classes/class_basis.html)
- [Godot stable Transform3D](https://docs.godotengine.org/en/stable/classes/class_transform3d.html)
- [Godot stable AABB](https://docs.godotengine.org/en/stable/classes/class_aabb.html)
- [Godot stable Geometry3D](https://docs.godotengine.org/en/stable/classes/class_geometry3d.html)
- [Godot stable Camera3D](https://docs.godotengine.org/en/stable/classes/class_camera3d.html)
- [Godot 3D transform tutorial](https://docs.godotengine.org/en/stable/tutorials/3d/using_transforms.html)

### DirectXMath and DirectXTK

- [DirectXMath June 2026 repository](https://github.com/microsoft/DirectXMath)
- [DirectXMath programming guide](https://learn.microsoft.com/en-us/windows/win32/dxmath/ovw-xnamath-progguide)
- [DirectXMath type-usage and matrix conventions](https://learn.microsoft.com/en-us/windows/win32/dxmath/pg-xnamath-getting-started)
- [XMVECTOR data type](https://learn.microsoft.com/en-us/windows/win32/dxmath/xmvector-data-type)
- [DirectXCollision source](https://github.com/microsoft/DirectXMath/blob/main/Inc/DirectXCollision.h)
- [BoundingBox](https://learn.microsoft.com/en-us/windows/win32/api/directxcollision/ns-directxcollision-boundingbox)
- [BoundingOrientedBox](https://learn.microsoft.com/en-us/windows/win32/api/directxcollision/ns-directxcollision-boundingorientedbox)
- [BoundingFrustum](https://learn.microsoft.com/en-us/windows/win32/api/directxcollision/ns-directxcollision-boundingfrustum)
- [Triangle-ray intersection](https://learn.microsoft.com/en-us/windows/win32/api/directxcollision/nf-directxcollision-intersects)
- [DirectXTK SimpleMath guide](https://github.com/microsoft/DirectXTK/wiki/Using-the-SimpleMath-library)
- [DirectXTK SimpleMath source](https://github.com/microsoft/DirectXTK/blob/main/Inc/SimpleMath.h)
