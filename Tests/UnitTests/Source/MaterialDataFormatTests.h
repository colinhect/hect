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

SUITE(MaterialDataFormat)
{
    TEST(NoBase)
    {
        FileSystem fileSystem;
        fileSystem.addDataSource("Data");

        AssetCache assetCache(fileSystem);

        Material& base = assetCache.get<Material>("Base.material");
        Shader& window = assetCache.get<Shader>("Window.shader");

        CHECK_EQUAL(1u, base.techniques().size());
        CHECK_EQUAL(1u, base.techniques()[0].passes().size());

        const Pass& pass = base.techniques()[0].passes()[0];

        CHECK_EQUAL(&window, &*pass.shader());

        const RenderMode& renderMode = pass.renderMode();
        CHECK(renderMode.isStateEnabled(RenderState::Blend));
        CHECK(!renderMode.isStateEnabled(RenderState::DepthTest));
        CHECK(!renderMode.isStateEnabled(RenderState::CullFace));
        CHECK(BlendFactor::Zero == renderMode.sourceBlendFactor());
        CHECK(BlendFactor::OneMinusSourceAlpha == renderMode.destBlendFactor());
    }
}