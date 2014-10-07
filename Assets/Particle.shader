{
    "modules": [
        "Common/Particle.vert",
        "Common/Rotation.geom",
        "Common/Particle.geom",
        "Common/Particle.frag"
    ],
    "parameters": {
        "colorMap": {
            "value": 0,
            "type": "Texture"
        },
        "model": {
            "binding": "ModelMatrix"
        },
        "cameraUp": {
            "binding": "CameraUp"
        },
        "intensityRange": {
            "value": [
                1,
                1
            ],
            "type": "Vector2"
        },
        "color": {
            "value": [
                1,
                1,
                1,
                1
            ],
            "type": "Vector4"
        },
        "cameraPosition": {
            "binding": "CameraPosition"
        },
        "sizeRange": {
            "value": [
                1,
                1
            ],
            "type": "Vector2"
        },
        "fadeRange": {
            "value": [
                0,
                1
            ],
            "type": "Vector2"
        },
        "viewProjection": {
            "binding": "ViewProjectionMatrix"
        }
    }
}