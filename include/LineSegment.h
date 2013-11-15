#ifndef GUARD_LINESEGMENT_H
#define GUARD_LINESEGMENT_H
#include <vector>
#include "itkImage.h"
#include "itkImageLinearIteratorWithIndex.h"

class LineSegment{
public:
	 LineSegment(int, int, int, int, double, double);
	~LineSegment();

	int startx, endx, starty, endy;
	double midx, midy;
};

std::vector<LineSegment> FindHorizontalLines(itk::Image<float, 2>::Pointer);
std::vector<LineSegment> FindVerticalLines(itk::Image<float, 2>::Pointer);
std::vector<LineSegment> FindPosDiagLines(itk::Image<float, 2>::Pointer);
std::vector<LineSegment> FindNegDiagLines(itk::Image<float, 2>::Pointer);
std::vector<LineSegment> LineExtraction(std::vector<LineSegment>, int, int);

#endif