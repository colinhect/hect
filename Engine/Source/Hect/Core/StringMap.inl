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
#include <cassert>

namespace hect
{

template <typename T>
StringMap<T>::Iterator::Iterator()
{
}

template <typename T>
StringMap<T>::Iterator::Iterator(T* value) :
    _value(value)
{
    assert(value);
}

template <typename T>
T& StringMap<T>::Iterator::operator*() const
{
    assert(_value);
    return *_value;
}

template <typename T>
T* StringMap<T>::Iterator::operator->() const
{
    assert(_value);
    return _value;
}

template <typename T>
bool StringMap<T>::Iterator::operator==(const Iterator& other) const
{
    return _value == other._value;
}

template <typename T>
bool StringMap<T>::Iterator::operator!=(const Iterator& other) const
{
    return _value != other._value;
}

template <typename T>
StringMap<T>::Iterator::operator bool() const
{
    return _value != nullptr;
}

template <typename T>
StringMap<T>::ConstIterator::ConstIterator()
{
}

template <typename T>
StringMap<T>::ConstIterator::ConstIterator(T* value) :
    _value(value)
{
    assert(value);
}

template <typename T>
const T& StringMap<T>::ConstIterator::operator*() const
{
    assert(_value);
    return *_value;
}

template <typename T>
const T* StringMap<T>::ConstIterator::operator->() const
{
    assert(_value);
    return _value;
}

template <typename T>
bool StringMap<T>::ConstIterator::operator==(const ConstIterator& other) const
{
    return _value == other._value;
}

template <typename T>
bool StringMap<T>::ConstIterator::operator!=(const ConstIterator& other) const
{
    return _value != other._value;
}

template <typename T>
StringMap<T>::ConstIterator::operator bool() const
{
    return _value != nullptr;
}

template <typename T>
void StringMap<T>::set(const std::string& key, const T& value)
{
    _map[key] = value;
}

template <typename T>
typename StringMap<T>::Iterator StringMap<T>::get(const std::string& key)
{
    auto it = _map.find(key);
    if (it != _map.end())
    {
        return typename StringMap<T>::Iterator(&it->second);
    }
    else
    {
        return typename StringMap<T>::Iterator();
    }
}

template <typename T>
typename StringMap<T>::ConstIterator StringMap<T>::get(const std::string& key) const
{
    auto it = _map.find(key);
    if (it != _map.end())
    {
        return typename StringMap<T>::ConstIterator(&it->second);
    }
    else
    {
        return typename StringMap<T>::ConstIterator();
    }
}

template <typename T>
typename StringMap<T>::Iterator StringMap<T>::get(const char* key)
{
    T* value = getHashed(key);
    if (value)
    {
        return typename StringMap<T>::Iterator(value);
    }
    else
    {
        return typename StringMap<T>::Iterator();
    }
}

template <typename T>
typename StringMap<T>::ConstIterator StringMap<T>::get(const char* key) const
{
    T* value = getHashed(key);
    if (value)
    {
        return typename StringMap<T>::ConstIterator(value);
    }
    else
    {
        return typename StringMap<T>::ConstIterator();
    }
}

template <typename T>
size_t StringMap<T>::size() const
{
    return _map.size();
}

template <typename T>
T* StringMap<T>::getHashed(const char* key) const
{
    if (_hashMap.size() > 1024)
    {
        // This function is likely being used from dynamic memory locations,
        // avoid indefinitely leaking by clearing the hashed values
        _hashMap.clear();
    }

    T* value = nullptr;
    auto it = _hashMap.find(key);
    if (it != _hashMap.end())
    {
        value = it->second;
    }
    else
    {
        // The value was not hashed for this specific string, so attempt
        // to hash it if it exists
        auto valueIt = _map.find(key);
        if (valueIt != _map.end())
        {
            value = &valueIt->second;
            _hashMap[key] = value;
        }
    }

    return value;
}

}
