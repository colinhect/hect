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

#include <unordered_map>
#include <map>

namespace hect
{

///
/// A string to value map that provides an accelerated look-up from static
/// strings.
template <typename T>
class StringMap
{
public:

    ///
    /// An iterator to a string map entry.
    class Iterator
    {
    public:
        Iterator();
        Iterator(T* value);

        ///
        /// Dereferences the iterator to a reference to the value.
        ///
        /// \returns A reference to the value.
        T& operator*() const;

        ///
        /// Dereferences the iterator to a pointer to the value.
        ///
        /// \returns A reference to the value.
        T* operator->() const;

        ///
        /// Returns whether the iterator is equivalent to another.
        ///
        /// \param other The other iterator.
        bool operator==(const Iterator& other) const;

        ///
        /// Returns whether the iterator is different from another.
        ///
        /// \param other The other iterator.
        bool operator!=(const Iterator& other) const;

        ///
        /// Returns whether the iterator is valid.
        operator bool() const;

    private:
        T* _value { nullptr };
    };

    ///
    /// An constant iterator to a string map entry.
    class ConstIterator
    {
    public:
        ConstIterator();
        ConstIterator(T* value);

        ///
        /// Dereferences the iterator to a reference to the value.
        ///
        /// \returns A reference to the value.
        const T& operator*() const;

        ///
        /// Dereferences the iterator to a pointer to the value.
        ///
        /// \returns A reference to the value.
        const T* operator->() const;

        ///
        /// Returns whether the iterator is equivalent to another.
        ///
        /// \param other The other iterator.
        bool operator==(const ConstIterator& other) const;

        ///
        /// Returns whether the iterator is different from another.
        ///
        /// \param other The other iterator.
        bool operator!=(const ConstIterator& other) const;

        ///
        /// Returns whether the iterator is valid.
        operator bool() const;

    private:
        const T* _value
        {
            nullptr
        };
    };

    ///
    /// Sets the value for a key.
    ///
    /// \param key The key to set the value for.
    /// \param value The value.
    void set(const std::string& key, const T& value);

    ///
    /// Gets the value for a key.
    ///
    /// \param key The key to get the value for.
    ///
    /// \returns An iterator to the found value (invalid if no value exists
    /// for the given key).
    Iterator get(const std::string& key);

    ///
    /// Gets the value for a key.
    ///
    /// \param key The key to get the value for.
    ///
    /// \returns An iterator to the found value (invalid if no value exists
    /// for the given key).
    ConstIterator get(const std::string& key) const;

    ///
    /// Gets the value for a key.
    ///
    /// \param key The key to get the value for.
    ///
    /// \returns An iterator to the found value (invalid if no value exists
    /// for the given key).
    Iterator get(const char* key);

    ///
    /// Gets the value for a key.
    ///
    /// \param key The key to get the value for.
    ///
    /// \returns An iterator to the found value (invalid if no value exists
    /// for the given key).
    ConstIterator get(const char* key) const;

    ///
    /// Returns the number of entries in the map.
    size_t size() const;

private:
    T* getHashed(const char* key) const;

    mutable std::map<std::string, T> _map;
    mutable std::unordered_map<const char*, T*> _hashMap;
};

}

#include "StringMap.inl"
