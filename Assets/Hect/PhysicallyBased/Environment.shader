{
    "modules" :
    [
        "Environment.frag",
        "Screen.vert"
    ],
    "uniforms" :
    [
        {
            "name" : "cameraPosition",
            "type" : "Vector3"
        },
        {
            "name" : "view",
            "type" : "Matrix4"
        },
        {
            "name" : "diffuseBuffer",
            "type" : "Texture",
            "value" : 0
        },
        {
            "name" : "materialBuffer",
            "type" : "Texture",
            "value" : 1
        },
        {
            "name" : "positionBuffer",
            "type" : "Texture",
            "value" : 2
        },
        {
            "name" : "normalBuffer",
            "type" : "Texture",
            "value" : 3
        },
        {
            "name" : "environmentMap",
            "type" : "Texture",
            "value" : 4
        }
    ]
}