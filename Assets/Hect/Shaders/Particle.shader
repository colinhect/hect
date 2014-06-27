{
    "modules" :
    [
        "Hect/Shaders/Particle.vert",
        "Hect/Shaders/Rotation.geom",
        "Hect/Shaders/Particle.geom",
        "Hect/Shaders/Particle.frag"
    ],
    "parameters" :
    {
        "colorMap" :
        {
            "type" : "Texture",
            "value" : 0
        },
        "color" :
        {
            "type" : "Vector4",
            "value" : [ 1, 1, 1, 1 ]
        },
        "sizeRange" :
        {
            "type" : "Vector2",
            "value" : [ 1, 1 ]
        },
        "intensityRange" :
        {
            "type" : "Vector2",
            "value" : [ 1, 1 ]
        },
        "fadeRange" :
        {
            "type" : "Vector2",
            "value" : [ 0, 1 ]
        },
        "cameraPosition" :
        {
            "binding" : "CameraPosition"
        },
        "cameraUp" :
        {
            "binding" : "CameraUp"
        },
        "viewProjection" :
        {
            "binding" : "ViewProjectionMatrix"
        },
        "model" :
        {
            "binding" : "ModelMatrix"
        }
    }
}