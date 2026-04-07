#ifndef SKELETONIZER_H
#define SKELETONIZER_H

// stl
#include <string>
#include <vector>

// cgal
#include <CGAL/Simple_cartesian.h>
#include <CGAL/Surface_mesh.h>
#include <CGAL/Mean_curvature_flow_skeletonization.h>
#include <CGAL/Point_set_3.h>

#include <CGAL/IO/read_ply_points.h>

// EPIC for more robustness when reconstructing the surface from point cloud
using Kernel            = CGAL::Exact_predicates_inexact_constructions_kernel;
using Point3            = Kernel::Point_3;
using Facet             = std::array<std::size_t, 3>;

using SurfaceMesh       = CGAL::Surface_mesh<Point3>;

using Skeletonization   = CGAL::Mean_curvature_flow_skeletonization<SurfaceMesh>;
using Skeleton          = Skeletonization::Skeleton;


/// @brief Construct functor — writes advancing front output directly into SurfaceMesh
struct MeshConstruct {
    SurfaceMesh& mesh;

    template<typename PointIterator>
    MeshConstruct(SurfaceMesh& mesh, PointIterator b, PointIterator e)
        : mesh(mesh)
    {
        for (; b != e; ++b) {
            auto v = add_vertex(mesh);
            mesh.point(v) = *b;
        }
    }

    MeshConstruct& operator=(const Facet& f) {
        using vd = boost::graph_traits<SurfaceMesh>::vertex_descriptor;
        using sz = boost::graph_traits<SurfaceMesh>::vertices_size_type;
        mesh.add_face(vd(static_cast<sz>(f[0])),
                      vd(static_cast<sz>(f[1])),
                      vd(static_cast<sz>(f[2])));
        return *this;
    }

    MeshConstruct& operator*()     { return *this; }
    MeshConstruct& operator++()    { return *this; }
    MeshConstruct  operator++(int) { return *this; }
};

/// @brief computes a skeleton of point cloud
class Skeletonizer {
public:
    explicit Skeletonizer(const std::string& labeledPlyPath);

    void run(const std::string& outputPlyPath);

private:
    std::string inputPath;
    SurfaceMesh mesh;

    void loadAndReconstruct();
    void skeletonize(const std::string& outputPath);
    void exportSkeleton(const Skeleton& skeleton, const std::string& outputPath);
};


#endif // SKELETONIZER_H
