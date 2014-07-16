{
    "modules" :
    [
        "GeometryBuffer.frag",
        "DirectionalLight.frag",
        "Screen.vert"
    ],
    "uniforms" :
    [
        {
            "name" : "lightColor",
            "type" : "Vector3",
            "value" : [ 1, 1, 1 ]
        },
        {
            "name" : "lightDirection",
            "type" : "Vector3",
            "value" : [ 1, 0, 0 ]
        },
        {
            "name" : "cameraPosition",
            "type" : "Vector3"
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
        }
    ]
}