#include <math.h>
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

LineSegment::LineSegment(const LineSegment& ls) {
	startx = ls.startx;
	endx = ls.endx;
	starty = ls.starty;
	endy = ls.endy;
	midx = ls.midx;
	midy = ls.midy;

	if (length) {
		this->SetLength();
	}
}

LineSegment::LineSegment() {
	startx = 0;
	endx = 0;
	starty = 0;
	endy = 0;
	midx = 0;
	midy = 0;
}

LineSegment::~LineSegment() { }

void LineSegment::SetLength() {
	length = sqrt( (float)(endx - startx) * (endx - startx) + (endy - starty) * (endy - starty));
}

float LineSegment::GetLength() {
	return length;
}

LineSegment& LineSegment::operator=(LineSegment& LS){
	LineSegment* newLine;
	if (this != &LS) {
		try {
			newLine = new LineSegment(LS);
		}

		catch (...){
			delete[] newLine;
			throw;
		}
	
	startx = newLine->startx;
	endx = newLine->endx;
	starty = newLine->starty;
	endy = newLine->endy;
	midx = newLine->midx;
	midy = newLine->midy;
	SetLength();
	}
	delete newLine;
	return *this;
}

bool operator==(LineSegment L1, LineSegment L2) {
	return ((L1.startx == L2.startx) && (L1.endx == L2.endx) && (L1.starty == L2.starty) && (L1.endy == L2.endy));
}