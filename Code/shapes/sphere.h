#ifndef SPHERE_H_
#define SPHERE_H_

#include "../object.h"

class Sphere: public Object
{
    public:
        Sphere(Point const &pos, double radius);
        Sphere(Point const &pos, double radius, double rotation, Point p);

        void computeTexCoords(Point center, double* u, double* v);
        virtual Hit intersect(Ray const &ray);

        Point const position;
        double const r;
        double angle;
        Point t;
};

#endif
