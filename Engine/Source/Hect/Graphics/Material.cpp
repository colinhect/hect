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
#include "Material.h"

using namespace hect;

Material::Material()
{
}

Material::Material(Name name) :
    Asset(name)
{
}

const Shader::Handle& Material::shader() const
{
    return _shader;
}

void Material::set_shader(const Shader::Handle& shader)
{
    clear_uniform_values();
    _shader = shader;
}

Material::UniformValueSequence Material::uniform_values()
{
    return _uniform_values;
}

const Material::UniformValueSequence Material::uniform_values() const
{
    return _uniform_values;
}

void Material::set_uniform_value(Name name, const UniformValue& value)
{
    if (!_shader)
    {
        throw InvalidOperation("Material does not have a shader set");
    }

    // Ensure the value is the same type as the uniform and is not bound
    const Uniform& uniform = _shader->uniform(name);
    if (uniform.type() != value.type())
    {
        const char* uniform_name = uniform.name().data();
        const char* shader_name = _shader->name().data();
        const char* expected_type = Enum::to_string(uniform.type()).data();
        const char* actual_type = Enum::to_string(value.type()).data();
        throw InvalidOperation(format("Uniform '%s' in shader '%s' is of type '%s' and cannot be assigned a value of type '%s'", uniform_name, shader_name, expected_type, actual_type));
    }
    else if (uniform.binding() != UniformBinding::None)
    {
        const char* uniform_name = uniform.name().data();
        const char* shader_name = _shader->name().data();
        throw InvalidOperation(format("Uniform '%s' in shader '%s' is bound and cannot be assigned a value", uniform_name, shader_name));
    }

    UniformIndex index = uniform.index();

    // Resize the uniform value vector if needed
    if (index >= _uniform_values.size())
    {
        _uniform_values.resize(index + 1);
    }

    // Set the value
    _uniform_values[index] = value;
}

void Material::clear_uniform_values()
{
    _uniform_values.clear();
}

CullMode Material::cull_mode() const
{
    return _cull_mode;
}

void Material::set_cull_mode(CullMode cull_mode)
{
    _cull_mode = cull_mode;
}

bool Material::operator==(const Material& material) const
{
    // Shader
    if (_shader != material._shader)
    {
        return false;
    }

    // Uniform value count
    if (_uniform_values.size() != material._uniform_values.size())
    {
        return false;
    }

    // Uniform values
    for (size_t i = 0; i < _uniform_values.size(); ++i)
    {
        if (_uniform_values[i] != material._uniform_values[i])
        {
            return false;
        }
    }

    return true;
}

bool Material::operator!=(const Material& material) const
{
    return !(*this == material);
}


void Material::encode(Encoder& encoder) const
{
    // Shader
    encoder << encode_value("shader", _shader);
    encoder << encode_enum("cull_mode", _cull_mode);

    // Uniform values
    encoder << begin_array("uniform_values");
    for (UniformIndex index = 0; index < _uniform_values.size(); ++index)
    {
        const UniformValue& value = _uniform_values[index];
        if (value)
        {
            if (encoder.is_binary_stream())
            {
                encoder << begin_object()
                        << encode_value(static_cast<uint8_t>(index));

                value.encode(encoder);

                encoder << end_object();
            }
            else
            {
                const Uniform& uniform = _shader->uniform(index);
                encoder << begin_object()
                        << encode_value("name", uniform.name());

                value.encode(encoder);

                encoder << end_object();
            }
        }
    }
    encoder << end_array();
}

void Material::decode(Decoder& decoder)
{
    // Base
    if (!decoder.is_binary_stream())
    {
        if (decoder.select_member("base"))
        {
            Path base_path;
            decoder >> decode_value(base_path);

            try
            {
                AssetDecoder base_decoder(decoder.asset_cache(), base_path);
                base_decoder >> decode_value(*this);
            }
            catch (const Exception& exception)
            {
                throw DecodeError(format("Failed to load base material '%s': %s", base_path.as_string().data(), exception.what()));
            }
        }
    }

    // Shader
    decoder >> decode_value("shader", _shader);
    decoder >> decode_enum("cull_mode", _cull_mode);

    // Uniform values
    if (decoder.select_member("uniform_values"))
    {
        decoder >> begin_array();
        while (decoder.has_more_elements())
        {
            try
            {
                Name name;
                UniformValue value;

                if (decoder.is_binary_stream())
                {
                    uint8_t index = 0;

                    decoder >> begin_object()
                            >> decode_value(index);

                    value.decode(decoder);

                    decoder>> end_object();

                    const Uniform& uniform = _shader->uniform(index);
                    name = uniform.name();
                }
                else
                {
                    decoder >> begin_object()
                            >> decode_value("name", name, true);

                    value.decode(decoder);

                    decoder >> end_object();
                }

                set_uniform_value(name, value);
            }
            catch (const InvalidOperation& exception)
            {
                throw DecodeError(exception.what());
            }
        }
        decoder >> end_array();
    }
}
