# Semantic Skeleton 3D
This project is to demonstrate the hybrid 3d semanatic pipeline for skeletonization purposes. A transformer model segments a given raw point cloud by class. Then afterwards, CGAL module extracts the topological skeleton of the target structure.

## Motivation
Most of the deep learning models classify points well but the result is volumentric and noisy outputs. These kind of outputs are not suitable for application where precision is key (Industrial CAD). This project experiments with a mix of transformer with correct geometric algorithm to get the best of the both worlds: semantic understanding and structural precision.

---
 
## Pipeline
 
### Stage 1 — Semantic Segmentation (Python)
- Dataset: [S3DIS](http://buildingparser.stanford.edu/dataset.html) (Stanford Large-Scale 3D Indoor Spaces)
- Model: **Point Transformer V3** (PTv3, CVPR 2024 Oral) via [Pointcept](https://github.com/Pointcept/Pointcept)
- Output: per-point semantic labels exported as a colored `.ply`
- Target class: `beam` (label 3) — thin, elongated ceiling structures
 
### Stage 2 — Geometric Skeletonization (C++17)
- Input: isolated beam point cloud from Stage 1
- Surface reconstruction via Ball-Pivoting Algorithm (Open3D)
- Skeletonization: **CGAL Mean Curvature Flow** (`extract_mean_curvature_flow_skeleton`)
- Output: 1D skeleton as a polyline
 
### Stage 3 — Visualization (C++ / Open3D)
Three-layer interactive viewer with keypress toggle:
- Layer 1: raw point cloud (grey, low opacity)
- Layer 2: AI-classified beam points (red)
- Layer 3: geometric skeleton (bright, overlaid)
 
---

## Build
 
**Dependencies:** CGAL, Open3D, CMake ≥ 3.16

```
pip install -r requirements.txt
```


---

## Repo Structure
 
```
SkeleFormer/
├── python/
│   └── infer.py          # PTv3 segmentation.
│   ├── viewer.py         # Open3D python viewer
├── src/
│   ├── main.cpp
│   ├── util.h            # I/O and other CGAL utility
│   ├── skeletonizer.cpp  # CGAL MCF skeletonization
│   └── viewer.cpp        # Qt 3D viewer
├── assets/
│   ├── labeled.ply       # sample AI-labeled cloud
│   └── skeleton.ply      # sample skeleton output
└── CMakeLists.txt
```

 ---

 ## TODO:
 - [x] Python inference pipeline via PTv3 and S3DIS
 - [ ] CGAL MCF Module
 - [x] Open3D view (Python for now)
 - [ ] Qt3D real-time viewer (will also replace the calling pipeline)
   - [ ] with layer toggle
- [ ] LibTorch interface 

## References
 
- [Point Transformer V3 — CVPR 2024](https://arxiv.org/abs/2312.10035)
- [Pointcept framework](https://github.com/Pointcept/Pointcept)
- [S3DIS Dataset](http://buildingparser.stanford.edu/dataset.html)
- [CGAL Mean Curvature Flow Skeletonization](https://doc.cgal.org/latest/Surface_mesh_skeletonization/index.html)