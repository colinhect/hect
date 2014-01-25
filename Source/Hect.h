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

#include "Hect/Core/Configuration.h"
#include "Hect/Core/Real.h"
#include "Hect/Core/Format.h"
#include "Hect/Core/Logging.h"
#include "Hect/Core/Error.h"
#include "Hect/Core/Uncopyable.h"
#include "Hect/Core/Any.h"
#include "Hect/Core/Memory.h"
#include "Hect/Core/TimeSpan.h"
#include "Hect/Core/Timer.h"
#include "Hect/Core/TaskPool.h"
#include "Hect/Core/IdPool.h"
#include "Hect/Core/Listener.h"
#include "Hect/Core/Dispatcher.h"

#include "Hect/Math/Constants.h"
#include "Hect/Math/Angle.h"
#include "Hect/Math/Vector2.h"
#include "Hect/Math/Vector3.h"
#include "Hect/Math/Vector4.h"
#include "Hect/Math/Quaternion.h"
#include "Hect/Math/Matrix4.h"
#include "Hect/Math/Box.h"
#include "Hect/Math/Sphere.h"
#include "Hect/Math/AxisAlignedBox.h"
#include "Hect/Math/Plane.h"
#include "Hect/Math/Frustum.h"

#include "Hect/Core/DataValue.h"

#include "Hect/IO/Path.h"
#include "Hect/IO/ReadStream.h"
#include "Hect/IO/WriteStream.h"
#include "Hect/IO/FileReadStream.h"
#include "Hect/IO/FileWriteStream.h"
#include "Hect/IO/MemoryReadStream.h"
#include "Hect/IO/MemoryWriteStream.h"
#include "Hect/IO/FileSystem.h"
#include "Hect/IO/Encodable.h"
#include "Hect/IO/DataEncoder.h"
#include "Hect/IO/DataDecoder.h"
#include "Hect/Asset/AssetLoader.h"
#include "Hect/Asset/AssetEntry.h"
#include "Hect/Asset/AssetHandle.h"
#include "Hect/Asset/AssetCache.h"

#include "Hect/Graphics/VertexAttribute.h"
#include "Hect/Graphics/VertexLayout.h"
#include "Hect/Graphics/RendererObject.h"
#include "Hect/Graphics/RenderMode.h"
#include "Hect/Graphics/Image.h"
#include "Hect/Graphics/ImagePngFormat.h"
#include "Hect/Graphics/RenderTarget.h"
#include "Hect/Graphics/Texture.h"
#include "Hect/Graphics/FrameBuffer.h"
#include "Hect/Graphics/ShaderModule.h"
#include "Hect/Graphics/UniformValue.h"
#include "Hect/Graphics/Uniform.h"
#include "Hect/Graphics/Shader.h"
#include "Hect/Graphics/PassUniformValue.h"
#include "Hect/Graphics/Pass.h"
#include "Hect/Graphics/Technique.h"
#include "Hect/Graphics/Material.h"
#include "Hect/Graphics/Mesh.h"
#include "Hect/Graphics/MeshWriter.h"
#include "Hect/Graphics/MeshReader.h"
#include "Hect/Graphics/VideoMode.h"
#include "Hect/Graphics/Window.h"
#include "Hect/Graphics/Renderer.h"

#include "Hect/Input/Mouse.h"
#include "Hect/Input/Keyboard.h"
#include "Hect/Input/InputAxis.h"
#include "Hect/Input/InputSystem.h"

#include "Hect/Noise/Random.h"

#include "Hect/Network/IpAddress.h"
#include "Hect/Network/Packet.h"
#include "Hect/Network/Peer.h"
#include "Hect/Network/Socket.h"

#include "Hect/Entity/Component.h"
#include "Hect/Entity/EntityData.h"
#include "Hect/Entity/Entity.h"
#include "Hect/Entity/System.h"
#include "Hect/Entity/Scene.h"

#include "Hect/Entity/Components/Camera.h"
#include "Hect/Entity/Components/Transform.h"
#include "Hect/Entity/Components/Geometry.h"
#include "Hect/Entity/Components/RigidBody.h"
#include "Hect/Entity/Components/AmbientLight.h"
#include "Hect/Entity/Components/DirectionalLight.h"

#include "Hect/Entity/Systems/CameraSystem.h"
#include "Hect/Entity/Systems/PhysicsSystem.h"
#include "Hect/Entity/Systems/RenderSystem.h"

#include "Hect/Core/LogicLayer.h"
#include "Hect/Core/LogicFlow.h"