---
schema: PostPhysicalGeometry
modules:
  - type: Vertex
    path: Particle.Vertex.glsl
  #- type: Geometry
  #  path: Particle.Geometry.glsl
  - type: Fragment
    path: Particle.Fragment.glsl
uniforms:
  - name: modelViewProjection
    binding: ModelViewProjectionMatrix
  - name: projection
    binding: ProjectionMatrix
  - name: particleTexture
    type: Texture
  - name: particleSize
    type: Float
blendMode:
  sourceFactor: One
  destinationFacto: One
