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

const std::string& Material::name() const
{
    return _name;
}

void Material::setName(const std::string& name)
{
    _name = name;
}

Material::TechniqueSequence Material::techniques()
{
    return _techniques;
}

const Material::TechniqueSequence Material::techniques() const
{
    return _techniques;
}

void Material::addTechnique(const Technique& technique)
{
    _techniques.push_back(technique);
}

void Material::clearTechniques()
{
    _techniques.clear();
}

Technique& Material::preferedTechnique()
{
    if (_techniques.empty())
    {
        throw Error{ "The material has no techniques" };
    }
    return _techniques[0];
}

bool Material::operator==(const Material& material) const
{
    if (_techniques.size() != material._techniques.size())
    {
        return false;
    }

    for (auto i = 0u; i < _techniques.size(); ++i)
    {
        if (_techniques[i] != material._techniques[i])
        {
            return false;
        }
    }

    return true;
}

bool Material::operator!=(const Material& material) const
{
    return !(*this == material);
}

namespace hect
{

Encoder& operator<<(Encoder& encoder, const Material& material)
{
    encoder << beginObject() << beginArray("techniques");
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
                encoder << encodeValue("shader", pass.shader());
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
                    encoder << beginObject()
                            << encodeValue("name", uniformValue.name())
                            << encodeValue(uniformValue.value())
                            << endObject();
                }
                encoder << endArray();
            }

            // Textures
            {
                encoder << beginArray("textures");
                for (const AssetHandle<Texture>& texture : pass.textures())
                {
                    encoder << texture;
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
                        encoder << encodeEnum(flag);
                    }

                    encoder << endArray();
                }

                // Disabled states
                {
                    encoder << beginArray("disabledFlags");

                    for (const RenderStateFlag& flag : disabledFlags)
                    {
                        encoder << encodeEnum(flag);
                    }

                    encoder << endArray();
                }

                // Blend factors
                {
                    encoder << beginArray("blendFactors");
                    encoder << encodeEnum(pass.renderState().sourceBlendFactor());
                    encoder << encodeEnum(pass.renderState().destBlendFactor());
                    encoder << endArray();
                }

                encoder << endObject();
            }
            encoder << endObject();
        }
        encoder << endArray();
    }

    return encoder << endArray() << endObject();
}

Decoder& operator>>(Decoder& decoder, Material& material)
{
    decoder >> beginObject() >> beginArray("techniques");
    while (decoder.hasMoreElements())
    {
        Technique technique;

        // Passes
        decoder >> beginArray();
        while (decoder.hasMoreElements())
        {
            decoder >> beginObject();

            Pass pass;

            // Shader
            AssetHandle<Shader> shader;
            decoder >> decodeValue("shader", shader);
            pass.setShader(shader);

            // Uniform values
            if (decoder.selectMember("uniformValues"))
            {
                decoder >> beginArray();
                while (decoder.hasMoreElements())
                {
                    decoder >> beginObject();

                    std::string name;
                    decoder >> decodeValue("name", name);

                    UniformValue value;
                    decoder >> decodeValue(value);

                    pass.addUniformValue(name, value);

                    decoder >> endObject();
                }
                decoder >> endArray();
            }

            // Textures
            if (decoder.selectMember("textures"))
            {
                decoder >> beginArray();
                while (decoder.hasMoreElements())
                {
                    AssetHandle<Texture> texture;
                    decoder >> decodeValue(texture);
                    pass.addTexture(texture);
                }
                decoder >> endArray();
            }

            // Render state
            if (decoder.selectMember("renderState"))
            {
                decoder >> beginObject();

                RenderState renderState;

                // Enabled states
                if (decoder.selectMember("enabledFlags"))
                {
                    decoder >> beginArray();
                    while (decoder.hasMoreElements())
                    {
                        RenderStateFlag flag;
                        decoder >> decodeEnum(flag);
                        renderState.enable(flag);
                    }
                    decoder >> endArray();
                }

                // Disabled states
                if (decoder.selectMember("disabledFlags"))
                {
                    decoder >> beginArray();
                    while (decoder.hasMoreElements())
                    {
                        RenderStateFlag flag;
                        decoder >> decodeEnum(flag);
                        renderState.disable(flag);
                    }
                    decoder >> endArray();
                }

                // Blend factors
                if (decoder.selectMember("blendFactors"))
                {
                    decoder >> beginArray();

                    BlendFactor sourceFactor, destFactor;
                    decoder >> decodeEnum(sourceFactor)
                            >> decodeEnum(destFactor);

                    renderState.setBlendFactors(sourceFactor, destFactor);

                    decoder >> endArray();
                }

                decoder >> endObject();

                pass.setRenderState(renderState);
            }
            decoder >> endObject();

            technique.addPass(pass);
        }
        decoder >> endArray();

        material.addTechnique(technique);
    }
    return decoder >> endArray() >> endObject();
}

}