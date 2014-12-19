---
modules:
  - type: Vertex
    path: ColoredLine.Vertex.glsl
  - type: Fragment
    path: ColoredLine.Fragment.glsl
uniforms:
  - name: modelViewProjection
    binding: ModelViewProjectionMatrix
  - name: colorOverride
    type: Vector3
    value: [ 100, 0, 0 ]
