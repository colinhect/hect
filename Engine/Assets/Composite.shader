---
modules:
  - type: Vertex
    path: Screen.glsl
  - type: Fragment
    path: Composite.glsl
uniforms:
  - name: diffuseBuffer
    binding: DiffuseBuffer
  - name: backBuffer
    binding: BackBuffer
depthTested: false
oneSided: false
