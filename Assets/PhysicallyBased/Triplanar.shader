{
   "modules" : [ "Triplanar.frag", "Triplanar.vert" ],
   "uniforms" : [
      {
         "name" : "diffuseTexture",
         "type" : "Texture",
         "value" : 0
      },
      {
         "name" : "materialTexture",
         "type" : "Texture",
         "value" : 1
      },
      {
         "name" : "normalTexture",
         "type" : "Texture",
         "value" : 2
      },
      {
         "name" : "textureScale",
         "type" : "Vector2",
         "value" : [ 1, 1 ]
      },
      {
         "binding" : "ModelMatrix",
         "name" : "model"
      },
      {
         "binding" : "ModelViewProjectionMatrix",
         "name" : "modelViewProjection"
      }
   ]
}
