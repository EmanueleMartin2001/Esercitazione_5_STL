#pragma once

//struct per le polygonalmesh

#include <iostream>
#include "Eigen/eigen"

using namespace std;
using namespace Eigen;

namespace PolygonalLibrary
{

struct PolygonalMesh{

    unsigned int NumberCell0D = 0; ///< number of Cell0D
    std::vector<unsigned int> Cell0DId = {}; ///< Cell0D id, size 1 x NumberCell0D
    std::vector<Vector2d> Cell0DCoordinates = {}; ///< Cell0D coordinates, size 2 x NumberCell0D (x,y)
    std::map<unsigned int, list<unsigned int>> Cell0DMarkers = {}; ///< Cell0D markers, size 1 x NumberCell0D (marker)


    unsigned int NumberCell1D = 0; ///< number of Cell1D
    std::vector<unsigned int> Cell1DId = {}; ///< Cell1D id, size 1 x NumberCell1D
    std::vector<Vector2i> Cell1DVertices = {}; ///< Cell1D vertices indices, size 2 x NumberCell1D (fromId,toId)
    std::map<unsigned int, list<unsigned int>> Cell1DMarkers = {}; ///< Cell1D properties, size 1 x NumberCell1D (marker)


    unsigned int NumberCell2D = 0; ///< number of Cell2D
    std::vector<unsigned int> Cell2DId = {}; ///< Cell2D id, size 1 x NumberCell2D
    std::map<unsigned int, list<unsigned int>> Cell2DMarkers = {}; ///< Cell1D properties, size 1 x NumberCell1D (marker)
    std::vector<std::vector<unsigned int>> Cell2DVertices = {}; ///< Cell2D Vertices indices, size 1 x NumberCell2DVertices[NumberCell2D]
    std::vector<std::vector<unsigned int>> Cell2DEdges = {};    ///< Cell2D Cell1D indices, size 1 x NumberCell2DEdges[NumberCell2D]

};

}
