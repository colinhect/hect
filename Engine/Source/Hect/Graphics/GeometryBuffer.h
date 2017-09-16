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
    FrameBuffer& frame_buffer();

    ///
    /// Returns the back frame buffer.
    FrameBuffer& back_frame_buffer();

    ///
    /// Returns the depth buffer.
    Texture2& depth_buffer();

    ///
    /// Returns the diffuse buffer.
    Texture2& diffuse_buffer();

    ///
    /// Returns the material buffer.
    Texture2& material_buffer();

    ///
    /// Returns the position buffer.
    Texture2& position_buffer();

    ///
    /// Returns the normal buffer.
    Texture2& normal_buffer();

    ///
    /// Swaps the back buffers.
    void swap_back_buffers();

    ///
    /// Returns the current back buffer.
    Texture2& back_buffer();

    ///
    /// Returns the last back buffer.
    Texture2& last_back_buffer();

private:
    Texture2 _depth_buffer;
    Texture2 _diffuse_buffer;
    Texture2 _material_buffer;
    Texture2 _position_buffer;
    Texture2 _normal_buffer;
    std::array<Texture2, 2> _back_buffers;

    size_t _back_buffer_index { 0 };

    FrameBuffer _frame_buffer;
    std::array<FrameBuffer, 2> _back_frame_buffers;
};

}
