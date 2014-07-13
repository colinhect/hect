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
            "value" : 0.15
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