#include <Windows.h>
#include <GL/glut.h>
#include <math.h>
#include "ScanConvert.h"
#include "PolygonDrawer.h"
#include <vector>

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
  *****************************************************************/

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
// those returned by the glutMouseFunc exactly - Scott Schaefer 
void setFramebuffer(int x, int y, float R, float G, float B)
{
	// changes the origin from the lower-left corner to the upper-left corner
	y = ImageH - 1 - y;
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

	// test clipping and drawing
	vector<Pt> pts;

	pts.push_back ( Pt ( 10, 10 ) );
	pts.push_back ( Pt ( 10, 100 ) );
	pts.push_back ( Pt ( 100, 200 ) );
	pts.push_back ( Pt ( 200, 100 ) );
	pts.push_back ( Pt ( 250, 100 ) );
	pts.push_back ( Pt ( 300, 200 ) );
	pts.push_back ( Pt ( 300, 10 ) );

	drawPolygon ( pts );
}

void keyboard_event ( unsigned char key, int x, int y )
{
  cout << "Keyboard event" << endl;
  if(key == 'q' || key == 'Q'){
    exit(0);
  }
  switch(key){
  case '1':{
    // Flat Shading
    break;
  }
  case '2':{
    // Gouraud Shading
    break;
  }
  case '3':{
    // Phong Shading
    break;
  }
  default : cout << "Keyboard event not recognized";
  }
}


int main(int argc, char** argv)
{
  glutInit(&argc,argv);
  glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
  glutInitWindowSize(ImageW,ImageH);
  glutInitWindowPosition(100,100);
  glutCreateWindow("Christopher Findeisen - Homework 4");
  init();
    if(argc != 2){
      cout << "Error: Incorrect Usage. Program requires the following arguments:\n";
      cout << "[initial filename] [goal filename]";
      cout << argc << endl;
      return -1;
    } 
  string obj_file = argv[1];
  ifstream initial_input;
  initial_input.open(initial_file, ios::in);
  //BlocksWorld blocks_world(parse_for_blocks(initial_input), parse_for_blocks(goal_input));
  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard_event);
  glutMainLoop();
  return 0;
}
