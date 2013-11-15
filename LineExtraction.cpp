#include <vector>
#include "LineSegment.h"
#include "itkImage.h"

using std::vector;

const int GAPX = 50;

vector<LineSegment> HorizLineExtraction(vector<LineSegment> horizSegments, int maxWidth, int maxHeight){
	vector<LineSegment> lineList;
	int leftBoundary, rightBoundary, lowBoundary, upBoundary;
	double slope1 //used to determine the upper/lower boundary of the window.
	for (vector<LineSegment>::iterator si = horizSegments.begin(); si != horizSegments.end(); ++si)
	{
		(si->startx >= GAPX) ? (leftBoundary = si->startx - GAPX) : (leftBoundary = 0);
		(si->endx <= (maxHeight - GAPX - 1)) ? (rightBoundary = si->endx + GAPX) : (rightBoundary = maxHeight);
		upBoundary = si->starty;
		slope1 = ( 2 * ( 
		for (vector<LineSegment>::iterator sq = si; sq != horizSegments.end(); ++ sq)
		{

		}
	}

	return lineList;
}

double Si(double x) {
	return x;
}

double Sq(double x) {
	return x;
}