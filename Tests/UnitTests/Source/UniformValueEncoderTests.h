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

SUITE(UniformValueEncoder)
{
    TEST_FIXTURE(AssetCacheFixture, Int)
    {
        UniformValue a(123, UniformType::Int);
        testEncodable(a, assetCache);
    }

    TEST_FIXTURE(AssetCacheFixture, Texture)
    {
        UniformValue a(123, UniformType::Texture);
        testEncodable(a, assetCache);
    }

    TEST_FIXTURE(AssetCacheFixture, Real)
    {
        UniformValue a((Real)123);
        testEncodable(a, assetCache);
    }

    TEST_FIXTURE(AssetCacheFixture, Vector2)
    {
        UniformValue a(Vector2(1, 2));
        testEncodable(a, assetCache);
    }

    TEST_FIXTURE(AssetCacheFixture, Vector3)
    {
        UniformValue a(Vector3(1, 2, 3));
        testEncodable(a, assetCache);
    }

    TEST_FIXTURE(AssetCacheFixture, Vector4)
    {
        UniformValue a(Vector4(1, 2, 3, 4));
        testEncodable(a, assetCache);
    }
}