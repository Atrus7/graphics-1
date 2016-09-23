#include <GL/gl.h>
#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <cmath>
#include "painter.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT (WINDOW_WIDTH * .75)

PainterConfig PAINTER;
int WINDOW_ID;

void display ( void )
{
	glFlush();
}

void keyboard ( unsigned char key, int x, int y )
{
  if(key > '0' && key < '8') {
    std::cout << "Changing color" << std::endl;
    int key_number = int(key);
    PAINTER.setColor(key_number);
  }
  else {
    switch ( key ) {
    case 'c':
      glClear(GL_COLOR_BUFFER_BIT);
      break;
    case 'r':
      PAINTER.increaseBrushAngle();
      break;
    case 'a':
      PAINTER.toggleSprayPaintMode();
      break;
    case 'b':
      PAINTER.cycleBrush();
      break;
    case '+':
      PAINTER.changeSize(true);
      break;
    case '-':
      PAINTER.changeSize(false);
      break;
    case 'q':
      glutDestroyWindow(WINDOW_ID);
      exit(1);
      break;
    default:
      std::cout << "Bad key. Check the MANUAL for user instructions" << std::endl;
    }
  }
  glutPostRedisplay ( );
}

void init ( void )
{
  glClearColor (0.0, 0.0, 0.0, 0.0);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0.0, WINDOW_WIDTH-1, WINDOW_HEIGHT-1, 0, -1.0, 1.0);
  glMatrixMode(GL_MODELVIEW);
  glClear(GL_COLOR_BUFFER_BIT);
  glEnable(GL_FOG);
}

// rotates a point in a clockwise direction around origin
float * rotatePoint(float r_x, float r_y, float angle) {
  float * rotated_point = new float[2];
  float rad_angle = angle * (M_PI / 180.0f); //convert to rads
  rotated_point[0] = r_x * cos(rad_angle) - r_y * sin(rad_angle);
  rotated_point[1] = r_x * sin(rad_angle) + r_y * cos(rad_angle); //rotate
  return rotated_point;
}

float * calcPoint(float x, float y, float angle, float off_x, float off_y){
  float *rotated_point = rotatePoint(off_x, off_y, angle);
  rotated_point[0] += x;
  rotated_point[1] += y;
  return rotated_point;
}

void mouseMove ( int x, int y )
{
  glSetColor();
  switch(PAINTER.getBrush()){
  case RECTANGLE:
    glDrawRectagle(x, y);
    break;
  case LINE:
    glDrawLine(x, y);
    break;
  case CIRCLE:
    glDrawCircle(x, y);
    break;
  case TRIANGLE:
    glDrawTriangle(x, y);
    break;
  }
  glutPostRedisplay ( );
}

void glSetColor(){
  float * color = PAINTER.getColor();
  glColor3f ( color[0], color[1], color[2] );
}

void glDrawCircle(int x, int y) {
  int size = PAINTER.getSize();
  int r_size;
  if(PAINTER.getSprayMode()){
    int percent_to_blend = 10;

    for(int i=1; i < percent_to_blend; ++i){
      r_size = size * ((100-i) / 100.0);
      float blend = (i)/10;
      float *colors = PAINTER.getColor();
      glColor3f(colors[0] * blend, colors[1] * blend, colors[2] * blend);
      glBegin ( GL_TRIANGLE_FAN );
      glVertex2f ( x, y ); // center
      for(int theta = 0; theta < 360; ++theta){ //mostly smooth
        glVertex2f (x + cos(theta) * r_size, y + sin(theta) * r_size);
      }
      glEnd ();
    }

    glSetColor();
    glBegin ( GL_TRIANGLE_FAN );
    glVertex2f ( x, y ); // center
    for(int theta = 0; theta < 360; ++theta){ //mostly smooth
      glVertex2f ( x + cos(theta) * r_size, y + sin(theta) * r_size);
    }
    glEnd ();
  } else {
    glBegin ( GL_TRIANGLE_FAN );
    glVertex2f ( x, y ); // center
    for(int theta = 0; theta < 360; ++theta){ //mostly smooth
      glVertex2f ( x + cos(theta) * PAINTER.getSize(), y + sin(theta) * PAINTER.getSize());
    }
    glEnd ();
  }
}

void glDrawTriangle(int x, int y) {
  int size = PAINTER.getSize();
  float * p1 = calcPoint(x, y, PAINTER.getAngle(), -size, -size);
  float * p2 = calcPoint(x, y, PAINTER.getAngle(), size, -size);
  float * p3 = calcPoint(x, y, PAINTER.getAngle(), 0, size);

  glBegin ( GL_TRIANGLES );
  glVertex2f(p1[0], p1[1]);
  glVertex2f(p2[0], p2[1]);
  glVertex2f(p3[0], p3[1]);
  glEnd ();
}

void glDrawLine(int x, int y) {
  int size = PAINTER.getSize();

  float * p1 = calcPoint(x, y, PAINTER.getAngle(), 0, -size);
  float * p2 = calcPoint(x, y, PAINTER.getAngle(), 0, size);
  glBegin (GL_LINES);
  glVertex2f(p1[0], p1[1]);
  glVertex2f(p2[0], p2[1]);
  glEnd ();
}

void glDrawRectagle(int x, int y) {
  int size = PAINTER.getSize();

  float * p1 = calcPoint(x, y, PAINTER.getAngle(), -size, -size);
  float * p2 = calcPoint(x, y, PAINTER.getAngle(), size, -size);
  float * p3 = calcPoint(x, y, PAINTER.getAngle(), size, size);
  float * p4 = calcPoint(x, y, PAINTER.getAngle(), -size, size);

  glBegin( GL_QUADS );
  glVertex2f(p1[0], p1[1]);
  glVertex2f(p2[0], p2[1]);
  glVertex2f(p3[0], p3[1]);
  glVertex2f(p4[0], p4[1]);
  glEnd();
}

int main ( int argc, char *argv[] )
{
  glutInit(&argc, argv);
  glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
  glutInitWindowSize (WINDOW_WIDTH, WINDOW_HEIGHT); 
  glutInitWindowPosition (100, 100);
  WINDOW_ID = glutCreateWindow ("Christopher Findeisen - Assignment 1");
  init ();
  glutDisplayFunc(display);
  glutMotionFunc(mouseMove);
  glutKeyboardFunc(keyboard);
  glutMainLoop();
  return 0;
}
