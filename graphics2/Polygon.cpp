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
  for(int i=0; i < points.size(); ++i){
    Edge edge(points[i], points[i+1]);
    polygon.add_edge(edge.get_start().y, edge);
  }

  return polygon;
}

list<Edge> Polygon::get_edges_starting_at(int line){
  return active_edge_table[line];
}
