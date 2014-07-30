// Generated by reflect.py
#include <Hect/Reflection/Type.h>

#include "..\Core\Logging.h"
#include "..\Graphics\Image.h"
#include "..\Graphics\Mesh.h"
#include "..\Graphics\RenderState.h"
#include "..\Graphics\ShaderModule.h"
#include "..\Graphics\Texture.h"
#include "..\Graphics\Uniform.h"
#include "..\Graphics\UniformValue.h"
#include "..\Graphics\VertexAttribute.h"
#include "..\IO\JsonValue.h"
#include "..\Input\InputAxis.h"
#include "..\Input\Keyboard.h"
#include "..\Input\Mouse.h"
#include "..\Logic\ComponentEvent.h"
#include "..\Logic\EntityEvent.h"
#include "..\Network\Packet.h"
#include "..\Network\PeerHandle.h"
#include "..\Network\Socket.h"
#include "..\Reflection\Type.h"
#include "..\Spacial\Frustum.h"

class HectTypes { };

template <>
void hect::Type::registerTypes<HectTypes>()
{
    {
        hect::Type& type = hect::Type::create<hect::LogLevel>(hect::Kind_Enum, "hect::LogLevel");
        hect::Enum& enumType = type.asEnum();
        enumType.addValue(LogLevel_Info, "Info");
        enumType.addValue(LogLevel_Debug, "Debug");
        enumType.addValue(LogLevel_Warning, "Warning");
        enumType.addValue(LogLevel_Error, "Error");
        enumType.addValue(LogLevel_Trace, "Trace");
    }
    {
        hect::Type& type = hect::Type::create<hect::ColorSpace>(hect::Kind_Enum, "hect::ColorSpace");
        hect::Enum& enumType = type.asEnum();
        enumType.addValue(ColorSpace_NonLinear, "NonLinear");
        enumType.addValue(ColorSpace_Linear, "Linear");
    }
    {
        hect::Type& type = hect::Type::create<hect::PixelType>(hect::Kind_Enum, "hect::PixelType");
        hect::Enum& enumType = type.asEnum();
        enumType.addValue(PixelType_Half, "Half");
        enumType.addValue(PixelType_Float, "Float");
        enumType.addValue(PixelType_Byte, "Byte");
    }
    {
        hect::Type& type = hect::Type::create<hect::PixelFormat>(hect::Kind_Enum, "hect::PixelFormat");
        hect::Enum& enumType = type.asEnum();
        enumType.addValue(PixelFormat_Rgb, "Rgb");
        enumType.addValue(PixelFormat_Rgba, "Rgba");
    }
    {
        hect::Type& type = hect::Type::create<hect::PrimitiveType>(hect::Kind_Enum, "hect::PrimitiveType");
        hect::Enum& enumType = type.asEnum();
        enumType.addValue(PrimitiveType_Triangles, "Triangles");
        enumType.addValue(PrimitiveType_TriangleStrip, "TriangleStrip");
        enumType.addValue(PrimitiveType_Lines, "Lines");
        enumType.addValue(PrimitiveType_LineStrip, "LineStrip");
        enumType.addValue(PrimitiveType_Points, "Points");
    }
    {
        hect::Type& type = hect::Type::create<hect::IndexType>(hect::Kind_Enum, "hect::IndexType");
        hect::Enum& enumType = type.asEnum();
        enumType.addValue(IndexType_UnsignedByte, "UnsignedByte");
        enumType.addValue(IndexType_UnsignedShort, "UnsignedShort");
        enumType.addValue(IndexType_UnsignedInt, "UnsignedInt");
    }
    {
        hect::Type& type = hect::Type::create<hect::BlendFactor>(hect::Kind_Enum, "hect::BlendFactor");
        hect::Enum& enumType = type.asEnum();
        enumType.addValue(BlendFactor_Zero, "Zero");
        enumType.addValue(BlendFactor_One, "One");
        enumType.addValue(BlendFactor_SourceColor, "SourceColor");
        enumType.addValue(BlendFactor_OneMinusSourceColor, "OneMinusSourceColor");
        enumType.addValue(BlendFactor_DestColor, "DestColor");
        enumType.addValue(BlendFactor_OneMinusDestColor, "OneMinusDestColor");
        enumType.addValue(BlendFactor_SourceAlpha, "SourceAlpha");
        enumType.addValue(BlendFactor_OneMinusSourceAlpha, "OneMinusSourceAlpha");
        enumType.addValue(BlendFactor_DestAlpha, "DestAlpha");
        enumType.addValue(BlendFactor_OneMinusDestAlpha, "OneMinusDestAlpha");
    }
    {
        hect::Type& type = hect::Type::create<hect::RenderStateFlag>(hect::Kind_Enum, "hect::RenderStateFlag");
        hect::Enum& enumType = type.asEnum();
        enumType.addValue(RenderStateFlag_Blend, "Blend");
        enumType.addValue(RenderStateFlag_DepthTest, "DepthTest");
        enumType.addValue(RenderStateFlag_DepthWrite, "DepthWrite");
        enumType.addValue(RenderStateFlag_CullFace, "CullFace");
    }
    {
        hect::Type& type = hect::Type::create<hect::ShaderModuleType>(hect::Kind_Enum, "hect::ShaderModuleType");
        hect::Enum& enumType = type.asEnum();
        enumType.addValue(ShaderModuleType_Vertex, "Vertex");
        enumType.addValue(ShaderModuleType_Pixel, "Pixel");
        enumType.addValue(ShaderModuleType_Geometry, "Geometry");
    }
    {
        hect::Type& type = hect::Type::create<hect::TextureFilter>(hect::Kind_Enum, "hect::TextureFilter");
        hect::Enum& enumType = type.asEnum();
        enumType.addValue(TextureFilter_Nearest, "Nearest");
        enumType.addValue(TextureFilter_Linear, "Linear");
    }
    {
        hect::Type& type = hect::Type::create<hect::TextureType>(hect::Kind_Enum, "hect::TextureType");
        hect::Enum& enumType = type.asEnum();
        enumType.addValue(TextureType_2D, "2D");
        enumType.addValue(TextureType_CubeMap, "CubeMap");
    }
    {
        hect::Type& type = hect::Type::create<hect::UniformBinding>(hect::Kind_Enum, "hect::UniformBinding");
        hect::Enum& enumType = type.asEnum();
        enumType.addValue(UniformBinding_None, "None");
        enumType.addValue(UniformBinding_RenderTargetSize, "RenderTargetSize");
        enumType.addValue(UniformBinding_CameraPosition, "CameraPosition");
        enumType.addValue(UniformBinding_CameraFront, "CameraFront");
        enumType.addValue(UniformBinding_CameraUp, "CameraUp");
        enumType.addValue(UniformBinding_ViewMatrix, "ViewMatrix");
        enumType.addValue(UniformBinding_ProjectionMatrix, "ProjectionMatrix");
        enumType.addValue(UniformBinding_ViewProjectionMatrix, "ViewProjectionMatrix");
        enumType.addValue(UniformBinding_ModelMatrix, "ModelMatrix");
        enumType.addValue(UniformBinding_ModelViewMatrix, "ModelViewMatrix");
        enumType.addValue(UniformBinding_ModelViewProjectionMatrix, "ModelViewProjectionMatrix");
    }
    {
        hect::Type& type = hect::Type::create<hect::UniformType>(hect::Kind_Enum, "hect::UniformType");
        hect::Enum& enumType = type.asEnum();
        enumType.addValue(UniformType_Int, "Int");
        enumType.addValue(UniformType_Float, "Float");
        enumType.addValue(UniformType_Vector2, "Vector2");
        enumType.addValue(UniformType_Vector3, "Vector3");
        enumType.addValue(UniformType_Vector4, "Vector4");
        enumType.addValue(UniformType_Matrix4, "Matrix4");
        enumType.addValue(UniformType_Texture, "Texture");
    }
    {
        hect::Type& type = hect::Type::create<hect::VertexAttributeSemantic>(hect::Kind_Enum, "hect::VertexAttributeSemantic");
        hect::Enum& enumType = type.asEnum();
        enumType.addValue(VertexAttributeSemantic_Position, "Position");
        enumType.addValue(VertexAttributeSemantic_Normal, "Normal");
        enumType.addValue(VertexAttributeSemantic_Color, "Color");
        enumType.addValue(VertexAttributeSemantic_Tangent, "Tangent");
        enumType.addValue(VertexAttributeSemantic_Binormal, "Binormal");
        enumType.addValue(VertexAttributeSemantic_Weight0, "Weight0");
        enumType.addValue(VertexAttributeSemantic_Weight1, "Weight1");
        enumType.addValue(VertexAttributeSemantic_Weight2, "Weight2");
        enumType.addValue(VertexAttributeSemantic_Weight3, "Weight3");
        enumType.addValue(VertexAttributeSemantic_TextureCoords0, "TextureCoords0");
        enumType.addValue(VertexAttributeSemantic_TextureCoords1, "TextureCoords1");
        enumType.addValue(VertexAttributeSemantic_TextureCoords2, "TextureCoords2");
        enumType.addValue(VertexAttributeSemantic_TextureCoords3, "TextureCoords3");
    }
    {
        hect::Type& type = hect::Type::create<hect::VertexAttributeType>(hect::Kind_Enum, "hect::VertexAttributeType");
        hect::Enum& enumType = type.asEnum();
        enumType.addValue(VertexAttributeType_Byte, "Byte");
        enumType.addValue(VertexAttributeType_UnsignedByte, "UnsignedByte");
        enumType.addValue(VertexAttributeType_Short, "Short");
        enumType.addValue(VertexAttributeType_UnsignedShort, "UnsignedShort");
        enumType.addValue(VertexAttributeType_Int, "Int");
        enumType.addValue(VertexAttributeType_UnsignedInt, "UnsignedInt");
        enumType.addValue(VertexAttributeType_Half, "Half");
        enumType.addValue(VertexAttributeType_Float, "Float");
    }
    {
        hect::Type& type = hect::Type::create<hect::InputAxisSource>(hect::Kind_Enum, "hect::InputAxisSource");
        hect::Enum& enumType = type.asEnum();
        enumType.addValue(InputAxisSource_MouseMoveX, "MouseMoveX");
        enumType.addValue(InputAxisSource_MouseMoveY, "MouseMoveY");
        enumType.addValue(InputAxisSource_MouseButton, "MouseButton");
        enumType.addValue(InputAxisSource_MouseScroll, "MouseScroll");
        enumType.addValue(InputAxisSource_Key, "Key");
    }
    {
        hect::Type& type = hect::Type::create<hect::Key>(hect::Kind_Enum, "hect::Key");
        hect::Enum& enumType = type.asEnum();
        enumType.addValue(Key_Unknown, "Unknown");
        enumType.addValue(Key_A, "A");
        enumType.addValue(Key_B, "B");
        enumType.addValue(Key_C, "C");
        enumType.addValue(Key_D, "D");
        enumType.addValue(Key_E, "E");
        enumType.addValue(Key_F, "F");
        enumType.addValue(Key_G, "G");
        enumType.addValue(Key_H, "H");
        enumType.addValue(Key_I, "I");
        enumType.addValue(Key_J, "J");
        enumType.addValue(Key_K, "K");
        enumType.addValue(Key_L, "L");
        enumType.addValue(Key_M, "M");
        enumType.addValue(Key_N, "N");
        enumType.addValue(Key_O, "O");
        enumType.addValue(Key_P, "P");
        enumType.addValue(Key_Q, "Q");
        enumType.addValue(Key_R, "R");
        enumType.addValue(Key_S, "S");
        enumType.addValue(Key_T, "T");
        enumType.addValue(Key_U, "U");
        enumType.addValue(Key_V, "V");
        enumType.addValue(Key_W, "W");
        enumType.addValue(Key_X, "X");
        enumType.addValue(Key_Y, "Y");
        enumType.addValue(Key_Z, "Z");
        enumType.addValue(Key_Num0, "Num0");
        enumType.addValue(Key_Num1, "Num1");
        enumType.addValue(Key_Num2, "Num2");
        enumType.addValue(Key_Num3, "Num3");
        enumType.addValue(Key_Num4, "Num4");
        enumType.addValue(Key_Num5, "Num5");
        enumType.addValue(Key_Num6, "Num6");
        enumType.addValue(Key_Num7, "Num7");
        enumType.addValue(Key_Num8, "Num8");
        enumType.addValue(Key_Num9, "Num9");
        enumType.addValue(Key_Esc, "Esc");
        enumType.addValue(Key_Space, "Space");
        enumType.addValue(Key_Enter, "Enter");
        enumType.addValue(Key_Backspace, "Backspace");
        enumType.addValue(Key_Tab, "Tab");
        enumType.addValue(Key_Tick, "Tick");
        enumType.addValue(Key_F1, "F1");
        enumType.addValue(Key_F2, "F2");
        enumType.addValue(Key_F3, "F3");
        enumType.addValue(Key_F4, "F4");
        enumType.addValue(Key_F5, "F5");
        enumType.addValue(Key_F6, "F6");
        enumType.addValue(Key_F7, "F7");
        enumType.addValue(Key_F8, "F8");
        enumType.addValue(Key_F9, "F9");
        enumType.addValue(Key_F10, "F10");
        enumType.addValue(Key_F11, "F11");
        enumType.addValue(Key_F12, "F12");
        enumType.addValue(Key_LeftCtrl, "LeftCtrl");
        enumType.addValue(Key_LeftShift, "LeftShift");
        enumType.addValue(Key_LeftAlt, "LeftAlt");
    }
    {
        hect::Type& type = hect::Type::create<hect::KeyboardEventType>(hect::Kind_Enum, "hect::KeyboardEventType");
        hect::Enum& enumType = type.asEnum();
        enumType.addValue(KeyboardEventType_KeyDown, "KeyDown");
        enumType.addValue(KeyboardEventType_KeyUp, "KeyUp");
    }
    {
        hect::Type& type = hect::Type::create<hect::MouseButton>(hect::Kind_Enum, "hect::MouseButton");
        hect::Enum& enumType = type.asEnum();
        enumType.addValue(MouseButton_Left, "Left");
        enumType.addValue(MouseButton_Right, "Right");
        enumType.addValue(MouseButton_Middle, "Middle");
    }
    {
        hect::Type& type = hect::Type::create<hect::MouseMode>(hect::Kind_Enum, "hect::MouseMode");
        hect::Enum& enumType = type.asEnum();
        enumType.addValue(MouseMode_Cursor, "Cursor");
        enumType.addValue(MouseMode_Relative, "Relative");
    }
    {
        hect::Type& type = hect::Type::create<hect::MouseEventType>(hect::Kind_Enum, "hect::MouseEventType");
        hect::Enum& enumType = type.asEnum();
        enumType.addValue(MouseEventType_Movement, "Movement");
        enumType.addValue(MouseEventType_ButtonDown, "ButtonDown");
        enumType.addValue(MouseEventType_ButtonUp, "ButtonUp");
        enumType.addValue(MouseEventType_ScrollDown, "ScrollDown");
        enumType.addValue(MouseEventType_ScrollUp, "ScrollUp");
    }
    {
        hect::Type& type = hect::Type::create<hect::JsonValueType>(hect::Kind_Enum, "hect::JsonValueType");
        hect::Enum& enumType = type.asEnum();
        enumType.addValue(JsonValueType_Null, "Null");
        enumType.addValue(JsonValueType_Bool, "Bool");
        enumType.addValue(JsonValueType_Number, "Number");
        enumType.addValue(JsonValueType_String, "String");
        enumType.addValue(JsonValueType_Array, "Array");
        enumType.addValue(JsonValueType_Object, "Object");
    }
    {
        hect::Type& type = hect::Type::create<hect::ComponentEventType>(hect::Kind_Enum, "hect::ComponentEventType");
        hect::Enum& enumType = type.asEnum();
        enumType.addValue(ComponentEventType_Add, "Add");
        enumType.addValue(ComponentEventType_Remove, "Remove");
    }
    {
        hect::Type& type = hect::Type::create<hect::EntityEventType>(hect::Kind_Enum, "hect::EntityEventType");
        hect::Enum& enumType = type.asEnum();
        enumType.addValue(EntityEventType_Create, "Create");
        enumType.addValue(EntityEventType_Activate, "Activate");
        enumType.addValue(EntityEventType_Destroy, "Destroy");
    }
    {
        hect::Type& type = hect::Type::create<hect::PacketFlag>(hect::Kind_Enum, "hect::PacketFlag");
        hect::Enum& enumType = type.asEnum();
        enumType.addValue(PacketFlag_Reliable, "Reliable");
        enumType.addValue(PacketFlag_Unsequenced, "Unsequenced");
    }
    {
        hect::Type& type = hect::Type::create<hect::PeerState>(hect::Kind_Enum, "hect::PeerState");
        hect::Enum& enumType = type.asEnum();
        enumType.addValue(PeerState_Disconnected, "Disconnected");
        enumType.addValue(PeerState_Connecting, "Connecting");
        enumType.addValue(PeerState_AcknowledgingConnect, "AcknowledgingConnect");
        enumType.addValue(PeerState_ConnectionPending, "ConnectionPending");
        enumType.addValue(PeerState_ConnectionSucceeded, "ConnectionSucceeded");
        enumType.addValue(PeerState_Connected, "Connected");
        enumType.addValue(PeerState_DisconnectLater, "DisconnectLater");
        enumType.addValue(PeerState_Disconnecting, "Disconnecting");
        enumType.addValue(PeerState_AcknowledgeDisconnect, "AcknowledgeDisconnect");
        enumType.addValue(PeerState_Unknown, "Unknown");
    }
    {
        hect::Type& type = hect::Type::create<hect::SocketEventType>(hect::Kind_Enum, "hect::SocketEventType");
        hect::Enum& enumType = type.asEnum();
        enumType.addValue(SocketEventType_None, "None");
        enumType.addValue(SocketEventType_Connect, "Connect");
        enumType.addValue(SocketEventType_Disconnect, "Disconnect");
        enumType.addValue(SocketEventType_Receive, "Receive");
    }
    {
        hect::Type& type = hect::Type::create<hect::Kind>(hect::Kind_Enum, "hect::Kind");
        hect::Enum& enumType = type.asEnum();
        enumType.addValue(Kind_None, "None");
        enumType.addValue(Kind_Namespace, "Namespace");
        enumType.addValue(Kind_Class, "Class");
        enumType.addValue(Kind_Enum, "Enum");
    }
    {
        hect::Type& type = hect::Type::create<hect::FrustumTestResult>(hect::Kind_Enum, "hect::FrustumTestResult");
        hect::Enum& enumType = type.asEnum();
        enumType.addValue(FrustumTestResult_Outside, "Outside");
        enumType.addValue(FrustumTestResult_Inside, "Inside");
        enumType.addValue(FrustumTestResult_Intersect, "Intersect");
    }
}
