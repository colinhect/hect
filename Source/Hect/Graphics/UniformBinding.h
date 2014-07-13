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
/// A binding from a shader uniform variable to a built-in value in the
/// rendering pipeline.
enum UniformBinding
{
    ///
    /// No binding.
    UniformBinding_None,

    ///
    /// Bound to the size of the active render target.
    UniformBinding_RenderTargetSize,

    ///
    /// Bound to the world-space position of the active camera.
    UniformBinding_CameraPosition,

    ///
    /// Bound to the world-space front direction of the active camera.
    UniformBinding_CameraFront,

    ///
    /// Bound to the world-space up direction of the active camera.
    UniformBinding_CameraUp,

    ///
    /// Bound to the view matrix.
    UniformBinding_ViewMatrix,

    ///
    /// Bound to the projection matrix.
    UniformBinding_ProjectionMatrix,

    ///
    /// Bound to the product of the view and projection matrices.
    UniformBinding_ViewProjectionMatrix,

    ///
    /// Bound to the model matrix.
    UniformBinding_ModelMatrix,

    ///
    /// Bound to the product of the model and the view matrix.
    UniformBinding_ModelViewMatrix,

    ///
    /// Bound to the product of the model, view, and projection matrices.
    UniformBinding_ModelViewProjectionMatrix
};

}