{
    "modules" :
	[
        "Hect/Effects/HdrCompositor.frag",
        "Hect/Effects/HdrCompositor.vert"
    ],
    "parameters" :
	{
        "exposure" :
		{
            "type" : "Float",
            "value" : 0.0015
        },
        "oneOverGamma" :
		{
            "type" : "Float",
            "value" : 0.4545
        },
        "sourceTexture" :
		{
            "type" : "Texture",
            "value" : 0
        }
    }
}