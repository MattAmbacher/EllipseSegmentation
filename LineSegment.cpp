#include "LineSegment.h"
#include "itkImageLinearIteratorWithIndex.h"

LineSegment::LineSegment(int sx, int ex, int sy, int ey, double mx, double my) 
{
	startx = sx;
	endx = ex;
	starty = sy;
	endy = ey;
	midx = mx;
	midy = my;
}
LineSegment::~LineSegment() { }