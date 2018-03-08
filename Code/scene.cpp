#include "scene.h"

#include "hit.h"
#include "image.h"
#include "material.h"
#include "ray.h"

#include <cmath>
#include <iostream>
#include <limits>

using namespace std;

Color Scene::trace(Ray const &ray, int depth)
{
    // Find hit object and distance
    Hit min_hit(numeric_limits<double>::infinity(), Vector());
    ObjectPtr obj = nullptr;
    for (unsigned idx = 0; idx != objects.size(); ++idx)
    {
        Hit hit(objects[idx]->intersect(ray));
        if (hit.t < min_hit.t)
        {
            min_hit = hit;
            obj = objects[idx];
        }
    }

    // No hit? Return background color.
    if (!obj) return Color(0.0, 0.0, 0.0);

    Material material = obj->material;          //the hit objects material
    Point hit = ray.at(min_hit.t);                 //the hit point
    Vector N = min_hit.N;                          //the normal at hit point
    Vector V = -ray.D;                             //the view vector

    /****************************************************
    * This is where you should insert the color
    * calculation (Phong model).
    *
    * Given: material, hit, N, V, lights[]
    * Sought: color
    *
    * Hints: (see triple.h)
    *        Triple.dot(Vector) dot product
    *        Vector + Vector    vector sum
    *        Vector - Vector    vector difference
    *        Point - Point      yields vector
    *        Vector.normalize() normalizes vector, returns length
    *        double * Color     scales each color component (r,g,b)
    *        Color * Color      dito
    *        pow(a,b)           a to the power of b
    ****************************************************/

    Color color = Color(0.0, 0.0, 0.0);
    Color matColor = Color(0.0,0.0,0.0);
    
    if(material.texture != nullptr){
    
      //If texture, get uv coords and color at that point
      double u = 0.0;
      double v = 0.0;
      obj->computeTexCoords(hit, &u, &v);
      matColor = material.texture->colorAt(u,v); 
      color = material.ka*matColor;
    } else {
      
      //Else just get the standard color
      matColor = material.color;
      color = material.ka*matColor;
    }
    
    //For each lightsource
    for (size_t idx = 0; idx != lights.size(); ++idx) {
    
      Hit min_hit(std::numeric_limits<double>::infinity(),Vector());
      obj = nullptr;
      
      //Calculate light vector
      Vector Lm = (lights[idx]->position - hit).normalized();
    
      // If shadows are enabled check for them
      if (shadows == true){
        // Calculate a ray to check for shadows
        Ray shadowRay(hit, Lm);

        // For each light source check if the ray overlaps with an object and
        // if so, do not add the specular and diffuse lighting
        for (unsigned int i = 0; i < objects.size(); ++i) {
          Hit hit(objects[i]->intersect(shadowRay));
          if (hit.t<min_hit.t) {
            min_hit = hit;
            obj = objects[i];
          }
        }
      }
      
      //If there was no object to cause shadow calculate light
      if (obj == nullptr){
      
        //Grab material variables
        double kd = material.kd;
        double ks = material.ks;
        double shinyness = material.n;
        Color colorLight = lights[idx]->color;
        
        //Calculate Lm*N
        double dotLN = Lm.dot(N);
        Vector Rm = 2*dotLN*N - Lm;
        double dotRV = Rm.dot(V);
        
        //Make sure dot products are not <0
        if (dotLN < 0)
          dotLN = 0;
        if (dotRV < 0)
          dotRV = 0;
          
        //Calculate diffuse and specular values
        Color diffuse = kd * dotLN * matColor * colorLight;
        Color specular = ks * pow(dotRV, shinyness) * colorLight;
        color += diffuse + specular;
      }
    }
    
    if (depth > 0){
      Triple reflection = 2*N.dot(V.normalized()) * N - V.normalized();
      Ray reflectionRay(hit, reflection);
      color += material.ks * trace(reflectionRay, depth-1);
    }
    
    return color;
}

void Scene::render(Image &img)
{
    unsigned w = img.width();
    unsigned h = img.height();
    
    #pragma omp parallel for
    for (unsigned y = 0; y < h; ++y)
    {
        for (unsigned x = 0; x < w; ++x)
        {
          Color c = Color(0);
          
          //Cast all the sub
          for (int h1 = 0; h1 < AAFactor; ++h1)
          {
            for (int w1 = 0; w1 < AAFactor; ++w1)
            {
            
              //Calculate sub-ray starting positions
              double xPos =  ((double) w1 + (w1 + 1.0))/(double)AAFactor * 0.5;
              double yPos = ((double) h1 + (h1 + 1.0))/(double)AAFactor * 0.5;

              Point pixel(x + xPos, h-1.0-y+yPos, 0);

              //Cast ray
              Ray ray(eye, (pixel-eye).normalized());
              Color col = trace(ray, maxDepth);
              col.clamp();
              c += col;
            }
          }

          //Divide over the amount of colors added
          img(x,y) = c / ((AAFactor) * (AAFactor));
        }
    }
}

// --- Misc functions ----------------------------------------------------------

void Scene::addObject(ObjectPtr obj)
{
    objects.push_back(obj);
}

void Scene::addLight(Light const &light)
{
    lights.push_back(LightPtr(new Light(light)));
}

void Scene::setEye(Triple const &position)
{
    eye = position;
}

void Scene::setShadows(bool flag)
{
    shadows = flag;
}

void Scene::setAAFactor(int factor)
{
  AAFactor = factor;
}

void Scene::setMaxDepth(int depth)
{
  maxDepth = depth;
}

unsigned Scene::getNumObject()
{
    return objects.size();
}

unsigned Scene::getNumLights()
{
    return lights.size();
}
