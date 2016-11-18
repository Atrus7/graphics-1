#include "scene.h"

Vertex Scene::ray_trace_recursive(Vertex eye_point, Vertex eye_vector){
  return ray_trace_recursive(eye_point, eye_vector, 1);
}

Vertex Scene::ray_trace_recursive(Vertex eye_point, Vertex eye_vector, int level){
  IShape* shape = get_closest_intersection_shape(eye_point, eye_vector);
  if(shape){// this is the shape we want
    Vertex intersect  = shape->get_intersection(eye_point, eye_vector);
    return get_light_at_point(intersect, shape->get_normal(intersect), shape->surface_properties);
  }
  return Vertex(0,0,0);
}
IShape* Scene::get_closest_intersection_shape(Vertex p, Vertex v){
  IShape * nearest_obj = nullptr;
  Vertex nearest_intersection;
  for(IShape* obj : objects){
    Vertex intersect = obj->get_intersection(p, v);
    if(intersect != INVALID_INTERSECT){
      //check if the intersection is the closest thus far
      if(!nearest_obj || intersect.z < nearest_intersection.z){
        nearest_obj = obj;
        nearest_intersection = intersect;
      }
    }
  }
  return nearest_obj;
}

Vertex Scene::get_light_at_point(Vertex point, Vertex normal, SurfaceProperties sp){
  Vertex color(0,0,0);
  // ambient light (independent of specific lights)
  color = color + (sp.AMB_COEFF * AMB_INTENSITY);

  for(Light l : lights){
    if(in_shadow(l,point)){
      continue;
    }


    Vertex light_vector = l.L_LOCATION - point; // lv points to source
    // diffuse light: C k_d cos(theta) = C k_d (L _dot_ N)
    float L_dot_N = dot(light_vector.get_unit_vector(), normal.get_unit_vector());
    color = color + (l.L_INTENSITY * sp.DIFF_COEFF * L_dot_N);

    //R=l−2(l⋅n)n
    //C * k_s * (R _dot_ E ) ^n
    //Vertex reflection = light_vector - (2*L_dot_N ) * normal;
    //float R_dot_E = dot(EYE_VECTOR, reflection);

    //color = color +  (L_INTENSITY * sp.SPEC_COEFF * pow(R_dot_E, SPEC_EXP));
  }
  return color;
}

bool Scene::in_shadow(Light l, Vertex p){
  return false;
}


Scene scene_1(){
  Light simple_light(Vertex(-500, 250, -800), Vertex(1,1,1));
  Light other_light(Vertex(250, 300, 1000), Vertex(1,.5,0));
  IShape *plane= new IPlane(
                            SurfaceProperties(Vertex(.3,.3,.3), Vertex(0.4,0.4,.4), Vertex(.5, .5, .5)),
                            Vertex(0,1,0),
                            Vertex(0,0,0)
                            );

  float sphere_radius = 200;
  IShape *sphere= new ISphere(
                              SurfaceProperties(Vertex(.3,.3,.3), Vertex(0.4,0.4,.4), Vertex(.5, .5, .5)),
                              sphere_radius,
                              Vertex(300,sphere_radius,500)
                              );

  plane->surface_properties.print();

  vector<Light> l_list;
  vector<IShape*> o_list;
  l_list.push_back(simple_light);
  l_list.push_back(other_light);
  o_list.push_back(plane);
  o_list.push_back(sphere);
  o_list[0]->surface_properties.print();

  return Scene(l_list, o_list);
}
