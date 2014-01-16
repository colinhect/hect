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
#include "Hect.h"

using namespace hect;

void MaterialDataValueFormat::load(Material& material, const std::string& name, const DataValue& dataValue, AssetCache& assetCache)
{
    Technique::Array techniques;

    // Base material
    if (dataValue["base"].isString())
    {
        Path basePath = dataValue["base"].asString();
        AssetHandle<Material> handle = assetCache.getHandle<Material>(basePath);
        techniques = handle.get().techniques(); // Copy the base material techniques
    }

    // Techniques
    size_t techniqueIndex = 0;
    for (const DataValue& techniqueValue : dataValue["techniques"])
    {
        Pass::Array passes;
        if (techniqueIndex < techniques.size())
        {
            passes = techniques[techniqueIndex].passes();
        }

        // Passes
        size_t passIndex = 0;
        for (const DataValue& passValue : techniqueValue)
        {
            RenderMode renderMode;
            AssetHandle<Texture>::Array textures;
            AssetHandle<Shader> shader;
            PassUniformValue::Array uniformValues;

            if (passIndex < passes.size())
            {
                renderMode = passes[passIndex].renderMode();
                textures = passes[passIndex].textures();
                shader = passes[passIndex].shader();
                uniformValues = passes[passIndex].uniformValues();
            }

            // Shader
            if (passValue["shader"].isString())
            {
                Path path = passValue["shader"].asString();
                shader = assetCache.getHandle<Shader>(path);
            }

            // Uniform values
            const DataValue& uniformValue = passValue["uniformValues"];
            for (const std::string& name : uniformValue.memberNames())
            {
                const Uniform& uniform = shader.get().uniformWithName(name);
                UniformValue value = ShaderDataValueFormat::parseValue(uniform.type(), uniformValue[name]);

                bool foundUniformValue = false;
                for (PassUniformValue& uniformValue : uniformValues)
                {
                    if (uniformValue.uniformName() == name)
                    {
                        uniformValue = PassUniformValue(name, value);
                        foundUniformValue = true;
                        break;
                    }
                }

                if (!foundUniformValue)
                {
                    uniformValues.push_back(PassUniformValue(name, value));
                }
            }

            // Textures
            size_t textureIndex = 0;
            for (const DataValue& textureValue : passValue["textures"])
            {
                if (!textureValue.isString())
                {
                    continue;
                }

                Path path = textureValue.asString();
                AssetHandle<Texture> texture = assetCache.getHandle<Texture>(path);

                // Append a new texture if needed
                if (textureIndex >= textures.size())
                {
                    textures.push_back(texture);
                }

                // Otherwise replace the texture already existing at the index
                else
                {
                    textures[textureIndex] = texture;
                }

                ++textureIndex;
            }

            // Render mode
            const DataValue& renderModeValue = passValue["renderMode"];
            if (renderModeValue.isObject())
            {
                // Enabled states
                for (const DataValue& stateValue : renderModeValue["enabledStates"])
                {
                    renderMode.enableState(_parseState(stateValue));
                }

                // Disabled states
                for (const DataValue& stateValue : renderModeValue["disabledStates"])
                {
                    renderMode.disableState(_parseState(stateValue));
                }

                // Blend factors
                const DataValue& blendFactorValues = renderModeValue["blendFactors"];
                if (blendFactorValues.isArray() && blendFactorValues.size() == 2)
                {
                    auto sourceFactor = _parseBlendFactor(blendFactorValues[0]);
                    auto destFactor = _parseBlendFactor(blendFactorValues[1]);
                    renderMode.setBlendFactors(sourceFactor, destFactor);
                }
            }

            // Append a new texture if needed
            if (passIndex >= passes.size())
            {
                passes.push_back(Pass(renderMode, textures, shader, uniformValues));
            }

            // Otherwise replace the texture already existing at the index
            else
            {
                passes[passIndex] = Pass(renderMode, textures, shader, uniformValues);
            }

            ++passIndex;
        }

        if (techniqueIndex < techniques.size())
        {
            techniques[techniqueIndex] = Technique(passes);
        }
        else
        {
            techniques.push_back(Technique(passes));
        }

        ++techniqueIndex;
    }

    material = Material(name, techniques);
}

RenderState MaterialDataValueFormat::_parseState(const DataValue& dataValue)
{
    static std::map<std::string, RenderState> states;

    if (states.empty())
    {
        states["Blend"] = RenderState::Blend;
        states["DepthTest"] = RenderState::DepthTest;
        states["DepthWrite"] = RenderState::DepthWrite;
        states["CullFace"] = RenderState::CullFace;
    }

    auto it = states.find(dataValue.asString());
    if (it == states.end())
    {
        throw Error(format("Invalid render state '%s'", dataValue.asString().c_str()));
    }

    return (*it).second;
}

BlendFactor MaterialDataValueFormat::_parseBlendFactor(const DataValue& dataValue)
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

    auto it = blendFactors.find(dataValue.asString());
    if (it == blendFactors.end())
    {
        throw Error(format("Invalid blend factor '%s'", dataValue.asString().c_str()));
    }

    return (*it).second;
}