{
    "modules": [
        {
            "type": "Vertex",
            "path": "Particle.glsl"
        },
        {
            "type": "Geometry",
            "path": "Rotation.glsl"
        },
        {
            "type": "Geometry",
            "path": "Particle.glsl"
        },
        {
            "type": "Fragment",
            "path": "Particle.glsl"
        }
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