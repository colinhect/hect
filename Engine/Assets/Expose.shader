---
modules:
  - type: Vertex
    path: Screen.glsl
  - type: Fragment
    path: Expose.glsl
uniforms:
  - name: exposure
    binding: CameraExposure
  - name: oneOverGamma
    binding: CameraOneOverGamma
  - name: backBuffer
    binding: BackBuffer
