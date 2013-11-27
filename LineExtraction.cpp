#include <vector>
#include <algorithm>
#include "LineSegment.h"
#include "itkImage.h"

using std::vector;

const int GAPX = 50;
double Sq(double);
double Si(double);
double y0(double);
double yn(double);
double xn(double);
double x0(double);
double LengthBetweenTwoLines(LineSegment, LineSegment);

vector<LineSegment> HorizLineExtraction(vector<LineSegment> horizSegments, int maxWidth, int maxHeight){
	vector<LineSegment> lineList;
	int leftBoundary, rightBoundary, lowBoundary, upBoundary;
	LineSegment* SL = new LineSegment();
	double slope1, a; //used to determine the upper/lower boundary of the window.

	bool validLine = false;

	for (vector<LineSegment>::iterator si = horizSegments.begin(); si != horizSegments.end(); ++si)
	{
		//Step 1
		lineList.push_back(*si);

		for (vector<LineSegment>::iterator sq = si+1; sq != horizSegments.end(); ++ sq)
		{
			vector<LineSegment> intermediateLineList;
			(x0(si->startx) >= GAPX) ? (leftBoundary = x0(si->startx) - GAPX) : (leftBoundary = 0);
			(xn(si->endx) <= (maxHeight - GAPX - 1)) ? (rightBoundary = xn(si->endx) + GAPX) : (rightBoundary = maxHeight);
			upBoundary = y0(si->starty);
			slope1 = ( 2 * ( yn(si->endy) - y0(si->starty)) - 1) / (2*(xn(si->endx) - x0(si->startx)) );
			lowBoundary = slope1 * (leftBoundary) + (yn(si->endx) - slope1*xn(si->endx));
			if (lowBoundary >= maxHeight)
				lowBoundary = maxHeight;
			if (lowBoundary < 0)
				lowBoundary = 0;

			if ( ((x0(sq->startx) >= leftBoundary && x0(sq->startx) <= rightBoundary) || (xn(sq->endx) >= leftBoundary && xn(sq->startx) <= rightBoundary) )
				&& ( (y0(sq->starty) <= lowBoundary && y0(sq->starty) >= upBoundary) || (yn(sq->endy) <= lowBoundary && yn(sq->endy) >= upBoundary))) //checks if they're in the window
			{

				//Step 2
				*SL = (si->GetLength() >= sq->GetLength()) ? (*si) : (*sq);
				
				//Step 3
				if (sq->midx == si->midx)
					continue; //lines are directly over one another
				if ( (sq->midy - si->midy) / (sq->midx - si->midx) >= 0) //positive slope
					a = (yn(sq->endy) - y0(si->starty)) / (xn(sq->endx) - x0(si->startx));
				else
					a = (yn(si->endy) - y0(sq->starty))/(xn(si->endx) - x0(sq->startx));

				//Step 4
				validLine = fabs( (y0(SL->starty) - a * x0(SL->startx)) - (yn(SL->endy) - a* xn(SL->endx)) ) < 1;

				//if (validLine) go to step 5, else go to step 7
				if (validLine) {
					//step 5 and 6 (step 6 is just repeating Steps 3 and 4)
					intermediateLineList.push_back(*sq);
					si = sq; //sq becomes new starting line
					continue; //start over
				}
				std::cout << horizSegments.size() <<std::endl;

				//Step 7
				if (LengthBetweenTwoLines(intermediateLineList[0], intermediateLineList.back()) > 3) {
					lineList.push_back(LineSegment(intermediateLineList[0].startx, intermediateLineList.back().endx, intermediateLineList[0].starty, intermediateLineList.back().endy, (intermediateLineList[0].startx + intermediateLineList.back().endx) / 2.0,
						(intermediateLineList[0].starty + intermediateLineList.back().endy) / 2.0));
					for (vector<LineSegment>::iterator iLLit = intermediateLineList.begin(); iLLit != intermediateLineList.end(); ++iLLit)
					{
						vector<LineSegment>::iterator eraseIt = std::find(horizSegments.begin(), horizSegments.end(), *iLLit);
						horizSegments.erase(eraseIt); 
						std::cout<< horizSegments.size() << std::endl;
					}
				}
			}
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

double y0(double y) {
	return y + 0.5;
}

double yn(double y) {
	return y + 0.5;
}

double x0(double y) {
	return y + 0.5;
}

double xn(double y) {
	return y + 0.5;
}

double LengthBetweenTwoLines(LineSegment L1, LineSegment L2) {
	return sqrt( (double)(L2.endx - L1.startx) * (L2.endx - L1.startx) + (L2.endy - L1.starty) * (L2.endy - L1.starty));
}