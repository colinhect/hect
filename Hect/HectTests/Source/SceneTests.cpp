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
#include <Hect.h>
using namespace hect;

#include <catch.hpp>

class Name :
    public Component<Name>
{
public:
    Name()
    {
    }

    Name(const std::string& value) :
        value(value)
    {
    }

    void encode(ObjectEncoder& encoder) const
    {
        encoder.encodeString("value", value);
    }

    void decode(ObjectDecoder& decoder, AssetCache& assetCache)
    {
        assetCache;

        if (decoder.hasMember("value"))
        {
            value = decoder.decodeString("value");
        }
    }

    std::string value;
};

HECT_COMPONENT(Name);

TEST_CASE("Scene_CreateAndDestroyEntities")
{
    Scene scene;
    REQUIRE(scene.entityCount() == 0);

    // Create a
    EntityId a = scene.createEntity();
    REQUIRE(scene.entityExists(a));
    REQUIRE(!scene.entityExists(a + 1));
    REQUIRE(!scene.entityExists(a - 1));
    REQUIRE(scene.entityCount() == 1);

    // Create b
    EntityId b = scene.createEntity();
    REQUIRE(scene.entityExists(a));
    REQUIRE(scene.entityExists(b));
    REQUIRE(!scene.entityExists(b + 1));
    REQUIRE(scene.entityCount() == 2);

    // Create c
    EntityId c = scene.createEntity();
    REQUIRE(scene.entityExists(a));
    REQUIRE(scene.entityExists(b));
    REQUIRE(scene.entityExists(c));
    REQUIRE(scene.entityCount() == 3);

    // Destroy a
    REQUIRE(scene.destroyEntity(a));
    REQUIRE(!scene.destroyEntity(a));
    REQUIRE(!scene.entityExists(a));
    REQUIRE(scene.entityExists(b));
    REQUIRE(scene.entityExists(c));
    REQUIRE(scene.entityCount() == 2);

    // Destroy b
    REQUIRE(scene.destroyEntity(b));
    REQUIRE(!scene.destroyEntity(b));
    REQUIRE(!scene.entityExists(a));
    REQUIRE(!scene.entityExists(b));
    REQUIRE(scene.entityExists(c));
    REQUIRE(scene.entityCount() == 1);

    // Destroy c
    REQUIRE(scene.destroyEntity(c));
    REQUIRE(!scene.destroyEntity(c));
    REQUIRE(!scene.entityExists(a));
    REQUIRE(!scene.entityExists(b));
    REQUIRE(!scene.entityExists(c));

    REQUIRE(scene.entityCount() == 0);
}

TEST_CASE("Scene_AddRemoveComponents")
{
    Scene scene;

    EntityId a = scene.createEntity();
    scene.addEntityComponent(a, Name("a"));

    SECTION("With more than one entity")
    {
        EntityId b = scene.createEntity();
        scene.addEntityComponent(b, Name("b"));

        REQUIRE(scene.entityHasComponent<Name>(b));

        Name& name = scene.entityComponent<Name>(b);
        REQUIRE(name.value == "b");
        REQUIRE(name.entityId() == b);

        SECTION("Remove component")
        {
            REQUIRE(scene.removeEntityComponent<Name>(b));
            REQUIRE(!scene.removeEntityComponent<Name>(b));
            REQUIRE_THROWS_AS(scene.entityComponent<Name>(b), Error);
        }
    }

    REQUIRE(scene.entityHasComponent<Name>(a));

    Name& name = scene.entityComponent<Name>(a);
    REQUIRE(name.value == "a");
    REQUIRE(name.entityId() == a);

    SECTION("Remove component")
    {
        REQUIRE(scene.removeEntityComponent<Name>(a));
        REQUIRE(!scene.removeEntityComponent<Name>(a));
        REQUIRE_THROWS_AS(scene.entityComponent<Name>(a), Error);
    }
}

TEST_CASE("Scene_AddComponentFromBase")
{
    Scene scene;

    EntityId a = scene.createEntity();
    Name name("a");
    const ComponentBase& base = name;
    scene.addEntityComponent(a, base);

    REQUIRE(scene.entityHasComponent<Name>(a));

    REQUIRE(scene.entityComponent<Name>(a).value == "a");
}

TEST_CASE("Scene_CloneEntity")
{
    Scene scene;
    REQUIRE(scene.entityCount() == 0);

    EntityId a = scene.createEntity();
    REQUIRE(scene.entityCount() == 1);

    scene.addEntityComponent(a, Name("a"));

    EntityId b = scene.cloneEntity(a);
    REQUIRE(scene.entityCount() == 2);

    REQUIRE(scene.entityHasComponent<Name>(a));
    REQUIRE(scene.entityHasComponent<Name>(b));

    Name* nameA = &scene.entityComponent<Name>(a);
    Name* nameB = &scene.entityComponent<Name>(b);

    REQUIRE(nameA != nameB);
    REQUIRE(nameA->value == nameB->value);
}

TEST_CASE("Scene_Decode")
{
    Scene scene;
    REQUIRE(scene.entityCount() == 0);

    JsonValue jsonValue;
    jsonValue.decodeFromJson("{ \"entities\" : [ { \"components\" : [ { \"type\" : \"Name\", \"value\" : \"a\" } ] } ] }");

    scene.decodeFromJsonValue(jsonValue);
}


TEST_CASE("Scene_IterateComponents")
{
    Scene scene;

    EntityId a = scene.createEntity();
    scene.addEntityComponent(a, Name("a"));

    EntityId b = scene.createEntity();
    scene.addEntityComponent(b, Name("b"));

    std::vector<std::string> foundNames;
    for (Name& name : scene.components<Name>())
    {
        foundNames.push_back(name.value);
    }

    REQUIRE(foundNames.size() == 2);
    REQUIRE(foundNames[0] == "a");
    REQUIRE(foundNames[1] == "b");
}
