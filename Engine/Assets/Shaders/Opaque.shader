---
schema: PhysicalGeometry
modules:
  - type: Vertex
    path: Opaque.Vertex.glsl
  - type: Fragment
    path: Opaque.Fragment.glsl
uniforms:
  - name: modelView
    binding: ModelViewMatrix
  - name: modelViewProjection
    binding: ModelViewProjectionMatrix
  - name: diffuseTexture
    type: Texture
  - name: materialTexture
    type: Texture
  - name: normalTexture
    type: Texture
