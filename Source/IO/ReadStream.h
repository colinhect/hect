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
/// An interface for reading streamed data.
class ReadStream
{
public:

    ///
    /// Reads raw bytes from the stream.
    ///
    /// \param bytes A pointer to the bytes to read to.
    /// \param byteCount The number of bytes to read.
    virtual void readBytes(uint8_t* bytes, size_t byteCount) = 0;

    ///
    /// Reads a string with its length prepended as an unsigned 32-bit
    /// integer.
    std::string readString();

    ///
    /// Reads entire contents of the remainder of the stream to a string.
    std::string readAllToString();

    ///
    /// Reads a signed 8-bit integer.
    int8_t readByte();

    ///
    /// Reads an unsigned 16-bit integer.
    uint8_t readUnsignedByte();

    ///
    /// Reads a signed 16-bit integer.
    int16_t readShort();

    ///
    /// Reads an unsigned 16-bit integer.
    uint16_t readUnsignedShort();

    ///
    /// Reads a signed 32-bit integer.
    int32_t readInt();

    ///
    /// Reads an unsigned 32-bit integer.
    uint32_t readUnsignedInt();

    ///
    /// Reads a signed 64-bit integer.
    int64_t readLong();

    ///
    /// Reads an unsigned 64-bit integer.
    uint64_t readUnsignedLong();

    ///
    /// Reads a 32-bit float.
    float readFloat();

    ///
    /// Reads a 64-bit float.
    double readDouble();

    ///
    /// Reads a 2-dimensional vector.
    Vector2<> readVector2();

    ///
    /// Reads a 3-dimensional vector.
    Vector3<> readVector3();

    ///
    /// Reads a 4-dimensional vector.
    Vector4<> readVector4();

    ///
    /// Reads a quaternion.
    Quaternion<> readQuaternion();

    ///
    /// Returns whether the end of the stream has been reached.
    virtual bool endOfStream() const = 0;

    ///
    /// Returns the length of the stream in bytes.
    virtual size_t length() const = 0;

    ///
    /// Returns the current byte offset position in the stream.
    virtual size_t position() const = 0;

    ///
    /// Sets the current byte offset position in the stream.
    ///
    /// \param position The new position.
    virtual void seek(size_t position) = 0;

};

}