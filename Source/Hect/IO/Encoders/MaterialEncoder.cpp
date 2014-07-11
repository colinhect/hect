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
#include "MaterialEncoder.h"

#include "Hect/Core/Enum.h"
#include "Hect/IO/Encoders/MaterialEncoder.h"

using namespace hect;

void MaterialEncoder::encode(const Material& material, ObjectEncoder& encoder)
{
    // Techniques
    ArrayEncoder techniquesEncoder = encoder.encodeArray("techniques");
    for (const Technique& technique : material.techniques())
    {
        // Passes
        ArrayEncoder passesEncoder = techniquesEncoder.encodeArray();
        for (const Pass& pass : technique.passes())
        {
            ObjectEncoder passEncoder = passesEncoder.encodeObject();

            // Shader
            if (pass.shader())
            {
                passEncoder.encodeString("shader", pass.shader().path().toString());
            }
            else
            {
                throw Error("Pass has no shader");
            }

            // Uniform values
            {
                ArrayEncoder uniformValuesEncoder = passEncoder.encodeArray("uniformValues");
                for (const PassUniformValue& uniformValue : pass.uniformValues())
                {
                    ObjectEncoder uniformValueEncoder = uniformValuesEncoder.encodeObject();
                    uniformValueEncoder.encodeString("name", uniformValue.name());
                    uniformValue.value().encode(uniformValueEncoder);
                }
            }

            // Textures
            {
                ArrayEncoder texturesEncoder = passEncoder.encodeArray("textures");
                for (const AssetHandle<Texture>& texture : pass.textures())
                {
                    texturesEncoder.encodeString(texture.path().toString());
                }
            }

            // Render state
            {
                ObjectEncoder renderStateEncoder = passEncoder.encodeObject("renderState");

                // Bulld a list of all states
                size_t stateCount = 4;
                RenderStateFlag states[] =
                {
                    RenderStateFlag_Blend,
                    RenderStateFlag_DepthTest,
                    RenderStateFlag_DepthWrite,
                    RenderStateFlag_CullFace
                };

                // Build a list of enabled/disabled states
                std::vector<RenderStateFlag> enabledFlags;
                std::vector<RenderStateFlag> disabledFlags;
                for (size_t i = 0; i < stateCount; ++i)
                {
                    if (pass.renderState().isEnabled(states[i]))
                    {
                        enabledFlags.push_back(states[i]);
                    }
                    else
                    {
                        disabledFlags.push_back(states[i]);
                    }
                }

                // Enabled states
                {
                    ArrayEncoder statesEncoder = renderStateEncoder.encodeArray("enabledFlags");

                    for (const RenderStateFlag& flag : enabledFlags)
                    {
                        statesEncoder.encodeEnum(flag);
                    }
                }

                // Disabled states
                {
                    ArrayEncoder statesEncoder = renderStateEncoder.encodeArray("disabledFlags");

                    for (const RenderStateFlag& flag : disabledFlags)
                    {
                        statesEncoder.encodeEnum(flag);
                    }
                }

                // Blend factors
                {
                    ArrayEncoder blendFactorsEncoder = passEncoder.encodeArray("blendFactors");

                    blendFactorsEncoder.encodeEnum(pass.renderState().sourceBlendFactor());
                    blendFactorsEncoder.encodeEnum(pass.renderState().destBlendFactor());
                }
            }
        }
    }
}

void MaterialEncoder::decode(Material& material, ObjectDecoder& decoder, AssetCache& assetCache)
{
    Technique::Array techniques;

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

            RenderState renderState;
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

                    std::string name;
                    if (uniformValueDecoder.hasMember("name"))
                    {
                        name = uniformValueDecoder.decodeString("name");
                    }
                    else
                    {
                        throw Error("A uniform value must have a name");
                    }

                    UniformValue value;
                    value.decode(uniformValueDecoder, assetCache);

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

            // Render state
            if (passDecoder.hasMember("renderState"))
            {
                ObjectDecoder renderStateDecoder = passDecoder.decodeObject("renderState");

                // Enabled states
                if (renderStateDecoder.hasMember("enabledFlags"))
                {
                    ArrayDecoder statesDecoder = renderStateDecoder.decodeArray("enabledFlags");
                    while (statesDecoder.hasMoreElements())
                    {

                        renderState.enable(statesDecoder.decodeEnum<RenderStateFlag>());
                    }
                }

                // Disabled states
                if (renderStateDecoder.hasMember("disabledFlags"))
                {
                    ArrayDecoder statesDecoder = renderStateDecoder.decodeArray("disabledFlags");
                    while (statesDecoder.hasMoreElements())
                    {
                        renderState.disable(statesDecoder.decodeEnum<RenderStateFlag>());
                    }
                }

                // Blend factors
                if (renderStateDecoder.hasMember("blendFactors"))
                {
                    ArrayDecoder blendFactorsDecoder = renderStateDecoder.decodeArray("blendFactors");
                    auto sourceFactor = blendFactorsDecoder.decodeEnum<BlendFactor>();
                    auto destFactor = blendFactorsDecoder.decodeEnum<BlendFactor>();
                    renderState.setBlendFactors(sourceFactor, destFactor);
                }
            }

            passes.push_back(Pass(renderState, textures, shader, uniformValues));
        }

        material.addTechnique(passes);
    }
}