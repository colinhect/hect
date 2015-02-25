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
class VertexAttributeSemantic:
    position = 0
    normal = 1
    color = 2
    tangent = 3
    binormal = 4
    weight0 = 5
    weight1 = 6
    weight2 = 7
    weight3 = 8
    texturecoords0 = 9
    texturecoords1 = 10
    texturecoords2 = 11
    texturecoords3 = 12

# VertexAttributeType (Hect/Graphics/VertexAttributeType.h)
class VertexAttributeType:
    int8 = 0
    uint8 = 1
    int16 = 2
    uint16 = 3
    int32 = 4
    uint32 = 5
    float16 = 6
    float32 = 7

# IndexType (Hect/Graphics/IndexType.h)
class IndexType:
    uint8 = 0
    uint16 = 1
    uint32 = 2

# PrimitiveType (Hect/Graphics/PrimitiveType.h)
class PrimitiveType:
    triangles = 0
    trianglestrip = 1
    lines = 2
    linestrip = 3
    points = 4

class Vertex:
    def __init__(self, co, normal, tangent, uv):
        self.co = co
        self.normal = normal
        self.tangent = tangent
        self.uv = uv

def mesh_triangulate(me):
    import bmesh
    bm = bmesh.new()
    bm.from_mesh(me)
    bmesh.ops.triangulate(bm, faces=bm.faces)
    bm.to_mesh(me)
    bm.free()

def export_mesh(obj, path, append_name):
    mesh = obj.data

    if not mesh.tessfaces and mesh.polygons:
        mesh.calc_tessface()

    mesh.calc_normals()
    mesh_triangulate(mesh)

    filepath = path
    if append_name:
        filepath += "." + obj.name + ".mesh"
    
    filepath += ".mesh"

    out = open(filepath, 'wb')

    out.write(struct.pack("<I", 4))

    out.write(struct.pack("<B", VertexAttributeSemantic.position))
    out.write(struct.pack("<B", VertexAttributeType.float32))
    out.write(struct.pack("<I", 3))

    out.write(struct.pack("<B", VertexAttributeSemantic.normal))
    out.write(struct.pack("<B", VertexAttributeType.float32))
    out.write(struct.pack("<I", 3))

    out.write(struct.pack("<B", VertexAttributeSemantic.tangent))
    out.write(struct.pack("<B", VertexAttributeType.float32))
    out.write(struct.pack("<I", 3))

    out.write(struct.pack("<B", VertexAttributeSemantic.texturecoords0))
    out.write(struct.pack("<B", VertexAttributeType.float32))
    out.write(struct.pack("<I", 2))

    out.write(struct.pack("<B", IndexType.uint32))
    out.write(struct.pack("<B", PrimitiveType.triangles))

    uv_layer = mesh.uv_layers.active

    vertices = []
    for polygon in mesh.polygons:
        indices = polygon.vertices
        loop_indices = polygon.loop_indices
        for i in range(3):
            co = mesh.vertices[indices[i]].co
            normal = mesh.vertices[indices[i]].normal
            tangent = [0, 0, 0]
            uv = uv_layer.data[loop_indices[i]].uv
            vertices.append(Vertex(co, normal, tangent, uv))

    out.write(struct.pack("<I", len(vertices) * 4 * (3 + 3 + 3 + 2)))
    for vertex in vertices:
        for i in range(3):
            out.write(struct.pack("<f", vertex.co[i]))
        for i in range(3):
            out.write(struct.pack("<f", vertex.normal[i]))
        for i in range(3):
            out.write(struct.pack("<f", vertex.tangent[i]))
        for i in range(2):
            out.write(struct.pack("<f", vertex.uv[i]))

    out.write(struct.pack("<I", len(mesh.polygons) * 4 * 3))
    for i in range(len(mesh.polygons) * 3):
        out.write(struct.pack("<I", i))

    out.close()

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
