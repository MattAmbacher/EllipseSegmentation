#include <math.h>
#include "LineSegment.h"
#include "ArcGroup.h"

using std::vector;

vector<ArcGroup> ArcExtraction (vector<LineSegment> lineSeg)
{
	vector<LineSegment> potentialArcGroup;
	vector<ArcGroup> arcGroup;
	const int DLINE = 100;
	const int GAPX2 = DLINE; //used for adaptive triangular window
	const int GAPY = DLINE - 1; //used for adaptive triangular window
	int lowBoundRight[GAPX2]; //used for adaptive triangular window
	int upBoundRight[GAPX2]; //used for adaptive triangular window
	int lowBoundLeft[GAPX2];
	int upRightBound[GAPX2];
	int downRightBound[GAPX2];
	int downLeftBound[GAPX2];
	int upLeftBound[GAPX2];
	int leftBound, rightBound, lowLeftBound, lowRightBound;
	double theta1 = 0;
	double theta2 = 0;
	double intersectionAngle = 0;
	double estTheta = 0;
	double R = 0;
	double xc = 0;
	double yc = 0;
	int startPtX, startPtY, endPtX, endPtY;
	double error = 15;

	for (vector<LineSegment>::iterator si = lineSeg.begin(); si != lineSeg.end(); ++si)
	{
		potentialArcGroup.clear();
		potentialArcGroup.push_back(*si);
		startPtX = si->endx;
		startPtY = si->endy;
		vector<LineSegment>::iterator temp_si = si; //create a temporary iterator to ensure we hit every line in the lineSeg vector.
		for (vector<LineSegment>::iterator sq = lineSeg.begin(); sq != lineSeg.end(); ++sq)
		{
			if (temp_si != sq)
			{
				int GAPX1 = abs((temp_si->endx - temp_si->startx + 1)/ 2 - 1);
				int lowBoundLeft = startPtY - 10; //used for adaptive triangular window
				int upBoundLeft = startPtY + 10; //used for adaptive triangular window
				bool inWindow = false;
				//Generate adaptive triangular window
				for (int i = 0; i != GAPX2; ++i)
				{
					lowBoundRight[i] = (yn(startPtY) - (i+1));
					upBoundRight[i] = (yn(startPtY) + (i+1));
				}
				if (temp_si->GetDirection() == "right")
				{
					leftBound = xn(startPtX) - GAPX1;
					rightBound = xn(startPtX) + GAPX2;
					if (sq->startx >= startPtX)
					{
						//use triangle part of window for horiz/diagonal lines
						if (sq->starty >= lowBoundRight[sq->startx - startPtX] && sq->starty <= upBoundRight[sq->startx - startPtX]) {
							endPtX = sq->startx;
							endPtY = sq->starty;
							inWindow = true;
						}
					}
					else if ( sq->startx < startPtX) {
						if (sq->starty >= lowBoundLeft && sq->starty <= upBoundLeft) {
							endPtX = sq->startx;
							endPtY = sq->starty;
							inWindow = true;
						}
					}

					else if (sq->endx >= startPtX) {
						//use triangle part of window for horiz/diagonal lines
						if (sq->endy >= lowBoundRight[sq->endy - temp_si->endx] && sq->endy <= upBoundRight[sq->startx - temp_si->endx]) {
							endPtX = sq->endx;
							endPtY = sq->endy;
							inWindow = true;
						}
					}
					else { //(sq->endx < startPtX) 
						if (sq->endy >= lowBoundLeft && sq->endy <= upBoundLeft) {
							endPtX = sq->endx;
							endPtY = sq->endy;
							inWindow = true;
						}
					}
				}

				else //reverse the window if the line is going to the left
				{
					rightBound = xn(startPtX) + GAPX1;
					leftBound = xn(startPtX) - GAPX2;
					if (sq->startx <= startPtX){
						//use triangle part of window for horiz/diagonal lines
						if (sq->starty >= lowBoundRight[startPtX - sq->startx] && sq->starty <= upBoundRight[startPtX - sq->startx]) {
							endPtX = sq->startx;
							endPtY = sq->starty;
							inWindow = true;
						}
					}
					else if (sq->startx > startPtX) {
						if (sq->starty >= lowBoundLeft && sq->starty <= upBoundLeft){
							endPtX = sq->startx;
							endPtY = sq->starty;
							inWindow = true;
						}
					}
					else if (sq->endx <= startPtX) {
						if (sq->endy >= lowBoundRight[startPtX - sq->endx] && sq->endy <= upBoundRight[startPtX - sq->endx]) {
							endPtX = sq->endx;
							endPtY = sq->endy;
							inWindow = true;
						}
					}
					else if (sq->endx > startPtX) {  
						if (sq->endy >= lowBoundLeft && sq->endy <= upBoundLeft) {
							endPtX = sq->endx;
							endPtY = sq->endy;
							inWindow = true;
						}
					}
				}
					
				//Check if in Triangular window for vertcal lines
				if (!inWindow) 
				{
					GAPX1 = abs((temp_si->endy - temp_si->starty + 1)/ 2 - 1);
					int lowBound, upBound;
					if (temp_si->GetDirection() == "down")
					{
						lowBound = yn(startPtY) - GAPX1;
						upBound = yn(startPtY) + GAPX2;
						for(int i=0; i < GAPX2; ++i)
						{
							upRightBound[i] = xn(startPtX) + (i + 1);
							upLeftBound[i] = xn(startPtX) - (i + 1);
						}

						lowLeftBound = startPtX - 10;
						lowRightBound = startPtX + 10;

						if (sq->starty >= startPtY && sq->starty <= upBound)
						{
							if (sq->startx >= upLeftBound[sq->starty - startPtY] && sq->startx <= upRightBound[sq->starty - startPtY]) {
								endPtX = sq->startx;
								endPtY = sq->starty;
								inWindow = true;
							}
						}

						else if (sq->endy >= startPtY && sq->endy <= upBound)
						{
							if (sq->endx >= upLeftBound[sq->endy - startPtY] && sq->endx <= upRightBound[sq->endy - startPtY]) {
								endPtX = sq->endx;
								endPtY = sq->endy;
								inWindow = true;
							}
						}

						else if (sq->starty >= lowBound && sq->starty < startPtY)
						{
							if (sq->startx >= lowLeftBound && sq->startx <= lowRightBound) {
								endPtX = sq->startx;
								endPtY = sq->starty;
								inWindow = true;
							}
						}

						else if (sq->endy >= lowBound && sq->endy < startPtY)
						{
							if (sq->endx >= lowLeftBound && sq->endx <= lowRightBound) {
								endPtX = sq->endx;
								endPtY = sq->endy;
								inWindow = true;
							}
						}
					}

					else //if (temp_si->GetDirection() == "up")
					{
						upBound = startPtY + GAPX1;
						lowBound = startPtY - GAPX2;

						lowLeftBound = startPtX - 10;
						lowRightBound = startPtX + 10;

						for (int i=0; i < GAPX2; ++i)
						{
							downRightBound[i] = xn(startPtX) + (i + 1);
							downLeftBound[i] =  xn(startPtX) - (i + 1);
						}

						if (sq->starty <= startPtY && sq->starty >= lowBound)
						{
							if (sq->startx >= downLeftBound[startPtY - sq->starty] && sq->startx <= downRightBound[startPtY - sq->starty]) {
								endPtX = sq->startx;
								endPtY = sq->starty;
								inWindow = true;
							}
						}

						else if (sq->endy <= startPtY && sq->endy >= lowBound)
						{
							if (sq->endx >= downLeftBound[startPtY - sq->endy] && sq->endx <= downRightBound[startPtY - sq->endy]) {
								endPtX = sq->endx;
								endPtY = sq->endy;
								inWindow = true;
							}
						}
						
						else if (sq->starty >= startPtY && sq->starty <= upBound)
						{
							if (sq->startx >= lowLeftBound && sq->startx <= lowRightBound) {
								endPtX = sq->startx;
								endPtY = sq->starty;
								inWindow = true;
							}
						}

						else if (sq->endy >= startPtY && sq->endy <= upBound)
						{
							if (sq->endx <= lowLeftBound && sq->endx <= lowRightBound) {
								endPtX = sq->endx;
								endPtY = sq->endy;
								inWindow = true;
							}
						}

					}
				}
				
				
				if (inWindow)
				{
					theta1 = temp_si->GetTheta();
					theta2 = sq->GetTheta();
					intersectionAngle = abs(theta1 - theta2);

					if (intersectionAngle < 60)
						{
							estTheta = ThetaEstimation(*(temp_si), *sq);
							if (fabs(estTheta - theta2) > error)
								continue; //no good, move to next line seg
							potentialArcGroup.push_back(*sq);
							temp_si = sq;
							if (endPtX == sq->endx) { //finished on endpoint of line, use start point of line as beginning of new search
								startPtY = temp_si->starty;
								startPtX = temp_si->startx;
							}
							else {
								startPtY = temp_si->endy;
								startPtX = temp_si->endx;
							}
						}
				}
			}
		}

		if (potentialArcGroup.size() > 1) 
		{
			double* center = CenterEstimation(potentialArcGroup);
			R = REstimation(potentialArcGroup);
			xc = center[0];
			yc = center[1];
			/* Get rid of outlier points that get picked up by imposing restrictions. Radius restriction gets rid of straight lines along side of images (R = infinity in this case).
			Center condition also helps get rid of any circular type shapes that occur near borders of image */
			if (R < 672 && ((xc-336)*(xc-336) + (yc-336)*(yc-336)) < 8000) 
				arcGroup.push_back(ArcGroup(potentialArcGroup, xc, yc, R));
		}
	}

	return arcGroup;
}