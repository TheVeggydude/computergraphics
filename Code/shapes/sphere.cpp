#include "sphere.h"

#include <cmath>

using namespace std;

void Sphere::computeTexCoords(Point hitPos, double* u, double* v) 
{		

    Triple norm = t.normalized();

    double cos_A = cos(angle * (M_PI / 180));
    double sin_A = sin(angle * (M_PI / 180));

    Triple v1 = Triple(cos_A + norm.x * norm.x * (1 - cos_A), norm.x * norm.y * (1 - cos_A) - norm.z * sin_A, norm.x * norm.z * (1 - cos_A) + norm.y * sin_A);
    Triple v2 = Triple(norm.y * norm.x * (1 - cos_A) + norm.z * sin_A, cos_A + norm.y * norm.y * (1 - cos_A), norm.y * norm.z * (1 - cos_A) - norm.x * sin_A);
    Triple v3 = Triple(norm.z * norm.x * (1 - cos_A) - norm.y * sin_A, norm.z * norm.y * (1 - cos_A) + norm.x * sin_A, cos_A + norm.z * norm.z * (1 - cos_A));

    double cx = v1.dot((hitPos - position)) + position.x;
    double cy = v2.dot((hitPos - position)) + position.y;
    double cz = v3.dot((hitPos - position)) + position.z;

	double theta = acos((cz - position.z) / r);
	double phi = atan2(cy - position.y, cx - position.x);
	
	if (phi < 0)
		phi = phi + 2 * M_PI;

	*u = phi / (2 * M_PI);
	*v = (M_PI - theta) / M_PI;
}

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

Sphere::Sphere(Point const &pos, double radius, double rotation, Point p)
:
    position(pos),
    r(radius),
    angle(rotation),
    t(p)
{}
