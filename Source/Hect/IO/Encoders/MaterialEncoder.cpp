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
#include "MaterialEncoder.h"

#include "Hect/IO/Encoders/MaterialEncoder.h"
#include "Hect/Reflection/Enum.h"

using namespace hect;

void MaterialEncoder::encode(const Material& material, ObjectEncoder& encoder)
{
    // Techniques
    ArrayEncoder techniquesEncoder = encoder.encodeArray("techniques");
    for (const Technique& technique : material.techniques())
    {
        // Passes
        ArrayEncoder passesEncoder = techniquesEncoder.encodeArray();
        for (const Pass& pass : technique.passes())
        {
            ObjectEncoder passEncoder = passesEncoder.encodeObject();

            // Shader
            if (pass.shader())
            {
                passEncoder.encodeString("shader", pass.shader().path().toString());
            }
            else
            {
                throw Error("Pass has no shader");
            }

            // Uniform values
            {
                // Issue #26
            }

            // Textures
            {
                ArrayEncoder texturesEncoder = passEncoder.encodeArray("textures");
                for (const AssetHandle<Texture>& texture : pass.textures())
                {
                    texturesEncoder.encodeString(texture.path().toString());
                }
            }

            // Render mode
            {
                ObjectEncoder renderModeEncoder = passEncoder.encodeObject("renderMode");

                // Bulld a list of all states
                size_t stateCount = 4;
                RenderState::Enum states[] =
                {
                    RenderState::Blend,
                    RenderState::DepthTest,
                    RenderState::DepthWrite,
                    RenderState::CullFace
                };

                // Build a list of enabled/disabled states
                std::vector<RenderState::Enum> enabledStates;
                std::vector<RenderState::Enum> disabledStates;
                for (size_t i = 0; i < stateCount; ++i)
                {
                    if (pass.renderMode().isStateEnabled(states[i]))
                    {
                        enabledStates.push_back(states[i]);
                    }
                    else
                    {
                        disabledStates.push_back(states[i]);
                    }
                }

                // Enabled states
                {
                    ArrayEncoder statesEncoder = renderModeEncoder.encodeArray("enabledStates");

                    for (const RenderState::Enum& state : enabledStates)
                    {
                        statesEncoder.encodeEnum(state);
                    }
                }

                // Disabled states
                {
                    ArrayEncoder statesEncoder = renderModeEncoder.encodeArray("disabledStates");

                    for (const RenderState::Enum& state : disabledStates)
                    {
                        statesEncoder.encodeEnum(state);
                    }
                }

                // Blend factors
                {
                    ArrayEncoder blendFactorsEncoder = passEncoder.encodeArray("blendFactors");

                    blendFactorsEncoder.encodeEnum(pass.renderMode().sourceBlendFactor());
                    blendFactorsEncoder.encodeEnum(pass.renderMode().destBlendFactor());
                }
            }
        }
    }
}

void MaterialEncoder::decode(Material& material, ObjectDecoder& decoder, AssetCache& assetCache)
{
    Technique::Array techniques;

    // Techniques
    ArrayDecoder techniquesDecoder = decoder.decodeArray("techniques");
    while (techniquesDecoder.hasMoreElements())
    {
        Pass::Array passes;

        // Passes
        ArrayDecoder passesDecoder = techniquesDecoder.decodeArray();
        while (passesDecoder.hasMoreElements())
        {
            ObjectDecoder passDecoder = passesDecoder.decodeObject();

            RenderMode renderMode;
            AssetHandle<Texture>::Array textures;
            AssetHandle<Shader> shader;
            PassUniformValue::Array uniformValues;

            // Shader
            Path path = passDecoder.decodeString("shader");
            shader = assetCache.getHandle<Shader>(path);

            // Uniform values
            if (passDecoder.hasMember("uniformValues"))
            {
                // Issue #26
            }

            // Textures
            if (passDecoder.hasMember("textures"))
            {
                ArrayDecoder texturesDecoder = passDecoder.decodeArray("textures");
                while (texturesDecoder.hasMoreElements())
                {
                    Path path = texturesDecoder.decodeString();
                    AssetHandle<Texture> texture = assetCache.getHandle<Texture>(path);
                    textures.push_back(texture);
                }
            }

            // Render mode
            if (passDecoder.hasMember("renderMode"))
            {
                ObjectDecoder renderModeDecoder = passDecoder.decodeObject("renderMode");

                // Enabled states
                if (renderModeDecoder.hasMember("enabledStates"))
                {
                    ArrayDecoder statesDecoder = renderModeDecoder.decodeArray("enabledStates");
                    while (statesDecoder.hasMoreElements())
                    {

                        renderMode.enableState(statesDecoder.decodeEnum<RenderState::Enum>());
                    }
                }

                // Disabled states
                if (renderModeDecoder.hasMember("disabledStates"))
                {
                    ArrayDecoder statesDecoder = renderModeDecoder.decodeArray("disabledStates");
                    while (statesDecoder.hasMoreElements())
                    {
                        renderMode.disableState(statesDecoder.decodeEnum<RenderState::Enum>());
                    }
                }

                // Blend factors
                if (renderModeDecoder.hasMember("blendFactors"))
                {
                    ArrayDecoder blendFactorsDecoder = renderModeDecoder.decodeArray("blendFactors");
                    auto sourceFactor = blendFactorsDecoder.decodeEnum<BlendFactor::Enum>();
                    auto destFactor = blendFactorsDecoder.decodeEnum<BlendFactor::Enum>();
                    renderMode.setBlendFactors(sourceFactor, destFactor);
                }
            }

            passes.push_back(Pass(renderMode, textures, shader, uniformValues));
        }

        material.addTechnique(passes);
    }
}