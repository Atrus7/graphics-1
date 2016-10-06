#include "painter.h"
#include <iostream>
#include <cmath>
#include <iostream>

Painter::Painter(Point window_bottom_right): clip_tl(0,0), clip_br(window_bottom_right.x, window_bottom_right.y), win_br(window_bottom_right.x, window_bottom_right.y), clipping_mode(false){
}

vector<Edge> Painter::ActiveEdgeList::get_horizontals(int line) const{
  if(active_edge_list.size() % 2 == 1){
    throw("Bad polygon: not even in edges");
  }
  vector<Edge> horizontals;
  for(int i=0; i<active_edge_list.size(); i+=2){
    //make a new line, drawing horizontally from outside edge - edge
    int left_x = std::ceil(active_edge_list[i].calc_current_x(line));

    float f_r_x = (active_edge_list[i+1].calc_current_x(line));
    int right_x=0;
    if(active_edge_list.size() - i <= 2){
      if(f_r_x == floor(f_r_x)){
        --f_r_x; // don't draw rightmost pixels
      } else{
        f_r_x = std::floor(f_r_x); //
      }
    }
    right_x = f_r_x;
    Edge horizontal(Point(left_x, line),
                    Point(f_r_x, line));
    horizontals.push_back(horizontal);
  }
  return horizontals;
}

void Painter::clip(){

  int min_x = std::min(clip_tl.x, clip_br.x);
  int min_y = std::min(clip_tl.y, clip_br.y);
  int max_x = std::max(clip_tl.x, clip_br.x);
  int max_y = std::max(clip_tl.y, clip_br.y);
  Point min = Point(min_x, min_y);
  Point max = Point(max_x, max_y);

  Clipper clipper(min, max);
  for(int i=0; i < polys.size(); ++i){
    Polygon p = clipper.clip_polygon(polys[i]);
    p.set_color(polys[i].get_color());
    if(clipped_polys.size() > i){
      clipped_polys[i] = p;
    }
    else{
      clipped_polys.push_back(p);
    }
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
    if((*it).get_high().y == line){ // if edge ends at line
      it = active_edge_list.erase(it);
    }
    else {
      ++it;
    }
  }
}


queue<ColoredEdge> Painter::draw_polygons() {
  std::vector<Polygon> *poly_list = (clipping_mode==2)? &clipped_polys :  &polys;
  queue<ColoredEdge> pixels_to_color;
  const int height = win_br.y;
  const int width = win_br.x;
  if(clipping_mode == 2){
    clip();
  }

  for(Polygon p : *poly_list){
    if(p.get_color().r == 1 &&  p.get_color().g == 1 && p.get_color().b == 1){
      continue;
    }
    ActiveEdgeList ael;

    EdgeSorter es(0);
    for(int line=0; line < height; ++line){
      ael += p.get_edges_starting_at(line);
      ael.remove_ending_at(line);
      es.set_line(line);

      std::sort(ael.active_edge_list.begin(), ael.active_edge_list.end(), es);

      if(ael.active_edge_list.size() > 0){
        for(Edge e : ael.get_horizontals(line)){ // add horizontal lines
          pixels_to_color.push({e, p.get_color()});
        }
      }
    }
  }
  return pixels_to_color;
}
void Painter::handle_left_press(int x, int y){
  if(clipping_mode){
    set_tl_clipping(Point(x, y));
    set_br_clipping(Point(x, y));
    clipping_mode = 1;
  }
  else{
    if(polys.size() > 9) {
      return;
    }
    unfinished_poly.add_point(Point(x,y));
  }
}
void Painter::handle_left_release(int x, int y){
  if(clipping_mode){
    set_br_clipping(Point(x, y));
    clipping_mode = 2;
  }
}
void Painter::handle_mouse_move(int x, int y){
  if(clipping_mode==1){
    set_br_clipping(Point(x, y));
  }

}
void Painter::handle_right_press(int x, int y){
  if(!clipping_mode){
    if(polys.size() > 9) {
      return;
    }
    unfinished_poly.add_point(Point(x,y));

    try{
      polys.push_back(unfinished_poly.build());
    } catch(...){cout << "Build FAILED...\n";}// nothing, reset builder either way
    unfinished_poly = Polygon::Builder();
  }
}
void Painter::handle_key_press(unsigned char key){
  if(key == 'c' || key == 'C'){
    toggle_clipping(4);
  } else {
    // don't recognize
  }
}
void Painter::toggle_clipping(int i) {
  if(polys.size() < 10) {
    try{
      polys.push_back(unfinished_poly.build());
    }catch(...){
      unfinished_poly = Polygon::Builder();
    }
  }

  clipping_mode = (clipping_mode)? 0 : i;
}

