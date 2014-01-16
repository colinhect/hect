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

from bpy.props import StringProperty, EnumProperty, BoolProperty

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

    out = open(path + "." + obj.name + ".mesh", 'w')
    out.write("{\n")

    out.write("    \"vertices\" :\n")
    out.write("    [\n")
    vertex_count = len(mesh.vertices)
    for i in range(vertex_count):
        co = mesh.vertices[i].co
        normal = mesh.vertices[i].normal

        out.write("        [\n");
        out.write("            { \"semantic\" : \"Position\", \"data\" : [ %f, %f, %f ] },\n" % (co[0], co[2], -co[1]))
        out.write("            { \"semantic\" : \"Normal\", \"data\" : [ %f, %f, %f ] }\n" % (normal[0], normal[2], -normal[1]))
        if i != vertex_count - 1:
            out.write("        ],\n");
        else:
            out.write("        ]\n");
    out.write("    ],\n")

    out.write("    \"indices\" :\n")
    out.write("    [\n")
    polygon_count = len(mesh.polygons)
    for i in range(polygon_count):
        indices = mesh.polygons[i].vertices
        out.write("        %d, %d, %d" % (indices[0], indices[1], indices[2]))
        if i != polygon_count - 1:
            out.write(",\n");
        else:
            out.write("\n");
    out.write("    ]\n")

    out.write("}")
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
