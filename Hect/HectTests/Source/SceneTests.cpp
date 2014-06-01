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
#include <Hect/Logic/Scene.h>
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

class ComponentPoolListener :
    public Listener<ComponentPoolEvent>
{
public:
    void receiveEvent(const ComponentPoolEvent& event)
    {
        receivedEvents.push_back(event);
    }

    std::vector<ComponentPoolEvent> receivedEvents;
};

TEST_CASE("Scene_CreateAndDestroyEntities")
{
    Scene scene;
    REQUIRE(scene.entityCount() == 0);

    // Create a
    Entity a = scene.createEntity();
    REQUIRE(a);
    REQUIRE(scene.entityCount() == 0);

    a.activate();
    REQUIRE(scene.entityCount() == 1);

    // Create b
    Entity b = scene.createEntity();
    REQUIRE(a);
    REQUIRE(b);
    b.activate();
    REQUIRE(scene.entityCount() == 2);

    // Create c
    Entity c = scene.createEntity();
    REQUIRE(a);
    REQUIRE(b);
    REQUIRE(c);
    c.activate();
    REQUIRE(c);
    REQUIRE(scene.entityCount() == 3);

    // Destroy a
    a.destroy();
    REQUIRE_THROWS_AS(a.destroy(), Error);
    REQUIRE(!a);
    REQUIRE(b);
    REQUIRE(c);
    REQUIRE(scene.entityCount() == 2);

    // Destroy b
    b.destroy();
    REQUIRE_THROWS_AS(b.destroy(), Error);
    REQUIRE(!a);
    REQUIRE(!b);
    REQUIRE(c);
    REQUIRE(scene.entityCount() == 1);

    // Destroy c
    c.destroy();
    REQUIRE_THROWS_AS(c.destroy(), Error);
    REQUIRE(!a);
    REQUIRE(!b);
    REQUIRE(!c);

    REQUIRE(scene.entityCount() == 0);
}

TEST_CASE("Scene_AddRemoveComponents")
{
    Scene scene;
    scene.registerComponent<Name>("Name");

    Entity a = scene.createEntity();
    a.addComponent(Name("a"));
    a.activate();

    SECTION("With more than one entity")
    {
        Entity b = scene.createEntity();
        b.addComponent(Name("b"));
        b.activate();
        
        auto name = b.component<Name>();
        REQUIRE(name);
        REQUIRE(name->value == "b");
        REQUIRE(name->entity() == b);

        SECTION("Remove component")
        {
            REQUIRE(b.removeComponent<Name>());
            REQUIRE(!b.removeComponent<Name>());
            REQUIRE(!b.component<Name>());
        }
    }
    
    auto name = a.component<Name>();
    REQUIRE(name);
    REQUIRE(name->value == "a");
    REQUIRE(name->entity() == a);

    SECTION("Remove component")
    {
        REQUIRE(a.removeComponent<Name>());
        REQUIRE(!a.removeComponent<Name>());
        REQUIRE(!a.component<Name>());
    }
}

TEST_CASE("Scene_CloneEntity")
{
    Scene scene;
    scene.registerComponent<Name>("Name");

    REQUIRE(scene.entityCount() == 0);

    Entity a = scene.createEntity();
    a.addComponent(Name("a"));
    a.activate();
    REQUIRE(scene.entityCount() == 1);

    Entity b = a.clone();
    b.activate();
    REQUIRE(scene.entityCount() == 2);

    Name* nameA = &*a.component<Name>();
    Name* nameB = &*b.component<Name>();

    REQUIRE(nameA != nameB);
    REQUIRE(nameA->value == nameB->value);

    REQUIRE(nameA->entity() == a);
    REQUIRE(nameB->entity() == b);
}

TEST_CASE("Scene_Decode")
{
    Scene scene;
    scene.registerComponent<Name>("Name");

    REQUIRE(scene.entityCount() == 0);

    JsonValue jsonValue;
    jsonValue.decodeFromJson("{ \"entities\" : [ { \"components\" : [ { \"type\" : \"Name\", \"value\" : \"a\" } ] } ] }");

    scene.decodeFromJsonValue(jsonValue);

    REQUIRE(scene.entityCount() == 1);
}

TEST_CASE("Scene_IterateComponents")
{
    Scene scene;
    scene.registerComponent<Name>("Name");

    Entity a = scene.createEntity();
    a.addComponent(Name("a"));
    a.activate();

    Entity b = scene.createEntity();
    b.addComponent(Name("b"));

    Entity c = scene.createEntity();
    c.addComponent(Name("c"));
    c.activate();

    std::vector<std::string> foundNames;
    for (Name& name : scene.components<Name>())
    {
        foundNames.push_back(name.value);
    }

    REQUIRE(foundNames.size() == 2);
    REQUIRE(foundNames[0] == "a");
    REQUIRE(foundNames[1] == "c");
}

TEST_CASE("Scene_IterateComponentsConst")
{
    Scene scene;
    scene.registerComponent<Name>("Name");

    Entity a = scene.createEntity();
    a.addComponent(Name("a"));
    a.activate();

    Entity b = scene.createEntity();
    b.addComponent(Name("b"));

    Entity c = scene.createEntity();
    c.addComponent(Name("c"));
    c.activate();

    std::vector<std::string> foundNames;
    const Scene& sceneConst = scene;
    for (const Name& name : sceneConst.components<Name>())
    {
        foundNames.push_back(name.value);
    }

    REQUIRE(foundNames.size() == 2);
    REQUIRE(foundNames[0] == "a");
    REQUIRE(foundNames[1] == "c");
}

TEST_CASE("Scene_ComponentPoolListeners")
{
    Scene scene;
    scene.registerComponent<Name>("Name");

    ComponentPoolListener listener;
    scene.components<Name>().dispatcher().addListener(listener);

    Entity a = scene.createEntity();
    a.addComponent(Name("a"));
    REQUIRE(listener.receivedEvents.size() == 0);
    a.activate();

    Entity b = scene.createEntity();
    b.addComponent(Name("b"));
    REQUIRE(listener.receivedEvents.size() == 1);
    b.activate();

    REQUIRE(listener.receivedEvents.size() == 2);
    REQUIRE(listener.receivedEvents[0].type == ComponentPoolEventType::Add);
    REQUIRE(listener.receivedEvents[0].entityId == a.id());
    REQUIRE(listener.receivedEvents[1].type == ComponentPoolEventType::Add);
    REQUIRE(listener.receivedEvents[1].entityId == b.id());
    listener.receivedEvents.clear();

    a.destroy();

    REQUIRE(listener.receivedEvents.size() == 1);
    REQUIRE(listener.receivedEvents[0].type == ComponentPoolEventType::Remove);
    REQUIRE(listener.receivedEvents[0].entityId == a.id());
    listener.receivedEvents.clear();
    
    b.removeComponent<Name>();

    REQUIRE(listener.receivedEvents.size() == 1);
    REQUIRE(listener.receivedEvents[0].type == ComponentPoolEventType::Remove);
    REQUIRE(listener.receivedEvents[0].entityId == b.id());
}
