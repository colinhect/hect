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
/// An interface for writing hierarchical data.
class DataWriter
{
public:
    virtual ~DataWriter() { }

    ///
    /// Begins an unnamed object.
    ///
    /// \throws Error If the current value is not an array.
    virtual void beginObject() = 0;

    ///
    /// Begins a named object.
    ///
    /// \param name The name of the object.
    ///
    /// \throws Error If the current value is not an object.
    virtual void beginObject(const char* name) = 0;

    ///
    /// Ends the current object.
    ///
    /// \throws Error If the current value is not an object.
    virtual void endObject() = 0;

    ///
    /// Begins a named array.
    ///
    /// \param name The name of the array.
    ///
    /// \throws Error If the current value is not an object.
    virtual void beginArray(const char* name) = 0;

    ///
    /// Ends the current array.
    ///
    /// \throws Error If the current value is not an array.
    virtual void endArray() = 0;

    ///
    /// Writes an unnamed double.
    ///
    /// \param value The value to write.
    ///
    /// \throws Error If the current value is not an array.
    virtual void writeDouble(double value) = 0;

    ///
    /// Writes a named double.
    ///
    /// \param name The member name of the value to write.
    /// \param value The value to write.
    ///
    /// \throws Error If the current value is not an object.
    virtual void writeDouble(const char* name, double value) = 0;

    ///
    /// Writes an unnamed string.
    ///
    /// \param value The value to write.
    ///
    /// \throws Error If the current value is not an array.
    virtual void writeString(const std::string& value) = 0;

    ///
    /// Writes a named string.
    ///
    /// \param name The member name of the value to write.
    /// \param value The value to write.
    ///
    /// \throws Error If the current value is not an object.
    virtual void writeString(const char* name, const std::string& value) = 0;

    ///
    /// Writes an unnamed 2-dimensional vector.
    ///
    /// \param value The value to write.
    ///
    /// \throws Error If the current value is not an array.
    virtual void writeVector2(const Vector2<>& value) = 0;

    ///
    /// Writes an unnamed 2-dimensional vector.
    ///
    /// \param name The member name of the value to write.
    /// \param value The value to write.
    ///
    /// \throws Error If the current value is not an object.
    virtual void writeVector2(const char* name, const Vector2<>& value) = 0;

    ///
    /// Writes an unnamed 3-dimensional vector.
    ///
    /// \param value The value to write.
    ///
    /// \throws Error If the current value is not an array.
    virtual void writeVector3(const Vector3<>& value) = 0;

    ///
    /// Writes an unnamed 3-dimensional vector.
    ///
    /// \param name The member name of the value to write.
    /// \param value The value to write.
    ///
    /// \throws Error If the current value is not an object.
    virtual void writeVector3(const char* name, const Vector3<>& value) = 0;

    ///
    /// Writes an unnamed 4-dimensional vector.
    ///
    /// \param value The value to write.
    ///
    /// \throws Error If the current value is not an array.
    virtual void writeVector4(const Vector4<>& value) = 0;

    ///
    /// Writes an unnamed 4-dimensional vector.
    ///
    /// \param name The member name of the value to write.
    /// \param value The value to write.
    ///
    /// \throws Error If the current value is not an object.
    virtual void writeVector4(const char* name, const Vector4<>& value) = 0;
};

///
/// An implementation of DataWriter for writing directly to a data value.
class DataValueWriter :
    public DataWriter
{
public:

    ///
    /// Constructs the data value writer.
    DataValueWriter();

    ///
    /// Returns the current value.
    DataValue currentDataValue() const;

    void beginObject();
    void beginObject(const char* name);
    void endObject();
    void beginArray(const char* name);
    void endArray();
    void writeDouble(double value);
    void writeDouble(const char* name, double value);
    void writeString(const std::string& value);
    void writeString(const char* name, const std::string& value);
    void writeVector2(const Vector2<>& value);
    void writeVector2(const char* name, const Vector2<>& value);
    void writeVector3(const Vector3<>& value);
    void writeVector3(const char* name, const Vector3<>& value);
    void writeVector4(const Vector4<>& value);
    void writeVector4(const char* name, const Vector4<>& value);

private:
    void _write(const DataValue& value);
    void _write(const char* name, const DataValue& value);

    std::stack<std::string> _nameStack;
    std::stack<DataValue> _valueStack;
};

///
/// An implementation of DataWriter for writing to a binary stream.
///
/// \remarks This implementation does not throw the exceptions described
/// in the DataWriter documentation for performance reasons.
class BinaryDataWriter :
    public DataWriter
{
public:

    ///
    /// Constructs a binary data writer given the stream to write to.
    ///
    /// \param stream The stream to write to.
    BinaryDataWriter(WriteStream& stream);

    void beginObject();
    void beginObject(const char* name);
    void endObject();
    void beginArray(const char* name);
    void endArray();
    void writeDouble(double value);
    void writeDouble(const char* name, double value);
    void writeString(const std::string& value);
    void writeString(const char* name, const std::string& value);
    void writeVector2(const Vector2<>& value);
    void writeVector2(const char* name, const Vector2<>& value);
    void writeVector3(const Vector3<>& value);
    void writeVector3(const char* name, const Vector3<>& value);
    void writeVector4(const Vector4<>& value);
    void writeVector4(const char* name, const Vector4<>& value);

private:
    size_t _elementCountPosition;
    unsigned _elementCount;
    WriteStream* _stream;
};

}