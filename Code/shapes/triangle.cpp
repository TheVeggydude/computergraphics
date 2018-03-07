#include "triangle.h"
#include <iostream>
#include <math.h>
#include <cmath>

using namespace std;

void Triangle::computeTexCoords(Point center, double* u, double* v) 
{
	  *u = 0.0;
  	*v = 0.0;
}

Hit Triangle::intersect(const Ray &ray)
{

    double smallNumber = 0.000001;
    double result;

    Vector e_1 = (B-A);
    Vector e_2 = (C-A);
    
    Vector h = ray.D.cross(e_2);
    double det = e_1.dot(h);
    if(det > -smallNumber && det < smallNumber)
    {
      return Hit::NO_HIT();
    }
    
    double inv_det = 1.0 / det;
    Vector s = ray.O - A;
    double u = inv_det * s.dot(h);
    
    if(u < 0.0 || u > 1.0)
      return Hit::NO_HIT();
    
    Vector Q = s.cross(e_1);
    double v = inv_det * ray.D.dot(Q);
    
    if(v < 0.0 || u + v > 1.0)
    {
      return Hit::NO_HIT();
    }
      
    result = inv_det * e_2.dot(Q);

    if(result > smallNumber)
    {
      Vector N = e_1.cross(e_2).normalized();
      return Hit(result, N);
    }
   
    return Hit::NO_HIT();

}


