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
#include "RendererObject.h"

#include "Hect/Core/Error.h"

using namespace hect;

RendererObject::RendererObject() :
    _uploaded(false),
    _data(nullptr),
    _renderer(nullptr)
{
}

RendererObject::RendererObject(const RendererObject& renderObject) :
    _uploaded(false),
    _data(nullptr),
    _renderer(nullptr)
{
    renderObject;
}

RendererObject::RendererObject(RendererObject&& renderObject) :
    _uploaded(renderObject._uploaded),
    _data(renderObject._data),
    _renderer(renderObject._renderer)
{
    renderObject._data = nullptr;
    renderObject._renderer = nullptr;
}

RendererObject::~RendererObject()
{
    if (_uploaded)
    {
        delete _data;
    }
}

bool RendererObject::isUploaded() const
{
    return _uploaded;
}

RendererObject& RendererObject::operator=(const RendererObject& rendererObject)
{
    rendererObject;

    _uploaded = false;
    _data = nullptr;
    _renderer = nullptr;

    return *this;
}

RendererObject& RendererObject::operator=(RendererObject&& rendererObject)
{
    _uploaded = rendererObject._uploaded;
    _data = rendererObject._data;
    _renderer = rendererObject._renderer;

    rendererObject._uploaded = nullptr;
    rendererObject._data = nullptr;
    rendererObject._renderer = nullptr;

    return *this;
}

Renderer& RendererObject::renderer() const
{
    if (!_renderer)
    {
        throw Error("Attempt to get the renderer of a renderer object which is not uploaded");
    }
    return *_renderer;
}