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
depthTested: false
blendMode:
  sourceFactor: One
  destinationFactor: One
