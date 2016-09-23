#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/glut.h>
#endif/*__APPLE__*/
#include "Edge.h"
#include <iostream>
#include "painter.h"

/******************************************************************
	(Notes):
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

const int ImageW = 800;
const int ImageH = 800;

float framebuffer[ImageH][ImageW][3];
Painter painter(Point(ImageW, ImageH));

// Draws the scene
void drawit(void)
{
   glDrawPixels(ImageW,ImageH,GL_RGB,GL_FLOAT,framebuffer);
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
	// should not be necessary but some GPUs aren't displaying anything until a clear call.
	glClear ( GL_COLOR_BUFFER_BIT );

	//The next two lines of code are for demonstration only.
	//They show how to draw a line from (0,0) to (100,100)

	//int i;
  queue<ColoredEdge> scan_lines = painter.draw_polygons();

  if(painter.get_clipping() != 1){ // don't redraw if currently clipping
    while(!scan_lines.empty()) {
      ColoredEdge ce = scan_lines.front();
      Edge e = ce.edge;
      int y = e.get_start().y; //horizontal, won't matter
      int sx = min(e.get_start().x, e.get_end().x);
      int ex = max(e.get_start().x, e.get_end().x);
      scan_lines.pop();
      cout << "Setting from " << sx <<" to " << ex << endl;
      for(int i= sx; i < ex; ++i){
        setFramebuffer(i,y,ce.color.r,ce.color.g,ce.color.b);
      }
    }
  }
  drawit();
  if(painter.get_clipping() == 1){
    Point tl = painter.get_tl_clipping();
    Point br = painter.get_br_clipping();
    glBegin ( GL_LINES );
    glVertex2f ( tl.x,  tl.y);
    glVertex2f ( br.x, tl.y );

    glVertex2f ( br.x, tl.y );
    glVertex2f ( br.x, br.y );

    glVertex2f ( br.x, br.y );
    glVertex2f ( tl.x, br.y );

    glVertex2f ( tl.x, br.y );
    glVertex2f ( tl.x,  tl.y);
    glEnd ( );
    glFlush ( );
  }
}
void mouse_move_event ( int x, int y ){
  if(painter.get_clipping() == 1){
    painter.handle_mouse_move(x, y);
    glutPostRedisplay();
  }
}

void mouse_click_event(int button, int state, int x, int y ){
  if(state == GLUT_DOWN){
    if(button == GLUT_LEFT_BUTTON){
      cout << "left press at " << x << ", " << y << endl;
      painter.handle_left_press(x, y);
    } else if(button == GLUT_RIGHT_BUTTON){
      cout << "right press at " << x << ", " << y << endl;
      painter.handle_right_press(x, y);
    }
  } else if(state == GLUT_UP && button == GLUT_LEFT_BUTTON) {
    cout << "left release at "<< x << ", " << y << endl;
    painter.handle_left_release(x, y);
  } else {
    //unrecognized mouse event.
  }
  glutPostRedisplay ( );
}

void keyboard_event ( unsigned char key, int x, int y )
{
  cout << "Keyboard event" << endl;
  if(key == 'q' || key == 'Q'){
    exit(0);
  }
  painter.handle_key_press(key);
}

void init(void)
{
  gluOrtho2D (0, ImageW - 1, ImageH - 1, 0);
  clearFramebuffer();
}

int main(int argc, char** argv)
{
  srand((unsigned) time(0)); //seed for random vals later
  glutInit(&argc,argv);
  glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
  glutInitWindowSize(ImageW,ImageH);
  glutInitWindowPosition(100,100);
  glutCreateWindow("Christopher Findeisen - Homework 2");
  init();
  glutDisplayFunc(display);
  glutMotionFunc(mouse_move_event);
  glutMouseFunc(mouse_click_event);
  glutKeyboardFunc(keyboard_event);
  glutMainLoop();
  return 0;
}
