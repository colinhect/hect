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
#include "Renderer.h"

#include "Hect/Graphics/Shader.h"

using namespace hect;

Renderer::Capabilities& Renderer::capabilities()
{
    return _capabilities;
}

const Renderer::Capabilities& Renderer::capabilities() const
{
    return _capabilities;
}

Renderer::Statistics& Renderer::statistics()
{
    return _statistics;
}

const Renderer::Statistics& Renderer::statistics() const
{
    return _statistics;
}

RenderTarget* Renderer::boundTarget()
{
    return _boundTarget;
}

void Renderer::setBoundTarget(RenderTarget* target)
{
    _boundTarget = target;
}

Shader* Renderer::boundShader()
{
    return _boundShader;
}

void Renderer::setBoundShader(Shader* shader)
{
    _boundShader = shader;
}

Mesh* Renderer::boundMesh()
{
    return _boundMesh;
}

void Renderer::setBoundMesh(Mesh* mesh)
{
    _boundMesh = mesh;
}

const ShaderParameter& Renderer::resolveShaderParameter(const std::string& parameterName)
{
    if (!boundShader())
    {
        throw Error("No shader bound");
    }

    return boundShader()->parameterWithName(parameterName);
}

const ShaderParameter& Renderer::resolveShaderParameter(const char* parameterName)
{
    if (!boundShader())
    {
        throw Error("No shader bound");
    }

    return boundShader()->parameterWithName(parameterName);
}
