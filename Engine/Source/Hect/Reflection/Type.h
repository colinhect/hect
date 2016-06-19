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

#include <functional>
#include <map>
#include <memory>
#include <typeindex>

#include "Hect/Core/Export.h"
#include "Hect/Core/Name.h"

namespace hect
{

class Decoder;
class Encoder;
class Enum;

///
/// The kind of a reflected type.
enum class Kind
{
    ///
    /// Not a type.
    None,

    ///
    /// A namespace.
    Namespace,

    ///
    /// A class or struct.
    Class,

    ///
    /// An enum.
    Enum
};

///
/// A reflected type.
class HECT_EXPORT Type
{
public:

    ///
    /// A function for encoding a reflected type.
    typedef std::function<void(const void*, Encoder& encoder)> EncodeFunction;

    ///
    /// A function for decoding a reflected type.
    typedef std::function<void(void*, Decoder& decoder)> DecodeFunction;

    ///
    /// Registers a reflected type.
    ///
    /// \param kind The kind of the type.
    /// \param name The name of the type.
    ///
    /// \returns A reference to the newly created type.
    ///
    /// \throws InvalidOperation If the type already exists.
    template <typename T>
    static Type& create(Kind kind, const Name& name);

    ///
    /// Returns a reflected type.
    ///
    /// \throws InvalidOperation If the type is not registered.
    template <typename T>
    static const Type& get();

    ///
    /// Returns the reflected type of a value.
    ///
    /// \param value The value to get the type of.
    ///
    /// \throws InvalidOperation If the type is not registered.
    template <typename T>
    static const Type& of(T& value);

    Type();

    ///
    /// Returns the kind.
    Kind kind() const;

    ///
    /// Returns the name.
    const Name& name() const;

    ///
    /// Returns the type as an enum.
    ///
    /// \throws InvalidOperation If the type is not an enum.
    Enum& asEnum();

    ///
    /// Returns the type as an enum.
    ///
    /// \throws InvalidOperation If the type is not an enum.
    const Enum& asEnum() const;

    ///
    /// Sets the function to use when encoding a value of this type.
    ///
    /// \param function The encode function.
    void setEncodeFunction(EncodeFunction function);

    ///
    /// Encodes a value of this type using the registered encode function.
    ///
    /// \warning Behavior is undefined if the given value is not of the
    /// associated type.
    ///
    /// \param value The value to encode
    /// \param encoder The encoder.
    void encode(const void* value, Encoder& encoder) const;

    ///
    /// Sets the function to use when decoding a value of this type.
    ///
    /// \param function The decode function.
    void setDecodeFunction(DecodeFunction function);

    ///
    /// Decodes a value of this type using the registered decode function.
    ///
    /// \warning Behavior is undefined if the given value is not of the
    /// associated type.
    ///
    /// \param value The value to decode
    /// \param decoder The decoder.
    void decode(void* value, Decoder& decoder) const;

private:
    Type(Kind kind, const std::string& name);

    static const Type& fromTypeInfo(const std::type_info& typeInfo);

    Kind _kind { Kind::None };
    Name _name;
    std::shared_ptr<Enum> _enum;

    EncodeFunction _encodeFunction;
    DecodeFunction _decodeFunction;

    static std::map<std::type_index, Type> _registeredTypes;
};

}

#include "Type.inl"
