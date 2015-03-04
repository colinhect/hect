---
schema: PostPhysicalGeometry
modules:
  - type: Vertex
    path: Additive.Vertex.glsl
  - type: Fragment
    path: Additive.Fragment.glsl
uniforms:
  - name: modelViewProjection
    binding: ModelViewProjectionMatrix
  - name: diffuseTexture
    type: Texture
blendMode:
    sourceFactor: One
    destinationFactor: One
