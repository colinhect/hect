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

#include <map>
#include <typeindex>

#include "Hect/Reflection/Enum.h"

namespace hect
{

///
/// The kind of a reflected type.
enum Kind
{
    ///
    /// Not a type.
    Kind_None,

    ///
    /// A namespace.
    Kind_Namespace,

    ///
    /// A class or struct.
    Kind_Class,

    ///
    /// An enum.
    Kind_Enum
};

///
/// A reflected type.
class Type
{
public:

    ///
    /// Registers a type collection.
    template <typename T>
    static void registerTypes();

    ///
    /// Creates a reflected type.
    ///
    /// \param kind The kind of the type.
    /// \param name The name of the type.
    ///
    /// \returns A reference to the newly created type.
    ///
    /// \throws Error If the type already exists.
    template <typename T>
    static Type& create(Kind kind, const std::string& name);
    
    ///
    /// Returns a reflected type.
    ///
    /// \throws Error If the type is not registered.
    template <typename T>
    static const Type& get();

    ///
    /// Returns the reflected type of a value.
    ///
    /// \param value The value to get the type of.
    ///
    /// \throws Error If the type is not registered.
    template <typename T>
    static const Type& of(T& value);

    Type();
    
    ///
    /// Returns the kind.
    Kind kind() const;

    ///
    /// Returns the name.
    const std::string& name() const;
    
    ///
    /// Returns the type as an enum.
    ///
    /// \throws Error If the type is not an enum.
    Enum& asEnum();

    ///
    /// Returns the type as an enum.
    ///
    /// \throws Error If the type is not an enum.
    const Enum& asEnum() const;

private:
    Type(Kind kind, const std::string& name);

    static const Type& fromTypeInfo(const std::type_info& typeInfo);

    Kind _kind;
    std::string _name;
    Enum _enum;

    static std::map<std::type_index, Type> _registeredTypes;
};

}

#include "Type.inl"