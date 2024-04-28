#include "PolygonalMesh.hpp"
#include "Utils.hpp"

#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;
using namespace Eigen;
using namespace PolygonalLibrary;
/* In ImportMesh sono presenti i diversi test da effettuare sulla mesh:
 * 1)marker registrati con successo
 * 2)congruenza vertici dei poligoni
 * 3)lati di lunghezza non nulla
 * 4)aree dei poligoni non nulle
*/
int main()
{
    string filepath = "PolygonalMesh";
    PolygonalMesh mesh;
    ImportMesh(filepath + "/Cell0Ds.csv",mesh);
    return 0;
}

