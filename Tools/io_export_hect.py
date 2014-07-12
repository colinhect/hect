#!BPY

bl_info = {
    "name": "Hect Mesh Format (.mesh)",
    "author": "Colin Hill",
    "version": (1, 0),
    "blender": (2, 6, 9),
    "api": 37702,
    "location": "File > Export > Hect (.mesh)",
    "description": "Export Hect Mesh Format (.mesh)",
    "warning": "",
    "wiki_url": "",
    "tracker_url": "",
    "category": "Import-Export"}

import os
import bpy
import struct

from bpy.props import StringProperty, EnumProperty, BoolProperty

# VertexAttributeSemantic (Hect/Graphics/VertexAttributeSemantic.h)
VertexAttributeSemantic_Position = 0
VertexAttributeSemantic_Normal = 1
VertexAttributeSemantic_Color = 2
VertexAttributeSemantic_Tangent = 3
VertexAttributeSemantic_Binormal = 4

# VertexAttributeType (Hect/Graphics/VertexAttributeType.h)
VertexAttributeType_Byte = 0
VertexAttributeType_UnsignedByte = 1
VertexAttributeType_Short = 2
VertexAttributeType_UnsignedShort = 3
VertexAttributeType_Int = 4
VertexAttributeType_UnsignedIn = 5
VertexAttributeType_Half = 6
VertexAttributeType_Float = 7

# IndexType (Hect/Graphics/IndexType.h)
IndexType_UnsignedByte = 0
IndexType_UnsignedShort = 1
IndexType_UnsignedInt = 2

# PrimitiveType (Hect/Graphics/PrimitiveType.h)
PrimitiveType_Triangles = 0
PrimitiveType_TriangleStrip = 1
PrimitiveType_Lines = 2
PrimitiveType_LineStrip = 3
PrimitiveType_Points = 4

def mesh_triangulate(me):
    import bmesh
    bm = bmesh.new()
    bm.from_mesh(me)
    bmesh.ops.triangulate(bm, faces=bm.faces)
    bm.to_mesh(me)
    bm.free()

def export_mesh(obj, path):
    mesh = obj.data

    if not mesh.tessfaces and mesh.polygons:
        mesh.calc_tessface()

    mesh.calc_normals()

    mesh_triangulate(mesh)

    out = open(path + "." + obj.name + ".mesh", 'wb')

    out.write(struct.pack("<Q", 0x84629573))
    out.write(struct.pack("<I", 2))

    out.write(struct.pack("<B", VertexAttributeSemantic_Position))
    out.write(struct.pack("<B", VertexAttributeType_Float))
    out.write(struct.pack("<I", 3))

    out.write(struct.pack("<B", VertexAttributeSemantic_Normal))
    out.write(struct.pack("<B", VertexAttributeType_Float))
    out.write(struct.pack("<I", 3))

    out.write(struct.pack("<B", IndexType_UnsignedShort))
    out.write(struct.pack("<B", PrimitiveType_Triangles))

    vertex_count = len(mesh.vertices)
    out.write(struct.pack("<I", vertex_count * 4 * 3 * 2))
    for i in range(vertex_count):
        co = mesh.vertices[i].co
        for j in range(3):
            out.write(struct.pack("<f", co[j]))

        normal = mesh.vertices[i].normal
        for j in range(3):
            out.write(struct.pack("<f", normal[j]))

    polygon_count = len(mesh.polygons)
    out.write(struct.pack("<I", polygon_count * 3 * 2))
    for i in range(polygon_count):
        indices = mesh.polygons[i].vertices
        for j in range(3):
            out.write(struct.pack("<H", indices[j]))

    out.close()

class HectExporter(bpy.types.Operator):
    """Export to the Hect mesh format (.mesh)"""

    bl_idname = "export.hect"
    bl_label = "Export Hect"

    filepath = StringProperty(subtype='FILE_PATH')

    def execute(self, context):
        path = bpy.path.ensure_ext(self.filepath, ".mesh")
        path = path[:-5]

        for obj in bpy.context.scene.objects:
            if obj.type == 'MESH':
                export_mesh(obj, path)

        return {"FINISHED"}

    def invoke(self, context, event):
        if not self.filepath:
            self.filepath = bpy.path.ensure_ext(bpy.data.filepath, ".mesh")
        WindowManager = context.window_manager
        WindowManager.fileselect_add(self)
        return {"RUNNING_MODAL"}

def menu_func(self, context):
    self.layout.operator(HectExporter.bl_idname, text="Hect (.mesh)")

def register():
    bpy.utils.register_module(__name__)
    bpy.types.INFO_MT_file_export.append(menu_func)

def unregister():
    bpy.utils.unregister_module(__name__)
    bpy.types.INFO_MT_file_export.remove(menu_func)
