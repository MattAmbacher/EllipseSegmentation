#include <math.h>
#include "LineSegment.h"

using std::vector;

double ThetaEstimation(LineSegment L1, LineSegment L2)
{
	//using Thomas and Chan's Algorithm
	double x[] = {L1.startx, L1.midx, L1.endx, L2.startx, L2.midx, L2.endx};
	double y[] = {L1.starty, L1.midy, L1.endy, L2.starty, L2.midy, L2.endy};
	double N = 6;
	double xsum, ysum, x2sum, y2sum, xysum, x2ysum, xy2sum, x3sum, y3sum;
	xsum = ysum = x2sum = y2sum = xysum = x2ysum = xy2sum = x3sum = y3sum = 0;
	for (int i=0; i < N; ++i)
	{
		xsum += x[i];
		ysum += y[i];
		x2sum += (x[i]*x[i]);
		y2sum += (y[i]*y[i]);
		xysum += (x[i]*y[i]);
		x2ysum += (x[i]*x[i]*y[i]);
		xy2sum += (x[i]*y[i]*y[i]);
		x3sum += (x[i]*x[i]*x[i]);
		y3sum += (y[i]*y[i]*y[i]);
	}

	double a1 = 2*(xsum*xsum - N*x2sum);
	double a2 = 2*(xsum * ysum - N*xysum);
	double b1 = a2;
	double b2 = 2*(ysum*ysum - N*y2sum);
	double c1 = (x2sum*xsum - N*x3sum + xsum*y2sum - N*xy2sum);
	double c2 = x2sum*ysum - N*y3sum + ysum*y2sum - N*x2ysum;
	double xc, yc;
	if (a1*b2 == a2*b1)
	{
		xc = 1000000;
		yc = 1000000;
	}
	else
	{
		xc = (c1*b2 - c2*b1) / (a1*b2 - a2*b1);
		yc = (a1*c2 - a2*c1) / (a1*b2 - a2*b1);
	}
	if (yc == L2.midy)
		return 90;
	else
		return atan2(L2.midx - xc, yc - L2.midy) * 180 / 3.14159;
}

double REstimation(vector<LineSegment> lines)
{
	//using Thomas and Chan's Algorithm
	const int lineSize = lines.size();
	double* x = new double[lineSize*3];
	double* y = new double[lineSize*3];
	int index = 0;
	for (vector<LineSegment>::const_iterator it = lines.begin(); it != lines.end(); ++it)
	{
		x[index] = it->startx;
		x[index + 1] = it->midx;
		x[index + 2] = it->endx;
		y[index] = it->starty;
		y[index + 1] = it->midy;
		y[index + 2] = it->endy;
		index += 3;
	}

	double N = lineSize*3;
	double xsum, ysum, x2sum, y2sum, xysum, x2ysum, xy2sum, x3sum, y3sum;
	xsum = ysum = x2sum = y2sum = xysum = x2ysum = xy2sum = x3sum = y3sum = 0;
	for (int i=0; i < N; ++i)
	{
		xsum += x[i];
		ysum += y[i];
		x2sum += (x[i]*x[i]);
		y2sum += (y[i]*y[i]);
		xysum += (x[i]*y[i]);
		x2ysum += (x[i]*x[i]*y[i]);
		xy2sum += (x[i]*y[i]*y[i]);
		x3sum += (x[i]*x[i]*x[i]);
		y3sum += (y[i]*y[i]*y[i]);
	}

	double a1 = 2*(xsum*xsum - N*x2sum);
	double a2 = 2*(xsum * ysum - N*xysum);
	double b1 = a2;
	double b2 = 2*(ysum*ysum - N*y2sum);
	double c1 = (x2sum*xsum - N*x3sum + xsum*y2sum - N*xy2sum);
	double c2 = x2sum*ysum - N*y3sum + ysum*y2sum - N*x2ysum;
	double xc, yc;

	if (a1*b2 == a2*b1)
	{
		xc = 1000000;
		yc = 1000000;
	}
	else
	{
		xc = (c1*b2 - c2*b1) / (a1*b2 - a2*b1);
		yc = (a1*c2 - a2*c1) / (a1*b2 - a2*b1);
	}
	return sqrt( (1/N)*(x2sum - 2*xsum*xc + N*xc*xc + y2sum - 2*ysum*yc + N*yc*yc));
}

double* CenterEstimation(vector<LineSegment> lines)
{
	double* center = new double[2*sizeof(double)];
		//using Thomas and Chan's Algorithm
	const int lineSize = lines.size();
	double* x = new double[lineSize*3];
	double* y = new double[lineSize*3];
	int index = 0;
	for (vector<LineSegment>::const_iterator it = lines.begin(); it != lines.end(); ++it)
	{
		x[index] = it->startx;
		x[index + 1] = it->midx;
		x[index + 2] = it->endx;
		y[index] = it->starty;
		y[index + 1] = it->midy;
		y[index + 2] = it->endy;
		index += 3;
	}

	double N = lineSize*3;
	double xsum, ysum, x2sum, y2sum, xysum, x2ysum, xy2sum, x3sum, y3sum;
	xsum = ysum = x2sum = y2sum = xysum = x2ysum = xy2sum = x3sum = y3sum = 0;
	for (int i=0; i < N; ++i)
	{
		xsum += x[i];
		ysum += y[i];
		x2sum += (x[i]*x[i]);
		y2sum += (y[i]*y[i]);
		xysum += (x[i]*y[i]);
		x2ysum += (x[i]*x[i]*y[i]);
		xy2sum += (x[i]*y[i]*y[i]);
		x3sum += (x[i]*x[i]*x[i]);
		y3sum += (y[i]*y[i]*y[i]);
	}


	double a1 = 2*(xsum*xsum - N*x2sum);
	double a2 = 2*(xsum * ysum - N*xysum);
	double b1 = a2;
	double b2 = 2*(ysum*ysum - N*y2sum);
	double c1 = (x2sum*xsum - N*x3sum + xsum*y2sum - N*xy2sum);
	double c2 = x2sum*ysum - N*y3sum + ysum*y2sum - N*x2ysum;

	if (a1*b2 == a2*b1)
	{
		*center = 1000000;
		*(center + 1) = 1000000;
	}
	else
	{
		*center = (c1*b2 - c2*b1) / (a1*b2 - a2*b1);
		*(center + 1) = (a1*c2 - a2*c1) / (a1*b2 - a2*b1);
	}
	 return center;
}
