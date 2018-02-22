#include "sphere.h"

#include <cmath>

using namespace std;

Hit Sphere::intersect(Ray const &ray)
{
    /****************************************************
    * RT1.1: INTERSECTION CALCULATION
    *
    * Given: ray, position, r
    * Sought: intersects? if true: *t
    *
    * Insert calculation of ray/sphere intersection here.
    *
    * You have the sphere's center (C) and radius (r) as well as
    * the ray's origin (ray.O) and direction (ray.D).
    *
    * If the ray does not intersect the sphere, return false.
    * Otherwise, return true and place the distance of the
    * intersection point from the ray origin in *t (see example).
    ****************************************************/

    /* Calculate intersection using Line–sphere_intersection method listed on wikipedia */
    /* a is the unit dot product of the unit vector with the unit vector */
    double a = ray.D.dot(ray.D);
	  /* b is 2 * dot product of unit vector with the origin minus center point of sphere */
    double b = 2*ray.D.dot(ray.O - position);
	  /* c is the dot product of origin of line - center point of sphere
	  with origin of line - center point of sphere. */
    double c = (ray.O - position).dot(ray.O - position) - r*r;
	  double d = b*b - 4*a*c;

    if(d < 0)
        return Hit::NO_HIT();

	  double solution1 = (-b - sqrt(d)) / (2*a);
    double solution2 = (-b + sqrt(d)) / (2*a);

    double t;

    if (solution1 < solution2)
    	t = solution1;
    else
    	t = solution2;

 	  /* Calculate normal */

 	  Vector normal = ((ray.O + t*ray.D) - position) / r;

    if (t < 0){
      return Hit::NO_HIT();
    }

    return Hit(t, normal);
}

Sphere::Sphere(Point const &pos, double radius)
:
    position(pos),
    r(radius)
{}
