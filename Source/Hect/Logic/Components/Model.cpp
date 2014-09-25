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
#include "Model.h"

using namespace hect;

ModelSurface::ModelSurface(const AssetHandle<Mesh>& mesh, const AssetHandle<Material>& material) :
    mesh(mesh),
    material(material)
{
}

void Model::encode(Encoder& encoder) const
{
    encoder << beginArray("surfaces");
    for (const ModelSurface& surface : surfaces)
    {
        encoder << beginObject()
                << encodeValue("mesh", surface.mesh)
                << encodeValue("material", surface.material)
                << endObject();
    }
    encoder << endArray();
}

void Model::decode(ObjectDecoder& decoder, AssetCache& assetCache)
{
    if (decoder.hasMember("surfaces"))
    {
        ArrayDecoder surfacesDecoder = decoder.decodeArray("surfaces");
        while (surfacesDecoder.hasMoreElements())
        {
            ObjectDecoder surfaceDecoder = surfacesDecoder.decodeObject();
            if (surfaceDecoder.hasMember("mesh") && surfaceDecoder.hasMember("material"))
            {
                Path meshPath = surfaceDecoder.decodeString("mesh");
                Path materialPath = surfaceDecoder.decodeString("material");

                AssetHandle<Mesh> mesh = assetCache.getHandle<Mesh>(meshPath);
                AssetHandle<Material> material = assetCache.getHandle<Material>(materialPath);

                surfaces.push_back(ModelSurface(mesh, material));
            }
        }
    }
}