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
#pragma once

#include "Hect/Core/Export.h"
#include "Hect/Interface/Widget.h"

namespace hect
{

///
/// A check box.
class HECT_EXPORT CheckBox :
    public Widget<CheckBox>
{
public:

    ///
    /// Constructs an empty check box widget.
    ///
    /// \param interfaceSystem The interface system.
    CheckBox(InterfaceSystem& interfaceSystem);

    ///
    /// Returns whether the check box is checked.
    bool isChecked() const;

    ///
    /// Sets whether the check box is checked.
    ///
    /// \param checked Whether the check box is checked.
    void setChecked(bool checked);

    virtual void render(VectorRenderer::Frame& frame, const Rectangle& clipping) override;
    virtual void onPressed() override;

private:
    bool _checked { false };
};

}
