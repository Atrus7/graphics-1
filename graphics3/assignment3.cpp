#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glut.h>
#endif/*__APPLE__*/
#include <cmath>
#include "assignment3.h"
#include "init.h"

vector<Pt> condensation_set;
Matrix IATTransformation;

Matrix setup_affine_skeleton_matrix() {
  Matrix m;
  //identity. No scaling
  m.data[0][0] = 1;
  m.data[0][1] = 0;
  m.data[1][0] = 0;
  m.data[1][1] = 1;

  m.data[2][2] = 1; // always the case for non-division
  return m;
}

Vec vec_product (Vec v1, Vec v2) {
  Vec v3;
  // v * v_T
  v3.x = v1.x * v2.x + v1.x * v2.y;
  v3.y = v1.y * v2.x + v1.y * v2.y;
  return v3;
}

Matrix translate ( Vec v )
{
  Matrix rvalue = setup_affine_skeleton_matrix();
  //translation
  rvalue.data[0][2] = v.x;
  rvalue.data[1][2] = v.y;
  return rvalue;
}

Matrix rotate ( Pt o, float theta )
{
  Matrix rvalue = setup_affine_skeleton_matrix();
  rvalue.data[0][0] = cos(theta);
  rvalue.data[0][1] = - sin(theta);
  rvalue.data[1][0] = sin (theta);
  rvalue.data[1][1] = cos(theta);

  rvalue.data[0][2] = o.x + o.y * sin(theta) - o.x * cos(theta);
  rvalue.data[1][2] = o.y - o.y * cos(theta) - o.x * sin(theta);
  return rvalue;
}

Matrix scale ( Pt o, float alpha )
{
  Matrix rvalue = setup_affine_skeleton_matrix();
  rvalue.data[0][0] *= alpha;
  rvalue.data[1][1] *= alpha;

  rvalue.data[0][2] = (1-alpha) * o.x;
  rvalue.data[1][2] = (1-alpha) * o.y;

  return rvalue;
}

Matrix nscale ( Pt o, Vec v, float alpha )
{
  Matrix rvalue = setup_affine_skeleton_matrix();
  Vec v_vT = vec_product(v,v);

  rvalue.data[0][0] += (alpha - 1) + v_vT.x;
  rvalue.data[1][1] += (alpha - 1) + v_vT.y;

  Vec v_vT_o = vec_product(v_vT, o);
  rvalue.data[0][2] = (1 - alpha) * v_vT_o.x;
  rvalue.data[1][2] = (1 - alpha) * v_vT_o.y;
  return rvalue;
}

Matrix image ( Pt p1, Pt p2, Pt p3, Pt q1, Pt q2, Pt q3 )
{
  Matrix rvalue;

  return rvalue;
}

Matrix compose ( Matrix m1, Matrix m2 )
{
  Matrix rvalue;
  for (int i = 0; i < 3; i++ )
    {
      for (int j = 0; j < 3; j++ )
        {
          for (int z = 0; z < 3; z++ )
            {
              rvalue.data[i][j] += m1.data[i][z] * m2.data[z][j];
            }
        }
    }

  return rvalue;
}

void setCondensationSet ( vector<Pt> pts )
{
  condensation_set = pts;
}

void setIATTransformations ( vector<Matrix> transformations )
{
  Matrix result = transformations[0];
  for (int i = 1 ; i < transformations.size(); i++) {
    result = compose(result, transformations[i]);
 }
  IATTransformation = result;
}


// Draws the current IAT
void display ( void )
{
  /*
  // glVertex2f(p2[0], p2[1]);
  // glEnd ();
  switch(condensation_set.size() == 0){
  case 0: //  no condensation set. draw a point?
    {
      glBegin (GL_POINTS);
      glVertex2f(100, 100);
      glEnd ();
      break;
    }
  case 1: {

    break;
  }
  case 2:{
    
    break;
  }
  default:{
    break;
  }

  }
    if(condensation_set.size() == 0){
  } else if (condensation_set.size() == 1){
    
  }
 else if (condensation_set.size() >2 1){
 }
  */

  glBegin(GL_LINES);
  glVertex2f(10, 10);
  glVertex2f(100, 100);
  glEnd();
  glFlush ( );
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

int main ( int argc, char** argv )
{
  glutInit ( &argc, argv );
  glutInitDisplayMode ( GLUT_SINGLE | GLUT_RGB );
  glutInitWindowSize ( 500, 500 );
  glutInitWindowPosition ( 100, 100 );
  glutCreateWindow ( "Christopher Findeisen - Homework 3" );
  init ( );	
  glutDisplayFunc ( display );
  glutReshapeFunc ( reshape );
  glutMainLoop ( );
  return 0;
}
