# @author Timur Sinnurow
# @date April 6th 2026
# @file viewer.py

import open3d as o3d
import numpy as np
import sys

def load_skeleton_as_lineset(ply_path):
    # skeleton.ply will have points as vertices and edges as lines
    # we store it as a lineset PLY from C++
    ls = o3d.io.read_line_set(ply_path)
    ls.paint_uniform_color([1.0, 0.9, 0.0])  # bright yellow
    return ls

raw     = o3d.io.read_point_cloud("../assets/labeled.ply")
skel    = load_skeleton_as_lineset("../assets/skeleton.ply")

room_colors = np.asarray(raw.colors)
raw.colors = o3d.utility.Vector3dVector(room_colors * 0.6)

# Dim the background points, keep beams red
vis = o3d.visualization.Visualizer()
vis.create_window("Semantic Skeleton", width=1280, height=720)
vis.add_geometry(raw)
vis.add_geometry(skel)

opt = vis.get_render_option()
opt.point_size = 1.5
opt.background_color = [0.05, 0.05, 0.05]
opt.line_width = 3.0  # thick skeleton lines

vis.run()
vis.destroy_window()