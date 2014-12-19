---
modules:
  - type: Vertex
    path: SkyBox.Vertex.glsl
  - type: Fragment
    path: SkyBox.Fragment.glsl
uniforms:
  - name: modelViewProjection
    binding: ModelViewProjectionMatrix
  - name: skyBoxCubeMap
    binding: SkyBoxCubeMap
oneSided: false
depthTested: false
priority: 1000
