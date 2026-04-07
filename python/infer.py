# @author Timur Sinnurow
# @date April 6th 2026
# @file infer.py

import numpy as np
import open3d as o3d

ROOM = "../Pointcept/data/s3dis/Area_5/office_39" # path to room with data set
TARGET_LABEL = 3  # beam

coord  = np.load(f"{ROOM}/coord.npy")    # (N, 3)
color  = np.load(f"{ROOM}/color.npy")    # (N, 3) uint8
seg    = np.load(f"{ROOM}/segment.npy").flatten()  # (N,)

# Build color map: beam=red, rest=dark grey
colors = np.full((len(coord), 3), 0.2)   # grey background
colors[seg == TARGET_LABEL] = [1.0, 0.1, 0.1]  # red beams

pcd = o3d.geometry.PointCloud()
pcd.points = o3d.utility.Vector3dVector(coord.astype(np.float64))
pcd.colors = o3d.utility.Vector3dVector(colors)

o3d.io.write_point_cloud("../assets/labeled.ply", pcd)

# export beam-only cloud for C++ skeletonizer
beamMask = (seg == TARGET_LABEL)
beamPoints = coord[beamMask].astype(np.float64)
beamNormals = np.load(f"{ROOM}/normal.npy")[beamMask].astype(np.float64)

beamCloud = o3d.geometry.PointCloud()
beamCloud.points = o3d.utility.Vector3dVector(beamPoints)
beamCloud.normals = o3d.utility.Vector3dVector(beamNormals)

o3d.io.write_point_cloud("../assets/beam_only.ply", beamCloud)
print(f"Exported {beamMask.sum()} beam points to assets/beam_only.ply")

beam_count = (seg == TARGET_LABEL).sum()
print(f"Exported {len(coord)} points ({beam_count} beams) to assets/labeled.ply")