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

class Asset
{
public:
    Asset() : value(-1) { }
    Asset(int value) : value(value) { }

    int value;
};

namespace hect
{
void AssetLoader<Asset>::load(Asset& asset, const Path& assetPath, AssetCache& assetCache)
{
    asset;
    assetPath;
    assetCache;
}
}

SUITE(AssetHandle)
{
    TEST(ConstructAndDereference)
    {
        Asset* a = new Asset(123);
        AssetHandle<Asset> b(a);

        CHECK_EQUAL(a, &*b);
        CHECK_EQUAL(123, b->value);
    }

    TEST(BoolOperator)
    {
        AssetHandle<Asset> a(new Asset(123));
        AssetHandle<Asset> b;

        CHECK(a);
        CHECK(!b);
    }

    TEST(CopyConstructor)
    {
        AssetHandle<Asset> a(new Asset(123));
        AssetHandle<Asset> b(a);

        CHECK_EQUAL(&*a, &*b);
    }

    TEST(AssignmentOperator)
    {
        AssetHandle<Asset> a(new Asset(123));

        {
            AssetHandle<Asset> b;
            b = a;
            CHECK_EQUAL(&*a, &*b);
        }

        CHECK(a);
    }
}