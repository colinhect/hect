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
#include "ShaderModule.h"

#include "Hect/IO/AssetCache.h"

using namespace hect;

ShaderModule::ShaderModule()
{
}

ShaderModule::ShaderModule(ShaderModuleType type, Name name, const std::string& source) :
    _type(type),
    _name(name),
    _source(source)
{
}

ShaderModuleType ShaderModule::type() const
{
    return _type;
}

Name ShaderModule::name() const
{
    return _name;
}

const std::string& ShaderModule::source() const
{
    return _source;
}

bool ShaderModule::operator==(const ShaderModule& shader_source) const
{
    return _type == shader_source._type
           && _name == shader_source._name
           && _source == shader_source._source;
}

bool ShaderModule::operator!=(const ShaderModule& shader_source) const
{
    return !(*this == shader_source);
}

void ShaderModule::encode(Encoder& encoder) const
{
    encoder << encode_enum("type", _type)
            << encode_value("path", _name);
}

void ShaderModule::decode(Decoder& decoder)
{
    Path path;
    decoder >> decode_enum("type", _type, true)
            >> decode_value("path", path, true);

    // Resolve the path
    AssetCache& asset_cache = decoder.asset_cache();
    path = asset_cache.resolve_path(path);
    _name = path.as_string();

    // Load the shader source
    auto stream = asset_cache.file_system().open_file_for_read(path);
    _source = stream->read_all_to_string();
}
