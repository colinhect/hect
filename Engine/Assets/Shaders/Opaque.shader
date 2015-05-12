---
renderStage: PhysicalGeometry
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
    type: Texture2
  - name: materialTexture
    type: Texture2
  - name: normalTexture
    type: Texture2
