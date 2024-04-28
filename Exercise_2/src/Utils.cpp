#include "Utils.hpp"
#include "PolygonalMesh.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>
using namespace std;
using namespace PolygonalLibrary;

bool ImportMesh(const string& filepath, //test
                PolygonalMesh& mesh)
{

    /*CHECK MARKERS INCELL0*/
    if(!ImportCell0Ds(filepath + "/Cell0Ds.csv",
                       mesh))
    {
        return false;
    }
    else
    {
        cout << "Cell0D marker:" << endl;
        for(auto it = mesh.Cell0DMarkers.begin(); it != mesh.Cell0DMarkers.end(); it++) //auto operator detect the type of it automatically
        {
            cout << "key:\t" << it -> first << "\t values:"; //dereference it to first (key)
            for(const auto& id : it->second){
                cout << "\t" << id;
            }
            cout << endl;
        }
    }

    /*CHECK MARKERS INCELL1*/
    if(!ImportCell1Ds(filepath + "/Cell1Ds.csv",
                       mesh))
    {
        return false;
    }
    else
    {
        cout << "Cell1D marker:" << endl;
        for(auto it = mesh.Cell1DMarkers.begin(); it != mesh.Cell1DMarkers.end(); it++)
        {
            cout << "key:\t" << it -> first << "\t values:";
            for(const unsigned int id : it -> second)
                cout << "\t" << id;

            cout << endl;
        }
    }

    /*CHECK IF VERTICES OF POLYGONS ARE CORRECT*/
    if(!ImportCell2Ds(filepath + "/Cell2Ds.csv",
                       mesh))
    {
        return false;
    }
    else
    {
        //
        double epsilon = numeric_limits<double>::epsilon();
        double x1;
        double y1;
        double x2;
        double y2;
        for(unsigned int i = 0; i < mesh.NumberCell2D; i++)
        {
            //vector<unsigned int> vertices = mesh.Cell2DEdges[i];
            vector<unsigned int> edges = mesh.Cell2DEdges[i]; //edges of the i-th polygon; edges[j] is the j-th edge's id of the i-th polygon

            for(unsigned int j = 0; j < edges.size(); j++){
                const unsigned int origin = mesh.Cell1DVertices[edges[j]][0]; //goes in Cell1Vertices, to the i-th edge and take the first input
                const unsigned int end = mesh.Cell1DVertices[edges[j]][1];

                auto findOrigin = find(mesh.Cell2DVertices[i].begin(), mesh.Cell2DVertices[i].end(), origin); //check if the origin vertice exist in the polygon that contain the particular edge that has the origin as a vertex
                if (findOrigin == mesh.Cell2DVertices[i].end()){
                    cerr << "Wrong mesh" << endl;
                    return false;
                }
                auto findEnd = find(mesh.Cell2DVertices[i].begin(),mesh.Cell2DVertices[i].end(),end); //same for the end vertex
                if (findEnd == mesh.Cell2DVertices[i].end()){
                    cerr << "Wrong mesh" << endl;
                    return false;
                }
            }
        }
        /*CHECK IF EDGES HAVE NON-ZERO LENGTH*/
        for(unsigned int i = 0; i < mesh.NumberCell2D; i++) {
            vector<unsigned int> edges = mesh.Cell2DEdges[i];
            for(unsigned int j = 0; j < edges.size(); j++){
                double distance;
                const unsigned int vertex1 = mesh.Cell1DVertices[edges[j]][0]; //vertex 1
                const unsigned int vertex2 = mesh.Cell1DVertices[edges[j]][1]; //vertex 2
                x1 = mesh.Cell0DCoordinates[vertex1](0); //coordinates of the first vertex
                y1 = mesh.Cell0DCoordinates[vertex1](1);
                x2 = mesh.Cell0DCoordinates[vertex2](0); //coordinates of the second vertex
                y2 = mesh.Cell0DCoordinates[vertex2](1);
                distance = sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
                if (distance < 10*epsilon){ //there is no need to normalize in this case (max(|vertex1|,|vertex2|,1) = 1 always)
                    cout << "edges collapsed to a point" << endl;
                    return false;
                }
            }

        }
        /*CHECK IF AREAS OF POLYGONS ARE NON ZERO */
        for (unsigned int i = 0; i < mesh.NumberCell2D; i++){
            vector<unsigned int> edges = mesh.Cell2DEdges[i];
            double area;
            for (unsigned int j = 0; j < edges.size(); j++){
                const unsigned int vertex1 = mesh.Cell1DVertices[edges[j]][0];
                const unsigned int vertex2 = mesh.Cell1DVertices[edges[j]][1];
                x1 = mesh.Cell0DCoordinates[vertex1](0);
                y1 = mesh.Cell0DCoordinates[vertex1](1);
                x2 = mesh.Cell0DCoordinates[vertex2](0);
                y2 = mesh.Cell0DCoordinates[vertex2](1);
                area = area + x1*y2 - y1*x2;
            }
            area = 0.5*abs(area);
            if (area < sqrt(3)/4*(epsilon*10)*(epsilon*10)){ //area of the smallest polygon (triangular triangle with edge long 10*epsilon
                cout << "polygon collapsed" << endl;
                return false;
            }
        }
    }
    return true;
}


bool ImportCell0Ds(const string &filename,
                   PolygonalMesh& mesh) //import Cell1d data type
{
    ifstream file;
    file.open(filename);

    if(file.fail())
        return false;

    list<string> listLines;
    string line;
    while (getline(file, line)){
        listLines.push_back(line);
    }
    listLines.pop_front(); //remove the first line Id;Marker;X;Y
    mesh.NumberCell0D = listLines.size();
    file.close();
    if (mesh.NumberCell0D == 0)
    {
        cerr << "There is no cell 0D" << endl; //check if the file is empty
        return false;
    }

    char separator = ';';

    mesh.Cell0DId.reserve(mesh.NumberCell0D);       //reserve space so that things can be stored
    mesh.Cell0DCoordinates.reserve(mesh.NumberCell0D);
    for (const string& line : listLines)//that is the range-based for loop version; one can alterantively write the extended form with iterators:
    {
        //for (auto itor = listLines.begin();itor !=  listLines.end(); itor++) //is the same for loop
        istringstream converter(line);
        unsigned int id;
        unsigned int marker;
        Vector2d coord;
        string info;

        /*ID*/
        getline(converter,info,separator);
        id = stoul(info);
        mesh.Cell0DId.push_back(id);

        /*MARKER*/
        getline(converter,info,separator);
        marker = stoul(info);
        if (marker != 0){
            if (mesh.Cell0DMarkers.find(marker) == mesh.Cell0DMarkers.end()){ //if the key (marker) is not found find() return an iterator which points at the end of Cell0Markers
                mesh.Cell0DMarkers.insert({marker, {id}}); //mesh.Cell0DMarkers.end() is an iterator that points to the position just after the end of Cell0DMarkers
            }
            else{
                mesh.Cell0DMarkers[marker].push_back(id);
            }
        }

        /*COORD*/
        getline(converter,info,separator);
        coord(0) = stod(info);
        getline(converter,info,separator);
        coord(1) = stod(info);
        mesh.Cell0DCoordinates.push_back(coord);

        }
    file.close();
    return true;
}

bool ImportCell1Ds(const string &filename,
                   PolygonalLibrary::PolygonalMesh& mesh) //import Cell1d data type
{
    ifstream file;
    file.open(filename);

    if(file.fail())
        return false;

    list<string> listLines;
    string line;
    while (getline(file, line))
        listLines.push_back(line);

    listLines.pop_front();

    mesh.NumberCell1D = listLines.size();

    if (mesh.NumberCell1D == 0)
    {
        cerr << "There is no cell 1D" << endl;
        return false;
    }

    char separator = ';';

    mesh.Cell1DId.reserve(mesh.NumberCell1D);
    mesh.Cell1DVertices.reserve(mesh.NumberCell1D);

    for (const string& line : listLines)
    {
        istringstream converter(line);

        unsigned int id;
        unsigned int marker;
        Vector2i vertices; //vertices(0) is origin and vertices(1) is end
        string info;

        /*ID*/
        getline(converter,info,separator);
        id = stoul(info);
        mesh.Cell1DId.push_back(id);

        /*MARKER*/
        getline(converter,info,separator);
        marker = stoul(info);
        if (marker != 0){
            if (mesh.Cell1DMarkers.find(marker) == mesh.Cell1DMarkers.end()){ //if the key (marker) is not found find() return an iterator which points at the end of Cell1Markers
                mesh.Cell1DMarkers.insert({marker, {id}}); //mesh.Cell1DMarkers.end() is an iterator that points to the position just after the end of Cell1DMarkers
            }
            else{
                mesh.Cell1DMarkers[marker].push_back(id);
            }
        }

        /*VERTICES*/
        getline(converter,info,separator);
        vertices(0) = stod(info);
        getline(converter,info,separator);
        vertices(1) = stod(info);
        mesh.Cell1DVertices.push_back(vertices);

    }
    file.close();
    return true;
}

bool ImportCell2Ds(const string &filename,
                   PolygonalMesh& mesh)
{

    ifstream file;
    file.open(filename);

    if(file.fail())
        return false;

    list<string> listLines;
    string line;
    while (getline(file, line))
        listLines.push_back(line);

    listLines.pop_front();

    mesh.NumberCell2D = listLines.size();

    if (mesh.NumberCell2D == 0)
    {
        cerr << "There is no cell 2D" << endl;
        return false;
    }

    char separator = ';';

    mesh.Cell2DId.reserve(mesh.NumberCell2D);
    mesh.Cell2DVertices.reserve(mesh.NumberCell2D);
    mesh.Cell2DEdges.reserve(mesh.NumberCell2D);

    for (const string& line : listLines)
    {
        istringstream converter(line);

        unsigned int id;
        unsigned int marker;
        unsigned int numvert;
        unsigned int numedg;
        vector<unsigned int> vertices = {};
        vector<unsigned int> edges = {};
        string info;

        /*ID*/
        getline(converter,info,separator);
        id = stoul(info);
        mesh.Cell2DId.push_back(id);

        /*MARKER*/
        getline(converter,info,separator);
        marker = stoul(info);
        if (marker != 0){
            if (mesh.Cell2DMarkers.find(marker) == mesh.Cell2DMarkers.end()){ //if the key (marker) is not found, find() return an iterator which points at the end of Cell2Markers
                mesh.Cell2DMarkers.insert({marker, {id}}); //mesh.Cell2DMarkers.end() is an iterator that points to the position just after the end of Cell2DMarkers
            }
            else{
                mesh.Cell2DMarkers[marker].push_back(id);
            }
        }

        /*VERTICES*/
        getline(converter,info,separator);
        numvert = stoul(info);
        vertices.resize(numvert);
        for (unsigned int i = 0; i < numvert; i++){
            getline(converter,info,separator);
            vertices[i] = stoul(info);
        }
        mesh.Cell2DVertices.push_back(vertices);

        /*EDGES*/
        getline(converter,info,separator);
        numedg = stoul(info);
        edges.resize(numedg);
        for (unsigned int i = 0; i < numedg; i++){
            getline(converter,info,separator);
            edges[i] = stoul(info);
        }
        mesh.Cell2DEdges.push_back(edges);

    }
    file.close();
    return true;
}


