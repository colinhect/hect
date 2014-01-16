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
#pragma once

namespace hect
{

///
/// An Internet Protocol address.
class IpAddress
{
public:

    ///
    /// Returns the local IP address.
    static IpAddress localAddress();

    ///
    /// Returns the public IP address.
    static IpAddress publicAddress();

    ///
    /// Constructs an IP address from a string.
    ///
    /// \param address A string representation of the address.
    IpAddress(const std::string& address);

    ///
    /// Constructs an IP address from a raw 32-bit value.
    ///
    /// \param address The 32-bit value of the address.
    IpAddress(uint32_t address);

    ///
    /// Returns whether the address is a valid IP address.
    bool isValid() const;

    ///
    /// Returns a string representation of the address.
    std::string toString() const;

    ///
    /// Casts the address to a 32-bit unsigned integer.
    operator uint32_t() const;

private:
    uint32_t _address;
};

}