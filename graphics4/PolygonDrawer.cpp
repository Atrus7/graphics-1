#include "PolygonDrawer.h"
#include "ScanConvert.h"
#include <algorithm>
#include <cmath>
#include <stdio.h>
#include "ScanConvert.h"

class Edge
{
public:
	float slopeRecip;
	float maxY;
	float currentX;
	float currentZ;
  Vertex currentF;
  Vertex fIncr;
  float zIncr;

	bool operator < ( const Edge &e )
	{
		if ( currentX == e.currentX )
		{
			return slopeRecip < e.slopeRecip;
		}
		else
		{
			return currentX < e.currentX;
		}
	}
};

vector<vector<Edge> > activeEdgeTable;
vector<Edge> activeEdgeList;

void Vertex::update_normal(Vertex face_normal){
  int old_avg_count = normal_avg_count;
  ++normal_avg_count;
  *normal = ((*normal * old_avg_count) + face_normal) / normal_avg_count;
}

void buildActiveEdgeTable ( vector<WinPt> &points )
{
	int i;

	activeEdgeTable.clear ( );

	// add rows equal to height of image to active edge table
	for ( i = 0; i < ImageH; i++ )
	{
		vector<Edge> row;

		activeEdgeTable.push_back ( row );
	}

	for ( i = 0; i < points.size ( ); i++ )
	{
		Edge e;
		int next = ( i + 1 ) % points.size ( );

		// ignore horizontal lines
		if ( points [ i ].y == points [ next ].y )
		{
			continue;
		}
		e.maxY = max ( points [ i ].y, points [ next ].y );
		e.slopeRecip = ( points [ i ].x - points [ next ].x ) / (float)( points [ i ].y - points [ next ].y );
    e.fIncr = (points[i].attr - points[next].attr) / (points[i].y - points[next].y);
    e.zIncr = (points[i].z - points[next].z) / (points[i].y - points[next].y);

		if ( points [ i ].y == e.maxY )
		{
			e.currentX = points [ next ].x;
			e.currentZ = points [ next ].z;
      e.currentF = points[next].attr;

			activeEdgeTable [ points [ next ].y ].push_back ( e );
		}
		else
		{
      e.currentF = points[i].attr;
			e.currentX = points [ i ].x;
			e.currentZ = points [ i ].z;
			activeEdgeTable [ points [ i ].y ].push_back ( e );
		}
	}
}


Vertex light(Vertex normal){
  Vertex color(0,0,0);
  // ambient l
  color = color + (AMB_REFLECTION * AMB_INTENSITY);

  // diffuse light: C k_d cos(theta) = C k_d (L _dot_ N)
  float L_dot_N = dot(LS_VECTOR, normal);
  color = color + (LS_INTENSITY * DIFF_REFLECTION * L_dot_N);

  //R=l−2(l⋅n)n
  //C * k_s * (R _dot_ E ) ^n
  Vertex reflection = LS_VECTOR - (2*L_dot_N ) * normal;
  float R_dot_E = dot(EYE_VECTOR, reflection);

  color = color +  (LS_INTENSITY * SPEC_REFLECTION * pow(R_dot_E, SPEC_EXP));
  return color;
}

// assumes all vertices are within window!!!
void drawPolygon ( Polygon poly )
{
  if(shading == Gourand){
    for(Vertex pt : poly.getPoints()){
      pt.normal->make_unit_vector();
      *pt.color = light(*pt.normal);
    }
  }

  printf("\n\nNormal:  %f, %f, %f\n",poly.normal.x, poly.normal.y, poly.normal.z);

  int x, y, i;
  activeEdgeList.clear ( );
  vector<WinPt> points = poly.getWindowPoints();
  buildActiveEdgeTable ( points );

  for ( y = 0; y < ImageH; y++ )
    {
      // add edges into active Edge List
      for ( i = 0; i < activeEdgeTable [ y ].size ( ); i++ )
        {
          activeEdgeList.push_back ( activeEdgeTable [ y ] [ i ] );
        }

      // delete edges from active Edge List
      for ( i = 0; i < activeEdgeList.size ( ); i++ )
        {
          if ( activeEdgeList [ i ].maxY <= y )
            {
              activeEdgeList.erase ( activeEdgeList.begin ( ) + i );
              i--;
            }
        }

      // sort according to x value... a little expensive since not always necessary
      sort ( activeEdgeList.begin ( ), activeEdgeList.end ( ) );

      // draw scan line
      for ( i = 0; i < activeEdgeList.size ( ); i += 2 )
        {
          float Z = activeEdgeList[i].currentZ;
          float dZ = (activeEdgeList[i+1].currentZ - activeEdgeList[i].currentZ) / (activeEdgeList[i+1].currentX - activeEdgeList[i].currentX);

          Vertex dF = (activeEdgeList[i+1].currentF - activeEdgeList[i].currentF) / (activeEdgeList[i+1].currentX - activeEdgeList[i].currentX);
          Vertex F = activeEdgeList[i].currentF;

          for ( x = (int)ceil ( activeEdgeList [ i ].currentX ); x < activeEdgeList [ i + 1 ].currentX; x++ ) {
              Vertex color(1,1,1);
              if(shading == Gourand){
                color = F;
              }

              else if(shading == Phong){
                Vertex fNormal = F;
                fNormal.make_unit_vector();
                color = light(fNormal);
              }

              else if(shading == Flat){
                 color = light(poly.normal);
              }

              setFramebuffer ( x, y, Z, color.x, color.y, color.z);
              F = F + dF;
              Z = Z + dZ;
            }
        }

      // update edges in active edge list
      for ( i = 0; i < activeEdgeList.size ( ); i++ )
        {
          activeEdgeList [ i ].currentX += activeEdgeList [ i ].slopeRecip;
          activeEdgeList [ i ].currentF = activeEdgeList[i].currentF + activeEdgeList[i].fIncr;
          activeEdgeList [ i ].currentZ = activeEdgeList[i].currentZ + activeEdgeList[i].zIncr;
        }
    }
}
