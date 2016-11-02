#ifndef ASSIGNMENT_3_H
#define ASSIGNMENT_3_H

#include <vector>

using namespace std;

/**
 * You should not change any of the definitions in this file.
 * We will be providing new init.cpp files to test your program
 * on new factals.  These functions define the interface between
 * the two files!
 */

class Matrix
{
public:
	float data [ 3 ] [ 3 ];

	Matrix ( void )
	{
		int i, j;

		for ( i = 0; i < 3; i++ )
		{
			for ( j = 0; j < 3; j++ )
			{
				data [ i ] [ j ] = 0;
			}
		}
	}
};

class Pt
{
public:
	float x, y;

	Pt ( float newX, float newY )
	{
		x = newX;
		y = newY;
	}

	Pt ( void )
	{
		x = y = 0;
	}
};

typedef Pt Vec;

//for randoms
enum AffineTransformType {
  ROTA, SCALE, NSCALE
};
struct AffineTransform {
  AffineTransformType att;
  float val;
  Pt pt1, pt2;

  AffineTransform(AffineTransformType _att, float _val, Pt _pt1):
    att(_att), val(_val), pt1(_pt1){}
  AffineTransform(AffineTransformType _att, float _val, Pt _pt1, Pt _pt2):
    att(_att), val(_val), pt1(_pt1), pt2(_pt2){}
  Matrix execute();
};

/**
 * Constructs a matrix to translate by the vector "v"
 */
Matrix translate ( Vec v );

/**
 * Constructs a matrix to rotate about the point "o" by the angle "theta"
 */
Matrix rotate ( Pt o, float theta ) ;

/**
 * Constructs a matrix to uniformly scale about
 * the point "o" by the scalar "alpha"
 */
Matrix scale ( Pt o, float alpha ) ;

/**
 * Constructs a matrix to non-uniformly scale about
 * the point "o" in the direction "v" by the scalar "alpha"
 */
Matrix nscale ( Pt o, Vec v, float alpha ) ;

/**
 * Constructs a matrix representing the affine transformation that takes the
 * points "p1", "p2", "p3" to the deformed points "q1", "q2", "q3"
 */
Matrix image ( Pt p1, Pt p2, Pt p3, Pt q1, Pt q2, Pt q3 );

/**
 * Multiplies the matrices "m2", "m1" to produce
 * the product m1 * m2 (in that order).
 */
Matrix compose ( Matrix m1, Matrix m2 );

/**
 * Sets the condensation set for this IAT.
 * If pts.size ( ) == 0, then there is no condensation set for this fractal
 * If pts.size ( ) == 1, then the set is a point
 * If pts.size ( ) == 2, then the set is a line
 * If pts.size ( ) > 2, then the set should be a
 *    poly-line (line with first and last points connected)
 */
void setCondensationSet ( vector<Pt> pts );

/**
 * Sets the transformations for the current IAT.
 */
void setIATTransformations ( vector<Matrix> transformations );

vector<Pt>  applyIATTransformations(vector<Pt> pts);

void fractalHangman();
void fractalStaircase();
void fractalSnowflake();
void hexFlower();
void myFractal1();
void myFractal2();
void printCurrentFractal();
void myRandomFractalGenerator() ;


/**
 * helper functions--not in specified api
 */
Vec vec_product (Vec v1, Vec v2);
Matrix setup_affine_skeleton_matrix();

#endif
