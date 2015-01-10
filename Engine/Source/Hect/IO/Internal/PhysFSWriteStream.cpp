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
#include "PhysFSWriteStream.h"

#include <cassert>

#include "Hect/Core/Exception.h"
#include "Hect/Core/Format.h"

using namespace hect;

PhysFSWriteStream::PhysFSWriteStream(const Path& path) :
    _path(path)
{
    _handle = PHYSFS_openWrite(path.asString().c_str());
    if (!_handle)
    {
        throw IOError(format("Failed to open file for writing: %s", PHYSFS_getLastError()));
    }
}

PhysFSWriteStream::~PhysFSWriteStream()
{
    if (_handle)
    {
        if (!PHYSFS_close(_handle))
        {
            throw IOError(format("Failed to close file for writing: %s", PHYSFS_getLastError()));
        }
    }
}

void PhysFSWriteStream::write(const uint8_t* bytes, size_t byteCount)
{
    assert(_handle);
    assert(bytes);

    PHYSFS_sint64 result = PHYSFS_write(_handle, bytes, 1, static_cast<PHYSFS_uint32>(byteCount));
    if (result != static_cast<PHYSFS_sint64>(byteCount))
    {
        throw IOError(format("Failed to write to file: %s", PHYSFS_getLastError()));
    }
}

size_t PhysFSWriteStream::position() const
{
    assert(_handle);
    return static_cast<size_t>(PHYSFS_tell(_handle));
}

void PhysFSWriteStream::seek(size_t position)
{
    assert(_handle);
    if (!PHYSFS_seek(_handle, position))
    {
        throw IOError(format("Failed to seek in file: %s", PHYSFS_getLastError()));
    }
}