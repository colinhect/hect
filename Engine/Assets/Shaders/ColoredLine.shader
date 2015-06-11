---
base: RenderStages/PostPhysicalGeometry.shader
modules:
  - type: Vertex
    path: ColoredLine.Vertex.glsl
  - type: Fragment
    path: ColoredLine.Fragment.glsl
uniforms:
  - name: modelViewProjection
    binding: ModelViewProjectionMatrix
  - name: color
    type: Color
    value: [ 1, 1, 1 ]
