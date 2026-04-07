cmake_minimum_required(VERSION 3.16...3.29)
project(SkeleFormer)

if(POLICY CMP0167)
  cmake_policy(SET CMP0167 NEW)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- Dependencies ---
find_package(Eigen3 3.2.0 REQUIRED)
find_package(CGAL REQUIRED)
# find_package(Qt6 REQUIRED COMPONENTS Core Widgets)
# find_package(Qt6 OPTIONAL_COMPONENTS 3DCore 3DRender 3DExtras)

include(${CGAL_MODULES_DIR}/CGAL_Macros.cmake)
set(CGAL_EIGEN3_ENABLED True)

# --- Core library ---
add_library(core STATIC
    src/skeletonizer.cpp
    src/util.h
    src/skeletonizer.h
)
target_include_directories(core PUBLIC ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(core PUBLIC
    CGAL::CGAL
    Eigen3::Eigen
    # Qt6::Core
)

# --- Main executable ---
add_executable(skeleformer
    src/main.cpp
    src/viewer.cpp
)
target_link_libraries(skeleformer PRIVATE
    core
    # Qt6::Core
    # Qt6::Widgets
    # $<$<TARGET_EXISTS:Qt6::3DCore>:Qt6::3DCore>
    # $<$<TARGET_EXISTS:Qt6::3DRender>:Qt6::3DRender>
    # $<$<TARGET_EXISTS:Qt6::3DExtras>:Qt6::3DExtras>
)

# Force C++17 and treat CGAL as a system library to ignore its internal warnings
target_compile_features(core PUBLIC cxx_std_17)
target_link_libraries(core PUBLIC
    Eigen3::Eigen
    # Qt6::Core
)

# Specifically disable the offending warning for this target
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "AppleClang")
    target_compile_options(core PUBLIC -Wno-unknown-warning-option) # Handle older clang
    target_compile_options(core PUBLIC -Wno-missing-template-arg-list-after-template-kw)
endif()
