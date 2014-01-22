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

SUITE(FileSystem)
{
    TEST(CreateAndRemoveDirectories)
    {
        FileSystem fileSystem;
        Path workingDirectory = fileSystem.workingDirectory();
        fileSystem.addDataSource(workingDirectory);
        fileSystem.setWriteDirectory(workingDirectory);

        Path path("Directory");

        CHECK(!fileSystem.exists(path));
        fileSystem.createDirectory(path);
        CHECK(fileSystem.exists(path));
        fileSystem.remove(path);
        CHECK(!fileSystem.exists(path));
    }

    TEST(OpenNonExistingFileForWrite)
    {
        FileSystem fileSystem;
        Path workingDirectory = fileSystem.workingDirectory();
        fileSystem.addDataSource(workingDirectory);
        fileSystem.setWriteDirectory(workingDirectory);

        Path path("File.txt");

        CHECK(!fileSystem.exists(path));
        {
            FileWriteStream stream = fileSystem.openFileForWrite(path);
        }
        CHECK(fileSystem.exists(path));
        fileSystem.remove(path);
        CHECK(!fileSystem.exists(path));
    }

    TEST(OpenExistingFileForWrite)
    {
        FileSystem fileSystem;
        Path workingDirectory = fileSystem.workingDirectory();
        fileSystem.addDataSource(workingDirectory);
        fileSystem.setWriteDirectory(workingDirectory);

        Path path("File.txt");

        CHECK(!fileSystem.exists(path));
        {
            FileWriteStream stream = fileSystem.openFileForWrite(path);
        }
        CHECK(fileSystem.exists(path));
        {
            FileWriteStream stream = fileSystem.openFileForWrite(path);
        }
        CHECK(fileSystem.exists(path));
        fileSystem.remove(path);
        CHECK(!fileSystem.exists(path));
    }

    TEST(OpenExistingFileForRead)
    {
        FileSystem fileSystem;
        Path workingDirectory = fileSystem.workingDirectory();
        fileSystem.addDataSource(workingDirectory);
        fileSystem.setWriteDirectory(workingDirectory);

        Path path("File.txt");

        CHECK(!fileSystem.exists(path));
        {
            FileWriteStream stream = fileSystem.openFileForWrite(path);
        }
        CHECK(fileSystem.exists(path));
        {
            FileReadStream stream = fileSystem.openFileForRead(path);
        }

        fileSystem.remove(path);
        CHECK(!fileSystem.exists(path));
    }

    TEST(OpenNonExistingFileForRead)
    {
        FileSystem fileSystem;
        Path workingDirectory = fileSystem.workingDirectory();
        fileSystem.addDataSource(workingDirectory);
        fileSystem.setWriteDirectory(workingDirectory);

        Path path("DoesNotExist.txt");

        bool errorOccurred = false;

        try
        {
            FileReadStream stream = fileSystem.openFileForRead(path);
        }
        catch (Error&)
        {
            errorOccurred = true;
        }

        CHECK(errorOccurred);
    }
}