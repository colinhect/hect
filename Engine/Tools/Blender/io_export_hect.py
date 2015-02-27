#!BPY

bl_info = {
    "name": "Hect Mesh Format (.mesh)",
    "author": "Colin Hill",
    "version": (1, 0),
    "blender": (2, 7, 1),
    "api": 35622,
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
from bpy_extras.io_utils import ExportHelper

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

def vertex_from_loop(mesh, loop_index):
    uv_layer = mesh.uv_layers.active.data
    loop = mesh.loops[loop_index]
    vert = mesh.vertices[loop.vertex_index]
    normal = loop.normal
    uv = uv_layer[loop_index].uv
    return Vertex(vert.co, vert.normal, loop.tangent, uv)

def export_mesh(context, mesh_obj, path):
    bpy.ops.object.mode_set(mode='OBJECT')

    # Prepare the mesh
    mesh = mesh_obj.to_mesh(context.scene, True, 'PREVIEW')
    mesh.transform(mesh_obj.matrix_world)
    mesh.calc_normals()
    mesh.calc_tangents()

    out = open(path + "." + mesh_obj.name + ".mesh", 'wb')

    # Write vertex layout
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

    # Write index layout and primitive type
    out.write(struct.pack("<B", IndexType.uint32))
    out.write(struct.pack("<B", PrimitiveType.triangles))

    # Build vertex list
    vertices = []
    for poly in mesh.polygons:
        loop_indices = poly.loop_indices
        if len(loop_indices) >= 3:
            vertices.append(vertex_from_loop(mesh, loop_indices[0]))
            vertices.append(vertex_from_loop(mesh, loop_indices[1]))
            vertices.append(vertex_from_loop(mesh, loop_indices[2]))
        if len(loop_indices) == 4:
            vertices.append(vertex_from_loop(mesh, loop_indices[2]))
            vertices.append(vertex_from_loop(mesh, loop_indices[3]))
            vertices.append(vertex_from_loop(mesh, loop_indices[0]))

    # Write the vertex list
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

    # Write the index list
    out.write(struct.pack("<I", len(mesh.polygons) * 4 * 3))
    for i in range(len(mesh.polygons) * 3):
        out.write(struct.pack("<I", i))

    out.close()

class HectExporter(bpy.types.Operator, ExportHelper):
    """Export to the Hect mesh format (.mesh)"""

    bl_idname = "export.hect"
    bl_label = "Export Hect Mesh (.mesh)"
    filename_ext = ".mesh"

    filepath = StringProperty(subtype='FILE_PATH')

    def execute(self, context):
        # Remove the file extension
        path = bpy.path.ensure_ext(self.filepath, ".mesh")
        path = path[:-5]

        # Get all of the selected mesh objects
        objs = []
        for obj in bpy.context.scene.objects:
            if obj.type == 'MESH' and obj.select:
                objs.append(obj)

        # Export the selected mesh objects
        for obj in objs:
            export_mesh(context, obj, path)

        return {"FINISHED"}

    def invoke(self, context, event):
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
