#include "LineSegment.h"
#include "itkImageLinearIteratorWithIndex.h"
#include "itkImage.h"

using std::vector;

typedef itk::Image<float, 2> ImageType;
typedef itk::ImageLinearIteratorWithIndex<ImageType> InIter;

vector<LineSegment> FindHorizontalLines(itk::Image<float, 2>::Pointer inputImage)
{
	vector<LineSegment> segList;

	InIter inIter(inputImage, inputImage->GetLargestPossibleRegion());
	inIter.Begin();
	inIter.SetDirection(0);

	ImageType::RegionType imageSize = inputImage->GetLargestPossibleRegion().GetSize();

	int startx, endx, starty, endy, midx, midy;
	while (inIter.Remaining())
	{
		inIter.GoToBeginOfLine();
		while (!inIter.IsAtEndOfLine())
		{
			if (inIter.Get() == 1)
			{
				startx = (inIter.GetIndex())[0];
				starty = (inIter.GetIndex())[1];
				while ((inIter.Get() == 1) && !inIter.IsAtEndOfLine())
					++inIter;
				--inIter; //subtract one because it goes one past the desired line with the while loop above
				endx = (inIter.GetIndex())[0];
				endy = (inIter.GetIndex())[1];
		
				if ( (endx - startx) >= 3 ) {
					segList.push_back(LineSegment(startx, endx, starty, endy, double(startx + endx) / 2.0, double(starty + endy) / 2.0));
				}
			}
			++inIter;
		}
		inIter.NextLine();
	}
	return segList;
}

vector<LineSegment> FindVerticalLines(itk::Image<float, 2>::Pointer inputImage)
{
	vector<LineSegment> segList;

	InIter inIter(inputImage, inputImage->GetLargestPossibleRegion());
	inIter.Begin();
	inIter.SetDirection(1);

	ImageType::RegionType imageSize = inputImage->GetLargestPossibleRegion().GetSize();

	int startx, endx, starty, endy, midx, midy;
	while (inIter.Remaining())
	{
		inIter.GoToBeginOfLine();
		while (!inIter.IsAtEndOfLine())
		{
			if (inIter.Get() == 1)
			{
				startx = (inIter.GetIndex())[0];
				starty = (inIter.GetIndex())[1];
				while ((inIter.Get() == 1) && !inIter.IsAtEndOfLine())
					++inIter;
				--inIter; //subtract one because it goes one past the desired line with the while loop above
				endx = (inIter.GetIndex())[0];
				endy = (inIter.GetIndex())[1];
		
				if ( (endy - starty) >= 2 ) {
					segList.push_back(LineSegment(startx, endx, starty, endy, double(startx + endx) / 2.0, double(starty + endy) / 2.0));
				}
			}
			++inIter;
		}
		inIter.NextLine();
	}
	return segList;
}

vector<LineSegment> FindPosDiagLines(itk::Image<float, 2>::Pointer inputImage)
{
	vector<LineSegment> segList;

	InIter inIter(inputImage, inputImage->GetLargestPossibleRegion());
	inIter.Begin();
	inIter.SetDirection(0);

	const itk::SizeValueType *imageSize = inputImage->GetLargestPossibleRegion().GetSize().GetSize();
	int startx, endx, starty, endy, midx, midy;
	int length = 0;
	while (inIter.Remaining())
	{
		inIter.GoToBeginOfLine();
		while (!inIter.IsAtEndOfLine())
		{
			
			if (inIter.Get() == 1)
			{
				InIter origIter = inIter;
				startx = (inIter.GetIndex())[0];
				starty = (inIter.GetIndex())[1];

				while ((inIter.Get() == 1) && !inIter.IsAtEndOfLine()) {
					inIter.NextLineImproved();
					inIter++;
					length++;
				}
				//----- Need to go back one diagonal pixel because while loop takes an extra one.----
				inIter--;
				inIter.PreviousLineImproved();
				//-----------------------------------------------------------------------------------
				endx = (inIter.GetIndex())[0];
				endy = (inIter.GetIndex())[1];
		
				if ( length >= 2 ) {
					segList.push_back(LineSegment(startx, endx, starty, endy, double(startx + endx) / 2.0, double(starty + endy) / 2.0));
				}

				for (int i=1; i < length; i++){
					inIter--;
					inIter.PreviousLineImproved();
				}
			}
			
			++inIter;
		}
		inIter.NextLine();
	}
	return segList;
}


vector<LineSegment> FindNegDiagLines(itk::Image<float, 2>::Pointer inputImage)
{
	vector<LineSegment> segList;

	InIter inIter(inputImage, inputImage->GetLargestPossibleRegion());
	inIter.Begin();
	inIter.SetDirection(0);

	const itk::SizeValueType *imageSize = inputImage->GetLargestPossibleRegion().GetSize().GetSize();
	int startx, endx, starty, endy, midx, midy;
	int length = 0;
	while (inIter.Remaining())
	{
		inIter.GoToBeginOfLine();
		while (!inIter.IsAtEndOfLine())
		{
			
			if (inIter.Get() == 1)
			{
				InIter origIter = inIter;
				startx = (inIter.GetIndex())[0];
				starty = (inIter.GetIndex())[1];

				while ((inIter.Get() == 1) && !inIter.IsAtEndOfLine()) {
					inIter.NextLineImproved();
					inIter--;
					length++;
				}
				//----- Need to go back one diagonal pixel because while loop takes an extra one.----
				inIter++;
				inIter.PreviousLineImproved();
				//-----------------------------------------------------------------------------------
				endx = (inIter.GetIndex())[0];
				endy = (inIter.GetIndex())[1];
		
				if ( length >= 2 ) {
					segList.push_back(LineSegment(startx, endx, starty, endy, double(startx + endx) / 2.0, double(starty + endy) / 2.0));
				}

				for (int i=1; i < length; i++){
					inIter++;
					inIter.PreviousLineImproved();
				}
			}
			
			++inIter;
		}
		inIter.NextLine();
	}
	return segList;
}