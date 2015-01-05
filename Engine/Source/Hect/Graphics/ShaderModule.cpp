///////////////////////////////////////////////////////////////////////////////
// This source file is part of Hect.
//
// Copyright (c) 2015 Colin Hill
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
#include "ShaderModule.h"

#include "Hect/IO/AssetCache.h"

using namespace hect;

ShaderModule::ShaderModule()
{
}

ShaderModule::ShaderModule(ShaderModuleType type, const std::string& name, const std::string& source) :
    _type(type),
    _name(name),
    _source(source)
{
}

ShaderModuleType ShaderModule::type() const
{
    return _type;
}

const std::string& ShaderModule::name() const
{
    return _name;
}

const std::string& ShaderModule::source() const
{
    return _source;
}

bool ShaderModule::operator==(const ShaderModule& shaderSource) const
{
    return _type == shaderSource._type
           && _name == shaderSource._name
           && _source == shaderSource._source;
}

bool ShaderModule::operator!=(const ShaderModule& shaderSource) const
{
    return !(*this == shaderSource);
}

namespace hect
{

Encoder& operator<<(Encoder& encoder, const ShaderModule& shaderModule)
{
    encoder << beginObject()
            << encodeEnum("type", shaderModule._type)
            << encodeValue("path", shaderModule._name)
            << endObject();

    return encoder;
}

Decoder& operator>>(Decoder& decoder, ShaderModule& shaderModule)
{
    Path path;
    decoder >> beginObject()
            >> decodeEnum("type", shaderModule._type, true)
            >> decodeValue("path", path, true)
            >> endObject();

    // Resolve the path
    AssetCache& assetCache = decoder.assetCache();
    path = assetCache.resolvePath(path);
    shaderModule._name = path.asString();

    // Load the shader source
    auto stream = assetCache.fileSystem().openFileForRead(path);
    shaderModule._source = stream->readAllToString();

    return decoder;
}

}