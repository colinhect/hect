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

#include <cstdint>
#include <string>

namespace hect
{

///
/// An interface for writing streamed data.
class WriteStream
{
public:
    virtual ~WriteStream() { }

    ///
    /// Writes raw bytes to the stream.
    ///
    /// \param bytes A pointer to the bytes to write.
    /// \param byteCount The number of bytes to write.
    virtual void write(const uint8_t* bytes, size_t byteCount) = 0;

    ///
    /// Returns the current byte offset position in the stream.
    virtual size_t position() const = 0;

    ///
    /// Sets the current byte offset position in the stream.
    ///
    /// \param position The new position.
    virtual void seek(size_t position) = 0;
};

///
/// Writes a string with its length prepended as an unsigned 32-bit integer.
///
/// \param stream The stream to write the value to.
/// \param value The value to write.
///
/// \returns The stream.
WriteStream& operator<<(WriteStream& stream, const char* value);

///
/// Writes a string with its length prepended as an unsigned 32-bit integer.
///
/// \param stream The stream to write the value to.
/// \param value The value to write.
///
/// \returns The stream.
WriteStream& operator<<(WriteStream& stream, const std::string& value);

///
/// Writes a signed 8-bit integer.
///
/// \param stream The stream to write the value to.
/// \param value The value to write.
///
/// \returns The stream.
WriteStream& operator<<(WriteStream& stream, int8_t value);

///
/// Writes an unsigned 8-bit integer.
///
/// \param stream The stream to write the value to.
/// \param value The value to write.
///
/// \returns The stream.
WriteStream& operator<<(WriteStream& stream, uint8_t value);

///
/// Writes a signed 16-bit integer.
///
/// \param stream The stream to write the value to.
/// \param value The value to write.
///
/// \returns The stream.
WriteStream& operator<<(WriteStream& stream, int16_t value);

///
/// Writes an unsigned 16-bit integer.
///
/// \param stream The stream to write the value to.
/// \param value The value to write.
///
/// \returns The stream.
WriteStream& operator<<(WriteStream& stream, uint16_t value);

///
/// Writes a signed 32-bit integer.
///
/// \param stream The stream to write the value to.
/// \param value The value to write.
///
/// \returns The stream.
WriteStream& operator<<(WriteStream& stream, int32_t value);

///
/// Writes an unsigned 32-bit integer.
///
/// \param stream The stream to write the value to.
/// \param value The value to write.
///
/// \returns The stream.
WriteStream& operator<<(WriteStream& stream, uint32_t value);

///
/// Writes a signed 64-bit integer.
///
/// \param stream The stream to write the value to.
/// \param value The value to write.
///
/// \returns The stream.
WriteStream& operator<<(WriteStream& stream, int64_t value);

///
/// Writes an unsigned 64-bit integer.
///
/// \param stream The stream to write the value to.
/// \param value The value to write.
///
/// \returns The stream.
WriteStream& operator<<(WriteStream& stream, uint64_t value);

///
/// Writes a 32-bit floating point number.
///
/// \param stream The stream to write the value to.
/// \param value The value to write.
///
/// \returns The stream.
WriteStream& operator<<(WriteStream& stream, float value);

///
/// Writes a 64-bit floating point number.
///
/// \param stream The stream to write the value to.
/// \param value The value to write.
///
/// \returns The stream.
WriteStream& operator<<(WriteStream& stream, double value);

///
/// Writes a boolean as a single byte.
///
/// \param stream The stream to write the value to.
/// \param value The value to write.
///
/// \returns The stream.
WriteStream& operator<<(WriteStream& stream, bool value);

}
