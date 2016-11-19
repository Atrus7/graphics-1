#include "scene.h"

Vertex Scene::ray_trace_recursive(Vertex eye_point, Vertex eye_vector){
  return ray_trace_recursive(eye_point, eye_vector, 1);
}

Vertex Scene::ray_trace_recursive(Vertex eye_point, Vertex eye_vector, int level){
  Vertex color(0,0,0);//start black
  IShape* shape = get_closest_intersection_shape(eye_point, eye_vector);
  if(shape){// this is the shape we want

    Vertex intersect  = shape->get_intersection(eye_point, eye_vector);
    Vertex normal  = shape->get_normal(intersect);
    if(shape->surface_properties.gamma_e > 0 && level < 4){
      Vertex direct_light = get_light_at_point(intersect, normal, shape->surface_properties) * shape->surface_properties.gamma_a;

      Vertex ray_point = new_starting_ray_offset_error_mod(intersect, normal);
      //Vertex reflection = light_vector.get_unit_vector() - ((2 * L_dot_N) * normal);
      Vertex reflected_ray = eye_vector - (2 * dot(eye_vector, normal)) * normal;
      Vertex reflected_light = ray_trace_recursive(ray_point, reflected_ray, level + 1) * shape->surface_properties.gamma_e;
      color += direct_light + reflected_light;
    }
    else{
      color += get_light_at_point(intersect, shape->get_normal(intersect), shape->surface_properties);
    }
  }
  return color;
}

IShape* Scene::get_closest_intersection_shape(Vertex p, Vertex v){
  IShape * nearest_obj = nullptr;
  float nearest_intersection;
  for(IShape* obj : objects){
    Vertex intersect = obj->get_intersection(p, v);
    if(intersect != INVALID_INTERSECT){
      //check if the intersection is the closest thus far
      float intersect_dist = (p - intersect).get_magnitude();
      if(!nearest_obj || intersect_dist < nearest_intersection){
        nearest_obj = obj;
        nearest_intersection = (p-intersect).get_magnitude();
      }
    }
  }
  return nearest_obj;
}

Vertex new_starting_ray_offset_error_mod(Vertex original, Vertex normal){
  return original + (0.001 * normal);
}

Vertex Scene::get_light_at_point(Vertex point, Vertex normal, SurfaceProperties sp){
  Vertex color(0,0,0);
    // ambient light (independent of specific lights AND reflection)
  color = color + (sp.AMB_COEFF * AMB_INTENSITY); //* shape->surface_properties.gamma_a;

  for(Light l : lights){
    Vertex light_vector = l.L_LOCATION - point; // lv points to source

    if(in_shadow(l, new_starting_ray_offset_error_mod(point, normal))){
      continue;
    }


    // diffuse light: C k_d cos(theta) = C k_d (L _dot_ N)
    float L_dot_N = dot(light_vector.get_unit_vector(), normal.get_unit_vector());
    if(L_dot_N >=0){
      color = color + (l.L_INTENSITY * sp.DIFF_COEFF * L_dot_N);
      //R=l−2(l⋅n)n
      //C * k_s * (R _dot_ E ) ^n
      Vertex reflection = light_vector.get_unit_vector() - ((2 * L_dot_N) * normal);
      float R_dot_E = dot((point - EYE_POINT).get_unit_vector(), reflection.get_unit_vector());
      if(R_dot_E >= 0){
        color = color +  (l.L_INTENSITY * sp.SPEC_COEFF * pow(R_dot_E, SPEC_EXP));
      }
    }

  }
  return color;
}

bool Scene::in_shadow(Light l, Vertex p){
  Vertex vector_toward_light = l.L_LOCATION - p;
  IShape* shape = get_closest_intersection_shape(p, vector_toward_light);
  if(shape){ // shape was hit between the light and it.
    //check if shape is not behind light
    if(dot(l.L_LOCATION - shape->get_intersection(p, vector_toward_light), vector_toward_light) >=0){
      return true;
    }
  }
  return false;
}


Scene scene_1(){
  Light front_light(Vertex(0, 750, -800), Vertex(1,1,1));
  Light front_light2(Vertex(800, 800, -800), Vertex(1,1,1));
  Light back_light(Vertex(-600, 800, 1000), Vertex(.4,.4,.4));
  Light overhead_light(Vertex(0, 1000, 0), Vertex(1,1,1));
  IShape *plane= new IPlane(
                            SurfaceProperties(Vertex(.1,.1,.1), Vertex(0.4,0.4,.4), Vertex(.5, .5, .5), 0.2),
                            Vertex(0,1,0),
                            Vertex(0,0,0)
                            );

  float sphere_radius = 150;
  IShape *sphere= new ISphere(
                              SurfaceProperties(Vertex(.2,.2,.2), Vertex(0.8,0,0), Vertex(.5, .5, .5), 0),
                              sphere_radius,
                              Vertex(0,sphere_radius,300)
                              );
  IShape *sphere2= new ISphere(
                               SurfaceProperties(Vertex(.2,.2,.2), Vertex(0,0.4,0), Vertex(.5, .5, .5), 0),
                               sphere_radius,
                               Vertex(170,sphere_radius,500)
                               );
  sphere->add_deformation(Vertex(1,2,1));
  //sphere2->add_deformation(Vertex(2,1,1));

  (plane)->surface_properties.print();

  vector<Light> l_list;
  vector<IShape*> o_list;
  l_list.push_back(front_light);
  //l_list.push_back(front_light2);
  //l_list.push_back(overhead_light);
  l_list.push_back(back_light);
  o_list.push_back(sphere);
  o_list.push_back(sphere2);
  o_list.push_back(plane);

  return Scene(l_list, o_list);
}

Scene scene_2(){
  Light front_light(Vertex(-500, 750, -800), Vertex(.7,.7,.7));
  Light back_light2(Vertex(600, 1000, 1000), Vertex(.7,.7,.7));
  //Light back_light(Vertex(-600, 1000, 1000), Vertex(.4,.4,.4));
  IShape *plane= new IPlane(
                            SurfaceProperties(Vertex(.1,.1,.1), Vertex(0.8,0,.2), Vertex(.5, .5, .5), 0),
                            Vertex(0,1,0),
                            Vertex(0,0,0)
                            );

  float cylinder_radius = 60;
  float sphere_radius = 80;

  IShape *sphere= new ISphere(
                              SurfaceProperties(Vertex(.2,.2,.2), Vertex(0,0,0.8), Vertex(.5, .5, .5), 0),
                              sphere_radius,
                              Vertex(50,sphere_radius,150)
                              );


  IShape *sphere2= new ISphere(
                               SurfaceProperties(Vertex(.2,.2,.2), Vertex(0,0.8,0), Vertex(.5, .5, .5), 0),
                               sphere_radius,
                               Vertex(400,sphere_radius,300)
                               );

  IShape *cylinder= new ICylinder(
                                  SurfaceProperties(Vertex(.2,.2,.2), Vertex(0,0.5,.5), Vertex(.5, .5, .5), 0),
                                  cylinder_radius,
                                  Vertex(250,1,360),
                                  Vertex(0,1,0)
                                  );

  vector<Light> l_list;
  vector<IShape*> o_list;
  l_list.push_back(front_light);
  //l_list.push_back(back_light);
  l_list.push_back(back_light2);
  o_list.push_back(cylinder);
  o_list.push_back(plane);
  o_list.push_back(sphere);
  //o_list.push_back(sphere2);

  return Scene(l_list, o_list);
}
Scene scene_3(){
  Light front_light(Vertex(0, 750, -800), Vertex(1,1,1));
  Light overhead_light(Vertex(0, 1000, 0), Vertex(1,1,1));
  IShape *plane= new IPlane(
                            SurfaceProperties(Vertex(.1,.1,.1), Vertex(0.4,0.4,.4), Vertex(.5, .5, .5), 0.2),
                            Vertex(0,1,0),
                            Vertex(0,0,0)
                            );

  float sphere_radius = 150;
  IShape *sphere= new ISphere(
                              SurfaceProperties(Vertex(.2,.2,.2), Vertex(0.8,0,0), Vertex(.5, .5, .5), 0.2),
                              sphere_radius,
                              Vertex(0,sphere_radius,250)
                              );
  IShape *sphere2= new ISphere(
                               SurfaceProperties(Vertex(.2,.2,.2), Vertex(0,0.4,0), Vertex(.5, .5, .5), 1),
                               sphere_radius,
                               Vertex(170,sphere_radius,500)
                               );
  //sphere->add_deformation(Vertex(1,2,1));
  sphere2->add_deformation(Vertex(2,1,1));

  (plane)->surface_properties.print();

  vector<Light> l_list;
  vector<IShape*> o_list;
  l_list.push_back(front_light);
  l_list.push_back(overhead_light);
  o_list.push_back(sphere);
  o_list.push_back(sphere2);
  o_list.push_back(plane);

  return Scene(l_list, o_list);

}


Scene scene_4(){
  Light front_light(Vertex(-800, 550, -600), Vertex(.4,.4,.4));
  Light front_light2(Vertex(400, 350, -600), Vertex(.4,.4,.4));
  Light front_light3(Vertex(800, 550, -800), Vertex(.4,.4,.4));
  Light front_light4(Vertex(-400, 550, -800), Vertex(.4,.4,.4));
  Light overhead_light(Vertex(0, 1000, 0), Vertex(1,1,1));
  Light blocking_light(Vertex(300, 1000, 399), Vertex(1,1,1));
  IShape *grass= new IPlane(
                            SurfaceProperties(Vertex(.1,.1,.1), Vertex(.1,.1,.3), Vertex(.5, .5, .5), .9),
                            Vertex(0,1,0),
                            Vertex(0,0,0)
                            );
  IShape *ceiling= new IPlane(
                            SurfaceProperties(Vertex(.1,.1,.1), Vertex(0,.1,.15), Vertex(.5, .5, .5), 0),
                            Vertex(0,0,-1),
                            Vertex(0,500,500)
                            );
  // IShape *ceiling2= new IPlane(
  //                              SurfaceProperties(Vertex(0,0,0), Vertex(0,.05,.10), Vertex(.2, .2, .2), 0),
  //                             Vertex(0,.1,-.9),
  //                             Vertex(0,100,500)
  //                             );

  float sphere_radius = 150;
  IShape *sphere= new ISphere(SurfaceProperties(Vertex(.2,.2,.2), Vertex(0.8,0,0), Vertex(0,0,0), 0),
                              sphere_radius,
                              Vertex(-200,sphere_radius,-250)
                              );
  IShape *sphere2= new ISphere(SurfaceProperties(Vertex(.2,.2,.2), Vertex(0.8,0,0), Vertex(0,0,0), 0),
                              sphere_radius,
                              Vertex(-200,sphere_radius+20,-250)
                              );
  IShape *sphere3= new ISphere(SurfaceProperties(Vertex(.2,.2,.2), Vertex(0.8,0,0), Vertex(0,0,0), 0),
                               40,
                               Vertex(-200,sphere_radius+140,-250)
                               );
  IShape *sphere4= new ISphere(SurfaceProperties(Vertex(.2,.2,.2), Vertex(0.8,0,0), Vertex(0,0,0), 0),
                               sphere_radius,
                               Vertex(545,sphere_radius,-260)
                               );


  IShape *sphere5= new ISphere(SurfaceProperties(Vertex(.2,.2,.2), Vertex(0.8,0,0), Vertex(0,0,0), 0),
                               sphere_radius,
                               Vertex(-100,sphere_radius+40,-250)
                               );



  //sphere->add_deformation(Vertex(1,2,1));
  //sphere2->add_deformation(Vertex(2,1,1));


  vector<Light> l_list;
  vector<IShape*> o_list;
  l_list.push_back(front_light);
  l_list.push_back(front_light2);
  //l_list.push_back(front_light3);
  l_list.push_back(front_light4);
  l_list.push_back(overhead_light);
  l_list.push_back(blocking_light);
  o_list.push_back(sphere);
  o_list.push_back(sphere2);
  o_list.push_back(sphere3);
  o_list.push_back(sphere4);
  o_list.push_back(sphere5);


  //SurfaceProperties(Vertex(.1,.1,.1), Ve)
  //IShape * cylinder = new ICylinder()

  o_list.push_back(grass);
  o_list.push_back(ceiling);
  //o_list.push_back(ceiling2);

  return Scene(l_list, o_list);

}
