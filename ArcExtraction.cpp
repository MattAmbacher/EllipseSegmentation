#include <math.h>
#include "LineSegment.h"
#include "ArcGroup.h"

using std::vector;

vector<ArcGroup> ArcExtraction (vector<LineSegment> lineSeg)
{
	vector<LineSegment> potentialArcGroup;
	vector<ArcGroup> arcGroup;
	const int DLINE = 1000;
	const int GAPX2 = DLINE; //used for adaptive triangular window
	const int GAPY = DLINE - 1; //used for adaptive triangular window
	int lowBoundRight[GAPX2]; //used for adaptive triangular window
	int upBoundRight[GAPX2]; //used for adaptive triangular window
	int leftBound = 0;
	int rightBound = 0;
	double theta1 = 0;
	double theta2 = 0;
	double intersectionAngle = 0;
	double estTheta = 0;
	double R = 0;
	double xc = 0;
	double yc = 0;

	double error = 14;
	for (vector<LineSegment>::iterator si = lineSeg.begin(); si != lineSeg.end(); ++si)
	{
		potentialArcGroup.clear();	

		for (vector<LineSegment>::iterator sq = si + 1; sq != lineSeg.end(); ++sq)
		{
			if (si != sq)
			{
				int GAPX1 = abs((si->endx - si->startx + 1)/ 2 - 1);
				
				int lowBoundLeft = si->endy - 1000; //used for adaptive triangular window
				int upBoundLeft = si->endy + 1000; //used for adaptive triangular window
		
				//Generate adaptive triangular window
				for (int i = 0; i != GAPX2; ++i)
				{
					//lowBoundRight[i] = ((yn(si->endy) - (10*i+1)) >= (yn(si->endy) - GAPY)) ? (yn(si->endy) - (10*i+1)) : (yn(si->endy) - GAPY);
					//upBoundRight[i] = ((yn(si->endy) + (10*i+1)) <= (yn(si->endy) + GAPY)) ? (yn(si->endy) + (10*i+1)) : (yn(si->endy) + GAPY);
					lowBoundRight[i] = (yn(si->endy) - (5*i+1));
					upBoundRight[i] = (yn(si->endy) + (5*i+1));
				}

				leftBound = xn(si->endx) - GAPX1 - 1000;
				rightBound = xn(si->endx) + GAPX2;

					//Check if in Triangular window for horizontal/diagonal lines
					bool inWindow = false;
					if (sq->startx >= leftBound && sq->startx <= rightBound)
					{
						if (sq->startx >= si->endx)
						{
							//use the triangle part of window for horizontal/diagonal lines
							if (sq->starty >= lowBoundRight[sq->startx - si->endx] && sq->starty <= upBoundRight[sq->startx - si->endx])
								inWindow = true;
						}
						else
							if (sq->starty >= lowBoundLeft && sq->starty <= upBoundLeft)
								inWindow = true;
					}
				
				//Check if in Triangular window for vertcal lines
				int lowBound = yn(si->endy) - GAPX1;
				int upBound = yn(si->endy) + GAPX2;
				int leftBoundLow = xn(si->endx) - 10;
				int rightBoundLow = xn(si->endx) + 10;
				int rightBoundUp[GAPX2];
				int leftBoundUp[GAPX2];

				for (int i = 0; i < GAPX2; ++i)
				{
					rightBoundUp[i] = xn(si->endx) + (15*i + 1);
					leftBoundUp[i] = xn(si->endx) - (15*i + 1);
				}

				if (sq->starty >= lowBound && sq->starty <= upBound)
				{
					if (sq->starty >= si->endy)
					{
						if (sq->startx >= leftBoundUp[sq->starty - si->endy] && sq->startx <= rightBoundUp[sq->starty - si->endy])
							inWindow = true;
					}
					else
						if (sq->startx >= leftBoundLow && sq->startx <= rightBoundLow)
							inWindow = true;
				}
				
				if (inWindow)
				{
					theta1 = si->GetTheta();
					theta2 = sq->GetTheta();

					intersectionAngle = fabs(theta1 - theta2);

					if (intersectionAngle > 90)
						continue; //no good, move to next line seg
					estTheta = ThetaEstimation(*(si), *sq);

					if (fabs(estTheta - theta2) > error)
						continue; //no good, move to next line seg
					potentialArcGroup.push_back(*sq);
					si = sq;
				}
			}
		}

		if (potentialArcGroup.size() > 1) 
		{
			double* center = CenterEstimation(potentialArcGroup);
			R = REstimation(potentialArcGroup);
			xc = center[0];
			yc = center[1];
			arcGroup.push_back(ArcGroup(potentialArcGroup, xc, yc, R));
		}
		
	}

	return arcGroup;
}