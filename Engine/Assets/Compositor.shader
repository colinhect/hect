{
    "modules": [
        {
            "type": "Vertex",
            "path": "Screen.glsl"
        },
        {
            "type": "Fragment",
            "path": "Compositor.glsl"
        }
    ],
    "parameters": [
        {
            "name": "exposure",
            "value": 1.0,
            "type": "Float"
        },
        {
            "name": "oneOverGamma",
            "value": 0.4545,
            "type": "Float"
        },
        {
            "name": "accumulationBuffer",
            "value": 0,
            "type": "Texture"
        }
    ]
}