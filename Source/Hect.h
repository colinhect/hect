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

#include <atomic>
#include <bitset>
#include <condition_variable>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <mutex>
#include <queue>
#include <vector>
#include <sstream>
#include <stack>
#include <string>
#include <thread>
#include <cstdint>

#ifdef _MSC_VER

// Some template tricks in ComponentSerializer causes this warning
#pragma warning(disable:4505)

#define HECT_WINDOWS
#ifdef _DEBUG
#define HECT_DEBUG
#endif
#endif

namespace hect
{

///
/// An approximation of Pi.
const double pi = 3.14159265358979323846;

}

#include "Core/Format.h"
#include "Core/Logging.h"
#include "Core/Error.h"
#include "Core/Uncopyable.h"
#include "Core/Any.h"
#include "Core/Memory.h"
#include "Core/TimeSpan.h"
#include "Core/Timer.h"
#include "Core/TaskPool.h"
#include "Core/IdPool.h"
#include "Core/Listener.h"
#include "Core/Dispatcher.h"

#include "Math/Angle.h"
#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Math/Vector4.h"
#include "Math/Quaternion.h"
#include "Math/Matrix4.h"
#include "Math/Box.h"
#include "Math/Sphere.h"
#include "Math/AxisAlignedBox.h"
#include "Math/Plane.h"
#include "Math/Frustum.h"

#include "Core/DataValue.h"

#include "IO/Path.h"
#include "IO/ReadStream.h"
#include "IO/WriteStream.h"
#include "IO/FileReadStream.h"
#include "IO/FileWriteStream.h"
#include "IO/MemoryReadStream.h"
#include "IO/MemoryWriteStream.h"
#include "IO/FileSystem.h"
#include "IO/DataReader.h"
#include "IO/DataWriter.h"
#include "Asset/AssetLoader.h"
#include "Asset/AssetEntry.h"
#include "Asset/AssetHandle.h"
#include "Asset/AssetCache.h"

#include "Core/DataValueJsonFormat.h"

#include "Graphics/VertexAttribute.h"
#include "Graphics/VertexLayout.h"
#include "Graphics/RendererObject.h"
#include "Graphics/RenderMode.h"
#include "Graphics/Image.h"
#include "Graphics/ImagePngFormat.h"
#include "Graphics/RenderTarget.h"
#include "Graphics/Texture.h"
#include "Graphics/TextureDataValueFormat.h"
#include "Graphics/FrameBuffer.h"
#include "Graphics/ShaderModule.h"
#include "Graphics/UniformValue.h"
#include "Graphics/Uniform.h"
#include "Graphics/Shader.h"
#include "Graphics/ShaderDataValueFormat.h"
#include "Graphics/PassUniformValue.h"
#include "Graphics/Pass.h"
#include "Graphics/Technique.h"
#include "Graphics/Material.h"
#include "Graphics/MaterialDataValueFormat.h"
#include "Graphics/Mesh.h"
#include "Graphics/MeshWriter.h"
#include "Graphics/MeshReader.h"
#include "Graphics/MeshBinaryFormat.h"
#include "Graphics/MeshDataValueFormat.h"
#include "Graphics/VideoMode.h"
#include "Graphics/VideoModeDataValueFormat.h"
#include "Graphics/Window.h"
#include "Graphics/Renderer.h"

#include "Input/Mouse.h"
#include "Input/Keyboard.h"
#include "Input/InputAxis.h"
#include "Input/InputAxisDataValueFormat.h"
#include "Input/InputSystem.h"

#include "Noise/Random.h"

#include "Network/IpAddress.h"
#include "Network/Packet.h"
#include "Network/Peer.h"
#include "Network/Socket.h"

#include "Entity/Component.h"
#include "Entity/EntityData.h"
#include "Entity/Entity.h"
#include "Entity/System.h"
#include "Entity/ComponentSerializer.h"
#include "Entity/EntitySerializer.h"
#include "Entity/Scene.h"

#include "Entity/Components/Camera.h"
#include "Entity/Components/Transform.h"
#include "Entity/Components/Geometry.h"
#include "Entity/Components/RigidBody.h"
#include "Entity/Components/AmbientLight.h"
#include "Entity/Components/DirectionalLight.h"

#include "Entity/Systems/CameraSystem.h"
#include "Entity/Systems/PhysicsSystem.h"
#include "Entity/Systems/BasicRenderSystem.h"

#include "Core/LogicLayer.h"
#include "Core/LogicFlow.h"