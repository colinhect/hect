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

#include "Hect/Graphics/BlendFactor.h"
#include "Hect/Graphics/RenderState.h"

namespace hect
{

///
/// Describes a mode that the renderer is in which affects the resulting
/// rendered pixels.
class RenderMode
{
public:

    ///
    /// Constructs a default render mode.
    RenderMode();

    ///
    /// Enables a state.
    ///
    /// \param state The state to enable.
    void enableState(RenderState::Enum state);

    ///
    /// Disables a state.
    ///
    /// \param state The state to disable.
    void disableState(RenderState::Enum state);

    ///
    /// Returns whether a given state is enabled.
    ///
    /// \param state The state.
    bool isStateEnabled(RenderState::Enum state) const;

    ///
    /// Sets the source and destination blend factors.
    ///
    /// \param sourceFactor The source factor.
    /// \param destFactor The destination factor.
    void setBlendFactors(BlendFactor::Enum sourceFactor, BlendFactor::Enum destFactor);

    ///
    /// Returns the source blend factor.
    BlendFactor::Enum sourceBlendFactor() const;

    ///
    /// Returns the destination blend factor.
    BlendFactor::Enum destBlendFactor() const;

    ///
    /// Returns whether the render mode is equivalent to another.
    ///
    /// \param renderMode The other render mode.
    bool operator==(const RenderMode& renderMode) const;

    ///
    /// Returns whether the render mode is different from another.
    ///
    /// \param renderMode The other render mode.
    bool operator!=(const RenderMode& renderMode) const;

private:
    int _stateBits;
    BlendFactor::Enum _sourceFactor;
    BlendFactor::Enum _destFactor;
};

}