# Engine

# Disable useless Visual Studio 2013 warnings
if(WIN32)
    add_definitions("/D_CRT_SECURE_NO_WARNINGS")
endif()

include_directories(
    "${PROJECT_SOURCE_DIR}/Engine/Source"
    "${PROJECT_SOURCE_DIR}/Dependencies/Bullet"
    "${PROJECT_SOURCE_DIR}/Dependencies/ENet"
    "${PROJECT_SOURCE_DIR}/Dependencies/Glew"
    "${PROJECT_SOURCE_DIR}/Dependencies/JsonCpp"
    "${PROJECT_SOURCE_DIR}/Dependencies/LodePng"
    "${PROJECT_SOURCE_DIR}/Dependencies/PhysFS"
    "${PROJECT_SOURCE_DIR}/Dependencies/SDL2/include"
    "${PROJECT_SOURCE_DIR}/Dependencies/Tclap/include"
    )
    
file(GLOB concurrency_files Source/Hect/Concurrency/*)
source_group(Hect\\Concurrency FILES ${concurrency_files})

file(GLOB core_files Source/Hect/Core/*)
source_group(Hect\\Core FILES ${core_files})

file(GLOB event_files Source/Hect/Event/*)
source_group(Hect\\Event FILES ${event_files})

file(GLOB graphics_files Source/Hect/Graphics/*)
source_group(Hect\\Graphics FILES ${graphics_files})

file(GLOB input_files Source/Hect/Input/*)
source_group(Hect\\Input FILES ${input_files})

file(GLOB io_files Source/Hect/IO/*)
source_group(Hect\\IO FILES ${io_files})

file(GLOB encoders_files Source/Hect/IO/Encoders/*)
source_group(Hect\\IO\\Encoders FILES ${encoders_files})

file(GLOB logic_files Source/Hect/Logic/*)
source_group(Hect\\Logic FILES ${logic_files})

file(GLOB components_files Source/Hect/Logic/Components/*)
source_group(Hect\\Logic\\Components FILES ${components_files})

file(GLOB systems_files Source/Hect/Logic/Systems/*)
source_group(Hect\\Logic\\Systems FILES ${systems_files})

file(GLOB math_files Source/Hect/Math/*)
source_group(Hect\\Math FILES ${math_files})

file(GLOB network_files Source/Hect/Network/*)
source_group(Hect\\Network FILES ${network_files})

file(GLOB noise_files Source/Hect/Noise/*)
source_group(Hect\\Noise FILES ${noise_files})

file(GLOB physics_files Source/Hect/Physics/*)
source_group(Hect\\Physics FILES ${physics_files})

file(GLOB platform_files Source/Hect/Platform/*)
source_group(Hect\\Platform FILES ${platform_files})

file(GLOB reflection_files Source/Hect/Reflection/*)
source_group(Hect\\Reflection FILES ${reflection_files})

file(GLOB runtime_files Source/Hect/Runtime/*)
source_group(Hect\\Runtime FILES ${runtime_files})

file(GLOB spacial_files Source/Hect/Spacial/*)
source_group(Hect\\Spacial FILES ${spacial_files})

file(GLOB timing_files Source/Hect/Timing/*)
source_group(Hect\\Timing FILES ${timing_files})
    
add_library(Hect STATIC
    "Source/Hect/Concurrency/Task.cpp"
    "Source/Hect/Concurrency/Task.h"
    "Source/Hect/Concurrency/TaskPool.cpp"
    "Source/Hect/Concurrency/TaskPool.h"
    "Source/Hect/Core/Any.cpp"
    "Source/Hect/Core/Any.h"
    "Source/Hect/Core/Any.inl"
    "Source/Hect/Core/Configuration.h"
    "Source/Hect/Core/Error.cpp"
    "Source/Hect/Core/Error.h"
    "Source/Hect/Core/Format.cpp"
    "Source/Hect/Core/Format.h"
    "Source/Hect/Core/Logging.cpp"
    "Source/Hect/Core/Logging.h"
    "Source/Hect/Core/Real.h"
    "Source/Hect/Core/Sequence.h"
    "Source/Hect/Core/Sequence.inl"
    "Source/Hect/Core/Uncopyable.cpp"
    "Source/Hect/Core/Uncopyable.h"
    "Source/Hect/Event/Dispatcher.h"
    "Source/Hect/Event/Dispatcher.inl"
    "Source/Hect/Event/Listener.h"
    "Source/Hect/Graphics/FrameBuffer.cpp"
    "Source/Hect/Graphics/FrameBuffer.h"
    "Source/Hect/Graphics/GraphicsContext.cpp"
    "Source/Hect/Graphics/GraphicsContext.h"
    "Source/Hect/Graphics/GraphicsContext.inl"
    "Source/Hect/Graphics/Image.cpp"
    "Source/Hect/Graphics/Image.h"
    "Source/Hect/Graphics/Material.cpp"
    "Source/Hect/Graphics/Material.h"
    "Source/Hect/Graphics/Mesh.cpp"
    "Source/Hect/Graphics/Mesh.h"
    "Source/Hect/Graphics/MeshReader.cpp"
    "Source/Hect/Graphics/MeshReader.h"
    "Source/Hect/Graphics/MeshWriter.cpp"
    "Source/Hect/Graphics/MeshWriter.h"
    "Source/Hect/Graphics/Pass.cpp"
    "Source/Hect/Graphics/Pass.h"
    "Source/Hect/Graphics/Renderer.cpp"
    "Source/Hect/Graphics/Renderer.h"
    "Source/Hect/Graphics/RenderState.cpp"
    "Source/Hect/Graphics/RenderState.h"
    "Source/Hect/Graphics/RenderTarget.cpp"
    "Source/Hect/Graphics/RenderTarget.h"
    "Source/Hect/Graphics/Shader.cpp"
    "Source/Hect/Graphics/Shader.h"
    "Source/Hect/Graphics/ShaderArgument.cpp"
    "Source/Hect/Graphics/ShaderArgument.h"
    "Source/Hect/Graphics/ShaderModule.cpp"
    "Source/Hect/Graphics/ShaderModule.h"
    "Source/Hect/Graphics/ShaderParameter.cpp"
    "Source/Hect/Graphics/ShaderParameter.h"
    "Source/Hect/Graphics/ShaderValue.cpp"
    "Source/Hect/Graphics/ShaderValue.h"
    "Source/Hect/Graphics/Technique.cpp"
    "Source/Hect/Graphics/Technique.h"
    "Source/Hect/Graphics/Texture.cpp"
    "Source/Hect/Graphics/Texture.h"
    "Source/Hect/Graphics/VertexAttribute.cpp"
    "Source/Hect/Graphics/VertexAttribute.h"
    "Source/Hect/Graphics/VertexLayout.cpp"
    "Source/Hect/Graphics/VertexLayout.h"
    "Source/Hect/Graphics/VideoMode.cpp"
    "Source/Hect/Graphics/VideoMode.h"
    "Source/Hect/Graphics/Window.cpp"
    "Source/Hect/Graphics/Window.h"
    "Source/Hect/Input/InputAxis.cpp"
    "Source/Hect/Input/InputAxis.h"
    "Source/Hect/Input/InputAxisBinding.cpp"
    "Source/Hect/Input/InputAxisBinding.h"
    "Source/Hect/Input/Joystick.cpp"
    "Source/Hect/Input/Joystick.h"
    "Source/Hect/Input/Keyboard.cpp"
    "Source/Hect/Input/Keyboard.h"
    "Source/Hect/Input/Mouse.cpp"
    "Source/Hect/Input/Mouse.h"
    "Source/Hect/IO/Asset.cpp"
    "Source/Hect/IO/Asset.h"
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
    "Source/Hect/IO/Decoder.cpp"
    "Source/Hect/IO/Decoder.h"
    "Source/Hect/IO/Decoder.inl"
    "Source/Hect/IO/EncodeOperations.cpp"
    "Source/Hect/IO/EncodeOperations.h"
    "Source/Hect/IO/EncodeOperations.inl"
    "Source/Hect/IO/Encoder.cpp"
    "Source/Hect/IO/Encoder.h"
    "Source/Hect/IO/Encoder.inl"
    "Source/Hect/IO/JsonDecoder.cpp"
    "Source/Hect/IO/JsonDecoder.h"
    "Source/Hect/IO/JsonEncoder.cpp"
    "Source/Hect/IO/JsonEncoder.h"
    "Source/Hect/IO/JsonValue.cpp"
    "Source/Hect/IO/JsonValue.h"
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
    "Source/Hect/IO/Encoders/ImagePngEncoder.cpp"
    "Source/Hect/IO/Encoders/ImagePngEncoder.h"
    "Source/Hect/Logic/Component.cpp"
    "Source/Hect/Logic/Component.h"
    "Source/Hect/Logic/Component.inl"
    "Source/Hect/Logic/ComponentEvent.h"
    "Source/Hect/Logic/ComponentEvent.inl"
    "Source/Hect/Logic/ComponentPool.h"
    "Source/Hect/Logic/ComponentPool.inl"
    "Source/Hect/Logic/ComponentRegistry.cpp"
    "Source/Hect/Logic/ComponentRegistry.h"
    "Source/Hect/Logic/ComponentRegistry.inl"
    "Source/Hect/Logic/Entity.cpp"
    "Source/Hect/Logic/Entity.h"
    "Source/Hect/Logic/Entity.inl"
    "Source/Hect/Logic/EntityEvent.cpp"
    "Source/Hect/Logic/EntityEvent.h"
    "Source/Hect/Logic/EntityPool.cpp"
    "Source/Hect/Logic/EntityPool.h"
    "Source/Hect/Logic/GameMode.cpp"
    "Source/Hect/Logic/GameMode.h"
    "Source/Hect/Logic/GameModeRegistry.cpp"
    "Source/Hect/Logic/GameModeRegistry.h"
    "Source/Hect/Logic/GameModeRegistry.inl"
    "Source/Hect/Logic/IdPool.h"
    "Source/Hect/Logic/IdPool.inl"
    "Source/Hect/Logic/Scene.cpp"
    "Source/Hect/Logic/Scene.h"
    "Source/Hect/Logic/Scene.inl"
    "Source/Hect/Logic/System.cpp"
    "Source/Hect/Logic/System.h"
    "Source/Hect/Logic/System.inl"
    "Source/Hect/Logic/SystemRegistry.cpp"
    "Source/Hect/Logic/SystemRegistry.h"
    "Source/Hect/Logic/SystemRegistry.inl"
    "Source/Hect/Logic/Components/BoundingBox.h"
    "Source/Hect/Logic/Components/Camera.cpp"
    "Source/Hect/Logic/Components/Camera.h"
    "Source/Hect/Logic/Components/DirectionalLight.cpp"
    "Source/Hect/Logic/Components/DirectionalLight.h"
    "Source/Hect/Logic/Components/LightProbe.cpp"
    "Source/Hect/Logic/Components/LightProbe.h"
    "Source/Hect/Logic/Components/Model.cpp"
    "Source/Hect/Logic/Components/Model.h"
    "Source/Hect/Logic/Components/RigidBody.cpp"
    "Source/Hect/Logic/Components/RigidBody.h"
    "Source/Hect/Logic/Components/SkyBox.cpp"
    "Source/Hect/Logic/Components/SkyBox.h"
    "Source/Hect/Logic/Components/Transform.cpp"
    "Source/Hect/Logic/Components/Transform.h"
    "Source/Hect/Logic/Systems/BoundingBoxSystem.cpp"
    "Source/Hect/Logic/Systems/BoundingBoxSystem.h"
    "Source/Hect/Logic/Systems/CameraSystem.cpp"
    "Source/Hect/Logic/Systems/CameraSystem.h"
    "Source/Hect/Logic/Systems/InputSystem.cpp"
    "Source/Hect/Logic/Systems/InputSystem.h"
    "Source/Hect/Logic/Systems/PhysicsSystem.cpp"
    "Source/Hect/Logic/Systems/PhysicsSystem.h"
    "Source/Hect/Logic/Systems/TransformSystem.cpp"
    "Source/Hect/Logic/Systems/TransformSystem.h"
    "Source/Hect/Math/Angle.cpp"
    "Source/Hect/Math/Angle.h"
    "Source/Hect/Math/Constants.h"
    "Source/Hect/Math/Matrix4.h"
    "Source/Hect/Math/Matrix4.inl"
    "Source/Hect/Math/Quaternion.h"
    "Source/Hect/Math/Quaternion.inl"
    "Source/Hect/Math/Utilities.h"
    "Source/Hect/Math/Utilities.inl"
    "Source/Hect/Math/Vector2.h"
    "Source/Hect/Math/Vector2.inl"
    "Source/Hect/Math/Vector3.h"
    "Source/Hect/Math/Vector3.inl"
    "Source/Hect/Math/Vector4.h"
    "Source/Hect/Math/Vector4.inl"
    "Source/Hect/Network/IpAddress.cpp"
    "Source/Hect/Network/IpAddress.h"
    "Source/Hect/Network/Packet.cpp"
    "Source/Hect/Network/Packet.h"
    "Source/Hect/Network/PeerHandle.cpp"
    "Source/Hect/Network/PeerHandle.h"
    "Source/Hect/Network/Socket.cpp"
    "Source/Hect/Network/Socket.h"
    "Source/Hect/Noise/Random.cpp"
    "Source/Hect/Noise/Random.h"
    "Source/Hect/Physics/Bullet.cpp"
    "Source/Hect/Physics/Bullet.h"
    "Source/Hect/Platform/FileSystem.h"
    "Source/Hect/Platform/FileSystem_PhysFS.cpp"
    "Source/Hect/Platform/Platform.h"
    "Source/Hect/Platform/Platform_SDL.cpp"
    "Source/Hect/Reflection/Enum.cpp"
    "Source/Hect/Reflection/Enum.h"
    "Source/Hect/Reflection/Enum.inl"
    "Source/Hect/Reflection/Type.cpp"
    "Source/Hect/Reflection/Type.h"
    "Source/Hect/Reflection/Type.inl"
    "Source/Hect/Runtime/Engine.cpp"
    "Source/Hect/Runtime/Engine.h"
    "Source/Hect/Spacial/AxisAlignedBox.cpp"
    "Source/Hect/Spacial/AxisAlignedBox.h"
    "Source/Hect/Spacial/Box.cpp"
    "Source/Hect/Spacial/Box.h"
    "Source/Hect/Spacial/Frustum.cpp"
    "Source/Hect/Spacial/Frustum.h"
    "Source/Hect/Spacial/Plane.cpp"
    "Source/Hect/Spacial/Plane.h"
    "Source/Hect/Spacial/Sphere.cpp"
    "Source/Hect/Spacial/Sphere.h"
    "Source/Hect/Timing/Timer.cpp"
    "Source/Hect/Timing/Timer.h"
    "Source/Hect/Timing/TimeSpan.cpp"
    "Source/Hect/Timing/TimeSpan.h"
    "Source/Hect/Timing/TimeStamp.h"
    )
    
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})

add_dependencies(Hect SDL2)
target_link_libraries(Hect Bullet ENet Glew JsonCpp LodePng PhysFS SDL2-static ${OPENGL_LIBRARIES})
    
set_target_properties(Hect PROPERTIES
    PROJECT_LABEL Hect
    LINKER_LANGUAGE CXX
    FOLDER "/Engine"
    )

# Generate reflection code
add_custom_command(TARGET Hect
    PRE_BUILD
    COMMAND python ${PROJECT_SOURCE_DIR}/Engine/Tools/reflect.py ${PROJECT_SOURCE_DIR}/Engine/Source/Hect/ ${PROJECT_SOURCE_DIR}/Engine/Source/Hect/Generated/ hect
    )

# Copy assets
add_custom_command(TARGET Hect
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/Engine/Assets ${OUTPUT_DIR}/Hect.data/
    )