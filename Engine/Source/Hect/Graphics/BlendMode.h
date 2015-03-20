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
#pragma once

#include "Hect/Core/Export.h"
#include "Hect/Graphics/BlendFactor.h"
#include "Hect/Graphics/BlendFunction.h"
#include "Hect/IO/Encodable.h"

namespace hect
{

///
/// Describes how rendered pixels combine with the pixels already rendered to
/// target.
class HECT_EXPORT BlendMode :
    public Encodable
{
public:

    ///
    /// Constructs a default blend mode.
    BlendMode();

    ///
    /// Constructs a blend mode.
    ///
    /// \param function The blend function.
    /// \param sourceFactor The source blend factor.
    /// \param destinationFactor The source blend factor.
    BlendMode(BlendFunction function, BlendFactor sourceFactor, BlendFactor destinationFactor);

    ///
    /// Returns the blend function.
    BlendFunction function() const;

    ///
    /// Sets the blend function.
    ///
    /// \param function The blend function.
    void setFunction(BlendFunction function);

    ///
    /// Returns the blend factor specifying how rendered pixels contribute
    /// to the final color.
    BlendFactor sourceFactor() const;

    ///
    /// Sets the blend factor specifying how rendered pixels contribute
    /// to the final color.
    ///
    /// \param factor The blend factor.
    void setSourceFactor(BlendFactor factor);

    ///
    /// Returns the blend factor specifying how the pixels in the target
    /// contribute to the final color.
    BlendFactor destinationFactor() const;

    ///
    /// Sets the blend factor specifying how the pixels in the target
    /// contribute to the final color.
    ///
    /// \param factor The blend factor.
    void setDestinationFactor(BlendFactor factor);

    ///
    /// Returns whether the blend mode is equivalent to another.
    ///
    /// \param blendMode The other blend mode.
    bool operator==(const BlendMode& blendMode) const;

    ///
    /// Returns whether the blend mode is different from another.
    ///
    /// \param blendMode The other blend mode.
    bool operator!=(const BlendMode& blendMode) const;

    void encode(Encoder& encoder) const override;
    void decode(Decoder& decoder);

private:
    BlendFunction _function { BlendFunction::Add };
    BlendFactor _sourceFactor { BlendFactor::One };
    BlendFactor _destinationFactor { BlendFactor::Zero };
};

}