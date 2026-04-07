#include "skeletonizer.h"

int main()
{
    // TODO: use stl 17 directory path things to find the path properly
    // runs skeletonization on the beam
    Skeletonizer skel("../../assets/beam_only.ply");
    skel.run("../../assets/skeleton.ply");
}
