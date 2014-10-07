{
    "modules": [
        "Compositor.frag",
        "Screen.vert"
    ],
    "uniforms": [
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
            "name": "diffuseBuffer",
            "value": 0,
            "type": "Texture"
        },
        {
            "name": "accumulationBuffer",
            "value": 1,
            "type": "Texture"
        }
    ]
}