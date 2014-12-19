---
modules:
  - type: Vertex
    path: Solid.Vertex.glsl
  - type: Fragment
    path: Solid.Fragment.glsl
uniforms:
  - name: diffuse
    type: Vector3
    value: [ 0.8, 0.8, 0.8 ]
  - name: roughness
    type: Float
    value: 0.3
  - name: metallic
    type: Float
    value: 1
  - name: model
    binding: ModelMatrix
  - name: modelViewProjection
    binding: ModelViewProjectionMatrix
