#include "quad.h"
#include <iostream>
#include <math.h>
#include <cmath>

using namespace std;

Quad::Quad(Point A, Point B, Point C, Point D)
{
  //points are given counter-clockwise
  triangles.push_back(Triangle(A, B, D));
  triangles.push_back(Triangle(B, C, D));
}

Hit Quad::intersect(const Ray &ray)
{

  //same function as in the mesh
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


