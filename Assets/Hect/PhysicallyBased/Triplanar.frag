#version 330

uniform sampler2D diffuseMap;
uniform sampler2D materialMap;
uniform sampler2D normalMap;
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

    vec3 diffuseColorX = texture(diffuseMap, coordX).rgb;
    vec3 diffuseColorY = texture(diffuseMap, coordY).rgb;
    vec3 diffuseColorZ = texture(diffuseMap, coordZ).rgb;

    outputDiffuse =
        diffuseColorX * blendWeights.x +
        diffuseColorY * blendWeights.y +
        diffuseColorZ * blendWeights.z;


    vec3 materialColorX = texture(materialMap, coordX).rgb;
    vec3 materialColorY = texture(materialMap, coordY).rgb;
    vec3 materialColorZ = texture(materialMap, coordZ).rgb;

    outputMaterial =
        materialColorX * blendWeights.x +
        materialColorY * blendWeights.y +
        materialColorZ * blendWeights.z;

    vec2 bumpFetch1 = texture(normalMap, coordX).xy - 0.5;  
    vec2 bumpFetch2 = texture(normalMap, coordY).xy - 0.5;  
    vec2 bumpFetch3 = texture(normalMap, coordZ).xy - 0.5;  
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