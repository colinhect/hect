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
/// A step in a material technique.
///
/// \remarks Contains the render mode, textures, shader, and uniform values
/// used when rendering geometry for this step in the technique.
class Pass
{
public:

    ///
    /// An array of passes.
    typedef std::vector<Pass> Array;

    ///
    /// Constructs a pass.
    ///
    /// \param renderMode The render mode that pass will bind.
    /// \param textures The textures that the pass will bind.
    /// \param shader The shader that the pass will bind.
    /// \param uniformValues The values for the uniforms in the shader.
    Pass(const RenderMode& renderMode, const AssetHandle<Texture>::Array& textures, const AssetHandle<Shader>& shader, const PassUniformValue::Array& uniformValues);

    ///
    /// Prepares a renderer to begin using this pass.
    ///
    /// \param renderer The renderer to prepare.
    void prepare(Renderer& renderer) const;

    ///
    /// Returns the render mode.
    RenderMode& renderMode();

    ///
    /// Returns the render mode.
    const RenderMode& renderMode() const;

    ///
    /// Returns the textures.
    const AssetHandle<Texture>::Array& textures() const;

    ///
    /// Returns the shader.
    const AssetHandle<Shader>& shader() const;

    ///
    /// Returns the uniform values.
    const PassUniformValue::Array& uniformValues() const;

private:

    // Resolves which uniforms the uniform values apply to for fast
    // binding
    void _resolvePassUniformValues();

    RenderMode _renderMode;
    AssetHandle<Texture>::Array _textures;
    AssetHandle<Shader> _shader;

    PassUniformValue::Array _uniformValues;
    std::map<const Uniform*, UniformValue> _resolvedUniformValues;
};

}