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
        _resolvePassUniformValues(shader);
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

const AssetHandle<Texture>::Array& Pass::textures() const
{
    return _textures;
}

void Pass::setTextures(const AssetHandle<Texture>::Array& textures)
{
    _textures = textures;
}

const AssetHandle<Shader>& Pass::shader() const
{
    return _shader;
}

void Pass::setShader(const AssetHandle<Shader>& shader)
{
    _shader = shader;
}

const PassUniformValue::Array& Pass::uniformValues() const
{
    return _uniformValues;
}

void Pass::setUniformValues(const PassUniformValue::Array& uniformValues)
{
    _uniformValues = uniformValues;
}

bool Pass::operator==(const Pass& pass) const
{
    // Render state
    if (_renderState != pass.renderState())
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

void Pass::_resolvePassUniformValues(Shader& shader)
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