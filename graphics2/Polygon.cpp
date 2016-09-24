#include "Polygon.h"

void Polygon::Builder::add_point(Point p){
  points.push_back(p);
}

void Polygon::add_edge(int key, const Edge &e){
  if(!active_edge_table.count(key)){ // create if doensn't exist
    active_edge_table.insert(pair<int, list<Edge>>(key, list<Edge>()));
  }
  active_edge_table.at(key).push_front(e);
}

Polygon Polygon::Builder::build(){
  //TODO: can we make lines? Is that a problem?
  if(points.size() < 3) {
    Polygon polygon(Color{1.0, 1.0, 1.0}); // return an undrawable poly
    return polygon;
  }

  Polygon polygon(color); //set color
  polygon.vertices = points;

  // add edges
  for(int i=0; i < points.size()-1; ++i){
    Edge edge(points[i], points[i+1]);
    if(edge.get_low().y - edge.get_high().y){ // should be non-zero, otherwise ignore(horizontal)
      polygon.add_edge(edge.get_low().y, edge);
    }
  }
  //connect the final and first points
  Edge last_edge(points[0], points[points.size() - 1]);
  polygon.add_edge(last_edge.get_low().y, last_edge);

  return polygon;
}

list<Edge> Polygon::get_edges_starting_at(int line){
  return active_edge_table[line];
}

Polygon Clipper::clip_polygon(const Polygon &p1){
  deque<Point>* vertices_q = new deque<Point>();
  deque<Point>* result_q = new deque<Point>();
  for(Point p : p1.vertices){
    vertices_q->push_front(p);
  }

  for(int i=0; i < 4; ++i){
    Point start, first;
    vertices_q->push_back(vertices_q->front());
    if(vertices_q->size() > 0){
      first = start = vertices_q->front();
      vertices_q->pop_front();
    }

    while(vertices_q->size()){
      Point end = (vertices_q->size())? vertices_q->front(): first; // last point should edge with the first point
      vector<Point> points = in_out_converter(Edge(start, end), static_cast<RectEdge>(i));
      if(points.size()){
        result_q->insert(result_q->begin(), points.begin(), points.end());
      }
      start = vertices_q->front();
      vertices_q->pop_front();
    }
    vertices_q->insert(vertices_q->begin(), result_q->begin(), result_q->end());
    result_q->erase(result_q->begin(), result_q->end());
  }
  Polygon::Builder clipped_poly;
  while(vertices_q->size()){
    Point start = vertices_q->front();
    vertices_q->pop_front();
    clipped_poly.add_point(start);
  }
  Polygon p = clipped_poly.build();
  return p;
}

vector<Point> Clipper::in_out_converter(Edge e, RectEdge r){
  vector<Point> output_list;
  ClipCase clip_case = in_out_determinator(e, r);
  int IX;
  int IY;
  double slope = e.get_slope();

  if(clip_case == IN_OUT || clip_case == OUT_IN){
    if(r == LEFT){
      IX = min.x;
      IY = slope * (min.x - e.get_start().x) + e.get_start().y;
    } else if(r == RIGHT){
      IX = max.x;
      IY = slope*(max.x-e.get_start().x) + e.get_start().y;
    } else if(r == TOP){
      if(slope==0){
        IX = e.get_start().x;
      } else {
        IX = e.get_start().x + (min.y - e.get_start().y) / slope;
      }
      IY = min.y;
    } else if(r == BOTTOM){ // BOTTOM
      if(slope==0){
        IX = e.get_start().x;
      } else {
        IX = e.get_start().x + (max.y - e.get_start().y) / slope;
      }
      IY = max.y;
    } else {
      throw("unrecognized edge");
    }
    if(clip_case == OUT_IN) {//also needs end
      output_list.push_back(e.get_end());
    }
    output_list.push_back(Point(IX, IY)); // for out_in & in_out
  } else if(clip_case == IN){
    output_list.push_back(e.get_end());
  }

  return output_list;
}



bool comp(bool flip, int i1, int i2){
  if(flip){
    return i1 <= i2;
  }
  return i1 >= i2;
}

ClipCase Clipper::in_out_determinator(Edge e, RectEdge r){
  int clipping_line;
  bool check_reversed;
  int start;
  int end;
  if(r == LEFT){
    start = e.get_start().x;
    end = e.get_end().x;

    clipping_line = min.x;
    check_reversed = false;
  } else if(r == RIGHT){
    start = e.get_start().x;
    end = e.get_end().x;
    clipping_line = max.x;

    check_reversed = true;
  } else if(r == TOP){
    start = e.get_start().y;
    end = e.get_end().y;

    check_reversed = false;
    clipping_line = min.y;
  } else if(r == BOTTOM){
    start = e.get_start().y;
    end = e.get_end().y;

    check_reversed = true;
    clipping_line = max.y;
  } else {
    throw("unrecognized edge");
  }

  if(comp(check_reversed, start, clipping_line)){
    if(comp(check_reversed, end, clipping_line)){
      return IN;
    }
    else{
      return IN_OUT;
    }
  }
  else { // start is out
    if(comp(check_reversed, end, clipping_line)) {
      return OUT_IN;
    }
    else{
      return OUT;
    }
  }

}
