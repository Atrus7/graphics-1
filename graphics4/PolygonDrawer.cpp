#include "PolygonDrawer.h"
#include "ScanConvert.h"
#include <algorithm>
#include <math.h>
#include "ScanConvert.h"

class Edge
{
public:
	float slopeRecip;
	float maxY;
	float currentX;

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

void buildActiveEdgeTable ( vector<Pt> &points )
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
		if ( points [ i ].y == e.maxY )
		{
			e.currentX = points [ next ].x;
			activeEdgeTable [ points [ next ].y ].push_back ( e );
		}
		else
		{
			e.currentX = points [ i ].x;
			activeEdgeTable [ points [ i ].y ].push_back ( e );
		}
	}
}

// assumes all vertices are within window!!!
void drawPolygon ( vector<Pt> points )
{
	int x, y, i;

	activeEdgeList.clear ( );
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
			for ( x = (int)ceil ( activeEdgeList [ i ].currentX ); x < activeEdgeList [ i + 1 ].currentX; x++ )
			{
				setFramebuffer ( x, y, 1, 1, 1 );
			}
		}

		// update edges in active edge list
		for ( i = 0; i < activeEdgeList.size ( ); i++ )
		{
			activeEdgeList [ i ].currentX += activeEdgeList [ i ].slopeRecip;
		}
	}
}
