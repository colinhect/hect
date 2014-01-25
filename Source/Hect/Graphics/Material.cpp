///////////////////////////////////////////////////////////////////////////////
// This source file is part of Hect.
//
// Copyright (c) 2014 Colin Hill
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
///////////////////////////////////////////////////////////////////////////////
#include "Material.h"

using namespace hect;

Material::Material()
{
}

Material::Material(const std::string& name) :
    _name(name)
{
}

Material::Material(const std::string& name, const Technique::Array& techniques) :
    _name(name),
    _techniques(techniques)
{
}

const std::string& Material::name() const
{
    return _name;
}

const Technique::Array& Material::techniques() const
{
    return _techniques;
}

void Material::save(ObjectEncoder& encoder) const
{
    encoder;
    throw Error("Not implemented");
}

void Material::load(ObjectDecoder& decoder, AssetCache& assetCache)
{
    // Techniques
    ArrayDecoder techniquesDecoder = decoder.decodeArray("techniques");
    while (techniquesDecoder.hasMoreElements())
    {
        Pass::Array passes;

        // Passes
        ArrayDecoder passesDecoder = techniquesDecoder.decodeArray();
        while (passesDecoder.hasMoreElements())
        {
            ObjectDecoder passDecoder = passesDecoder.decodeObject();

            RenderMode renderMode;
            AssetHandle<Texture>::Array textures;
            AssetHandle<Shader> shader;
            PassUniformValue::Array uniformValues;

            // Shader
            Path path = passDecoder.decodeString("shader");
            shader = assetCache.getHandle<Shader>(path);

            // Uniform values
            if (passDecoder.hasMember("uniformValues"))
            {
                ArrayDecoder uniformValuesDecoder = passDecoder.decodeArray("uniformValues");
                while (uniformValuesDecoder.hasMoreElements())
                {
                    ObjectDecoder uniformValueDecoder = uniformValuesDecoder.decodeObject();
                    std::string name = uniformValueDecoder.decodeString("name");

                    const Uniform& uniform = shader->uniformWithName(name);

                    UniformValue value;
                    switch (uniform.type())
                    {
                    case UniformType::Int:
                    case UniformType::Texture:
                        value = UniformValue(uniformValueDecoder.decodeInt("value"), uniform.type());
                        break;
                    case UniformType::Float:
                        value = UniformValue(uniformValueDecoder.decodeReal("value"));
                        break;
                    case UniformType::Vector2:
                        value = UniformValue(uniformValueDecoder.decodeVector2("value"));
                        break;
                    case UniformType::Vector3:
                        value = UniformValue(uniformValueDecoder.decodeVector3("value"));
                        break;
                    case UniformType::Vector4:
                        value = UniformValue(uniformValueDecoder.decodeVector4("value"));
                        break;
                    default:
                        throw Error("Unsupported uniform value type");
                    }

                    uniformValues.push_back(PassUniformValue(name, value));
                }
            }

            // Textures
            if (passDecoder.hasMember("textures"))
            {
                ArrayDecoder texturesDecoder = passDecoder.decodeArray("textures");
                while (texturesDecoder.hasMoreElements())
                {
                    Path path = texturesDecoder.decodeString();
                    AssetHandle<Texture> texture = assetCache.getHandle<Texture>(path);
                    textures.push_back(texture);
                }
            }

            // Render mode
            if (passDecoder.hasMember("renderMode"))
            {
                ObjectDecoder renderModeDecoder = passDecoder.decodeObject("renderMode");

                // Enabled states
                if (renderModeDecoder.hasMember("enabledStates"))
                {
                    ArrayDecoder statesDecoder = renderModeDecoder.decodeArray("enabledStates");
                    while (statesDecoder.hasMoreElements())
                    {
                        renderMode.enableState(_parseState(statesDecoder.decodeString()));
                    }
                }

                // Disabled states
                if (renderModeDecoder.hasMember("disabledStates"))
                {
                    ArrayDecoder statesDecoder = renderModeDecoder.decodeArray("disabledStates");
                    while (statesDecoder.hasMoreElements())
                    {
                        renderMode.disableState(_parseState(statesDecoder.decodeString()));
                    }
                }

                // Blend factors
                if (renderModeDecoder.hasMember("blendFactors"))
                {
                    ArrayDecoder blendFactorsDecoder = renderModeDecoder.decodeArray("blendFactors");
                    auto sourceFactor = _parseBlendFactor(blendFactorsDecoder.decodeString());
                    auto destFactor = _parseBlendFactor(blendFactorsDecoder.decodeString());
                    renderMode.setBlendFactors(sourceFactor, destFactor);
                }
            }

            passes.push_back(Pass(renderMode, textures, shader, uniformValues));
        }
        _techniques.push_back(Technique(passes));
    }
}

RenderState Material::_parseState(const std::string& value)
{
    static std::map<std::string, RenderState> states;

    if (states.empty())
    {
        states["Blend"] = RenderState::Blend;
        states["DepthTest"] = RenderState::DepthTest;
        states["DepthWrite"] = RenderState::DepthWrite;
        states["CullFace"] = RenderState::CullFace;
    }

    auto it = states.find(value);
    if (it == states.end())
    {
        throw Error(format("Invalid render state '%s'", value.c_str()));
    }

    return (*it).second;
}

BlendFactor Material::_parseBlendFactor(const std::string& value)
{
    static std::map<std::string, BlendFactor> blendFactors;

    if (blendFactors.empty())
    {
        blendFactors["Zero"] = BlendFactor::Zero;
        blendFactors["One"] = BlendFactor::One;
        blendFactors["SourceColor"] = BlendFactor::SourceColor;
        blendFactors["OneMinusSourceColor"] = BlendFactor::OneMinusSourceColor;
        blendFactors["DestColor"] = BlendFactor::DestColor;
        blendFactors["OneMinusDestColor"] = BlendFactor::OneMinusDestColor;
        blendFactors["SourceAlpha"] = BlendFactor::SourceAlpha;
        blendFactors["OneMinusSourceAlpha"] = BlendFactor::OneMinusSourceAlpha;
        blendFactors["DestAlpha"] = BlendFactor::DestAlpha;
        blendFactors["OneMinusDestAlpha"] = BlendFactor::OneMinusDestAlpha;
    }

    auto it = blendFactors.find(value);
    if (it == blendFactors.end())
    {
        throw Error(format("Invalid blend factor '%s'", value.c_str()));
    }

    return (*it).second;
}