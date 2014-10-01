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

ConstSequence<AssetHandle<Texture>> Pass::textures() const
{
    return ConstSequence<AssetHandle<Texture>>(_textures.begin(), _textures.end());
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

ConstSequence<PassUniformValue> Pass::uniformValues() const
{
    return ConstSequence<PassUniformValue>(_uniformValues.begin(), _uniformValues.end());
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