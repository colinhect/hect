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

namespace hect
{

///
/// A state that can either be enabled or disabled in a mode.
enum RenderState
{
    ///
    /// Blends source and destination pixels using a formula.
    Blend = 1,

    ///
    /// Tests whether a pixel should be occluded based on the depth buffer.
    DepthTest = 2,

    ///
    // Writes a pixel's depth to the depth buffer.
    DepthWrite = 4,

    ///
    /// Culls faces pointing away from the camera.
    CullFace = 8
};

///
/// A formula used to compute blending from either the source or
/// destination.
enum class BlendFactor
{
    ///
    /// Blend forumla: 0 0 0 0
    Zero,

    ///
    /// Blend forumla: 1 1 1 1
    One,

    ///
    /// Blend forumla: R s0 k R G s0 k G B s0 k B A s0 k A
    SourceColor,

    ///
    /// Blend forumla: 1 1 1 1 - R s0 k R G s0 k G B s0 k B A s0 k A
    OneMinusSourceColor,

    ///
    /// Blend forumla: R d k R G d k G B d k B A d k A
    DestColor,

    ///
    /// Blend forumla: 1 1 1 1 - R d k R G d k G B d k B A d k A
    OneMinusDestColor,

    ///
    /// Blend forumla: A s0 k A A s0 k A A s0 k A A s0 k A
    SourceAlpha,

    ///
    /// Blend forumla: 1 1 1 1 - A s0 k A A s0 k A A s0 k A A s0 k A
    OneMinusSourceAlpha,

    ///
    /// Blend forumla: A d k A A d k A A d k A A d k A
    DestAlpha,

    ///
    /// Blend forumla: 1 1 1 1 - A d k A A d k A A d k A A d k A
    OneMinusDestAlpha
};

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
    void enableState(RenderState state);

    ///
    /// Disables a state.
    ///
    /// \param state The state to disable.
    void disableState(RenderState state);

    ///
    /// Returns whether a given state is enabled.
    ///
    /// \param state The state.
    bool isStateEnabled(RenderState state) const;

    ///
    /// Sets the source and destination blend factors.
    ///
    /// \param sourceFactor The source factor.
    /// \param destFactor The destination factor.
    void setBlendFactors(BlendFactor sourceFactor, BlendFactor destFactor);

    ///
    /// Returns the source blend factor.
    BlendFactor sourceBlendFactor() const;

    ///
    /// Returns the destination blend factor.
    BlendFactor destBlendFactor() const;

private:
    int _stateBits;
    BlendFactor _sourceFactor;
    BlendFactor _destFactor;
};

}