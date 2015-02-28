---
modules:
  - type: Vertex
    path: Screen.glsl
  - type: Fragment
    path: Environment.glsl
uniforms:
  - name: cameraPosition
    binding: CameraPosition
  - name: diffuseBuffer
    binding: DiffuseBuffer
  - name: materialBuffer
    binding: MaterialBuffer
  - name: positionBuffer
    binding: PositionBuffer
  - name: normalBuffer
    binding: NormalBuffer
  - name: lightProbeCubeMap
    binding: LightProbeCubeMap
depthTested: false
blendMode:
    sourceFactor: One
    destinationFactor: One
