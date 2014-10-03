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
#include <Hect/IO/Encoder.h>
#include <Hect/IO/Decoder.h>
#include <Hect/IO/JsonEncoder.h>
#include <Hect/IO/JsonDecoder.h>
#include <Hect/IO/BinaryEncoder.h>
#include <Hect/IO/BinaryDecoder.h>
#include <Hect/IO/MemoryWriteStream.h>
#include <Hect/IO/MemoryReadStream.h>
#include <Hect/Runtime/Engine.h>
using namespace hect;

#include <functional>

extern Engine* engine;

template <typename T>
void testEncoding(const Path& assetPath)
{
    REQUIRE(engine);
    auto handle = engine->assetCache().getHandle<T>(assetPath);
    REQUIRE(handle);

    T& asset = *handle;

    // JSON
    {
        JsonValue jsonValue;
        {
            JsonEncoder encoder;
            encoder << asset;
            jsonValue = *encoder.jsonValues().begin();

            std::string json = jsonValue.encodeToJson();
            int i = 0;
        }
        {
            T decodedAsset;

            JsonDecoder decoder{ jsonValue, engine->assetCache() };
            decoder >> decodedAsset;

            REQUIRE(asset == decodedAsset);
        }
    }

    // Binary
    {
        std::vector<uint8_t> data;
        {
            MemoryWriteStream stream{ data };
            BinaryEncoder encoder{ stream };
            encoder << asset;
        }
        {
            T decodedAsset;

            MemoryReadStream stream{ data };
            BinaryDecoder decoder{ stream, engine->assetCache() };
            decoder >> decodedAsset;

            REQUIRE(asset == decodedAsset);
        }
    }
}