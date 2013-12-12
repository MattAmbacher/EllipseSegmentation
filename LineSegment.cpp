#include <math.h>
#include <iostream>
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
	SetTheta();
	SetLength();
	SetDirection();
}
LineSegment::~LineSegment() { }

LineSegment::LineSegment(const LineSegment& LS) {
	startx = LS.startx;
	endx = LS.endx;
	starty = LS.starty;
	endy = LS.endy;
	midx = LS.midx;
	midy = LS.midy;
	SetTheta();
	SetLength();
	SetDirection();
}

void LineSegment::SetLength(void) {
	length = sqrt( (double)(endx - startx) * (endx - startx) + (endy - starty) * (endy - starty));
}

void LineSegment::SetDirection(void) {
	if (startx > endx)
		direction = "left";
	else if (startx < endx)
		direction = "right";
	else
	{
		if (starty > endy)
			direction = "up";
		else
			direction = "down";
	}

}

std::string LineSegment::GetDirection(void) {
	return direction;
}

void LineSegment::SetTheta(void) {
	if (startx == endx) 
		slope = 90;
	else
		slope = atan2((double)endy - midy, endx - midx) * 180 / 3.14159;
	if (slope < -90)
		slope += 180;
	if (slope > 90)
		slope -= 180;
}

double LineSegment::GetTheta(void) {
	return slope;
}
float LineSegment::GetLength(void) {
	return length;
}

LineSegment& LineSegment::operator=(const LineSegment& LS) {
	LineSegment* newLine;
	if (this != &LS)
	{
		try {
			newLine = new LineSegment(LS);
		}
		catch (...) {
			delete newLine;
			throw;		
		}

		this->startx = newLine->startx;
		this->endx = newLine->endx;
		this->starty = newLine->starty;
		this->endy = newLine -> endy;
		this->midx = newLine->midx;
		this->midy = newLine->midy;
		this->SetLength();

		delete newLine;
	}
	return *this;
}

bool operator==(const LineSegment& L1, const LineSegment& L2) {
	return ((L1.startx == L2.startx) && (L1.endx == L2.endx) && (L1.starty == L2.starty) && (L1.endy == L2.endy));
}

std::ostream& operator<<(std::ostream& out, const LineSegment& LS) {
	out << &LS;
	return out;
}

double y0(double y) {
	return y + 0.5;
}

double x0(double x) {
	return x + 0.5;
}

double yn(double y) {
	return y + 0.5;
}

double xn(double x) {
	return x + 0.5;
}