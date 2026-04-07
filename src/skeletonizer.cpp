#include <fstream>
#include <iostream>
#include <vector>

#include <CGAL/IO/read_ply_points.h>
#include <CGAL/property_map.h>
#include <CGAL/Scale_space_surface_reconstruction_3.h>
#include <CGAL/Advancing_front_surface_reconstruction.h>

#include "skeletonizer.h"

Skeletonizer::Skeletonizer(const std::string& labeledPlyPath)
    : inputPath(labeledPlyPath) {}

void Skeletonizer::run(const std::string& outputPlyPath)
{
    loadAndReconstruct();
    skeletonize(outputPlyPath);
}

/// @brief loads the ply file and reconstructs it from point cloud
void Skeletonizer::loadAndReconstruct()
{
    using PointWithNormal = std::pair<Point3, CGAL::Vector_3<Kernel>>;
    std::vector<PointWithNormal> points;

    std::ifstream in(inputPath, std::ios::binary);
    if (!in) {
        std::cerr << "Failed to open: " << inputPath << "\n";
        return;
    }

    CGAL::IO::read_PLY_with_properties(
        in,
        std::back_inserter(points),
        CGAL::make_ply_point_reader(
            CGAL::First_of_pair_property_map<PointWithNormal>()),
        CGAL::make_ply_normal_reader(
            CGAL::Second_of_pair_property_map<PointWithNormal>())
        );
    std::cout << "Loaded " << points.size() << " beam points\n";

    // extract just the Point3s for advancing front
    std::vector<Point3> pts;
    pts.reserve(points.size());

    for (auto& p : points)
        pts.push_back(p.first);

    MeshConstruct construct(mesh, pts.begin(), pts.end());
    CGAL::advancing_front_surface_reconstruction(pts.begin(), pts.end(), construct);

    std::cout << "Mesh: " << mesh.number_of_vertices()
              << " vertices, "   << mesh.number_of_faces() << " faces\n";
}

/// @brief uses Mean-Curvature-Flow algorithm to compute a skeleton and exports it
void Skeletonizer::skeletonize(const std::string& outputPath) {
    Skeleton skeleton;
    Skeletonization mcf(mesh);

    mcf.contract_until_convergence();
    mcf.convert_to_skeleton(skeleton);

    std::cout << "Skeleton: "
              << boost::num_vertices(skeleton) << " vertices, "
              << boost::num_edges(skeleton) << " edges\n";

    exportSkeleton(skeleton, outputPath);
}

void Skeletonizer::exportSkeleton(const Skeleton& skeleton,
                                  const std::string& outputPath) {
    std::ofstream out(outputPath);

    out << "ply\n"
        << "format ascii 1.0\n"
        << "element vertex " << boost::num_vertices(skeleton) << "\n"
        << "property double x\n"
        << "property double y\n"
        << "property double z\n"
        << "element edge " << boost::num_edges(skeleton) << "\n"
        << "property int vertex1\n"
        << "property int vertex2\n"
        << "end_header\n";

    for (auto v : CGAL::make_range(boost::vertices(skeleton)))
        out << skeleton[v].point << "\n";

    for (auto e : CGAL::make_range(boost::edges(skeleton)))
        out << boost::source(e, skeleton) << " "
            << boost::target(e, skeleton) << "\n";

    std::cout << "Skeleton exported: " << outputPath << "\n";
}
