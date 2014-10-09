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
#include "Pass.h"

#include "Hect/Graphics/Renderer.h"

using namespace hect;

Pass::Pass() :
    _resolvedFromShader(nullptr)
{
}

void Pass::prepare(Renderer& renderer)
{
    Shader& shader = *_shader;
    if (_resolvedFromShader != &shader)
    {
        resolvePassUniformValues(shader);
    }

    // Bind the render state
    renderer.bindState(_renderState);

    // Bind the textures in the pass
    unsigned textureIndex = 0;
    for (const AssetHandle<Texture>& texture : _textures)
    {
        renderer.bindTexture(*texture, textureIndex);
        ++textureIndex;
    }

    // Bind the shader
    renderer.bindShader(shader);

    // Set the uniform values
    for (auto& pair : _resolvedUniformValues)
    {
        const Uniform& uniform = *pair.first;
        const UniformValue& value = pair.second;
        renderer.setUniform(uniform, value);
    }
}

const RenderState& Pass::renderState() const
{
    return _renderState;
}

void Pass::setRenderState(const RenderState& renderState)
{
    _renderState = renderState;
}

const Pass::TextureSequence Pass::textures() const
{
    return _textures;
}

void Pass::addTexture(const AssetHandle<Texture>& texture)
{
    _textures.push_back(texture);
}

void Pass::clearTextures()
{
    _textures.clear();
}

const AssetHandle<Shader>& Pass::shader() const
{
    return _shader;
}

void Pass::setShader(const AssetHandle<Shader>& shader)
{
    _shader = shader;
}

const Pass::UniformValueSequence Pass::uniformValues() const
{
    return _uniformValues;
}

void Pass::addUniformValue(const std::string& name, const UniformValue& uniformValue)
{
    _uniformValues.push_back(PassUniformValue(name, uniformValue));
}

void Pass::resolvePassUniformValues(Shader& shader)
{
    _resolvedUniformValues.clear();

    // Resolve the uniforms that the uniform values refer to (this would be
    // invalidated if the shader changes)
    for (const PassUniformValue& uniformValue : _uniformValues)
    {
        const Uniform& uniform = shader.uniformWithName(uniformValue.name());
        _resolvedUniformValues[&uniform] = uniformValue.value();
    }

    _resolvedFromShader = &shader;
}

bool Pass::operator==(const Pass& pass) const
{
    if (_renderState != pass._renderState)
    {
        return false;
    }

    if (_textures.size() != pass._textures.size())
    {
        return false;
    }

    for (size_t i = 0; i < _textures.size(); ++i)
    {
        if (_textures[i] != pass._textures[i])
        {
            return false;
        }
    }

    if (_shader != pass._shader)
    {
        return false;
    }

    if (_uniformValues.size() != pass._uniformValues.size())
    {
        return false;
    }

    for (size_t i = 0; i < _uniformValues.size(); ++i)
    {
        if (_uniformValues[i] != pass._uniformValues[i])
        {
            return false;
        }
    }

    return true;
}

bool Pass::operator!=(const Pass& pass) const
{
    return !(*this == pass);
}

namespace hect
{

Encoder& operator<<(Encoder& encoder, const Pass& pass)
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

    return encoder;
}

Decoder& operator>>(Decoder& decoder, Pass& pass)
{
    // Passes
    decoder >> beginObject();

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
    return decoder;
}

}