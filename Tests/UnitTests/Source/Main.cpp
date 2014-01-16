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
#include <UnitTest++.h>

#include <Hect.h>

#ifdef HECT_WINDOWS
#ifdef HECT_DEBUG
#include <vld.h>
#endif
#endif

using namespace hect;

const double epsilon = 0.0001;

#include "AngleTests.h"
#include "AnyTests.h"
#include "AssetCacheTests.h"
#include "DataValueJsonFormatTests.h"
#include "DataReaderWriterTests.h"
#include "DataValueTests.h"
#include "EntityTests.h"
#include "EventTests.h"
#include "FileSystemTests.h"
#include "FormatTests.h"
#include "FrustumTests.h"
#include "MaterialDataFormatTests.h"
#include "Matrix4Tests.h"
#include "MemoryStreamTests.h"
#include "MeshDataFormatTests.h"
#include "MeshTests.h"
#include "MeshWriterTests.h"
#include "MeshReaderTests.h"
#include "NetworkTests.h"
#include "PathTests.h"
#include "PlaneTests.h"
#include "QuaternionTests.h"
#include "SceneTests.h"
#include "TaskPoolTests.h"
#include "TimeSpanTests.h"
#include "Vector2Tests.h"
#include "Vector3Tests.h"
#include "Vector4Tests.h"
#include "VertexAttributeTests.h"
#include "VertexLayoutTests.h"

int main()
{
    int failed = UnitTest::RunAllTests();
    if (failed)
    {
        Window::showFatalError(format("%d failures.", failed));
    }

    return failed;
}