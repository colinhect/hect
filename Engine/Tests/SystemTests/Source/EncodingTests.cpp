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
#include <Hect/Graphics/Material.h>
#include <Hect/Graphics/Mesh.h>
#include <Hect/Graphics/Texture.h>
#include <Hect/IO/BinaryDecoder.h>
#include <Hect/IO/BinaryEncoder.h>
#include <Hect/IO/DataValueDecoder.h>
#include <Hect/IO/DataValueEncoder.h>
#include <Hect/IO/MemoryReadStream.h>
#include <Hect/IO/MemoryWriteStream.h>
#include <Hect/Runtime/Engine.h>

using namespace hect;

#include <catch.hpp>

extern Engine* engine;

// Loads the asset at the given path and verifies that re-encoding and decoding
// the asset results in equivalence for both binary and data encoding
template <typename T>
void testEncoding(const Path& assetPath)
{
    REQUIRE(engine);
    auto handle = engine->assetCache().getHandle<T>(assetPath);
    REQUIRE(handle);

    T& asset = *handle;

    // JSON
    {
        DataValue dataValue;
        {
            DataValueEncoder encoder;
            encoder << asset;
            dataValue = *encoder.dataValues().begin();
        }
        {
            T decodedAsset;

            DataValueDecoder decoder(dataValue, engine->assetCache());
            decoder >> decodedAsset;

            REQUIRE(asset == decodedAsset);
        }
    }

    // Binary
    {
        std::vector<uint8_t> data;
        {
            MemoryWriteStream stream(data);
            BinaryEncoder encoder(stream);
            encoder << asset;
        }
        {
            T decodedAsset;

            MemoryReadStream stream(data);
            BinaryDecoder decoder(stream, engine->assetCache());
            decoder >> decodedAsset;

            REQUIRE(asset == decodedAsset);
        }
    }
}

// Loads all files of a certain extension and verifies that re-encoding and
// decoding the asset results in equivalence for both binary and data encoding
template <typename T>
void testEncodingForExtension(const std::string& extension)
{
    for (auto& filePath : engine->fileSystem().filesInDirectory("Hect"))
    {
        if (filePath.extension() == extension)
        {
            testEncoding<T>(filePath);
        }
    }
}

TEST_CASE("Decode and re-encode all built-in Hect materials", "[Encoding]")
{
    testEncodingForExtension<Material>("material");
}

TEST_CASE("Decode and re-encode all built-in Hect meshes", "[Encoding]")
{
    testEncodingForExtension<Mesh>("mesh");
}

TEST_CASE("Decode and re-encode built-in Hect textures", "[Encoding]")
{
    testEncodingForExtension<Texture>("texture");
}

