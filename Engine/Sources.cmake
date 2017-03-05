# Generated by Build Tool (see Engine/Tools/Build for details)

set(SOURCE
    "Source/./Hect.h"
    )

source_group("Source" FILES ${SOURCE})

set(SOURCE_HECT_CONCURRENCY
    "Source/Hect/Concurrency/Task.cpp"
    "Source/Hect/Concurrency/Task.h"
    "Source/Hect/Concurrency/TaskError.cpp"
    "Source/Hect/Concurrency/TaskError.h"
    "Source/Hect/Concurrency/TaskPool.cpp"
    "Source/Hect/Concurrency/TaskPool.h"
    )

source_group("Source\\Hect\\Concurrency" FILES ${SOURCE_HECT_CONCURRENCY})

set(SOURCE_HECT_CORE
    "Source/Hect/Core/Any.cpp"
    "Source/Hect/Core/Any.h"
    "Source/Hect/Core/Any.inl"
    "Source/Hect/Core/Configuration.h"
    "Source/Hect/Core/Configuration.h.in"
    "Source/Hect/Core/EventDispatcher.h"
    "Source/Hect/Core/EventDispatcher.inl"
    "Source/Hect/Core/EventListener.h"
    "Source/Hect/Core/EventListener.inl"
    "Source/Hect/Core/Exception.cpp"
    "Source/Hect/Core/Exception.h"
    "Source/Hect/Core/Export.h"
    "Source/Hect/Core/Format.cpp"
    "Source/Hect/Core/Format.h"
    "Source/Hect/Core/Logging.cpp"
    "Source/Hect/Core/Logging.h"
    "Source/Hect/Core/LogLevel.h"
    "Source/Hect/Core/LogMessageEvent.h"
    "Source/Hect/Core/Name.cpp"
    "Source/Hect/Core/Name.h"
    "Source/Hect/Core/Optional.h"
    "Source/Hect/Core/Optional.inl"
    "Source/Hect/Core/Sequence.h"
    "Source/Hect/Core/Sequence.inl"
    "Source/Hect/Core/Uncopyable.cpp"
    "Source/Hect/Core/Uncopyable.h"
    )

source_group("Source\\Hect\\Core" FILES ${SOURCE_HECT_CORE})

set(SOURCE_HECT_GRAPHICS
    "Source/Hect/Graphics/BlendFactor.h"
    "Source/Hect/Graphics/BlendFunction.h"
    "Source/Hect/Graphics/BlendMode.cpp"
    "Source/Hect/Graphics/BlendMode.h"
    "Source/Hect/Graphics/Color.cpp"
    "Source/Hect/Graphics/Color.h"
    "Source/Hect/Graphics/ColorSpace.h"
    "Source/Hect/Graphics/CubeSide.h"
    "Source/Hect/Graphics/CullMode.h"
    "Source/Hect/Graphics/Font.cpp"
    "Source/Hect/Graphics/Font.h"
    "Source/Hect/Graphics/FrameBuffer.cpp"
    "Source/Hect/Graphics/FrameBuffer.h"
    "Source/Hect/Graphics/FrameBufferAttachment.cpp"
    "Source/Hect/Graphics/FrameBufferAttachment.h"
    "Source/Hect/Graphics/FrameBufferAttachmentType.h"
    "Source/Hect/Graphics/FrameBufferSlot.h"
    "Source/Hect/Graphics/GeometryBuffer.cpp"
    "Source/Hect/Graphics/GeometryBuffer.h"
    "Source/Hect/Graphics/Image.cpp"
    "Source/Hect/Graphics/Image.h"
    "Source/Hect/Graphics/IndexType.h"
    "Source/Hect/Graphics/Material.cpp"
    "Source/Hect/Graphics/Material.h"
    "Source/Hect/Graphics/Mesh.cpp"
    "Source/Hect/Graphics/Mesh.h"
    "Source/Hect/Graphics/MeshReader.cpp"
    "Source/Hect/Graphics/MeshReader.h"
    "Source/Hect/Graphics/MeshWriter.cpp"
    "Source/Hect/Graphics/MeshWriter.h"
    "Source/Hect/Graphics/PhysicallyBasedSceneRenderer.cpp"
    "Source/Hect/Graphics/PhysicallyBasedSceneRenderer.h"
    "Source/Hect/Graphics/PixelFormat.cpp"
    "Source/Hect/Graphics/PixelFormat.h"
    "Source/Hect/Graphics/PixelType.h"
    "Source/Hect/Graphics/PrimitiveType.h"
    "Source/Hect/Graphics/Renderer.cpp"
    "Source/Hect/Graphics/Renderer.h"
    "Source/Hect/Graphics/Renderer.inl"
    "Source/Hect/Graphics/RenderStage.h"
    "Source/Hect/Graphics/RenderTarget.cpp"
    "Source/Hect/Graphics/RenderTarget.h"
    "Source/Hect/Graphics/Shader.cpp"
    "Source/Hect/Graphics/Shader.h"
    "Source/Hect/Graphics/ShaderModule.cpp"
    "Source/Hect/Graphics/ShaderModule.h"
    "Source/Hect/Graphics/ShaderModuleType.h"
    "Source/Hect/Graphics/Texture2.cpp"
    "Source/Hect/Graphics/Texture2.h"
    "Source/Hect/Graphics/Texture3.cpp"
    "Source/Hect/Graphics/Texture3.h"
    "Source/Hect/Graphics/TextureCube.cpp"
    "Source/Hect/Graphics/TextureCube.h"
    "Source/Hect/Graphics/TextureFilter.h"
    "Source/Hect/Graphics/Uniform.cpp"
    "Source/Hect/Graphics/Uniform.h"
    "Source/Hect/Graphics/UniformBinding.h"
    "Source/Hect/Graphics/UniformType.h"
    "Source/Hect/Graphics/UniformValue.cpp"
    "Source/Hect/Graphics/UniformValue.h"
    "Source/Hect/Graphics/VectorRenderer.cpp"
    "Source/Hect/Graphics/VectorRenderer.h"
    "Source/Hect/Graphics/VertexAttribute.cpp"
    "Source/Hect/Graphics/VertexAttribute.h"
    "Source/Hect/Graphics/VertexAttributeSemantic.h"
    "Source/Hect/Graphics/VertexAttributeType.h"
    "Source/Hect/Graphics/VertexLayout.cpp"
    "Source/Hect/Graphics/VertexLayout.h"
    )

source_group("Source\\Hect\\Graphics" FILES ${SOURCE_HECT_GRAPHICS})

set(SOURCE_HECT_GRAPHICS_OPENGL
    "Source/Hect/Graphics/OpenGL/Renderer.cpp"
    "Source/Hect/Graphics/OpenGL/VectorRenderer.cpp"
    )

source_group("Source\\Hect\\Graphics\\OpenGL" FILES ${SOURCE_HECT_GRAPHICS_OPENGL})

set(SOURCE_HECT_GRAPHICS_STUB
    "Source/Hect/Graphics/Stub/Renderer.cpp"
    "Source/Hect/Graphics/Stub/VectorRenderer.cpp"
    )

source_group("Source\\Hect\\Graphics\\Stub" FILES ${SOURCE_HECT_GRAPHICS_STUB})

set(SOURCE_HECT_INPUT
    "Source/Hect/Input/InputAxis.cpp"
    "Source/Hect/Input/InputAxis.h"
    "Source/Hect/Input/InputAxisBinding.cpp"
    "Source/Hect/Input/InputAxisBinding.h"
    "Source/Hect/Input/InputAxisBindingType.h"
    "Source/Hect/Input/Joystick.cpp"
    "Source/Hect/Input/Joystick.h"
    "Source/Hect/Input/JoystickAxis.h"
    "Source/Hect/Input/JoystickButton.h"
    "Source/Hect/Input/JoystickEvent.h"
    "Source/Hect/Input/JoystickEventType.h"
    "Source/Hect/Input/JoystickIndex.h"
    "Source/Hect/Input/Key.h"
    "Source/Hect/Input/Keyboard.cpp"
    "Source/Hect/Input/Keyboard.h"
    "Source/Hect/Input/KeyboardEvent.cpp"
    "Source/Hect/Input/KeyboardEvent.h"
    "Source/Hect/Input/KeyboardEventType.h"
    "Source/Hect/Input/Mouse.cpp"
    "Source/Hect/Input/Mouse.h"
    "Source/Hect/Input/MouseButton.h"
    "Source/Hect/Input/MouseEvent.h"
    "Source/Hect/Input/MouseEventType.h"
    "Source/Hect/Input/MouseMode.h"
    )

source_group("Source\\Hect\\Input" FILES ${SOURCE_HECT_INPUT})

set(SOURCE_HECT_INTERFACE
    "Source/Hect/Interface/HorizontalAlign.h"
    "Source/Hect/Interface/Interface.cpp"
    "Source/Hect/Interface/Interface.h"
    "Source/Hect/Interface/StyleColor.h"
    "Source/Hect/Interface/VerticalAlign.h"
    "Source/Hect/Interface/Widget.h"
    "Source/Hect/Interface/Widget.inl"
    "Source/Hect/Interface/WidgetBase.cpp"
    "Source/Hect/Interface/WidgetBase.h"
    "Source/Hect/Interface/WidgetBase.inl"
    )

source_group("Source\\Hect\\Interface" FILES ${SOURCE_HECT_INTERFACE})

set(SOURCE_HECT_INTERFACE_WIDGETS
    "Source/Hect/Interface/Widgets/ButtonWidget.cpp"
    "Source/Hect/Interface/Widgets/ButtonWidget.h"
    "Source/Hect/Interface/Widgets/CheckBoxWidget.cpp"
    "Source/Hect/Interface/Widgets/CheckBoxWidget.h"
    "Source/Hect/Interface/Widgets/LabelWidget.cpp"
    "Source/Hect/Interface/Widgets/LabelWidget.h"
    "Source/Hect/Interface/Widgets/PanelWidget.cpp"
    "Source/Hect/Interface/Widgets/PanelWidget.h"
    "Source/Hect/Interface/Widgets/TableWidget.cpp"
    "Source/Hect/Interface/Widgets/TableWidget.h"
    )

source_group("Source\\Hect\\Interface\\Widgets" FILES ${SOURCE_HECT_INTERFACE_WIDGETS})

set(SOURCE_HECT_IO
    "Source/Hect/IO/Asset.h"
    "Source/Hect/IO/Asset.inl"
    "Source/Hect/IO/AssetCache.cpp"
    "Source/Hect/IO/AssetCache.h"
    "Source/Hect/IO/AssetCache.inl"
    "Source/Hect/IO/AssetDecoder.cpp"
    "Source/Hect/IO/AssetDecoder.h"
    "Source/Hect/IO/AssetEntry.h"
    "Source/Hect/IO/AssetEntry.inl"
    "Source/Hect/IO/AssetHandle.h"
    "Source/Hect/IO/AssetHandle.inl"
    "Source/Hect/IO/BinaryDecoder.cpp"
    "Source/Hect/IO/BinaryDecoder.h"
    "Source/Hect/IO/BinaryEncoder.cpp"
    "Source/Hect/IO/BinaryEncoder.h"
    "Source/Hect/IO/ByteVector.h"
    "Source/Hect/IO/DataValue.cpp"
    "Source/Hect/IO/DataValue.h"
    "Source/Hect/IO/DataValueDecoder.cpp"
    "Source/Hect/IO/DataValueDecoder.h"
    "Source/Hect/IO/DataValueEncoder.cpp"
    "Source/Hect/IO/DataValueEncoder.h"
    "Source/Hect/IO/DataValueType.h"
    "Source/Hect/IO/DecodeError.cpp"
    "Source/Hect/IO/DecodeError.h"
    "Source/Hect/IO/Decoder.cpp"
    "Source/Hect/IO/Decoder.h"
    "Source/Hect/IO/Decoder.inl"
    "Source/Hect/IO/Encodable.cpp"
    "Source/Hect/IO/Encodable.h"
    "Source/Hect/IO/EncodeError.cpp"
    "Source/Hect/IO/EncodeError.h"
    "Source/Hect/IO/EncodeOperations.cpp"
    "Source/Hect/IO/EncodeOperations.h"
    "Source/Hect/IO/EncodeOperations.inl"
    "Source/Hect/IO/Encoder.cpp"
    "Source/Hect/IO/Encoder.h"
    "Source/Hect/IO/Encoder.inl"
    "Source/Hect/IO/FileSystem.cpp"
    "Source/Hect/IO/FileSystem.h"
    "Source/Hect/IO/IOError.cpp"
    "Source/Hect/IO/IOError.h"
    "Source/Hect/IO/MemoryReadStream.cpp"
    "Source/Hect/IO/MemoryReadStream.h"
    "Source/Hect/IO/MemoryWriteStream.cpp"
    "Source/Hect/IO/MemoryWriteStream.h"
    "Source/Hect/IO/Path.cpp"
    "Source/Hect/IO/Path.h"
    "Source/Hect/IO/ReadStream.cpp"
    "Source/Hect/IO/ReadStream.h"
    "Source/Hect/IO/WriteStream.cpp"
    "Source/Hect/IO/WriteStream.h"
    )

source_group("Source\\Hect\\IO" FILES ${SOURCE_HECT_IO})

set(SOURCE_HECT_MATH
    "Source/Hect/Math/AxisAlignedBox.cpp"
    "Source/Hect/Math/AxisAlignedBox.h"
    "Source/Hect/Math/Box.cpp"
    "Source/Hect/Math/Box.h"
    "Source/Hect/Math/Constants.h"
    "Source/Hect/Math/Frustum.cpp"
    "Source/Hect/Math/Frustum.h"
    "Source/Hect/Math/Functions.h"
    "Source/Hect/Math/Functions.inl"
    "Source/Hect/Math/Matrix4.h"
    "Source/Hect/Math/Matrix4.inl"
    "Source/Hect/Math/Plane.cpp"
    "Source/Hect/Math/Plane.h"
    "Source/Hect/Math/Quaternion.h"
    "Source/Hect/Math/Quaternion.inl"
    "Source/Hect/Math/Rectangle.cpp"
    "Source/Hect/Math/Rectangle.h"
    "Source/Hect/Math/Sphere.cpp"
    "Source/Hect/Math/Sphere.h"
    "Source/Hect/Math/Vector2.h"
    "Source/Hect/Math/Vector2.inl"
    "Source/Hect/Math/Vector3.h"
    "Source/Hect/Math/Vector3.inl"
    "Source/Hect/Math/Vector4.h"
    "Source/Hect/Math/Vector4.inl"
    )

source_group("Source\\Hect\\Math" FILES ${SOURCE_HECT_MATH})

set(SOURCE_HECT_NETWORK
    "Source/Hect/Network/Host.cpp"
    "Source/Hect/Network/Host.h"
    "Source/Hect/Network/IPAddress.cpp"
    "Source/Hect/Network/IPAddress.h"
    "Source/Hect/Network/Peer.cpp"
    "Source/Hect/Network/Peer.h"
    )

source_group("Source\\Hect\\Network" FILES ${SOURCE_HECT_NETWORK})

set(SOURCE_HECT_NOISE
    "Source/Hect/Noise/Random.cpp"
    "Source/Hect/Noise/Random.h"
    )

source_group("Source\\Hect\\Noise" FILES ${SOURCE_HECT_NOISE})

set(SOURCE_HECT_REFLECTION
    "Source/Hect/Reflection/Enum.cpp"
    "Source/Hect/Reflection/Enum.h"
    "Source/Hect/Reflection/Enum.inl"
    "Source/Hect/Reflection/Type.cpp"
    "Source/Hect/Reflection/Type.h"
    "Source/Hect/Reflection/Type.inl"
    )

source_group("Source\\Hect\\Reflection" FILES ${SOURCE_HECT_REFLECTION})

set(SOURCE_HECT_RUNTIME
    "Source/Hect/Runtime/Engine.cpp"
    "Source/Hect/Runtime/Engine.h"
    "Source/Hect/Runtime/Platform.h"
    "Source/Hect/Runtime/VideoMode.cpp"
    "Source/Hect/Runtime/VideoMode.h"
    "Source/Hect/Runtime/Window.cpp"
    "Source/Hect/Runtime/Window.h"
    )

source_group("Source\\Hect\\Runtime" FILES ${SOURCE_HECT_RUNTIME})

set(SOURCE_HECT_RUNTIME_OPENGL
    "Source/Hect/Runtime/OpenGL/Platform.cpp"
    "Source/Hect/Runtime/OpenGL/Window.cpp"
    )

source_group("Source\\Hect\\Runtime\\OpenGL" FILES ${SOURCE_HECT_RUNTIME_OPENGL})

set(SOURCE_HECT_RUNTIME_STUB
    "Source/Hect/Runtime/Stub/Platform.cpp"
    "Source/Hect/Runtime/Stub/Window.cpp"
    )

source_group("Source\\Hect\\Runtime\\Stub" FILES ${SOURCE_HECT_RUNTIME_STUB})

set(SOURCE_HECT_SCENE
    "Source/Hect/Scene/Component.h"
    "Source/Hect/Scene/Component.inl"
    "Source/Hect/Scene/ComponentEvent.h"
    "Source/Hect/Scene/ComponentEventType.h"
    "Source/Hect/Scene/ComponentIterator.h"
    "Source/Hect/Scene/ComponentIterator.inl"
    "Source/Hect/Scene/ComponentListener.h"
    "Source/Hect/Scene/ComponentListener.inl"
    "Source/Hect/Scene/ComponentPool.h"
    "Source/Hect/Scene/ComponentPool.inl"
    "Source/Hect/Scene/ComponentRegistry.cpp"
    "Source/Hect/Scene/ComponentRegistry.h"
    "Source/Hect/Scene/ComponentRegistry.inl"
    "Source/Hect/Scene/Entity.cpp"
    "Source/Hect/Scene/Entity.h"
    "Source/Hect/Scene/Entity.inl"
    "Source/Hect/Scene/EntityChildIterator.cpp"
    "Source/Hect/Scene/EntityChildIterator.h"
    "Source/Hect/Scene/EntityChildren.cpp"
    "Source/Hect/Scene/EntityChildren.h"
    "Source/Hect/Scene/EntityEvent.h"
    "Source/Hect/Scene/EntityEventType.h"
    "Source/Hect/Scene/EntityHandle.cpp"
    "Source/Hect/Scene/EntityHandle.h"
    "Source/Hect/Scene/EntityIterator.cpp"
    "Source/Hect/Scene/EntityIterator.h"
    "Source/Hect/Scene/EntityPool.cpp"
    "Source/Hect/Scene/EntityPool.h"
    "Source/Hect/Scene/EntityPool.inl"
    "Source/Hect/Scene/IdPool.h"
    "Source/Hect/Scene/IdPool.inl"
    "Source/Hect/Scene/Scene.cpp"
    "Source/Hect/Scene/Scene.h"
    "Source/Hect/Scene/Scene.inl"
    "Source/Hect/Scene/SceneRegistry.cpp"
    "Source/Hect/Scene/SceneRegistry.h"
    "Source/Hect/Scene/SceneRegistry.inl"
    "Source/Hect/Scene/System.h"
    "Source/Hect/Scene/System.inl"
    "Source/Hect/Scene/SystemBase.cpp"
    "Source/Hect/Scene/SystemBase.h"
    "Source/Hect/Scene/SystemRegistry.cpp"
    "Source/Hect/Scene/SystemRegistry.h"
    "Source/Hect/Scene/SystemRegistry.inl"
    )

source_group("Source\\Hect\\Scene" FILES ${SOURCE_HECT_SCENE})

set(SOURCE_HECT_SCENE_COMPONENTS
    "Source/Hect/Scene/Components/BoundingBoxComponent.h"
    "Source/Hect/Scene/Components/CameraComponent.h"
    "Source/Hect/Scene/Components/DirectionalLightComponent.h"
    "Source/Hect/Scene/Components/GeometryComponent.cpp"
    "Source/Hect/Scene/Components/GeometryComponent.h"
    "Source/Hect/Scene/Components/LightProbeComponent.h"
    "Source/Hect/Scene/Components/RigidBodyComponent.h"
    "Source/Hect/Scene/Components/SkyBoxComponent.h"
    "Source/Hect/Scene/Components/TransformComponent.cpp"
    "Source/Hect/Scene/Components/TransformComponent.h"
    )

source_group("Source\\Hect\\Scene\\Components" FILES ${SOURCE_HECT_SCENE_COMPONENTS})

set(SOURCE_HECT_SCENE_SCENES
    "Source/Hect/Scene/Scenes/DefaultScene.cpp"
    "Source/Hect/Scene/Scenes/DefaultScene.h"
    )

source_group("Source\\Hect\\Scene\\Scenes" FILES ${SOURCE_HECT_SCENE_SCENES})

set(SOURCE_HECT_SCENE_SYSTEMS
    "Source/Hect/Scene/Systems/BoundingBoxSystem.cpp"
    "Source/Hect/Scene/Systems/BoundingBoxSystem.h"
    "Source/Hect/Scene/Systems/CameraSystem.cpp"
    "Source/Hect/Scene/Systems/CameraSystem.h"
    "Source/Hect/Scene/Systems/DebugSystem.cpp"
    "Source/Hect/Scene/Systems/DebugSystem.h"
    "Source/Hect/Scene/Systems/InputSystem.cpp"
    "Source/Hect/Scene/Systems/InputSystem.h"
    "Source/Hect/Scene/Systems/InterfaceSystem.cpp"
    "Source/Hect/Scene/Systems/InterfaceSystem.h"
    "Source/Hect/Scene/Systems/PhysicsSystem.cpp"
    "Source/Hect/Scene/Systems/PhysicsSystem.h"
    "Source/Hect/Scene/Systems/ScriptSystem.cpp"
    "Source/Hect/Scene/Systems/ScriptSystem.h"
    "Source/Hect/Scene/Systems/TransformSystem.cpp"
    "Source/Hect/Scene/Systems/TransformSystem.h"
    )

source_group("Source\\Hect\\Scene\\Systems" FILES ${SOURCE_HECT_SCENE_SYSTEMS})

set(SOURCE_HECT_TIMING
    "Source/Hect/Timing/Timer.cpp"
    "Source/Hect/Timing/Timer.h"
    "Source/Hect/Timing/TimeStamp.h"
    )

source_group("Source\\Hect\\Timing" FILES ${SOURCE_HECT_TIMING})

set(SOURCE_HECT_UNITS
    "Source/Hect/Units/Angle.cpp"
    "Source/Hect/Units/Angle.h"
    "Source/Hect/Units/Distance.cpp"
    "Source/Hect/Units/Distance.h"
    "Source/Hect/Units/Time.cpp"
    "Source/Hect/Units/Time.h"
    "Source/Hect/Units/Unit.h"
    "Source/Hect/Units/Unit.inl"
    )

source_group("Source\\Hect\\Units" FILES ${SOURCE_HECT_UNITS})

set(SOURCE_FILES
    ${SOURCE}
    ${SOURCE_HECT_CONCURRENCY}
    ${SOURCE_HECT_CORE}
    ${SOURCE_HECT_GRAPHICS}
    ${SOURCE_HECT_GRAPHICS_OPENGL}
    ${SOURCE_HECT_GRAPHICS_STUB}
    ${SOURCE_HECT_INPUT}
    ${SOURCE_HECT_INTERFACE}
    ${SOURCE_HECT_INTERFACE_WIDGETS}
    ${SOURCE_HECT_IO}
    ${SOURCE_HECT_MATH}
    ${SOURCE_HECT_NETWORK}
    ${SOURCE_HECT_NOISE}
    ${SOURCE_HECT_REFLECTION}
    ${SOURCE_HECT_RUNTIME}
    ${SOURCE_HECT_RUNTIME_OPENGL}
    ${SOURCE_HECT_RUNTIME_STUB}
    ${SOURCE_HECT_SCENE}
    ${SOURCE_HECT_SCENE_COMPONENTS}
    ${SOURCE_HECT_SCENE_SCENES}
    ${SOURCE_HECT_SCENE_SYSTEMS}
    ${SOURCE_HECT_TIMING}
    ${SOURCE_HECT_UNITS}
    )

