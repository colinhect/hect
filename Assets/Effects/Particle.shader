{
   "modules" : [
      "Common/Particle.vert",
      "Common/Rotation.geom",
      "Common/Particle.geom",
      "Common/Particle.frag"
   ],
   "parameters" : {
      "cameraPosition" : {
         "binding" : "CameraPosition"
      },
      "cameraUp" : {
         "binding" : "CameraUp"
      },
      "color" : {
         "type" : "Vector4",
         "value" : [ 1, 1, 1, 1 ]
      },
      "colorMap" : {
         "type" : "Texture",
         "value" : 0
      },
      "fadeRange" : {
         "type" : "Vector2",
         "value" : [ 0, 1 ]
      },
      "intensityRange" : {
         "type" : "Vector2",
         "value" : [ 1, 1 ]
      },
      "model" : {
         "binding" : "ModelMatrix"
      },
      "sizeRange" : {
         "type" : "Vector2",
         "value" : [ 1, 1 ]
      },
      "viewProjection" : {
         "binding" : "ViewProjectionMatrix"
      }
   }
}
