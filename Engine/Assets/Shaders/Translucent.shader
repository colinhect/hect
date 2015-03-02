---
schema: TranslucentPhysicalGeometry
modules:
  - type: Vertex
    path: Translucent.Vertex.glsl
  - type: Fragment
    path: Translucent.Fragment.glsl
uniforms:
  - name: modelViewProjection
    binding: ModelViewProjectionMatrix
  - name: diffuseTexture
    type: Texture
