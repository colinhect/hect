#version 410

#ifdef VERTEX

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

uniform mat4 model;
uniform mat4 modelViewProjection;

out vec3 vertexPosition;
out vec3 vertexWorldPosition;
out vec3 vertexNormal;
out vec3 vertexWorldNormal;

mat3 normalMatrix = mat3(
    model[0][0], model[0][1], model[0][2],
    model[1][0], model[1][1], model[1][2],
    model[2][0], model[2][1], model[2][2]
);

void main()
{
	vertexWorldPosition = (model * vec4(position, 1.0)).xyz;
    gl_Position = modelViewProjection * vec4(position, 1.0);
    vertexPosition = position;
    vertexNormal = normal;
    vertexWorldNormal = normalize(normalMatrix * normal);
}

#endif

#ifdef FRAGMENT

uniform sampler2D diffuseTexture;
uniform sampler2D materialTexture;
uniform sampler2D normalTexture;
uniform vec2 textureScale;

in vec3 vertexPosition;
in vec3 vertexWorldPosition;
in vec3 vertexNormal;
in vec3 vertexWorldNormal;

out vec3 outputDiffuse;
out vec3 outputMaterial;
out vec3 outputPosition;
out vec4 outputNormal;

void main()
{
    vec3 blendWeights = abs(vertexNormal);
    blendWeights = (blendWeights - 0.2) * 7;
    blendWeights = max(blendWeights, 0);
    blendWeights /= blendWeights.x + blendWeights.y + blendWeights.z;

    vec2 coordX = vertexPosition.yz * textureScale;
    vec2 coordY = vertexPosition.zx * textureScale;
    vec2 coordZ = vertexPosition.xy * textureScale;

    vec3 diffuseColorX = texture(diffuseTexture, coordX).rgb;
    vec3 diffuseColorY = texture(diffuseTexture, coordY).rgb;
    vec3 diffuseColorZ = texture(diffuseTexture, coordZ).rgb;

    outputDiffuse =
        diffuseColorX * blendWeights.x +
        diffuseColorY * blendWeights.y +
        diffuseColorZ * blendWeights.z;


    vec3 materialColorX = texture(materialTexture, coordX).rgb;
    vec3 materialColorY = texture(materialTexture, coordY).rgb;
    vec3 materialColorZ = texture(materialTexture, coordZ).rgb;

    outputMaterial =
        materialColorX * blendWeights.x +
        materialColorY * blendWeights.y +
        materialColorZ * blendWeights.z;

    vec2 bumpFetch1 = texture(normalTexture, coordX).xy - 0.5;  
    vec2 bumpFetch2 = texture(normalTexture, coordY).xy - 0.5;  
    vec2 bumpFetch3 = texture(normalTexture, coordZ).xy - 0.5;  
    vec3 bump1 = vec3(0.0, bumpFetch1.x, bumpFetch1.y);  
    vec3 bump2 = vec3(bumpFetch2.y, 0.0, bumpFetch2.x);  
    vec3 bump3 = vec3(bumpFetch3.x, bumpFetch3.y, 0.0);

    vec3 normalBump = bump1.xyz * blendWeights.x +  
                   bump2.xyz * blendWeights.y +  
                   bump3.xyz * blendWeights.z;  

    float depth = gl_FragCoord.z;
    outputNormal = vec4(normalize(vertexWorldNormal + normalBump * 0.5), depth);

    outputPosition = vertexWorldPosition;
}

#endif