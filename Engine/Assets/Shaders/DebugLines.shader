---
base: RenderStages/PostPhysicalGeometry.shader
modules:
  - type: Vertex
    path: DebugLines.Vertex.glsl
  - type: Fragment
    path: DebugLines.Fragment.glsl
uniforms:
  - name: modelViewProjection
    binding: ModelViewProjectionMatrix
  - name: intensity
    type: Float
    value: 1000.0
