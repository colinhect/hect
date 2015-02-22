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
import bmesh
import bpy
import struct

from bpy.props import StringProperty, EnumProperty, BoolProperty

# VertexAttributeSemantic (Hect/Graphics/VertexAttributeSemantic.h)
VertexAttributeSemantic_Position = 0
VertexAttributeSemantic_Normal = 1
VertexAttributeSemantic_Color = 2
VertexAttributeSemantic_Tangent = 3
VertexAttributeSemantic_Binormal = 4
VertexAttributeSemantic_Weight0 = 5
VertexAttributeSemantic_Weight1 = 6
VertexAttributeSemantic_Weight2 = 7
VertexAttributeSemantic_Weight3 = 8
VertexAttributeSemantic_TextureCoords0 = 9
VertexAttributeSemantic_TextureCoords1 = 10
VertexAttributeSemantic_TextureCoords2 = 11
VertexAttributeSemantic_TextureCoords3 = 2

# VertexAttributeType (Hect/Graphics/VertexAttributeType.h)
VertexAttributeType_Int8 = 0
VertexAttributeType_UInt8 = 1
VertexAttributeType_Int16 = 2
VertexAttributeType_UInt16 = 3
VertexAttributeType_Int32 = 4
VertexAttributeType_UInt32 = 5
VertexAttributeType_Float16 = 6
VertexAttributeType_Float32 = 7

# IndexType (Hect/Graphics/IndexType.h)
IndexType_UInt8 = 0
IndexType_UInt16 = 1
IndexType_UInt32 = 2

# PrimitiveType (Hect/Graphics/PrimitiveType.h)
PrimitiveType_Triangles = 0
PrimitiveType_TriangleStrip = 1
PrimitiveType_Lines = 2
PrimitiveType_LineStrip = 3
PrimitiveType_Points = 4

def export_mesh(obj, path, append_name):
    mesh = obj.data

    if not mesh.tessfaces and mesh.polygons:
        mesh.calc_tessface()

    mesh.calc_normals()

    bm = bmesh.new()
    bm.from_mesh(mesh)
    bmesh.ops.triangulate(bm, faces=bm.faces)
    uv_lay = bm.loops.layers.uv.active

    filepath = path
    if append_name:
        filepath += "." + obj.name + ".mesh"
    
    filepath += ".mesh"

    out = open(filepath, 'wb')

    out.write(struct.pack("<I", 4))

    out.write(struct.pack("<B", VertexAttributeSemantic_Position))
    out.write(struct.pack("<B", VertexAttributeType_Float32))
    out.write(struct.pack("<I", 3))

    out.write(struct.pack("<B", VertexAttributeSemantic_Normal))
    out.write(struct.pack("<B", VertexAttributeType_Float32))
    out.write(struct.pack("<I", 3))

    out.write(struct.pack("<B", VertexAttributeSemantic_Tangent))
    out.write(struct.pack("<B", VertexAttributeType_Float32))
    out.write(struct.pack("<I", 3))

    out.write(struct.pack("<B", VertexAttributeSemantic_TextureCoords0))
    out.write(struct.pack("<B", VertexAttributeType_Float32))
    out.write(struct.pack("<I", 2))

    out.write(struct.pack("<B", IndexType_UInt32))
    out.write(struct.pack("<B", PrimitiveType_Triangles))

    vertex_count = len(bm.verts)
    out.write(struct.pack("<I", vertex_count * 4 * (3 + 3 + 3 + 2)))
    for i in range(vertex_count):
        vertex = bm.verts[i]

        co = vertex.co
        for j in range(3):
            out.write(struct.pack("<f", co[j]))

        vertex.normal_update()
        normal = vertex.normal
        for j in range(3):
            out.write(struct.pack("<f", normal[j]))

        loop = vertex.link_loops[0]

        tangent = loop.calc_tangent()
        for j in range(3):
            out.write(struct.pack("<f", tangent[j]))

        uv = loop[uv_lay].uv if not uv_lay is None else [0, 0]
        for j in range(2):
            out.write(struct.pack("<f", uv[j]))

    face_count = len(bm.faces)
    out.write(struct.pack("<I", face_count * 4 * 3))
    for i in range(face_count):
        verts = bm.faces[i].verts
        for j in range(3):
            out.write(struct.pack("<I", verts[j].index))

    out.close()
    bm.free()

class HectExporter(bpy.types.Operator):
    """Export to the Hect mesh format (.mesh)"""

    bl_idname = "export.hect"
    bl_label = "Export Hect"

    filepath = StringProperty(subtype='FILE_PATH')

    def execute(self, context):
        # Remove the file extension from the path
        path = bpy.path.ensure_ext(self.filepath, ".mesh")
        path = path[:-5]

        # Get all of the mesh objects
        objs = []
        for obj in bpy.context.scene.objects:
            if obj.type == 'MESH':
                objs.append(obj)

        # Only append the object name if we are exporting more than one mesh
        append_name = len(objs) > 1
        for obj in objs:
            export_mesh(obj, path, append_name)

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
