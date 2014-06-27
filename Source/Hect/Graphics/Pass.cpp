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

Pass::Pass(const RenderMode& renderMode, const AssetHandle<Texture>::Array& textures, const AssetHandle<Shader>& shader, const PassUniformValue::Array& uniformValues) :
    _renderMode(renderMode),
    _textures(textures),
    _shader(shader),
    _uniformValues(uniformValues)
{
    _resolvePassUniformValues();
}

void Pass::prepare(Renderer& renderer) const
{
    // Bind the render mode
    renderer.bindMode(_renderMode);

    // Bind the textures in the pass
    unsigned textureIndex = 0;
    for (const AssetHandle<Texture>& texture : _textures)
    {
        renderer.bindTexture(*texture, textureIndex);
        ++textureIndex;
    }

    // Bind the shader
    renderer.bindShader(*_shader);

    // Set the uniform values
    for (auto& pair : _resolvedUniformValues)
    {
        const Uniform& uniform = *pair.first;
        const UniformValue& value = pair.second;
        renderer.setUniform(uniform, value);
    }
}

RenderMode& Pass::renderMode()
{
    return _renderMode;
}

const RenderMode& Pass::renderMode() const
{
    return _renderMode;
}

const AssetHandle<Texture>::Array& Pass::textures() const
{
    return _textures;
}

const AssetHandle<Shader>& Pass::shader() const
{
    return _shader;
}

const PassUniformValue::Array& Pass::uniformValues() const
{
    return _uniformValues;
}

bool Pass::operator==(const Pass& pass) const
{
    // Render mode
    if (_renderMode != pass.renderMode())
    {
        return false;
    }

    // Texture count
    if (_textures.size() != pass.textures().size())
    {
        return false;
    }

    // Textures
    size_t textureCount = _textures.size();
    for (size_t i = 0; i < textureCount; ++i)
    {
        if (_textures[i] != pass.textures()[i])
        {
            return false;
        }
    }

    // Shader
    if (_shader != pass.shader())
    {
        return _shader;
    }

    // Uniform value count
    if (_uniformValues.size() != pass.uniformValues().size())
    {
        return false;
    }

    // Textures
    size_t uniformValueCount = _uniformValues.size();
    for (size_t i = 0; i < uniformValueCount; ++i)
    {
        if (_uniformValues[i] != pass.uniformValues()[i])
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

void Pass::_resolvePassUniformValues()
{
    // Resolve the uniforms that the uniform values refer to (this would be
    // invalidated if the shader changes)
    for (const PassUniformValue& uniformValue : _uniformValues)
    {
        const Uniform& uniform = _shader->uniformWithName(uniformValue.uniformName());
        _resolvedUniformValues[&uniform] = uniformValue.value();
    }
}