#include "raytracer.h"

#include "image.h"
#include "light.h"
#include "material.h"
#include "triple.h"

// =============================================================================
// -- Include all your shapes here ---------------------------------------------
// =============================================================================

#include "shapes/sphere.h"
#include "shapes/triangle.h"
#include "shapes/plane.h"
#include "shapes/mesh.h"
#include "shapes/quad.h"

// =============================================================================
// -- End of shape includes ----------------------------------------------------
// =============================================================================

#include "json/json.h"

#include <exception>
#include <fstream>
#include <iostream>

using namespace std;        // no std:: required
using json = nlohmann::json;

bool Raytracer::parseObjectNode(json const &node)
{
    ObjectPtr obj = nullptr;

// =============================================================================
// -- Determine type and parse object parametrers ------------------------------
// =============================================================================

    if (node["type"] == "sphere")
    {
        Point pos(node["position"]);
        double radius = node["radius"];
        obj = ObjectPtr(new Sphere(pos, radius));
    } else if (node["type"] == "triangle") {
		    Point A, B, C;
		    A = Triple(node["A"]);
		    B = Triple(node["B"]);
		    C = Triple(node["C"]);
		    obj = ObjectPtr(new Triangle(A, B, C));
	  } else if (node["type"]== "plane"){
	      Point A, B, C;
	      A = Triple(node["A"]);
		    B = Triple(node["B"]);
		    C = Triple(node["C"]);
		    obj = ObjectPtr(new Plane(A, B, C));
    } else if (node["type"]== "quad"){
	      Point A, B, C, D;
	      A = Triple(node["A"]);
		    B = Triple(node["B"]);
		    C = Triple(node["C"]);
		    D = Triple(node["D"]);
		    obj = ObjectPtr(new Quad(A, B, C, D));
    } else if (node["type"]== "mesh"){
        string filename = node["filename"];
        Point pos(node["position"]);
        Point scale(node["scale"]);
		    obj = ObjectPtr(new Mesh(filename, pos, scale));
    } else 
    {
        cerr << "Unknown object type: " << node["type"] << ".\n";
    }

// =============================================================================
// -- End of object reading ----------------------------------------------------
// =============================================================================

    if (!obj)
        return false;

    // Parse material and add object to the scene
    obj->material = parseMaterialNode(node["material"]);
    scene.addObject(obj);
    return true;
}

Light Raytracer::parseLightNode(json const &node) const
{
    Point pos(node["position"]);
    Color col(node["color"]);
    return Light(pos, col);
}

Material Raytracer::parseMaterialNode(json const &node) const
{
    double ka = node["ka"];
    double kd = node["kd"];
    double ks = node["ks"];
    double n  = node["n"];
    
    //Store the texturefile path if there is one, else use the color
    string img = "";
    Color color= Color(0.0, 0.0, 0.0);
    if(node.count("texture")==1){
    cout << "test\n";
    
      img = node["texture"];
    }else{
      color = Color(node["color"]);
    }
      
    cout << img;
    return Material(color, ka, kd, ks, n, img);
}

bool Raytracer::readScene(string const &ifname)
try
{
    // Read and parse input json file
    ifstream infile(ifname);
    if (!infile) throw runtime_error("Could not open input file for reading.");
    json jsonscene;
    infile >> jsonscene;

// =============================================================================
// -- Read your scene data in this section -------------------------------------
// =============================================================================

    Point eye(jsonscene["Eye"]);
    scene.setEye(eye);
    
    if(jsonscene.count("Shadows")==1)
    {
      scene.setShadows(jsonscene["Shadows"]);
    }

    if(jsonscene.count("SuperSamplingFactor")==1)
    {
      scene.setAAFactor(jsonscene["SuperSamplingFactor"]);
    } else {
      scene.setAAFactor(1);
    }
    
    if(jsonscene.count("MaxRecursionDepth")==1)
    {
      scene.setMaxDepth(jsonscene["MaxRecursionDepth"]);
    }

    for (auto const &lightNode : jsonscene["Lights"])
        scene.addLight(parseLightNode(lightNode));

    unsigned objCount = 0;
    for (auto const &objectNode : jsonscene["Objects"])
        if (parseObjectNode(objectNode))
            ++objCount;

    cout << "Parsed " << objCount << " objects.\n";

// =============================================================================
// -- End of scene data reading ------------------------------------------------
// =============================================================================

    return true;
}
catch (exception const &ex)
{
    cerr << ex.what() << '\n';
    return false;
}

void Raytracer::renderToFile(string const &ofname)
{
    // TODO: the size may be a settings in your file
    Image img(400, 400);
    cout << "Tracing...\n";
    scene.render(img);
    cout << "Writing image to " << ofname << "...\n";
    img.write_png(ofname);
    cout << "Done.\n";
}
