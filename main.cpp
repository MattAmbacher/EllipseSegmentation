#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include "itkGDCMImageIO.h"
#include "gdcmImageReader.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkCannyEdgeDetectionImageFilter.h"
#include "itkImageRegionIterator.h"
#include "itkCastImageFilter.h"
#include "FilterSkull.h"
#include "LineSegment.h"
#include "ArcGroup.h"

int main() {

	using std::string;			using std::vector;

	const char file[] = "C:\\Users\\Matthew Ambacher\\Documents\\8yr Skull v4 T2 TSE\\Export\\8Yrskull_V4\\Localizers - 435769225\\T2TSE_High_Res_1001\\IM-0001-0150.dcm";
	const char outfile[] = "C:\\Users\\Matthew Ambacher\\Desktop\\test.dcm";
	std::ofstream ofile;
	ofile.open("C:\\Users\\Matthew Ambacher\\Desktop\\SkullPoints.txt");
	typedef itk::Image<float, 2> InputImageType;
	typedef itk::Image<float, 2> OutputImageType;
	
	OutputImageType::Pointer binarizedImage = FilterSkull(file);

	typedef itk::ImageFileWriter< OutputImageType > WriterType;
	WriterType::Pointer writer = WriterType::New();
	std::cout << "-------------------------------------------------------------------" << std::endl;
	std::cout << std::endl;
	std::cout << "\t\t\tFINDING SEGMENTS" << std::endl;
	std::cout << std::endl;
	std::cout << "-------------------------------------------------------------------" << std::endl;
	vector<LineSegment> horizSegments = FindHorizontalLines(binarizedImage);
	vector<LineSegment> vertSegments = FindVerticalLines(binarizedImage);
	vector<LineSegment> posDiagSegments = FindPosDiagLines(binarizedImage);
	vector<LineSegment> negDiagSegments = FindNegDiagLines(binarizedImage);
	std::cout << "Done!" << std::endl;

	std::cout << "-------------------------------------------------------------------" << std::endl;
	std::cout << std::endl;
	std::cout << "\t\t\tEXTRACTING LINES" << std::endl;
	std::cout << std::endl;
	std::cout << "-------------------------------------------------------------------" << std::endl;
	vector<LineSegment> horizLines = HorizLineExtraction(horizSegments, 672, 672);
	vector<LineSegment> vertLines = VertLineExtraction(vertSegments, 672, 672);
	vector<LineSegment> posDiagLines = DiagExtraction(posDiagSegments, 672, 672);
	vector<LineSegment> negDiagLines = DiagExtraction(negDiagSegments, 672, 672);
	std::cout << "Done!" << std::endl;

	
	vector<LineSegment> lines;
	lines.reserve(horizLines.size() + vertLines.size() + posDiagLines.size() + negDiagLines.size());
	lines.insert(lines.end(), horizLines.begin(), horizLines.end());
	lines.insert(lines.end(), vertLines.begin(), vertLines.end());
	lines.insert(lines.end(), posDiagLines.begin(), posDiagLines.end());
	lines.insert(lines.end(), negDiagLines.begin(), negDiagLines.end());
	
	std::cout << "-------------------------------------------------------------------" << std::endl;
	std::cout << std::endl;
	std::cout << "\t\t\tEXTRACTING ARCS" << std::endl;
	std::cout << std::endl;
	std::cout << "-------------------------------------------------------------------" << std::endl;

	vector<ArcGroup> arcs = ArcExtraction(lines);
	
	std::cout << "Done!" << std::endl;
	std::cout << "-------------------------------------------------------------------" << std::endl;
	std::cout << std::endl;
	std::cout << "\t\t\tWRITING OUTPUT TO FILE" << std::endl;
	std::cout << std::endl;
	std::cout << "-------------------------------------------------------------------" << std::endl;
	

	 /* -------------- for testing purposes -------------- */
	for (vector<ArcGroup>::iterator it = arcs.begin(); it != arcs.end(); ++it)
	{
		for (vector<LineSegment>::iterator lineIt = it->LA.begin(); lineIt != it->LA.end(); ++lineIt) {
			ofile << lineIt->startx << '\t' << lineIt->starty << std::endl;
			ofile << lineIt->endx << '\t' << lineIt->endy << std::endl;
		}
	}
	
	ofile.close();
	/* -------------- end of testing -------------- */

	std::cout << "Done!" << std::endl;

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