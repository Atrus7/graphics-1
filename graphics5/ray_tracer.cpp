//#include <Windows.h>

//#include "PolygonDrawer.h"

#include <GL/glut.h>
#include <math.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include "ray_tracer.h"

using namespace std;

float framebuffer[ImageH][ImageW][3];

// Draws the scene
void drawit(void)
{
  glDrawPixels(ImageW,ImageH,GL_RGB,GL_FLOAT,framebuffer);
  glFlush();
}

// Clears framebuffer to black
void clearFramebuffer()
{
	int i,j;

	for(i=0;i<ImageH;i++) {
		for (j=0;j<ImageW;j++) {
			framebuffer[i][j][0] = 0.0;
			framebuffer[i][j][1] = 0.0;
			framebuffer[i][j][2] = 0.0;
		}
	}
}

// Sets pixel x,y to the color RGB
// I've made a small change to this function to make the pixels match
// those returned by the glutMouseFunc exactly
void setFramebuffer(int x, int y, float R, float G, float B)
{
	if (R<=1.0)
		if (R>=0.0)
			framebuffer[y][x][0]=R;
		else
			framebuffer[y][x][0]=0.0;
	else
		framebuffer[y][x][0]=1.0;
	if (G<=1.0)
		if (G>=0.0)
			framebuffer[y][x][1]=G;
		else
			framebuffer[y][x][1]=0.0;
	else
		framebuffer[y][x][1]=1.0;
	if (B<=1.0)
		if (B>=0.0)
			framebuffer[y][x][2]=B;
		else
			framebuffer[y][x][2]=0.0;
	else
		framebuffer[y][x][2]=1.0;
}

Vertex eye_vector_to_pixel(int x, int y) {
  return Vertex(x, y, VIEWPORT_Z) - EYE_POINT;
}


void display(void)
{
	drawit();
}


void ray_trace(Scene scene){
  for (int x = 0; x < ImageW; ++x) {
    for (int y = 0; y < ImageH; ++y) {
      Vertex color_at_point = scene.ray_trace_recursive(EYE_POINT, eye_vector_to_pixel(x,y));
      setFramebuffer(x,y, color_at_point.x, color_at_point.y, color_at_point.z);
    }
  }
}

void init(void)
{
	clearFramebuffer();
  ray_trace(scene_1());
}

void keyboard_event ( unsigned char key, int x, int y )
{
  cout << "Keyboard event" << endl;
  if(key == 'q' || key == 'Q'){
    exit(0);
  }
  switch(key){
  case '1':{
    break;
  }
  case '2':{
    break;
  }
  case '3':{
    break;
  }
  case '4':{
    break;
  }
  default : cout << "Keyboard event not recognized" << endl;
  }
  init();
  display();
  glutPostRedisplay();
}

/* do not modify the reshape function */
void reshape ( int width, int height )
{
  glViewport ( 0, 0, width, height );
  glMatrixMode ( GL_PROJECTION );
  glLoadIdentity ( );
  gluOrtho2D (-1, 1, -1, 1);
  glMatrixMode ( GL_MODELVIEW );
  glLoadIdentity ( );
}

int main(int argc, char** argv)
{
  glutInit(&argc,argv);
  glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
  glutInitWindowSize(ImageW,ImageH);
  glutInitWindowPosition(0,0);
  glutCreateWindow("Christopher Findeisen - Homework 5");
  init();
  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard_event);
  glutMainLoop();
  return 0;
}
