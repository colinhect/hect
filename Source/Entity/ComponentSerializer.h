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

#include "Core/Configuration.h"
#include "Asset/AssetCache.h"
#include "Entity/Component.h"
#include "IO/DataReader.h"
#include "IO/DataWriter.h"

namespace hect
{

///
/// Base component serializer.
class BaseComponentSerializer
{
public:

    ///
    /// A shared reference to a base component serializer.
    typedef std::shared_ptr<BaseComponentSerializer> Ref;

    virtual ~BaseComponentSerializer() { }

    ///
    /// Serializes a component using a data writer.
    ///
    /// \param component The component.
    /// \param writer The data writer.
    virtual void save(const BaseComponent* component, DataWriter& writer) const = 0;

    ///
    /// Deserializes a component using a data reader.
    ///
    /// \param component The component.
    /// \param reader The data reader.
    /// \param assetCache The asset cache to load referenced assets from.
    virtual void load(BaseComponent* component, DataReader& reader, AssetCache& assetCache) const = 0;
};

///
/// Provides functionality for serializing/deserializing an entity component.
template <typename T>
class ComponentSerializer :
    public BaseComponentSerializer
{
public:

    ///
    /// Serializes a component using a data writer.
    ///
    /// \param component The component.
    /// \param writer The data writer.
    virtual void save(const T& component, DataWriter& writer) const;

    ///
    /// Deserializes a component using a data reader.
    ///
    /// \param component The component.
    /// \param reader The data reader.
    /// \param assetCache The asset cache to load referenced assets from.
    virtual void load(T& component, DataReader& reader, AssetCache& assetCache) const;

    void save(const BaseComponent* component, DataWriter& writer) const;
    void load(BaseComponent* component, DataReader& reader, AssetCache& assetCache) const;
};

}

#include "ComponentSerializer.inl"
