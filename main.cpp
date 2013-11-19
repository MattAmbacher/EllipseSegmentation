#include <string>
#include <iostream>
#include <vector>

#include "itkGDCMImageIO.h"
#include "gdcmImageReader.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkCannyEdgeDetectionImageFilter.h"
#include "itkImageRegionIterator.h"
#include "itkCastImageFilter.h"
#include "FilterSkull.h"
#include "LineSegment.h"

int main() {

	using std::string;			using std::vector;

	const char file[] = "C:\\Users\\Matthew Ambacher\\Documents\\8yr Skull v4 T2 TSE\\Export\\8Yrskull_V4\\Localizers - 435769225\\T2TSE_High_Res_1001\\IM-0001-0150.dcm";
	const char outfile[] = "C:\\Users\\Matthew Ambacher\\Desktop\\test.dcm";
	typedef itk::Image<float, 2> InputImageType;
	typedef itk::Image<float, 2> OutputImageType;
	
	OutputImageType::Pointer binarizedImage = FilterSkull(file);

	typedef itk::ImageFileWriter< OutputImageType > WriterType;
	WriterType::Pointer writer = WriterType::New();
	
	vector<LineSegment> horizSegments = FindHorizontalLines(binarizedImage);
	vector<LineSegment> vertSegments = FindVerticalLines(binarizedImage);
	vector<LineSegment> posDiagSegments = FindPosDiagLines(binarizedImage);
	vector<LineSegment> negDiagSegments = FindNegDiagLines(binarizedImage);

	vector<LineSegment> horizLines = HorizLineExtraction(horizSegments, 672, 672);
	std::cin.get();



/*
	writer->SetFileName( outfile );
	writer->SetInput( binarizedImage );


	try
    {
		writer->Update();
    }

    catch (itk::ExceptionObject & e)
    {
		std::cerr << "exception in file writer " << std::endl;
		std::cerr << e << std::endl;
	    std::cin.get();
		return EXIT_FAILURE;
    }
	*/
	return 0;

}