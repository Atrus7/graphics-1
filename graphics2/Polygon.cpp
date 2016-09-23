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
    throw("Can't build a polygon with less than two sides.");
  }

  Polygon polygon(color); //set color

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
}
vector<Point> Clipper::in_out_converter(Edge e, RectEdge r){
  int IX;
  int IY;
  double slope = e.get_slope();
  Edge clipping_edge = rect_edges[r];
  if(r == LEFT){
    IX = min.x;
    IY = slope * (min.x-e.get_start().x) + e.get_start().y;
  } else if(r == RIGHT){
    IX = max.x;
    IY = slope*(max.x-e.get_start().x) + e.get_start().y;
  } else if(r == TOP){
    IX = e.get_start().x + (max.y - e.get_start().y) / slope;
    IY = min.y;
  } else if(r == BOTTOM){ // BOTTOM
    IX = e.get_start().x + (min.y - e.get_start().y) / slope;
    IY = max.y;
  } else {
    throw("unrecognized edge");
  }

}
