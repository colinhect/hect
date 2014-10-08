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
#include "Hect/IO/AssetLoader.h"
#include "Hect/IO/AssetCache.h"
#include "Hect/IO/Data.h"
#include "Hect/IO/JsonValue.h"

using namespace hect;

void AssetLoader<Data>::load(Data& data, const Path& assetPath, AssetCache& assetCache)
{
    assetCache;

    ReadStream stream = FileSystem::openFileForRead(assetPath);
    uint8_t firstCharacter;
    stream >> firstCharacter;
    if (firstCharacter == '{')
    {
        stream.seek(0);
        data.jsonValue.decodeFromJson(stream);
    }
    else
    {
        stream.seek(0);
        while (!stream.endOfStream())
        {
            uint8_t byte;
            stream >> byte;
            data.binaryData.push_back(byte);
        }
    }
}
