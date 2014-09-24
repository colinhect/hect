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
#include "ShaderEncoder.h"

using namespace hect;

void ShaderEncoder::encode(const Shader& shader, Encoder& encoder)
{
    /* TODO: Re-enable encoding
    // Modules
    {
        ArrayEncoder modulesEncoder = encoder.encodeArray("modules");
        for (const AssetHandle<ShaderModule>& module : shader.modules())
        {
            modulesEncoder.encodeString(module.path().toString());
        }
    }

    // Uniforms
    {
        ArrayEncoder uniformsEncoder = encoder.encodeArray("uniforms");
        for (const Uniform& uniform : shader.uniforms())
        {
            Encoder uniformEncoder = uniformsEncoder.encodeObject();
            uniform.encode(uniformEncoder);
        }
    }
    */
}

void ShaderEncoder::decode(Shader& shader, ObjectDecoder& decoder, AssetCache& assetCache)
{
    // Modules
    if (decoder.hasMember("modules"))
    {
        ArrayDecoder modulesDecoder = decoder.decodeArray("modules");
        while (modulesDecoder.hasMoreElements())
        {
            Path modulePath = modulesDecoder.decodeString();
            AssetHandle<ShaderModule> module = assetCache.getHandle<ShaderModule>(modulePath);
            shader.addModule(module);
        }
    }

    // Uniforms
    if (decoder.hasMember("uniforms"))
    {
        ArrayDecoder uniformsDecoder = decoder.decodeArray("uniforms");
        while (uniformsDecoder.hasMoreElements())
        {
            ObjectDecoder uniformDecoder = uniformsDecoder.decodeObject();
            Uniform uniform;
            uniform.decode(uniformDecoder, assetCache);
            shader.addUniform(uniform);
        }
    }
}