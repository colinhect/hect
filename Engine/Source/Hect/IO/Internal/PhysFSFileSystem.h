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

#include "Hect/Core/Export.h"
#include "Hect/IO/FileSystem.h"

namespace hect
{

class HECT_EXPORT PhysFSFileSystem :
    public FileSystem
{
public:
    PhysFSFileSystem(int argc, char* const argv[]);
    ~PhysFSFileSystem();

    Path baseDirectory() override;
    Path workingDirectory() override;
    Path userDirectory() override;
    Path applicationDataDirectory() override;
    void setWriteDirectory(const Path& path) override;
    void mountArchive(const Path& path, const Path& mountPoint = Path()) override;
    std::unique_ptr<ReadStream> openFileForRead(const Path& path) override;
    std::unique_ptr<WriteStream> openFileForWrite(const Path& path) override;
    void createDirectory(const Path& path) override;
    std::vector<Path> filesInDirectory(const Path& path) override;
    void remove(const Path& path) override;
    bool exists(const Path& path) override;
    TimeStamp lastModified(const Path& path) override;

private:
    Path convertPath(const char* rawPath);

    static bool _initialized;
};

}