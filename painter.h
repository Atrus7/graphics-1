#ifndef _PAINTER_H_
#define _PAINTER_H_
#include <algorithm>
enum BrushType {
  RECTANGLE, TRIANGLE, LINE, CIRCLE
} brushes[4] = {RECTANGLE, TRIANGLE, LINE, CIRCLE};

// Class that tracks configuration to paint. Config meant to tie to a specific window
class PainterConfig {
  const int MAX_BRUSH_SIZE = 128;
  const int MIN_BRUSH_SIZE = 1;
  const int BRUSH_SIZE_DELTA = 2;
  const int BRUSH_ANGLE_DELTA = 10;

  int b_size = 25;
  int b_angle = 0; 
  float b_color[3] = {1,1,1}; // rgb stored as [R, G, B]
  int b_type = 0; // indexes brushes: Simply a counter

  bool spray_mode = true;

public:
  void setColor(int c_bits){
    int r = c_bits & 1;
    int g = c_bits & 2;
    int b = c_bits & 4;
    b_color[0] = r;
    b_color[1] = g;
    b_color[2] = b;
  }

  int getAngle(){
    return b_angle;
  }

  int getSize(){
    return b_size;
  }

  void changeSize(bool increase){
    if(increase){
      b_size = std::min(MAX_BRUSH_SIZE, b_size * BRUSH_SIZE_DELTA);
    }
    else{
      b_size = std::max(MIN_BRUSH_SIZE, b_size / BRUSH_SIZE_DELTA);
    }
  }

  bool getSprayMode(){
    return spray_mode;
  }

  float * getColor(){
    return b_color;
  }

  BrushType getBrush(){
    return brushes[b_type];
  }

  void cycleBrush(){
    b_type = (b_type + 1) % 4;
  }

  void toggleSprayPaintMode(){
    spray_mode = !spray_mode;
  }

  void increaseBrushAngle(){
    b_angle = (b_angle + BRUSH_ANGLE_DELTA) % 360;
  }

  PainterConfig(){}
};


void glDrawCircle(int x, int y);
void glDrawTriangle(int x, int y);
void glSetColor();
void glDrawLine(int x, int y);
void glDrawRectagle(int x, int y);
int *rotatePoint(int o_x, int o_y, int r_x, int r_y, float angle);
#endif
