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
#include "RenderState.h"

using namespace hect;

void RenderState::enable(RenderStateFlag flag)
{
    _flags |= flag;
}

void RenderState::disable(RenderStateFlag flag)
{
    _flags &= ~flag;
}

bool RenderState::isEnabled(RenderStateFlag flag) const
{
    return (_flags & flag) == flag;
}

void RenderState::setBlendFactors(BlendFactor source, BlendFactor destination)
{
    _sourceFactor = source;
    _destinationFactor = destination;
}

BlendFactor RenderState::sourceBlendFactor() const
{
    return _sourceFactor;
}

BlendFactor RenderState::destinationBlendFactor() const
{
    return _destinationFactor;
}

bool RenderState::operator==(const RenderState& renderState) const
{
    return _flags == renderState._flags
           && _sourceFactor == renderState._sourceFactor
           && _destinationFactor == renderState._destinationFactor;
}

bool RenderState::operator!=(const RenderState& renderState) const
{
    return !(*this == renderState);
}

namespace hect
{

Encoder& operator<<(Encoder& encoder, const RenderState& renderState)
{
    encoder << beginObject();

    if (encoder.isBinaryStream())
    {
        WriteStream& stream = encoder.binaryStream();
        stream << renderState._flags;
    }
    else
    {
        // Bulld a list of all states
        size_t stateCount = 4;
        RenderStateFlag states[] =
        {
            RenderStateFlag_Blend,
            RenderStateFlag_DepthTest,
            RenderStateFlag_DepthWrite,
            RenderStateFlag_CullFace
        };

        // Build a list of enabled/disabled states
        std::vector<RenderStateFlag> enabledFlags;
        std::vector<RenderStateFlag> disabledFlags;
        for (size_t i = 0; i < stateCount; ++i)
        {
            if (renderState.isEnabled(states[i]))
            {
                enabledFlags.push_back(states[i]);
            }
            else
            {
                disabledFlags.push_back(states[i]);
            }
        }

        // Enabled states
        encoder << beginArray("enabledFlags");
        for (const RenderStateFlag& flag : enabledFlags)
        {
            encoder << encodeEnum(flag);
        }
        encoder << endArray();

        // Disabled states
        encoder << beginArray("disabledFlags");
        for (const RenderStateFlag& flag : disabledFlags)
        {
            encoder << encodeEnum(flag);
        }
        encoder << endArray();
    }

    // Blend factors
    encoder << beginObject("blendFactors")
            << encodeEnum("source", renderState._sourceFactor)
            << encodeEnum("destination", renderState._destinationFactor)
            << endObject();

    encoder << endObject();
    return encoder;
}

Decoder& operator>>(Decoder& decoder, RenderState& renderState)
{
    decoder >> beginObject();

    if (decoder.isBinaryStream())
    {
        ReadStream& stream = decoder.binaryStream();
        stream >> renderState._flags;
    }
    else
    {
        // Enabled states
        if (decoder.selectMember("enabledFlags"))
        {
            decoder >> beginArray();
            while (decoder.hasMoreElements())
            {
                RenderStateFlag flag;
                decoder >> decodeEnum(flag);
                renderState.enable(flag);
            }
            decoder >> endArray();
        }

        // Disabled states
        if (decoder.selectMember("disabledFlags"))
        {
            decoder >> beginArray();
            while (decoder.hasMoreElements())
            {
                RenderStateFlag flag;
                decoder >> decodeEnum(flag);
                renderState.disable(flag);
            }
            decoder >> endArray();
        }
    }

    // Blend factors
    decoder >> beginObject("blendFactors")
            >> decodeEnum("source", renderState._sourceFactor)
            >> decodeEnum("destination", renderState._destinationFactor)
            >> endObject();

    decoder >> endObject();

    return decoder;
}

}
