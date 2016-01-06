///////////////////////////////////////////////////////////////////////////////
// This source file is part of Hect.
//
// Copyright (c) 2016 Colin Hill
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

#include "Hect/Core/Sequence.h"
#include "Hect/Core/Export.h"
#include "Hect/Graphics/FrameBuffer.h"
#include "Hect/Graphics/Renderer.h"
#include "Hect/Graphics/RenderTarget.h"
#include "Hect/Graphics/Texture2.h"
#include "Hect/Graphics/Texture3.h"

#include <array>

namespace hect
{

///
/// A multi-target geometry buffer for physically-based rendering.
class HECT_EXPORT GeometryBuffer
{
public:

    ///
    /// Constructs a geometry buffer.
    ///
    /// \param width The width.
    /// \param height The height.
    GeometryBuffer(unsigned width, unsigned height);

    ///
    /// Returns the primary frame buffer.
    FrameBuffer& frameBuffer();

    ///
    /// Returns the back frame buffer.
    FrameBuffer& backFrameBuffer();

    ///
    /// Returns the depth buffer.
    Texture2& depthBuffer();

    ///
    /// Returns the diffuse buffer.
    Texture2& diffuseBuffer();

    ///
    /// Returns the material buffer.
    Texture2& materialBuffer();

    ///
    /// Returns the position buffer.
    Texture2& positionBuffer();

    ///
    /// Returns the normal buffer.
    Texture2& normalBuffer();

    ///
    /// Swaps the back buffers.
    void swapBackBuffers();

    ///
    /// Returns the current back buffer.
    Texture2& backBuffer();

    ///
    /// Returns the last back buffer.
    Texture2& lastBackBuffer();

private:
    Texture2 _depthBuffer;
    Texture2 _diffuseBuffer;
    Texture2 _materialBuffer;
    Texture2 _positionBuffer;
    Texture2 _normalBuffer;
    std::array<Texture2, 2> _backBuffers;

    size_t _backBufferIndex { 0 };

    FrameBuffer _frameBuffer;
    std::array<FrameBuffer, 2> _backFrameBuffers;
};

}
