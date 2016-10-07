#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glut.h>
#endif/*__APPLE__*/
#include <cmath>
#include <cstdio>
#include <iostream>
#include <stdlib.h>
#include "assignment3.h"
#include "init.h"

vector<Pt> condensation_set;
vector<Matrix> IATTransformation;
vector<AffineTransform> RATTransformation;
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

#define PI 3.14159265
Matrix rotate ( Pt o, float theta )
{
  theta = theta * PI / 180; // convert to radians
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

	Matrix transformed; //points after transformation
	transformed.data[0][0] = q1.x;
	transformed.data[0][1] = q2.x;
	transformed.data[0][2] = q3.x;

	transformed.data[1][0] = q1.y;
	transformed.data[1][1] = q2.y;
	transformed.data[1][2] = q3.y;

	transformed.data[2][0] = 0;
	transformed.data[2][1] = 0;
	transformed.data[2][2] = 1;

	Matrix beforeInverse; //points before transformation but the inverse of the matrix;
	float c = 1 / (-(p2.x * p1.y) + p3.x * p1.y + p1.x * p2.y - p3.x * p2.y - p1.x * p3.y + p2.x * p3.y);

	beforeInverse.data[0][0] = (p2.y - p3.y) * c;
	beforeInverse.data[0][1] = (p3.x - p2.x) * c;
	beforeInverse.data[0][2] = (p2.x * p3.y - p3.x * p2.y) * c;

	beforeInverse.data[1][0] = (p3.y - p1.y) * c;
	beforeInverse.data[1][1] = (p3.x - p1.x) * c;
	beforeInverse.data[1][2] = (p3.x * p1.y - p1.x * p3.y) * c;

	beforeInverse.data[2][0] = (p1.y - p2.y) * c;
	beforeInverse.data[2][1] = (p1.x - p1.x) * c;
	beforeInverse.data[2][2] = (p1.x * p2.y - p2.x * p1.y) * c;

	rvalue = compose(transformed, beforeInverse);



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
  IATTransformation = transformations;
}

vector<vector<Pt> >  applyIATTransformations(vector<vector<Pt> > shapes){
  vector<vector<Pt> > result_pts;
  for(Matrix m : IATTransformation){
    for(vector<Pt> pts : shapes){
      vector<Pt> new_shape;
      for(Pt pt : pts){
        Pt new_pt;
        new_pt.x += m.data[0][0] * pt.x;
        new_pt.x += m.data[0][1] * pt.y;
        new_pt.x += m.data[0][2] * 1;
        new_pt.y += m.data[1][0] * pt.x;
        new_pt.y += m.data[1][1] * pt.y;
        new_pt.y += m.data[1][2] * 1;

        new_shape.push_back(new_pt);
      }
      result_pts.push_back(new_shape);
    }
  }
  if(condensation_set.size()){
    result_pts.push_back(condensation_set);
  }
  return result_pts;
}

void fractalHangman(){
  vector<Matrix> iat;
  iat.push_back ( scale ( Pt ( -.9, -.9 ), 0.5 ) );
  iat.push_back ( scale ( Pt ( 0, -.9 ), 0.5 ) );
  iat.push_back(compose (
                         scale ( Pt (- .9, .9 ), 0.5 ),
                         rotate(Pt(-.9, -.9), -90) )
                );

  setIATTransformations ( iat );

  vector<Pt> pts;
  // no condensation set
  setCondensationSet ( pts );
}

void fractalStaircase(){
  vector<Matrix> iat;
  iat.push_back ( scale ( Pt ( -.9, -.9 ), 0.5 ) );
  iat.push_back ( scale ( Pt ( .9, .9 ), 0.5 ) );

  setIATTransformations ( iat );

  vector<Pt> pts;
  pts.push_back(Pt(0.9, -0.9));
  pts.push_back(Pt(-0.9, -0.9));
  pts.push_back(Pt(0.9, 0.9));
  setCondensationSet (pts);
}

void fractalSnowflake(){
  vector<Matrix> iat;
  for(int i=0; i < 5; ++i){
    float angle =  (360 + (72 * i  + 18)) * (PI / 180);
    iat.push_back (scale ( Pt (0.9 * cos(angle), 0.9 * sin(angle)), 0.38 ));
  }

  iat.push_back(compose(scale ( Pt (0.0 , 0 ), 0.385 ),
                          rotate(Pt(0, 0), 108) ));

  setIATTransformations ( iat );

  vector<Pt> pts;
  setCondensationSet (pts);
}
void hexFlower(){
  vector<Matrix> iat;
  for(int i=0; i < 6; ++i){
    float angle =  (360 + (60 * i)) * (PI / 180);
    iat.push_back (scale ( Pt (0.9 * cos(angle), 0.9 * sin(angle)), 0.5 ));
  }

  setIATTransformations ( iat );

  vector<Pt> pts;
  setCondensationSet (pts);
}

void myPirateFlag(){
  vector<Matrix> iat;
  iat.push_back(scale ( Pt (-.9 , 0 ), 0.33 ));
  iat.push_back(scale ( Pt (.9 , 0 ), 0.33 ));
  iat.push_back(compose (
                         scale ( Pt (0.0 , 0 ), 0.33 ),
                         rotate(Pt(0, 0), -60) )
                );
  iat.push_back(compose (
                         scale ( Pt (0.0 , 0 ), 0.33 ),
                         rotate(Pt(0, 0), 60) )
                );
  setIATTransformations(iat);
  vector<Pt> pts;
  setCondensationSet (pts);

}

//print current IAT values
void printCurrentFractal(){
  for(AffineTransform atm : RATTransformation){

    string affine_type;
    switch(atm.att){
    case ROTA:
      affine_type = "Rotation";
      break;
    case SCALE:
      affine_type = "Scale";
    case NSCALE:
      affine_type = "Non-uniform Scale";

      break;
    }

    printf("\t %s Transformation \n", affine_type.c_str());
    printf("Values: %f, Pt(%f, %f)", atm.val, atm.pt1.x, atm.pt1.y);
    if(NSCALE){
      printf(", Pt(%f, %f)", atm.pt2.x, atm.pt2.y);
    }
  }
}

Matrix AffineTransform::execute(){
  if(att == ROTA){
    return rotate(pt1, val);
  }
  else if(att == SCALE){
    return scale( pt1, val);
  }
  else if(att == NSCALE){
    return nscale(pt1, pt2, val);
  }
  throw("Bad ATType");
}

//random fractal generator
void myRandomFractalGenerator() {
    int transformation_count = (rand() % 3) + 2; //between 3 and 5
    vector<Matrix> iat;


    for(int i=0; i < transformation_count; ++i){
      bool compose_transformation = rand() % 2;
      int rand_transformation = rand() % 3;
      if(rand_transformation)


      Matrix m1 = 
      // if(compose_transformation){
      // }
    }

    rand() % 100;
    iat.push_back(scale ( Pt (-.9 , 0 ), 0.33 ));
}

void myFractal2(){

  // iat.push_back(scale ( Pt (-.9 , 0 ), 0.33 ));
  // iat.push_back(scale ( Pt (.9 , 0 ), 0.33 ));
  // iat.push_back(compose (
  //                        scale ( Pt (0.0 , 0 ), 0.33 ),
  //                        rotate(Pt(0, 0), -60) )
  //               );
  // iat.push_back(compose (
  //                        scale ( Pt (0.0 , 0 ), 0.33 ),
  //                        rotate(Pt(0, 0), 60) )
  //               );
  // setIATTransformations(iat);
  // vector<Pt> pts;
  // setCondensationSet (pts);
}


// Draws the current IAT
void display ( void )
{
  glClear ( GL_COLOR_BUFFER_BIT );
  glColor3f ( 1, 1, 1 );
  vector<vector<Pt> > fractal_set;
  vector<Pt> initial_set = condensation_set;
  if(initial_set.size() == 0){
    initial_set.push_back(Pt(1,1));
  }

  fractal_set.push_back(initial_set);

  static const int SHAPE_BOUND = 700000;

  const int DEPTH_BOUND = log(SHAPE_BOUND)  / log(IATTransformation.size() * fractal_set.size() * fractal_set[0].size());

  for(int i=0; i < DEPTH_BOUND; ++i){
    fractal_set = applyIATTransformations(fractal_set);
  }

  cout << "shape count: " << fractal_set.size() * fractal_set[0].size() << endl;
  for(vector<Pt> shape : fractal_set){
    if(shape.size() == 1){
      glBegin (GL_POINTS);
    }
    else if(shape.size() == 2){
      glBegin (GL_LINES);
    }
    else if(shape.size() >2 ){
      glBegin (GL_LINE_LOOP);
    }

    for(Pt pt : shape){
      glVertex2f(pt.x, pt.y);
    }
    glEnd();
  }

  glFlush ( );

  cout << "redisplaying" << endl;
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

void keyboard_event ( unsigned char key, int x, int y )
{
  cout << "Keyboard event" << endl;
  if(key == 'q' || key == 'Q'){
  }
  switch(key){
  case 'q' :
  case 'Q' :
    {
      exit(0);
    }
  case '0' :
    {
      init();
      break;
    }
  case '1' :
    {
      fractalHangman();
      break;
    }
  case '2' :
    {
      fractalStaircase();
      break;
    }
  case '3' :
    {
      fractalSnowflake();
      break;
    }
  case '4' :
    {
      hexFlower();
      break;
    }
  case '5' :
    {
      myPirateFlag();
      break;
    }
  case '6' :
    {
      myFractal2();
      break;
    }
  case 'p' :
    {
      printCurrentFractal();
      break;
    }
  }
  glutPostRedisplay();
}

int main ( int argc, char** argv )
{
  srand((unsigned) time(0)); //seed for random vals later
  glutInit ( &argc, argv );
  glutInitDisplayMode ( GLUT_SINGLE | GLUT_RGB );
  glutInitWindowSize ( 500, 500 );
  glutInitWindowPosition ( 100, 100 );
  glutCreateWindow ( "Christopher Findeisen - Homework 3" );
  init ( );
  glClearColor (0.0, 0.0, 0.0, 0.4);
  glutDisplayFunc ( display );
  glutReshapeFunc ( reshape );
  glutKeyboardFunc(keyboard_event);
  glutMainLoop ( );
  return 0;
}
