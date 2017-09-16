///////////////////////////////////////////////////////////////////////////////
// This source file is part of Hect.
//
// Copyright (c) 2016 Colin Hill
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
#include <Hect.h>
using namespace hect;

#include <catch.hpp>

// Loads the asset at the given path and verifies that re-encoding and decoding
// the asset results in equivalence for both binary and data encoding
template <typename T>
void test_encoding(const Path& asset_path)
{
    Engine& engine = Engine::instance();
    auto handle = engine.asset_cache().get_handle<T>(asset_path);
    REQUIRE(handle);

    T& asset = *handle;

    // JSON
    DataValue data_value;
    {
        DataValueEncoder encoder;
        encoder << asset;
        data_value = *encoder.data_values().begin();
    }
    {
        T decoded_asset;

        DataValueDecoder decoder(data_value, engine.asset_cache());
        decoder >> decoded_asset;

        INFO(asset_path.as_string());
        REQUIRE(asset == decoded_asset);
    }

    // Binary
    std::vector<uint8_t> data;
    {
        MemoryWriteStream stream(data);
        BinaryEncoder encoder(stream);
        encoder << asset;
    }
    {
        T decoded_asset;

        MemoryReadStream stream(data);
        BinaryDecoder decoder(stream, engine.asset_cache());
        decoder >> decoded_asset;

        INFO(asset_path.as_string());
        REQUIRE(asset == decoded_asset);
    }
}

const std::vector<Path> directory_paths { "Hect/Materials", "Hect/Rendering", "Hect/Scenes", "Hect/Shaders" };

// Loads all files of a certain extension and verifies that re-encoding and
// decoding the asset results in equivalence for both binary and data encoding
template <typename T>
void test_encoding_for_extension(const std::string& extension)
{
    for (const Path& directory_path : directory_paths)
    {
        Engine& engine = Engine::instance();
        FileSystem& file_system = engine.file_system();
        for (const Path& file_path : file_system.files_in_directory(directory_path))
        {
            if (file_path.extension() == extension)
            {
                test_encoding<T>(file_path);
            }
            else if (file_path.extension() == "yaml")
            {
                const std::string& file_path_string = file_path.as_string();
                if (file_path_string.find("." + extension) != std::string::npos)
                {
                    test_encoding<T>(file_path);
                }
            }
        }
    }
}

TEST_CASE("Decode and re-encode all built-in Hect materials", "[Encoding][Material]")
{
    test_encoding_for_extension<Material>("material");
}

TEST_CASE("Decode and re-encode all built-in Hect meshes", "[Encoding][Mesh]")
{
    test_encoding_for_extension<Mesh>("mesh");
}

TEST_CASE("Decode and re-encode all built-in Hect shaders", "[Encoding][Shader]")
{
    test_encoding_for_extension<Shader>("shader");
}

TEST_CASE("Decode and re-encode built-in Hect textures", "[Encoding][Texture2]")
{
    test_encoding_for_extension<Texture2>("texture2");
}
