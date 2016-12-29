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
namespace hect
{

template <typename T>
Entity::Iterator EntityPool::findFirst(T&& predicate)
{
    for (auto iterator = begin(); iterator != end(); ++iterator)
    {
        if (predicate(*iterator))
        {
            return iterator;
        }
    }
    return end();
}

template <typename T>
Entity::ConstIterator EntityPool::findFirst(T&& predicate) const
{
    for (auto iterator = begin(); iterator != end(); ++iterator)
    {
        if (predicate(*iterator))
        {
            return iterator;
        }
    }
    return end();
}

template <typename T>
std::vector<Entity::Iterator> EntityPool::find(T&& predicate)
{
    std::vector<Entity::Iterator> results;
    for (auto iterator = begin(); iterator != end(); ++iterator)
    {
        if (predicate(*iterator))
        {
            results.push_back(iterator);
        }
    }
    return results;
}

template <typename T>
std::vector<Entity::ConstIterator> EntityPool::find(T&& predicate) const
{
    std::vector<Entity::ConstIterator> results;
    for (auto iterator = begin(); iterator != end(); ++iterator)
    {
        if (predicate(*iterator))
        {
            results.push_back(iterator);
        }
    }
    return results;
}

}