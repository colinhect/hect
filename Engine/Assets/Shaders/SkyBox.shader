---
schema: PrePhysicalGeometry
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
depthTested: false
