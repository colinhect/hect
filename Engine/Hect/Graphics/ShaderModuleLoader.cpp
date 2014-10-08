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
#include "Hect/IO/AssetLoader.h"
#include "Hect/IO/AssetCache.h"
#include "Hect/Graphics/ShaderModule.h"

using namespace hect;

void AssetLoader<ShaderModule>::load(ShaderModule& shaderModule, const Path& assetPath, AssetCache& assetCache)
{
    assetCache;

    // Determine the shader module type by the extension
    ShaderModuleType type;
    std::string extension = assetPath.extension();
    if (extension == "vert")
    {
        type = ShaderModuleType_Vertex;
    }
    else if (extension == "frag")
    {
        type = ShaderModuleType_Pixel;
    }
    else if (extension == "geom")
    {
        type = ShaderModuleType_Geometry;
    }
    else
    {
        throw Error(format("Unknown shader module type '%s'", extension.c_str()));
    }

    // Read the source contents
    ReadStream stream = FileSystem::openFileForRead(assetPath);
    std::string source = stream.readAllToString();

    // Create the shader module
    shaderModule.setName(assetPath.asString());
    shaderModule.setType(type);
    shaderModule.setSource(source);
}
