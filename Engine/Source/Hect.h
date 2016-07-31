///////////////////////////////////////////////////////////////////////////////
// This source file is part of Hect.
//
// Copyright (c) 2016 Colin Hill
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
#include "Hect/Core/Optional.h"
#include "Hect/Core/Sequence.h"
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
#include "Hect/Interface/Widget.h"
#include "Hect/Interface/Widgets/Button.h"
#include "Hect/Interface/Widgets/CheckBox.h"
#include "Hect/Interface/Widgets/Grid.h"
#include "Hect/Interface/Widgets/Label.h"
#include "Hect/Interface/Widgets/Panel.h"
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
#include "Hect/Math/Angle.h"
#include "Hect/Math/AxisAlignedBox.h"
#include "Hect/Math/Box.h"
#include "Hect/Math/Constants.h"
#include "Hect/Math/Degrees.h"
#include "Hect/Math/Frustum.h"
#include "Hect/Math/Functions.h"
#include "Hect/Math/Matrix4.h"
#include "Hect/Math/Plane.h"
#include "Hect/Math/Quaternion.h"
#include "Hect/Math/Radians.h"
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
#include "Hect/Scene/Component.h"
#include "Hect/Scene/ComponentEvent.h"
#include "Hect/Scene/ComponentPool.h"
#include "Hect/Scene/ComponentRegistry.h"
#include "Hect/Scene/DefaultScene.h"
#include "Hect/Scene/Entity.h"
#include "Hect/Scene/EntityEvent.h"
#include "Hect/Scene/EntityPool.h"
#include "Hect/Scene/IdPool.h"
#include "Hect/Scene/Scene.h"
#include "Hect/Scene/System.h"
#include "Hect/Scene/SystemRegistry.h"
#include "Hect/Scene/Components/BoundingBoxComponent.h"
#include "Hect/Scene/Components/CameraComponent.h"
#include "Hect/Scene/Components/DirectionalLightComponent.h"
#include "Hect/Scene/Components/LightProbeComponent.h"
#include "Hect/Scene/Components/MeshComponent.h"
#include "Hect/Scene/Components/RigidBodyComponent.h"
#include "Hect/Scene/Components/SkyBoxComponent.h"
#include "Hect/Scene/Components/TransformComponent.h"
#include "Hect/Scene/Systems/BoundingBoxSystem.h"
#include "Hect/Scene/Systems/CameraSystem.h"
#include "Hect/Scene/Systems/DebugSystem.h"
#include "Hect/Scene/Systems/InputSystem.h"
#include "Hect/Scene/Systems/InterfaceSystem.h"
#include "Hect/Scene/Systems/PhysicsSystem.h"
#include "Hect/Scene/Systems/RenderSystem.h"
#include "Hect/Scene/Systems/ScriptSystem.h"
#include "Hect/Scene/Systems/TransformSystem.h"
#include "Hect/Timing/Timer.h"
#include "Hect/Timing/TimeSpan.h"
#include "Hect/Timing/TimeStamp.h"
