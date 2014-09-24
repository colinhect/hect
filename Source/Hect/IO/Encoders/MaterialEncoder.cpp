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

#include "Hect/Reflection/Enum.h"
#include "Hect/IO/Encoders/MaterialEncoder.h"

using namespace hect;

void MaterialEncoder::encode(const Material& material, Encoder& encoder)
{
    // Techniques
    encoder << beginArray("techniques");
    for (const Technique& technique : material.techniques())
    {
        // Passes
        encoder << beginArray();
        for (const Pass& pass : technique.passes())
        {
            encoder << beginObject();

            // Shader
            if (pass.shader())
            {
                encoder << member("shader", pass.shader().path().toString());
            }
            else
            {
                throw Error("Pass has no shader");
            }

            // Uniform values
            {
                encoder << beginArray("uniformValues");
                for (const PassUniformValue& uniformValue : pass.uniformValues())
                {
                    encoder << beginObject() << member("name", uniformValue.name());

                    uniformValue.value().encode(encoder);

                    encoder << endObject();
                }
                encoder << endArray();
            }

            // Textures
            {
                encoder << beginArray("textures");
                for (const AssetHandle<Texture>& texture : pass.textures())
                {
                    encoder << texture.path().toString();
                }
                encoder << endArray();
            }

            // Render state
            {
                encoder << beginObject("renderState");

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
                    encoder << beginArray("enabledFlags");

                    for (const RenderStateFlag& flag : enabledFlags)
                    {
                        encoder.encodeEnum(flag);
                    }

                    encoder << endArray();
                }

                // Disabled states
                {
                    encoder << beginArray("disabledFlags");

                    for (const RenderStateFlag& flag : disabledFlags)
                    {
                        encoder.encodeEnum(flag);
                    }

                    encoder << endArray();
                }

                // Blend factors
                {
                    encoder << beginArray("blendFactors");

                    encoder.encodeEnum(pass.renderState().sourceBlendFactor());
                    encoder.encodeEnum(pass.renderState().destBlendFactor());

                    encoder << endArray();
                }

                encoder << endObject();
            }
            encoder << endObject();
        }
        encoder << endArray();
    }
    encoder << endArray();
}

void MaterialEncoder::decode(Material& material, ObjectDecoder& decoder, AssetCache& assetCache)
{
    // Techniques
    ArrayDecoder techniquesDecoder = decoder.decodeArray("techniques");
    while (techniquesDecoder.hasMoreElements())
    {
        Technique technique;

        // Passes
        ArrayDecoder passesDecoder = techniquesDecoder.decodeArray();
        while (passesDecoder.hasMoreElements())
        {
            ObjectDecoder passDecoder = passesDecoder.decodeObject();

            Pass pass;

            AssetHandle<Shader> shader;

            // Shader
            Path path = passDecoder.decodeString("shader");
            shader = assetCache.getHandle<Shader>(path);

            pass.setShader(shader);

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

                    pass.addUniformValue(name, value);
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
                    pass.addTexture(texture);
                }
            }

            // Render state
            if (passDecoder.hasMember("renderState"))
            {
                ObjectDecoder renderStateDecoder = passDecoder.decodeObject("renderState");

                RenderState renderState;

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

                pass.setRenderState(renderState);
            }

            technique.addPass(pass);
        }

        material.addTechnique(technique);
    }
}