include_directories(
    "${PROJECT_SOURCE_DIR}/Engine"
    "${PROJECT_SOURCE_DIR}/Dependencies/Bullet"
    "${PROJECT_SOURCE_DIR}/Dependencies/ENet"
    "${PROJECT_SOURCE_DIR}/Dependencies/Glew"
    "${PROJECT_SOURCE_DIR}/Dependencies/JsonCpp"
    "${PROJECT_SOURCE_DIR}/Dependencies/LodePng"
    "${PROJECT_SOURCE_DIR}/Dependencies/PhysFS"
    "${PROJECT_SOURCE_DIR}/Dependencies/SDL2/include"
    "${PROJECT_SOURCE_DIR}/Dependencies/Tclap/include"
    )
    
FILE(GLOB concurrency_files Hect/Concurrency/*)
source_group(Hect\\Concurrency FILES ${concurrency_files})

FILE(GLOB core_files Hect/Core/*)
source_group(Hect\\Core FILES ${core_files})

FILE(GLOB event_files Hect/Event/*)
source_group(Hect\\Event FILES ${event_files})

FILE(GLOB graphics_files Hect/Graphics/*)
source_group(Hect\\Graphics FILES ${graphics_files})

FILE(GLOB input_files Hect/Input/*)
source_group(Hect\\Input FILES ${input_files})

FILE(GLOB io_files Hect/IO/*)
source_group(Hect\\IO FILES ${io_files})

FILE(GLOB encoders_files Hect/IO/Encoders/*)
source_group(Hect\\IO\\Encoders FILES ${encoders_files})

FILE(GLOB logic_files Hect/Logic/*)
source_group(Hect\\Logic FILES ${logic_files})

FILE(GLOB components_files Hect/Logic/Components/*)
source_group(Hect\\Logic\\Components FILES ${components_files})

FILE(GLOB systems_files Hect/Logic/Systems/*)
source_group(Hect\\Logic\\Systems FILES ${systems_files})

FILE(GLOB math_files Hect/Math/*)
source_group(Hect\\Math FILES ${math_files})

FILE(GLOB network_files Hect/Network/*)
source_group(Hect\\Network FILES ${network_files})

FILE(GLOB noise_files Hect/Noise/*)
source_group(Hect\\Noise FILES ${noise_files})

FILE(GLOB physics_files Hect/Physics/*)
source_group(Hect\\Physics FILES ${physics_files})

FILE(GLOB platform_files Hect/Platform/*)
source_group(Hect\\Platform FILES ${platform_files})

FILE(GLOB reflection_files Hect/Reflection/*)
source_group(Hect\\Reflection FILES ${reflection_files})

FILE(GLOB runtime_files Hect/Runtime/*)
source_group(Hect\\Runtime FILES ${runtime_files})

FILE(GLOB spacial_files Hect/Spacial/*)
source_group(Hect\\Spacial FILES ${spacial_files})

FILE(GLOB timing_files Hect/Timing/*)
source_group(Hect\\Timing FILES ${timing_files})
    
add_library(Hect STATIC
    "Hect/Concurrency/Task.cpp"
    "Hect/Concurrency/Task.h"
    "Hect/Concurrency/TaskPool.cpp"
    "Hect/Concurrency/TaskPool.h"
    "Hect/Core/Any.cpp"
    "Hect/Core/Any.h"
    "Hect/Core/Any.inl"
    "Hect/Core/Configuration.h"
    "Hect/Core/Error.cpp"
    "Hect/Core/Error.h"
    "Hect/Core/Format.cpp"
    "Hect/Core/Format.h"
    "Hect/Core/Logging.cpp"
    "Hect/Core/Logging.h"
    "Hect/Core/Real.h"
    "Hect/Core/Sequence.h"
    "Hect/Core/Sequence.inl"
    "Hect/Core/Uncopyable.cpp"
    "Hect/Core/Uncopyable.h"
    "Hect/Event/Dispatcher.h"
    "Hect/Event/Dispatcher.inl"
    "Hect/Event/Listener.h"
    "Hect/Graphics/FrameBuffer.cpp"
    "Hect/Graphics/FrameBuffer.h"
    "Hect/Graphics/GraphicsContext.cpp"
    "Hect/Graphics/GraphicsContext.h"
    "Hect/Graphics/GraphicsContext.inl"
    "Hect/Graphics/Image.cpp"
    "Hect/Graphics/Image.h"
    "Hect/Graphics/Material.cpp"
    "Hect/Graphics/Material.h"
    "Hect/Graphics/Mesh.cpp"
    "Hect/Graphics/Mesh.h"
    "Hect/Graphics/MeshReader.cpp"
    "Hect/Graphics/MeshReader.h"
    "Hect/Graphics/MeshWriter.cpp"
    "Hect/Graphics/MeshWriter.h"
    "Hect/Graphics/Pass.cpp"
    "Hect/Graphics/Pass.h"
    "Hect/Graphics/Renderer.cpp"
    "Hect/Graphics/Renderer.h"
    "Hect/Graphics/RenderState.cpp"
    "Hect/Graphics/RenderState.h"
    "Hect/Graphics/RenderTarget.cpp"
    "Hect/Graphics/RenderTarget.h"
    "Hect/Graphics/Shader.cpp"
    "Hect/Graphics/Shader.h"
    "Hect/Graphics/ShaderArgument.cpp"
    "Hect/Graphics/ShaderArgument.h"
    "Hect/Graphics/ShaderModule.cpp"
    "Hect/Graphics/ShaderModule.h"
    "Hect/Graphics/ShaderParameter.cpp"
    "Hect/Graphics/ShaderParameter.h"
    "Hect/Graphics/ShaderValue.cpp"
    "Hect/Graphics/ShaderValue.h"
    "Hect/Graphics/Technique.cpp"
    "Hect/Graphics/Technique.h"
    "Hect/Graphics/Texture.cpp"
    "Hect/Graphics/Texture.h"
    "Hect/Graphics/VertexAttribute.cpp"
    "Hect/Graphics/VertexAttribute.h"
    "Hect/Graphics/VertexLayout.cpp"
    "Hect/Graphics/VertexLayout.h"
    "Hect/Graphics/VideoMode.cpp"
    "Hect/Graphics/VideoMode.h"
    "Hect/Graphics/Window.cpp"
    "Hect/Graphics/Window.h"
    "Hect/Input/InputAxis.cpp"
    "Hect/Input/InputAxis.h"
    "Hect/Input/InputAxisBinding.cpp"
    "Hect/Input/InputAxisBinding.h"
    "Hect/Input/Joystick.cpp"
    "Hect/Input/Joystick.h"
    "Hect/Input/Keyboard.cpp"
    "Hect/Input/Keyboard.h"
    "Hect/Input/Mouse.cpp"
    "Hect/Input/Mouse.h"
    "Hect/IO/Asset.cpp"
    "Hect/IO/Asset.h"
    "Hect/IO/AssetCache.cpp"
    "Hect/IO/AssetCache.h"
    "Hect/IO/AssetCache.inl"
    "Hect/IO/AssetDecoder.cpp"
    "Hect/IO/AssetDecoder.h"
    "Hect/IO/AssetEntry.h"
    "Hect/IO/AssetEntry.inl"
    "Hect/IO/AssetHandle.h"
    "Hect/IO/AssetHandle.inl"
    "Hect/IO/BinaryDecoder.cpp"
    "Hect/IO/BinaryDecoder.h"
    "Hect/IO/BinaryEncoder.cpp"
    "Hect/IO/BinaryEncoder.h"
    "Hect/IO/Decoder.cpp"
    "Hect/IO/Decoder.h"
    "Hect/IO/Decoder.inl"
    "Hect/IO/EncodeOperations.cpp"
    "Hect/IO/EncodeOperations.h"
    "Hect/IO/EncodeOperations.inl"
    "Hect/IO/Encoder.cpp"
    "Hect/IO/Encoder.h"
    "Hect/IO/Encoder.inl"
    "Hect/IO/JsonDecoder.cpp"
    "Hect/IO/JsonDecoder.h"
    "Hect/IO/JsonEncoder.cpp"
    "Hect/IO/JsonEncoder.h"
    "Hect/IO/JsonValue.cpp"
    "Hect/IO/JsonValue.h"
    "Hect/IO/MemoryReadStream.cpp"
    "Hect/IO/MemoryReadStream.h"
    "Hect/IO/MemoryWriteStream.cpp"
    "Hect/IO/MemoryWriteStream.h"
    "Hect/IO/Path.cpp"
    "Hect/IO/Path.h"
    "Hect/IO/ReadStream.cpp"
    "Hect/IO/ReadStream.h"
    "Hect/IO/WriteStream.cpp"
    "Hect/IO/WriteStream.h"
    "Hect/IO/Encoders/ImagePngEncoder.cpp"
    "Hect/IO/Encoders/ImagePngEncoder.h"
    "Hect/Logic/Component.cpp"
    "Hect/Logic/Component.h"
    "Hect/Logic/Component.inl"
    "Hect/Logic/ComponentEvent.h"
    "Hect/Logic/ComponentEvent.inl"
    "Hect/Logic/ComponentPool.h"
    "Hect/Logic/ComponentPool.inl"
    "Hect/Logic/ComponentRegistry.cpp"
    "Hect/Logic/ComponentRegistry.h"
    "Hect/Logic/ComponentRegistry.inl"
    "Hect/Logic/Entity.cpp"
    "Hect/Logic/Entity.h"
    "Hect/Logic/Entity.inl"
    "Hect/Logic/EntityEvent.cpp"
    "Hect/Logic/EntityEvent.h"
    "Hect/Logic/EntityPool.cpp"
    "Hect/Logic/EntityPool.h"
    "Hect/Logic/GameMode.cpp"
    "Hect/Logic/GameMode.h"
    "Hect/Logic/GameModeRegistry.cpp"
    "Hect/Logic/GameModeRegistry.h"
    "Hect/Logic/GameModeRegistry.inl"
    "Hect/Logic/IdPool.h"
    "Hect/Logic/IdPool.inl"
    "Hect/Logic/Scene.cpp"
    "Hect/Logic/Scene.h"
    "Hect/Logic/Scene.inl"
    "Hect/Logic/System.cpp"
    "Hect/Logic/System.h"
    "Hect/Logic/System.inl"
    "Hect/Logic/SystemRegistry.cpp"
    "Hect/Logic/SystemRegistry.h"
    "Hect/Logic/SystemRegistry.inl"
    "Hect/Logic/Components/BoundingBox.h"
    "Hect/Logic/Components/Camera.cpp"
    "Hect/Logic/Components/Camera.h"
    "Hect/Logic/Components/DirectionalLight.cpp"
    "Hect/Logic/Components/DirectionalLight.h"
    "Hect/Logic/Components/LightProbe.cpp"
    "Hect/Logic/Components/LightProbe.h"
    "Hect/Logic/Components/Model.cpp"
    "Hect/Logic/Components/Model.h"
    "Hect/Logic/Components/RigidBody.cpp"
    "Hect/Logic/Components/RigidBody.h"
    "Hect/Logic/Components/SkyBox.cpp"
    "Hect/Logic/Components/SkyBox.h"
    "Hect/Logic/Components/Transform.cpp"
    "Hect/Logic/Components/Transform.h"
    "Hect/Logic/Systems/BoundingBoxSystem.cpp"
    "Hect/Logic/Systems/BoundingBoxSystem.h"
    "Hect/Logic/Systems/CameraSystem.cpp"
    "Hect/Logic/Systems/CameraSystem.h"
    "Hect/Logic/Systems/InputSystem.cpp"
    "Hect/Logic/Systems/InputSystem.h"
    "Hect/Logic/Systems/PhysicsSystem.cpp"
    "Hect/Logic/Systems/PhysicsSystem.h"
    "Hect/Logic/Systems/TransformSystem.cpp"
    "Hect/Logic/Systems/TransformSystem.h"
    "Hect/Math/Angle.cpp"
    "Hect/Math/Angle.h"
    "Hect/Math/Constants.h"
    "Hect/Math/Matrix4.h"
    "Hect/Math/Matrix4.inl"
    "Hect/Math/Quaternion.h"
    "Hect/Math/Quaternion.inl"
    "Hect/Math/Utilities.h"
    "Hect/Math/Utilities.inl"
    "Hect/Math/Vector2.h"
    "Hect/Math/Vector2.inl"
    "Hect/Math/Vector3.h"
    "Hect/Math/Vector3.inl"
    "Hect/Math/Vector4.h"
    "Hect/Math/Vector4.inl"
    "Hect/Network/IpAddress.cpp"
    "Hect/Network/IpAddress.h"
    "Hect/Network/Packet.cpp"
    "Hect/Network/Packet.h"
    "Hect/Network/PeerHandle.cpp"
    "Hect/Network/PeerHandle.h"
    "Hect/Network/Socket.cpp"
    "Hect/Network/Socket.h"
    "Hect/Noise/Random.cpp"
    "Hect/Noise/Random.h"
    "Hect/Physics/Bullet.cpp"
    "Hect/Physics/Bullet.h"
    "Hect/Platform/FileSystem.h"
    "Hect/Platform/FileSystem_PhysFS.cpp"
    "Hect/Platform/Platform.h"
    "Hect/Platform/Platform_SDL.cpp"
    "Hect/Reflection/Enum.cpp"
    "Hect/Reflection/Enum.h"
    "Hect/Reflection/Enum.inl"
    "Hect/Reflection/Type.cpp"
    "Hect/Reflection/Type.h"
    "Hect/Reflection/Type.inl"
    "Hect/Runtime/Engine.cpp"
    "Hect/Runtime/Engine.h"
    "Hect/Spacial/AxisAlignedBox.cpp"
    "Hect/Spacial/AxisAlignedBox.h"
    "Hect/Spacial/Box.cpp"
    "Hect/Spacial/Box.h"
    "Hect/Spacial/Frustum.cpp"
    "Hect/Spacial/Frustum.h"
    "Hect/Spacial/Plane.cpp"
    "Hect/Spacial/Plane.h"
    "Hect/Spacial/Sphere.cpp"
    "Hect/Spacial/Sphere.h"
    "Hect/Timing/Timer.cpp"
    "Hect/Timing/Timer.h"
    "Hect/Timing/TimeSpan.cpp"
    "Hect/Timing/TimeSpan.h"
    "Hect/Timing/TimeStamp.h"
    )
    
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})

target_link_libraries(Hect Bullet ENet Glew JsonCpp LodePng PhysFS SDL2-static ${OPENGL_LIBRARIES})
    
set_target_properties(Hect PROPERTIES
    PROJECT_LABEL Hect
    LINKER_LANGUAGE CXX
    FOLDER "/Hect"
    )

add_custom_command(TARGET Hect
    PRE_BUILD
    COMMAND python ${PROJECT_SOURCE_DIR}/Tools/reflect.py ${PROJECT_SOURCE_DIR}/Engine/Hect/ ${PROJECT_SOURCE_DIR}/Engine/Hect/Generated/ hect
)