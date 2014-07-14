{
    "modules" :
    [
        "Compositor.frag",
        "Screen.vert"
    ],
    "uniforms" :
    [
        {
            "name" : "exposure",
            "type" : "Float",
            "value" : 1.5
        },
        {
            "name" : "oneOverGamma",
            "type" : "Float",
            "value" : 0.4545
        },
        {
            "name" : "accumulationBuffer",
            "type" : "Texture",
            "value" : 0
        }
    ]
}