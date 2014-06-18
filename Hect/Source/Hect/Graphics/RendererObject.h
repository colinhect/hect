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
#pragma once

#include "Hect/Core/Export.h"

namespace hect
{

class Renderer;

///
/// Data which provides a handle to the API-specific data.
struct RendererObjectData
{
    virtual ~RendererObjectData() { }
};

///
/// An object which can be uploaded to the GPU.
///
/// \warning Classes which inherit from RenderObject must implement a copy
/// constructor, move constructor, destructor, assignment operator, and
/// move assignment operator all which destroy the object from the renderer
/// if needed.
class RendererObject
{
    friend class Renderer;
public:
    RendererObject();
    RendererObject(const RendererObject& renderObject);
    RendererObject(RendererObject&& renderObject);
    virtual ~RendererObject();

    ///
    /// Returns whether the object is uploaded to the GPU.
    bool isUploaded() const;

    RendererObject& operator=(const RendererObject& rendererObject);
    RendererObject& operator=(RendererObject&& rendererObject);

protected:

    ///
    /// Returns the renderer that the object is uploaded to.
    ///
    /// \throws Error If the object is not uploaded.
    Renderer& renderer() const;

private:
    mutable bool _uploaded;
    mutable RendererObjectData* _data;
    mutable Renderer* _renderer;
};

}