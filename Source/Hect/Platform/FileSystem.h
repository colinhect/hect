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

#include "Hect/IO/Path.h"
#include "Hect/IO/ReadStream.h"
#include "Hect/IO/WriteStream.h"
#include "Hect/Timing/TimeStamp.h"

namespace hect
{

///
/// Provides read and write access to files.
class FileSystem
{
public:
    static void initialize();
    static void deinitialize();

    ///
    /// Returns the full path to the working directory of the executable.
    static Path workingDirectory();

    ///
    /// Returns the full path to the user's home directory.
    static Path userDirectory();

    ///
    /// Sets the directory where write access is allowed and directed to.
    ///
    /// \param path The path to the directory to direct write access.
    static void setWriteDirectory(const Path& path);

    ///
    /// Mounts a directory or archive.
    ///
    /// \note Any file with the same path as a file in a previously mounted
    /// directory or archive will be overriden by the file in the directory or
    /// archive which was mounted first.
    ///
    /// \param path Path to the directory or archive to mount.
    static void mount(const Path& path);

    ///
    /// Opens a file for reading.
    ///
    /// \param path The path to the file to open for reading.
    ///
    /// \returns A stream for the opened file.
    static ReadStream::Pointer openFileForRead(const Path& path);

    ///
    /// Opens a file for writing.
    ///
    /// \warning The write directory must be set using the
    /// setWriteDirectory() method.
    ///
    /// \param path The path to the file to open for writing relative to
    /// the write directory path.
    ///
    /// \returns A stream for the opened file.
    static WriteStream::Pointer openFileForWrite(const Path& path);

    ///
    /// Creates a directory.
    ///
    /// \note Any missing parent directories are also created if they don't
    /// exist.  If the directory already exists then nothing happens.
    ///
    /// \warning The write directory must first be set using the
    /// setWriteDirectory() method.
    ///
    /// \param path The path to the directory to create relative to the
    /// write directory path.
    static void createDirectory(const Path& path);

    ///
    /// Removes the file or directory at the given path.  If the path does
    /// not exist then nothing happens.
    ///
    /// \warning The write directory mustfirst  be set using the
    /// setWriteDirectory() method.
    ///
    /// \param path The path to the file or directory to remove relative to
    /// the write directory path.
    static void remove(const Path& path);

    ///
    /// Returns whether there is a file or directory at the given path.
    ///
    /// \param path The path to check the existence of.
    static bool exists(const Path& path);

    ///
    /// Returns the time the given file was last modified.
    ///
    /// \note If the last modified time cannot be determined then -1 is
    /// returned.
    static TimeStamp lastModified(const Path& path);

private:
    FileSystem() { }
};

}