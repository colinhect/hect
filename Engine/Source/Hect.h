///////////////////////////////////////////////////////////////////////////////
// This source file is part of Hect.
//
// Copyright (c) 2015 Colin Hill
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

///
/// The namespace containing the Hect %Engine API.
namespace hect {}

#include "Hect/Core/Configuration.h"

#ifdef HECT_USE_VLD
#include <vld.h>
#endif

#include "Hect/Concurrency/Task.h"
#include "Hect/Concurrency/TaskPool.h"
#include "Hect/Core/Any.h"
#include "Hect/Core/Exception.h"
#include "Hect/Core/Event.h"
#include "Hect/Core/Format.h"
#include "Hect/Core/Logging.h"
#include "Hect/Core/Sequence.h"
#include "Hect/Core/StringMap.h"
#include "Hect/Core/Uncopyable.h"
#include "Hect/Graphics/BlendFactor.h"
#include "Hect/Graphics/Font.h"
#include "Hect/Graphics/FrameBuffer.h"
#include "Hect/Graphics/Image.h"
#include "Hect/Graphics/Material.h"
#include "Hect/Graphics/Mesh.h"
#include "Hect/Graphics/MeshReader.h"
#include "Hect/Graphics/MeshWriter.h"
#include "Hect/Graphics/Renderer.h"
#include "Hect/Graphics/RenderTarget.h"
#include "Hect/Graphics/Shader.h"
#include "Hect/Graphics/Texture2.h"
#include "Hect/Graphics/Uniform.h"
#include "Hect/Graphics/UniformBinding.h"
#include "Hect/Graphics/UniformValue.h"
#include "Hect/Graphics/VectorRenderer.h"
#include "Hect/Graphics/VertexAttribute.h"
#include "Hect/Graphics/VertexLayout.h"
#include "Hect/Input/InputAxis.h"
#include "Hect/Input/InputAxisBinding.h"
#include "Hect/Input/Joystick.h"
#include "Hect/Input/Keyboard.h"
#include "Hect/Input/Mouse.h"
#include "Hect/Interface/Label.h"
#include "Hect/Interface/MessageLog.h"
#include "Hect/Interface/Panel.h"
#include "Hect/Interface/Widget.h"
#include "Hect/IO/Asset.h"
#include "Hect/IO/AssetCache.h"
#include "Hect/IO/AssetDecoder.h"
#include "Hect/IO/AssetEntry.h"
#include "Hect/IO/AssetHandle.h"
#include "Hect/IO/BinaryDecoder.h"
#include "Hect/IO/BinaryEncoder.h"
#include "Hect/IO/ByteVector.h"
#include "Hect/IO/DataValue.h"
#include "Hect/IO/DataValueDecoder.h"
#include "Hect/IO/DataValueEncoder.h"
#include "Hect/IO/DataValueType.h"
#include "Hect/IO/Decoder.h"
#include "Hect/IO/Encodable.h"
#include "Hect/IO/EncodeOperations.h"
#include "Hect/IO/Encoder.h"
#include "Hect/IO/FileSystem.h"
#include "Hect/IO/MemoryReadStream.h"
#include "Hect/IO/MemoryWriteStream.h"
#include "Hect/IO/Path.h"
#include "Hect/IO/ReadStream.h"
#include "Hect/IO/WriteStream.h"
#include "Hect/Logic/Component.h"
#include "Hect/Logic/ComponentEvent.h"
#include "Hect/Logic/ComponentPool.h"
#include "Hect/Logic/ComponentRegistry.h"
#include "Hect/Logic/Entity.h"
#include "Hect/Logic/EntityEvent.h"
#include "Hect/Logic/EntityPool.h"
#include "Hect/Logic/IdPool.h"
#include "Hect/Logic/Scene.h"
#include "Hect/Logic/System.h"
#include "Hect/Logic/SystemRegistry.h"
#include "Hect/Logic/Components/BoundingBox.h"
#include "Hect/Logic/Components/Camera.h"
#include "Hect/Logic/Components/DirectionalLight.h"
#include "Hect/Logic/Components/LightProbe.h"
#include "Hect/Logic/Components/Model.h"
#include "Hect/Logic/Components/RigidBody.h"
#include "Hect/Logic/Components/SkyBox.h"
#include "Hect/Logic/Components/Transform.h"
#include "Hect/Logic/Systems/BoundingBoxSystem.h"
#include "Hect/Logic/Systems/CameraSystem.h"
#include "Hect/Logic/Systems/DebugSystem.h"
#include "Hect/Logic/Systems/InputSystem.h"
#include "Hect/Logic/Systems/InterfaceSystem.h"
#include "Hect/Logic/Systems/PhysicsSystem.h"
#include "Hect/Logic/Systems/RenderSystem.h"
#include "Hect/Logic/Systems/TransformSystem.h"
#include "Hect/Math/Angle.h"
#include "Hect/Math/AxisAlignedBox.h"
#include "Hect/Math/Box.h"
#include "Hect/Math/Constants.h"
#include "Hect/Math/Frustum.h"
#include "Hect/Math/Functions.h"
#include "Hect/Math/Matrix4.h"
#include "Hect/Math/Plane.h"
#include "Hect/Math/Quaternion.h"
#include "Hect/Math/Rectangle.h"
#include "Hect/Math/Sphere.h"
#include "Hect/Math/Vector2.h"
#include "Hect/Math/Vector3.h"
#include "Hect/Math/Vector4.h"
#include "Hect/Network/Host.h"
#include "Hect/Network/IPAddress.h"
#include "Hect/Network/Peer.h"
#include "Hect/Noise/Random.h"
#include "Hect/Reflection/Enum.h"
#include "Hect/Reflection/Type.h"
#include "Hect/Runtime/Engine.h"
#include "Hect/Runtime/VideoMode.h"
#include "Hect/Runtime/Window.h"
#include "Hect/Timing/Timer.h"
#include "Hect/Timing/TimeSpan.h"
#include "Hect/Timing/TimeStamp.h"
