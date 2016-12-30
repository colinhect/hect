---
base: RenderStages/PostPhysicalGeometry.shader
modules:
  - type: Vertex
    path: Additive.Vertex.glsl
  - type: Fragment
    path: Additive.Fragment.glsl
uniforms:
  - name: modelViewProjection
    binding: ModelViewProjectionMatrix
  - name: additiveTexture
    type: Texture2
  - name: intensity
    type: Float
    value: 1.0
depthTested: false
blendMode:
  sourceFactor: One
  destinationFactor: One
