#include "painter.h"

Painter::Painter(Point window_bottom_right): win_tl(0,0), win_br(window_bottom_right.x, window_bottom_right.y), clipping_mode(false){}

vector<Edge> Painter::ActiveEdgeList::get_horizontals(int line) const{
  if(active_edge_list.size() % 2){
    throw("Bad polygon: not even in edges");
  }
  vector<Edge> horizontals;
  for(int i=0; i<active_edge_list.size(); i+=2){
    //make a new line, drawing horizontally from outside edge - edge
    Edge horizontal(Point(active_edge_list[i].calc_current_x(line), line),
      Point(active_edge_list[i+1].calc_current_x(line), line));
    horizontals.push_back(horizontal);
  }
}


std::vector<Edge>& Painter::ActiveEdgeList::operator+=(const std::list<Edge>& l)
{
  vector<Edge> &ael = active_edge_list; //for short
  ael.insert(ael.end(), l.begin(), l.end());
  return ael;
}
void Painter::ActiveEdgeList::remove_ending_at(int line){
  vector<Edge>::iterator it = active_edge_list.begin();

  for(; it != active_edge_list.end(); ){
    if((*it).get_end().y == line){ // if edge ends at line
      it = active_edge_list.erase(it);
    }
    else {
      ++it;
    }
  }
}

void Painter::ActiveEdgeList::sort(){
  //vector<Edge> &ael = active_edge_list; //for short
  //std::sort(ael.begin(), ael.end());
}

queue<ColoredEdge> Painter::draw_polygons() {
  std::vector<Polygon> *poly_list = (clipping_mode)? &clipped_polys :  &polys;
  queue<ColoredEdge> pixels_to_color;
  const int height = win_br.y;
  const int width = win_br.x;

  for(Polygon p : *poly_list){
    ActiveEdgeList ael; //active_edge_list

    for(int line=0; line < height; ++line){
      ael += p.get_edges_starting_at(line);
      ael.remove_ending_at(line);
      ael.sort();
      for(Edge e : ael.get_horizontals(line)){ // add horizontal lines
        pixels_to_color.push({e, p.get_color()});
      }
    }
  }
  return pixels_to_color;
}
void Painter::handle_left_press(int x, int y){
  if(polys.size() > 9) {
    return;
  }
  unfinished_poly.add_point(Point(x,y));
}
void Painter::handle_left_release(int x, int y){
  if(clipping_mode){
    //assign clipping_corner
  }
}
void Painter::handle_right_press(int x, int y){
  if(polys.size() > 9) {
    return;
  }
  unfinished_poly.add_point(Point(x,y));

  try{
    polys.push_back(unfinished_poly.build());
  } catch(...){}// nothing, reset builder either way
  unfinished_poly = Polygon::Builder();
}
void Painter::handle_key_press(unsigned char key){
  if(key == 'c' || key == 'C'){
    toggle_clipping();
  } else {
    // don't recognize
  }
}
void Painter::toggle_clipping() {
  if(polys.size() < 10) {
    try{
      polys.push_back(unfinished_poly.build());
    }catch(...){
      unfinished_poly = Polygon::Builder();
    }
  }

  clipping_mode = (clipping_mode)? false : true;
}

