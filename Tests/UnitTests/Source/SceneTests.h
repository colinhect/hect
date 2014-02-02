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

SUITE(Scene)
{
    class Name :
        public Component<Name>
    {
    public:
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

    class Position :
        public Component<Position>
    {
    public:
        void encode(ObjectEncoder& encoder) const
        {
            encoder.encodeVector3("value", value);
        }

        void decode(ObjectDecoder& decoder, AssetCache& assetCache)
        {
            assetCache;

            if (decoder.hasMember("value"))
            {
                value = decoder.decodeVector3("value");
            }
        }

        Vector3 value;
    };

    class Velocity :
        public Component<Velocity>
    {
    public:
        void encode(ObjectEncoder& encoder) const
        {
            encoder.encodeVector3("value", value);
        }

        void decode(ObjectDecoder& decoder, AssetCache& assetCache)
        {
            assetCache;

            if (decoder.hasMember("value"))
            {
                value = decoder.decodeVector3("value");
            }
        }

        Vector3 value;
    };

    class MovementSystem :
        public System
    {
    public:
        bool includesEntity(const Entity& entity) const
        {
            return entity.hasComponent<Position>() && entity.hasComponent<Velocity>();
        }

        bool hasEntity(Entity entity)
        {
            auto& es = entities();
            return std::find(es.begin(), es.end(), entity) != es.end();
        }
    };

    class NamingSystem :
        public System
    {
    public:
        bool includesEntity(const Entity& entity) const
        {
            return entity.hasComponent<Name>();
        }

        bool hasEntity(Entity entity)
        {
            auto& es = entities();
            return std::find(es.begin(), es.end(), entity) != es.end();
        }
    };

    TEST(CreateAndDestroy)
    {
        Scene scene;

        Entity a = scene.createEntity();

        CHECK(!a.isNull());
        CHECK(!a.isActivated());
        CHECK(a.isEncodable());

        a.destroy();

        CHECK(!a.isNull());
        CHECK(!a.isActivated());

        scene.refresh();

        CHECK(a.isNull());
    }

    TEST(AddComponent)
    {
        Scene scene;

        Entity a = scene.createEntity();

        CHECK(!a.hasComponent<Name>());

        a.addComponent<Name>();

        CHECK(a.hasComponent<Name>());

        Name& name = a.component<Name>();
        name.value = "Testing";

        CHECK_EQUAL("Testing", a.component<Name>().value);
    }

    TEST(ActivationAndDestruction)
    {
        Scene scene;

        Entity a = scene.createEntity();

        CHECK(!a.isActivated());

        a.addComponent<Name>();

        CHECK(!a.isActivated());
        CHECK(a.hasComponent<Name>());

        a.activate();

        CHECK(a.isActivated());

        scene.refresh();

        CHECK(a.isActivated());

        a.destroy();
        scene.refresh();

        CHECK(a.isNull());
    }

    TEST(PoolResize)
    {
        Scene scene;

        // Add a lot of entities
        std::vector<Entity> entities;
        for (int i = 0 ; i < 255; ++i)
        {
            Entity entity = scene.createEntity();
            entity.addComponent<Name>();
            entities.push_back(entity);
        }

        for (const Entity& entity : entities)
        {
            CHECK(!entity.isNull());
            CHECK(entity.hasComponent<Name>());
            CHECK_EQUAL("", entity.component<Name>().value);
        }
    }

    TEST(Systems)
    {
        MovementSystem movementSystem;
        NamingSystem namingSystem;

        Scene scene;

        scene.addSystem(movementSystem);
        scene.addSystem(namingSystem);

        Entity frank = scene.createEntity();
        frank.addComponent<Name>().value = "Frank";
        frank.addComponent<Velocity>();
        frank.activate();

        Entity joe = scene.createEntity();
        joe.addComponent<Name>().value = "Joe";
        joe.addComponent<Velocity>();
        joe.addComponent<Position>();
        joe.activate();

        Entity namelessHerold =  scene.createEntity();
        namelessHerold.addComponent<Velocity>();
        namelessHerold.addComponent<Position>();
        namelessHerold.activate();

        scene.refresh();

        CHECK(!movementSystem.hasEntity(frank));
        CHECK(movementSystem.hasEntity(joe));
        CHECK(movementSystem.hasEntity(namelessHerold));

        CHECK(namingSystem.hasEntity(frank));
        CHECK(namingSystem.hasEntity(joe));
        CHECK(!namingSystem.hasEntity(namelessHerold));

        frank.destroy();
        scene.refresh();

        CHECK(!movementSystem.hasEntity(frank));
        CHECK(movementSystem.hasEntity(joe));
        CHECK(movementSystem.hasEntity(namelessHerold));

        CHECK(!namingSystem.hasEntity(frank));
        CHECK(namingSystem.hasEntity(joe));
        CHECK(!namingSystem.hasEntity(namelessHerold));

        joe.destroy();
        scene.refresh();

        CHECK(!movementSystem.hasEntity(frank));
        CHECK(!movementSystem.hasEntity(joe));
        CHECK(movementSystem.hasEntity(namelessHerold));

        CHECK(!namingSystem.hasEntity(frank));
        CHECK(!namingSystem.hasEntity(joe));
        CHECK(!namingSystem.hasEntity(namelessHerold));
    }

    TEST(CloneSimpleEntity)
    {
        Scene scene;

        Entity a = scene.createEntity();
        Entity b = a.clone();

        CHECK(a != b);
    }

    TEST(CloneEntityWithComponent)
    {
        Scene scene;

        Entity a = scene.createEntity();
        a.addComponent<Name>().value = "Testing";

        CHECK_EQUAL("Testing", a.component<Name>().value);

        Entity b = a.clone();

        CHECK(b.hasComponent<Name>());
        CHECK(&a.component<Name>() != &b.component<Name>());
        CHECK_EQUAL("Testing", b.component<Name>().value);
    }

    TEST(CloneActivatedEntity)
    {
        Scene scene;

        Entity a = scene.createEntity();
        a.activate();

        CHECK(a.isActivated());

        Entity b = a.clone();

        CHECK(!b.isActivated());
    }

    TEST(EncodeAndDecodeEntityUsingJsonValue)
    {
        FileSystem fileSystem;
        AssetCache assetCache(fileSystem);

        Scene scene;
        scene.registerComponent<Name>("Name");
        scene.registerComponent<Position>("Position");

        Entity frank = scene.createEntity();
        frank.addComponent<Name>().value = "Frank";
        frank.addComponent<Position>().value = Vector3(1, 2, 3);

        JsonValue frankValue = frank.encodeToJsonValue();

        Entity frankDecoded = scene.createEntity();
        frankDecoded.decodeFromJsonValue(frankValue, assetCache);

        CHECK(frankDecoded.hasComponent<Name>());
        CHECK(frankDecoded.hasComponent<Position>());

        CHECK_EQUAL("Frank", frankDecoded.component<Name>().value);
        CHECK_EQUAL(2, frankDecoded.component<Position>().value.y);
    }

    TEST(EncodeAndDecodeEntityUsingStream)
    {
        FileSystem fileSystem;
        AssetCache assetCache(fileSystem);

        Scene scene;
        scene.registerComponent<Name>("Name");
        scene.registerComponent<Position>("Position");

        Entity frank = scene.createEntity();
        frank.addComponent<Name>().value = "Frank";
        frank.addComponent<Position>().value = Vector3(1, 2, 3);

        std::vector<uint8_t> data;

        {
            MemoryWriteStream stream(data);
            frank.encodeToBinary(stream);
        }

        Entity frankDecoded = scene.createEntity();

        {
            MemoryReadStream stream(data);
            frankDecoded.decodeFromBinary(stream, assetCache);
        }

        CHECK(frankDecoded.hasComponent<Name>());
        CHECK(frankDecoded.hasComponent<Position>());

        CHECK_EQUAL("Frank", frankDecoded.component<Name>().value);
        CHECK_EQUAL(2, frankDecoded.component<Position>().value.y);
    }

    TEST(EncodeAndDecodeSceneUsingJsonValue)
    {
        FileSystem fileSystem;
        AssetCache assetCache(fileSystem);

        JsonValue sceneValue;
        {
            Scene scene;
            scene.registerComponent<Name>("Name");

            Entity frank = scene.createEntity();
            frank.addComponent<Name>().value = "Frank";
            frank.activate();

            Entity joe = scene.createEntity();
            joe.addComponent<Name>().value = "Joe";
            joe.activate();

            Entity billy = scene.createEntity();
            billy.addComponent<Name>().value = "Billy";
            billy.setEncodable(false);
            billy.activate();

            scene.refresh();
            sceneValue = scene.encodeToJsonValue();
        }

        NamingSystem namingSystem;

        Scene scene;
        scene.registerComponent<Name>("Name");

        scene.addSystem(namingSystem);

        scene.decodeFromJsonValue(sceneValue, assetCache);
        scene.refresh();

        auto& entities = namingSystem.entities();
        CHECK_EQUAL(2u, entities.size());
        CHECK(entities[0].hasComponent<Name>());
        CHECK_EQUAL("Frank", entities[0].component<Name>().value);
        CHECK(entities[1].hasComponent<Name>());
        CHECK_EQUAL("Joe", entities[1].component<Name>().value);
    }

    TEST(EncodeAndDecodeSceneUsingStream)
    {
        FileSystem fileSystem;
        AssetCache assetCache(fileSystem);

        std::vector<uint8_t> data;
        {
            Scene scene;
            scene.registerComponent<Name>("Name");

            Entity frank = scene.createEntity();
            frank.addComponent<Name>().value = "Frank";
            frank.activate();

            Entity joe = scene.createEntity();
            joe.addComponent<Name>().value = "Joe";
            joe.activate();

            Entity billy = scene.createEntity();
            billy.addComponent<Name>().value = "Billy";
            billy.setEncodable(false);
            billy.activate();

            scene.refresh();
            {
                MemoryWriteStream stream(data);
                scene.encodeToBinary(stream);
            }
        }

        NamingSystem namingSystem;

        Scene scene;
        scene.registerComponent<Name>("Name");

        scene.addSystem(namingSystem);

        {
            MemoryReadStream stream(data);
            scene.decodeFromBinary(stream, assetCache);
        }
        scene.refresh();

        auto& entities = namingSystem.entities();
        CHECK_EQUAL(2u, entities.size());
        CHECK(entities[0].hasComponent<Name>());
        CHECK_EQUAL("Frank", entities[0].component<Name>().value);
        CHECK(entities[1].hasComponent<Name>());
        CHECK_EQUAL("Joe", entities[1].component<Name>().value);
    }
}
