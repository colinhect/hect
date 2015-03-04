---
schema: PhysicalGeometry
modules:
  - type: Vertex
    path: Solid.Vertex.glsl
  - type: Fragment
    path: Solid.Fragment.glsl
uniforms:
  - name: diffuse
    type: Color
    value: [ 1, 1, 1 ]
  - name: roughness
    type: Float
    value: 1
  - name: metallic
    type: Float
    value: 0
  - name: modelView
    binding: ModelViewMatrix
  - name: modelViewProjection
    binding: ModelViewProjectionMatrix
