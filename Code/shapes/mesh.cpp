#include "../objloader.h"
#include "../vertex.h"

#include "mesh.h"
#include <iostream>
#include <math.h>
#include <cmath>

using namespace std;

Mesh::Mesh(string const &filename, Point pos)
{
  //create objloader and get data
  OBJLoader * loader = new OBJLoader(filename);
  vector<Vertex> data = loader->vertex_data();
  
  //until all vertices have been made into triangles
  unsigned idx = 0;
  while (idx != data.size())
    {
      //create points out of the vertices
      Point A, B, C;
      A = pos+Triple(data[idx].x, data[idx].y,data[idx].z);
      B = pos+Triple(data[idx+1].x, data[idx+1].y,data[idx+1].z);
      C = pos+Triple(data[idx+2].x, data[idx+2].y,data[idx+2].z);
      
      //create triangle out of points and add to triangle vector
      triangles.push_back(Triangle(A,B,C));
      
      //increase iterator by three to get the next triangle
      idx+=3;
        
    }
  
  
  //delete objloader
  delete loader;
  
}

Hit Mesh::intersect(const Ray &ray)
{

  Hit min_hit(numeric_limits<double>::infinity(), Vector());
  bool contact = false;
  
  //for every triangle
  for(Triangle &t : triangles){
    
    //see if it intersects with the ray
    Hit hit(t.intersect(ray));
    
    //check if triangle is the closest one
    if (hit.t < min_hit.t)
    {
        min_hit = hit;
        contact=true;
    }
  }
  
  //if a triangle was found, return the hit
  if(contact) return min_hit;
  
  //otherwise return no hit
  return Hit::NO_HIT();

}


