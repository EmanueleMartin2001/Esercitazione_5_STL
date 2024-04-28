#pragma once

#include <iostream>
#include "PolygonalMesh.hpp"

using namespace std;
bool ImportMesh(const string& filepath, PolygonalLibrary::PolygonalMesh& mesh); //tests on the mesh, returns true if everything is ok

bool ImportCell0Ds(const string &filename, PolygonalLibrary::PolygonalMesh& mesh); //import Cell0d type of data

bool ImportCell1Ds(const string &filename, PolygonalLibrary::PolygonalMesh& mesh); //import Cell1d type of data

bool ImportCell2Ds(const string &filename,PolygonalLibrary::PolygonalMesh& mesh); //import Cell2d type of data
