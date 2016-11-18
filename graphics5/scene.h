#ifndef SCENE_H
#define SCENE_H
#include <vector>
#include "vertex.h"
#include "lighting.h"
#include "i_shape.h"

#define ImageW 500
#define ImageH 500
const Vertex EYE_POINT = Vertex(ImageW/2,ImageW/2,-400); //eye pos is center of screen

const int MAX_RAY_RECURSION = 4;

class Scene
{
  vector<Light> lights;
  vector<IShape*> objects;
public:
  Scene(vector<Light> _lights, vector<IShape*> _objects): lights(_lights), objects(_objects){}
  //returns a color
  Vertex ray_trace_recursive(Vertex eye_point, Vertex eye_vector);
  Vertex ray_trace_recursive(Vertex eye_point, Vertex eye_vector, int level);
  IShape* get_closest_intersection_shape(Vertex p, Vertex v);

  Vertex get_light_at_point(Vertex point, Vertex normal, SurfaceProperties sp);

  //casts shadow ray and determines if the light will affect this shape
  bool in_shadow(Light l, Vertex p);
};

Vertex new_starting_ray_offset_error_mod(Vertex original, Vertex normal);

// Some pre-built scenes
/* Scene 1: at least one sphere and one ellipsoid with an infinite plane. Each object should have different ambient, diffuse, specular
  reflection coefficients. There should also be two different white lights. You should be able to see shadows and the shadow of one
  sphere/ellipsoid should be on part of the other sphere/ellipsoid. */
Scene scene_1();

// Scene 2: this scene should contain at least one infinite plane, sphere and cylinder showing all three different kinds of objects.
Scene scene_2();

// Scene 3: this scene should illustrate reflection. At least one object must be reflective and at least one of the reflective objects must be curved.
Scene scene_3();

// Scene 4: display anything you wish, but points will be given for creativity and complexity. You may implement other primitives (polygon, csg) if you wish; however, interesting objects can be built with an infinite plane and lots of spheres.
Scene scene_4();

#endif /* SCENE_H */
