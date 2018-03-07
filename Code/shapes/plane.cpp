#include "plane.h"
#include <iostream>
#include <math.h>

/************************** Plane **********************************/

void Plane::computeTexCoords(Point center, double* u, double* v) 
{
	  *u = 0.0;
  	*v = 0.0;
}

Hit Plane::intersect(const Ray &ray)
{

  float d = normal.dot(ray.D);
  Vector p = origin - ray.O;
  double t = p.dot(normal) / d;

  if (t >= 0) {
    if(d < -1e-6){
      return Hit(t, -normal);
    }
    return Hit(t, normal);
  }

  return Hit::NO_HIT();
}
