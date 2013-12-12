#include <vector>
#include <algorithm>
#include "LineSegment.h"
#include "itkImage.h"

using std::vector;

const int GAPX = 20;
const int GAPY = 20;

vector<LineSegment> HorizLineExtraction(vector<LineSegment> horizSegments, int maxWidth, int maxHeight){
	vector<LineSegment> lineList;
	vector<LineSegment> tempLineList;
	int leftBoundary, rightBoundary, lowBoundary, upBoundary;
	double slope1, a; //used to determine the upper/lower boundary of the window.
	for (vector<LineSegment>::iterator si = horizSegments.begin(); si != horizSegments.end(); si++)
	{
		//step 1
		tempLineList.push_back(*si);
		for (vector<LineSegment>::iterator sq = si+1; sq != horizSegments.end(); ++sq)
		{	
			//step 2
			LineSegment SL = (si->GetLength() >= sq->GetLength()) ? *si: *sq; //longest line between sq and si	

			leftBoundary = (si->startx - GAPX);
			rightBoundary = (si->endx + GAPX);
			lowBoundary = si->starty;
			slope1 = ( 2 * ( yn(si->endy) - y0(si->starty)) - 1 ) / (2*(xn(si->endx) - x0(si->startx)));
			upBoundary = slope1 * (leftBoundary) + (yn(si->endy) - slope1 * xn(si->endx));
			if ( ((sq->startx >= leftBoundary && sq->startx <= rightBoundary) || (sq->endx >= leftBoundary && sq->endx <=rightBoundary)) &&
				((sq->starty >= lowBoundary && sq->starty <= upBoundary) || (sq->endy >= lowBoundary && sq->endy <= upBoundary))) //is segment in window?
			{
				//step 3
				if(sq->endx > si->endx) //positive line
					a = (yn(sq->endy) - y0(si->starty))/ (xn(sq->endx) - x0(si->startx));
				else if (sq->startx < si->startx) //negative line
					a = (yn(si->endy) - y0(sq->starty)) / (xn(si->endx) - x0(sq->startx));
				else //vertical line
					continue;
				
				//step4
				bool validLine = false;
				validLine = fabs( ((double)y0(SL.starty) - a*x0(SL.startx)) - (yn(SL.endy) - a*xn(SL.endx))) < 1.0 ;

				if (validLine) //if valid go to step 5, if not go to step 7
				{
					//step 5
					tempLineList.push_back(*sq);
					si = sq;
					continue; //continue is step 6, aka starts the process over and keeps looking
				}
			}
		}
		//step 7
		if (tempLineList[0].startx < tempLineList.back().endx)
			lineList.push_back( LineSegment(tempLineList.back().endx, tempLineList[0].startx, tempLineList.back().endy, tempLineList[0].starty,  (tempLineList[0].startx + tempLineList.back().endx) / 2.0, 
				(tempLineList[0].starty + tempLineList.back().endy) / 2.0));
		else
			lineList.push_back(LineSegment(tempLineList[0].startx, tempLineList.back().endx, tempLineList[0].starty, tempLineList.back().endy, (tempLineList[0].startx + tempLineList.back().endx) / 2.0, 
				(tempLineList[0].starty + tempLineList.back().endy) / 2.0));
		tempLineList.clear();
	}
	return lineList;
}

vector<LineSegment> VertLineExtraction(vector<LineSegment> vertSegments, int maxWidth, int maxHeight){

	// To extract Vertical lines simply exchange x and y parameters
	vector<LineSegment> lineList;
	vector<LineSegment> tempLineList;
	int leftBoundary, rightBoundary, lowBoundary, upBoundary;
	double slope1, a; //used to determine the upper/lower boundary of the window.
	for (vector<LineSegment>::iterator si = vertSegments.begin(); si != vertSegments.end(); si++)
	{
		//step 1
		tempLineList.push_back(*si);
		for (vector<LineSegment>::iterator sq = si+1; sq != vertSegments.end(); ++sq)
		{	
			//step 2
			LineSegment SL = (si->GetLength() >= sq->GetLength()) ? *si: *sq; //longest line between sq and si	
			leftBoundary = (si->startx);
			lowBoundary = (si->starty - GAPY >= 0) ? (si->starty - GAPY) : 0;
			slope1 = ( 2 * ( xn(si->endx) - x0(si->startx)) - 1 ) / (2*(yn(si->endy) - y0(si->starty)));
			upBoundary = (si->endy + GAPY < maxHeight) ? (si->endy + GAPY) : maxHeight;
			rightBoundary = slope1 * (lowBoundary) + (xn(si->endx) - slope1 * yn(si->endy));
			if ( ((sq->startx >= leftBoundary && sq->startx <= rightBoundary) || (sq->endx >= leftBoundary && sq->endx <=rightBoundary)) &&
				((sq->starty >= lowBoundary && sq->starty <= upBoundary) || (sq->endy >= lowBoundary && sq->endy <= upBoundary))) //is segment in window?
			{
				//step 3
				if(sq->endy > si->endy) //positive line
					a = (xn(sq->endx) - x0(si->startx))/ (yn(sq->endy) - y0(si->starty));
				else if (sq->starty < si->starty) //negative line
					a = (xn(si->endx) - x0(sq->startx)) / (yn(si->endy) - y0(sq->starty));
				else //vertical line
					continue;
				//step4
				bool validLine = false;
				validLine = fabs( ((double)x0(SL.startx) - a*y0(SL.starty)) - (xn(SL.endx) - a*yn(SL.endy))) < 1.0 ;

				if (validLine) //if valid go to step 5, if not go to step 7
				{
					//step 5
					tempLineList.push_back(*sq);
					si = sq;
					continue; //continue is step 6, aka starts the process over and keeps looking
				}
			}
		}
		//step 7
		lineList.push_back(LineSegment(tempLineList[0].startx, tempLineList.back().endx, tempLineList[0].starty, tempLineList.back().endy, (tempLineList[0].startx + tempLineList.back().endx) / 2.0, 
			(tempLineList[0].starty + tempLineList.back().endy) / 2.0));
				if (tempLineList[0].startx > tempLineList.back().endx)
		tempLineList.clear();
	}
	return lineList;
}

vector<LineSegment> DiagExtraction(vector<LineSegment> diagSegments, int maxWidth, int maxHeight) 
{
	vector<LineSegment> lineList;
	vector<LineSegment> tempLineList;
	int leftBoundary, rightBoundary, lowBoundary, upBoundary;
	double a; 
	
	double cosTheta = cos(3.14159 / 4.0);
	for (vector<LineSegment>::iterator si = diagSegments.begin(); si != diagSegments.end(); si++)
	{
		//step 1
		tempLineList.push_back(*si);
		for (vector<LineSegment>::iterator sq = si+1; sq != diagSegments.end(); ++sq)
		{	
			//step 2
			//Diverged a bit from the paper here, making a simpler window rather than the skewed window they create.
			lowBoundary = (si->midy - (si->GetLength()/2.0*cosTheta - GAPY));
			upBoundary = (si->midy + (si->GetLength()/2.0*cosTheta + GAPY));
			leftBoundary = (si->midx - (si->GetLength()/2.0*cosTheta - GAPX)); //technically sin(theta), but cos(45) sin(45) are the same
			rightBoundary = (si->midx + (si->GetLength()/2.0*cosTheta + GAPX)); //technically sin(theta), but cos(45) sin(45) are the same
			LineSegment SL = (si->GetLength() >= sq->GetLength()) ? *si: *sq; //longest line between sq and si	
			if ( ((sq->startx >= leftBoundary && sq->startx <= rightBoundary) || (sq->endx >= leftBoundary && sq->endx <=rightBoundary)) &&
				((sq->starty >= lowBoundary && sq->starty <= upBoundary) || (sq->endy >= lowBoundary && sq->endy <= upBoundary))) //is segment in window?
			{
				//step 3
				if (sq->startx > si->startx)
					a = (yn(sq->endy) - y0(si->starty))/ (xn(sq->endx) - x0(si->startx));
				else if (sq->startx < si->startx) //negative line
					a = (yn(si->endy) - y0(sq->starty)) / (xn(si->endx) - x0(sq->startx));
				else //vertical line
					continue;
				//step4
				bool validLine = false;
				validLine = fabs(((double)x0(SL.startx) - a*y0(SL.starty)) - (xn(SL.endx) - a*yn(SL.endy))) < 1.0 ;

				if (validLine) //if valid go to step 5, if not go to step 7
				{
					//step 5
					tempLineList.push_back(*sq);
					si = sq;
					continue; //continue is step 6, aka starts the process over and keeps looking
				}
			}
		}
		//step 7

		lineList.push_back(LineSegment(tempLineList[0].startx, tempLineList.back().endx, tempLineList[0].starty, tempLineList.back().endy, (tempLineList[0].startx + tempLineList.back().endx) / 2.0, 
			(tempLineList[0].starty + tempLineList.back().endy) / 2.0));
		tempLineList.clear();
	}
	return lineList;
}
