---
modules:
  - type: Vertex
    path: Screen.glsl
  - type: Fragment
    path: DirectionalLight.glsl
uniforms:
  - name: lightDirection
    binding: PrimaryLightDirection
  - name: lightColor
    binding: PrimaryLightColor
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
depthTested: false
blendMode:
    sourceFactor: One
    destinationFactor: One
