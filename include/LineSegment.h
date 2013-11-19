#ifndef GUARD_LINESEGMENT_H
#define GUARD_LINESEGMENT_H
#include <vector>
#include "itkImage.h"
#include "itkImageLinearIteratorWithIndex.h"

class LineSegment{
public:
	 LineSegment(int, int, int, int, double, double);
	 LineSegment(const LineSegment&);
	 LineSegment();
	 ~LineSegment();

	void SetLength();
	float GetLength();
	int startx, endx, starty, endy;
	double midx, midy;

	LineSegment& operator= (const LineSegment& );
	friend bool operator==(const LineSegment&, const LineSegment&);
	friend std::ostream& operator<<(std::ostream&, const LineSegment&);

private:	
	float length;

};

std::vector<LineSegment> FindHorizontalLines(itk::Image<float, 2>::Pointer);
std::vector<LineSegment> FindVerticalLines(itk::Image<float, 2>::Pointer);
std::vector<LineSegment> FindPosDiagLines(itk::Image<float, 2>::Pointer);
std::vector<LineSegment> FindNegDiagLines(itk::Image<float, 2>::Pointer);
std::vector<LineSegment> HorizLineExtraction(std::vector<LineSegment>, int, int);

#endif