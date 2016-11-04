//#include <Windows.h>
#include <GL/glut.h>
#include <math.h>
#include "ScanConvert.h"
#include "PolygonDrawer.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>

using namespace std;

/******************************************************************
	Notes:
	Image size is 400 by 400 by default.  You may adjust this if
		you want to.
	You can assume the window will NOT be resized.
	Call clearFramebuffer to clear the entire framebuffer.
	Call setFramebuffer to set a pixel.  This should be the only
		routine you use to set the color (other than clearing the
		entire framebuffer).  drawit() will cause the current
		framebuffer to be displayed.
	As is, your scan conversion should probably be called from
		within the display function.  There is a very short sample
		of code there now.
	You may add code to any of the subroutines here,  You probably
		want to leave the drawit, clearFramebuffer, and
		setFramebuffer commands alone, though.






The scene itself will be specified with an ambient light amount of [.5, .5, .5] and an infinitely far away light source, which is emanating light in the [-1, -1, 1] direction and has an intensity of [1, 1, 1]. Finally, the object should have an ambient reflection coefficient of [.1, 0, 0], a diffuse reflection coefficient of [.7, 0, 0], a specular reflection coefficient of [.5, .5, .5] and a specular exponent of 5. 

  *****************************************************************/

float framebuffer[ImageH][ImageW][3];
vector<Polygon > polygons;

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

void display(void)
{
	drawit();
}

void init(void)
{
	clearFramebuffer();

  printf("have %d faces", polygons.size());
  for(Polygon poly : polygons){

    //backface culling
     float E_dot_N = dot(EYE_VECTOR, poly.normal);
     if(E_dot_N > 0){
       continue; // remove surface
     }

    for(Vertex pt : poly.getPoints()){
      printf("Vertices:  %f, %f, %f \n", pt.x, pt.y, pt.z);
    }
    drawPolygon (poly);
  }
}


void keyboard_event ( unsigned char key, int x, int y )
{
  cout << "Keyboard event" << endl;
  if(key == 'q' || key == 'Q'){
    exit(0);
  }
  switch(key){
  case '1':{
    shading = Flat;
    break;
  }
  case '2':{
    shading = Gourand;
    break;
  }
  case '3':{
    shading = Phong;
    break;
  }
  default : cout << "Keyboard event not recognized" << endl;
  }
}

void readPolygonsFromFile(string file_name){
  polygons.clear();
  ifstream poly_objs(file_name);
  string line;
  char c;
  vector<Vertex> *vert_list = new vector<Vertex>();
  vert_list->push_back(Vertex()); // dummy at index 0, since vertices index at 1
  while(getline(poly_objs, line)){
    stringstream ss(line);
    ss >> c;
    switch(c){
    case 'v':{
      float f1, f2, f3;
      ss >> f1 >> f2 >> f3;
      vert_list->push_back(Vertex(f1, f2, f3));
      printf("Vertices:  %f, %f, %f \n", f1, f2, f3);
      break;
    }
    case 'f':{ // face
      int i1, i2, i3;
      ss >> i1 >> i2 >> i3;
      printf("Face:  %f, %f, %f \n", vert_list->at(i1).x, vert_list->at(i2).x, vert_list->at(i3).x);
      Polygon poly(&(vert_list->at(i1)), &(vert_list->at(i2)), &(vert_list->at(i3)));
      // vert_list[i1].update_normal(poly.normal);
      // vert_list[i2].update_normal(poly.normal);
      // vert_list[i3].update_normal(poly.normal);
      polygons.push_back(poly);
      break;
    }
    case '#':{
      break; // ignore, # indicates comment
    }
    }
  }
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
  glutCreateWindow("Christopher Findeisen - Homework 4");
    if(argc != 2){
      cout << "Error: Incorrect Usage. Program requires the following arguments:\n";
      cout << "./prog [obj filename]";
      cout << argc << endl;
      return -1;
    }
  string obj_file = argv[1];
  readPolygonsFromFile(obj_file);
  shading = Flat;
  init();
  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard_event);
  glutMainLoop();
  return 0;
}
