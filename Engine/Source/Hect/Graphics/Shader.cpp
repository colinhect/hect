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
#include "Shader.h"

using namespace hect;

Shader::Shader()
{
}

Shader::Shader(Name name) :
    Asset(name)
{
}

RenderStage Shader::render_stage() const
{
    return _render_stage;
}

void Shader::set_render_stage(RenderStage render_stage)
{
    _render_stage = render_stage;
}

void Shader::add_module(const ShaderModule& module)
{
    // Destroy the shader if it is uploaded so that any changes the added
    // module contributes to the shader when it is compiled will take effect
    if (is_uploaded())
    {
        renderer().destroy_shader(*this);
    }

    _modules.push_back(module);
}

const Shader::ModuleSequence Shader::modules() const
{
    return _modules;
}

UniformIndex Shader::add_uniform(const Uniform& uniform)
{
    // Destroy the shader if it is uploaded so that the uniforms location can
    // be established when the shader is uploaded
    if (is_uploaded())
    {
        renderer().destroy_shader(*this);
    }

    _uniforms.push_back(uniform);
    resolve_uniforms();

    return _uniforms.size() - 1;
}

Shader::UniformSequence Shader::uniforms()
{
    return _uniforms;
}

const Shader::UniformSequence Shader::uniforms() const
{
    return _uniforms;
}

Uniform& Shader::uniform(UniformIndex index)
{
    if (index >= _uniforms.size())
    {
        throw InvalidOperation(format("Shader '%s' does not have a uniform at index %i", name().data(), index));
    }
    return _uniforms[index];
}

const Uniform& Shader::uniform(UniformIndex index) const
{
    return const_cast<Shader*>(this)->uniform(index);
}

Uniform& Shader::uniform(Name name)
{
    auto it = _uniform_indices.find(name);
    if (it == _uniform_indices.end())
    {
        throw InvalidOperation(format("Shader '%s' does not have a uniform named '%s'", this->name().data(), name.data()));
    }
    return _uniforms[it->second];
}

const Uniform& Shader::uniform(Name name) const
{
    return const_cast<Shader*>(this)->uniform(name);
}

Uniform& Shader::uniform(const char* name)
{
    auto it = _uniform_indices.find(name);
    if (it == _uniform_indices.end())
    {
        throw InvalidOperation(format("Shader '%s' does not have a uniform named '%s'", this->name().data(), name));
    }
    return _uniforms[it->second];
}

const Uniform& Shader::uniform(const char* name) const
{
    return const_cast<Shader*>(this)->uniform(name);
}

bool Shader::has_uniform(Name name) const
{
    auto it = _uniform_indices.find(name);
    return it != _uniform_indices.end();
}

const BlendMode& Shader::blend_mode() const
{
    return _blend_mode;
}

void Shader::set_blend_mode(const BlendMode& blend_mode)
{
    _blend_mode = blend_mode;
}

bool Shader::is_depth_tested() const
{
    return _depth_tested;
}

void Shader::set_depth_tested(bool depth_tested)
{
    _depth_tested = depth_tested;
}

int Shader::priority() const
{
    return _priority;
}

void Shader::set_priority(int priority)
{
    _priority = priority;
}

bool Shader::operator==(const Shader& shader) const
{
    // Module count
    if (_modules.size() != shader._modules.size())
    {
        return false;
    }

    // Modules
    for (size_t i = 0; i < _modules.size(); ++i)
    {
        if (_modules[i] != shader._modules[i])
        {
            return false;
        }
    }

    // Uniform count
    if (_uniforms.size() != shader._uniforms.size())
    {
        return false;
    }

    // Uniforms
    for (size_t i = 0; i < _uniforms.size(); ++i)
    {
        if (_uniforms[i] != shader._uniforms[i])
        {
            return false;
        }
    }

    // Blend mode
    if (_blend_mode != shader._blend_mode)
    {
        return false;
    }

    // Depth tested
    if (_depth_tested != shader._depth_tested)
    {
        return false;
    }

    // Priority
    if (_priority != shader._priority)
    {
        return false;
    }

    return true;
}

bool Shader::operator!=(const Shader& shader) const
{
    return !(*this == shader);
}

void Shader::encode(Encoder& encoder) const
{
    encoder << encode_enum("render_stage", _render_stage)
            << encode_vector("modules", _modules)
            << encode_vector("uniforms", _uniforms)
            << encode_value("depth_tested", _depth_tested)
            << encode_value("blend_mode", _blend_mode)
            << encode_value("priority", _priority);
}

void Shader::decode(Decoder& decoder)
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
                throw DecodeError(format("Failed to load base shader '%s': %s", base_path.as_string().data(), exception.what()));
            }
        }
    }

    decoder >> decode_enum("render_stage", _render_stage)
            >> decode_vector("modules", _modules, true)
            >> decode_vector("uniforms", _uniforms)
            >> decode_value("depth_tested", _depth_tested)
            >> decode_value("blend_mode", _blend_mode)
            >> decode_value("priority", _priority);

    resolve_uniforms();
}

void Shader::resolve_uniforms()
{
    size_t texture_index = 0;
    for (UniformIndex index = 0; index < _uniforms.size(); ++index)
    {
        Uniform& uniform = _uniforms[index];
        uniform._index = index;

        _uniform_indices[uniform._name] = index;
        if (uniform.type() == UniformType::Texture2 ||
                uniform.type() == UniformType::Texture3 ||
                uniform.type() == UniformType::TextureCube)
        {
            uniform._texture_index = texture_index;
            ++texture_index;
        }
    }
}
