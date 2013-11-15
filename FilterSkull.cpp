#include "itkImageFileWriter.h"
#include "itkImageFileReader.h"
#include "itkCannyEdgeDetectionImageFilter.h"
#include "itkImageRegionIterator.h"
#include "itkCastImageFilter.h"
#include "itkImage.h"

 itk::Image<float, 2>::Pointer FilterSkull(const char file[])
{
	double variance = 2.0;
	double lowerThresh = 50;
	double upperThresh = 100;

	typedef itk::Image<float, 2> InputImageType;
	typedef itk::Image<float, 2> OutputImageType;
	typedef itk::CastImageFilter<InputImageType, OutputImageType> CastFilterType;
	typedef itk::ImageFileReader< InputImageType > ReaderType;
	typedef itk::CannyEdgeDetectionImageFilter <InputImageType, OutputImageType> CannyEdge;

	ReaderType::Pointer reader = ReaderType::New();
	ReaderType::Pointer reversed = ReaderType::New();

	reader->SetFileName(file);
	reader->Update();
	itk::ImageRegionIterator<InputImageType> imageIter(reader->GetOutput() , reader->GetOutput()->GetRequestedRegion() );
	
	while( !imageIter.IsAtEnd() )
	{
		imageIter.Set(255 - imageIter.Get()); //reverse image colours when working with MRI
		++imageIter;
	}

	CannyEdge::Pointer cannyFilter = CannyEdge::New();
	cannyFilter->SetInput(reader->GetOutput() );
	cannyFilter->SetVariance( variance );

	cannyFilter->SetLowerThreshold( lowerThresh );
	cannyFilter->SetUpperThreshold( upperThresh );
	cannyFilter->Update();

	return cannyFilter->GetOutput();
}