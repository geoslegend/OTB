/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include "otbVectorImage.h"
#include "otbStreamingVectorizedSegmentation.h"
#include "otbImageFileReader.h"
#include "otbVectorDataFileWriter.h"
#include "otbVectorData.h"
#include "otbMeanShiftVectorImageFilter.h"
#include "itkConnectedComponentFunctorImageFilter.h"
#include "otbConnectedComponentMuParserFunctor.h"

#include "otbPersistentImageToVectorDataFilter.h"
#include "otbPersistentFilterStreamingDecorator.h"

int otbStreamingVectorizedSegmentationNew(int argc, char * argv[])
{
  typedef float InputPixelType;
  const unsigned int Dimension = 2;

  /** Typedefs */
  typedef otb::Image<InputPixelType,  Dimension>          ImageType;
  typedef otb::VectorData<double, 2>                      VectorDataType;
  typedef otb::MeanShiftImageFilter<ImageType, ImageType> MeanShiftImageFilterType;
  typedef otb::StreamingVectorizedSegmentation<ImageType, VectorDataType, MeanShiftImageFilterType>::FilterType StreamingVectorizedSegmentationType;

  StreamingVectorizedSegmentationType::Pointer filter = StreamingVectorizedSegmentationType::New();

  std::cout << filter << std::endl;

  return EXIT_SUCCESS;
}

int otbStreamingVectorizedSegmentation(int argc, char * argv[])
{

  typedef float InputPixelType;
  const unsigned int Dimension = 2;
  const std::string fieldName("DN");

  // Typedefs
  typedef otb::VectorImage<InputPixelType,  Dimension>          ImageType;
  typedef otb::VectorData<double, 2>                      VectorDataType;
  typedef otb::Image<unsigned int, Dimension>             LabelImageType;
  typedef otb::MeanShiftVectorImageFilter<ImageType, ImageType, LabelImageType> MeanShiftImageFilterType;
  
  typedef otb::Functor::ConnectedComponentMuParserFunctor<ImageType::PixelType>  FunctorType;
  typedef itk::ConnectedComponentFunctorImageFilter<ImageType, LabelImageType, FunctorType, LabelImageType > SegmentationFilterType;
  typedef otb::StreamingVectorizedSegmentation<ImageType, VectorDataType, SegmentationFilterType> StreamingVectorizedSegmentationType;
  //typedef otb::StreamingVectorizedSegmentation<ImageType, VectorDataType, MeanShiftImageFilterType> StreamingVectorizedSegmentationType;

  typedef otb::ImageFileReader<ImageType>                      ReaderType;
  typedef otb::VectorDataFileWriter<VectorDataType>            WriterType;

  ReaderType::Pointer             reader = ReaderType::New();
  StreamingVectorizedSegmentationType::Pointer filter = StreamingVectorizedSegmentationType::New();
  WriterType::Pointer             writer = WriterType::New();

  reader->SetFileName(argv[1]);
  reader->GenerateOutputInformation();
  filter->SetInput(reader->GetOutput());
  //filter->GetStreamer()->SetNumberOfLinesStrippedStreaming(atoi(argv[3]));
  filter->GetStreamer()->SetTileDimensionTiledStreaming(atoi(argv[3]));
  filter->SetFieldName(fieldName);
  filter->SetStartLabel(1);
  /*filter->GetSegmentationFilter()->SetSpatialRadius(10);
  filter->GetSegmentationFilter()->SetRangeRadius(15);
  filter->GetSegmentationFilter()->SetMinimumRegionSize(400);*/
  filter->GetSegmentationFilter()->GetFunctor().SetExpression("distance<40");
  
  filter->SetFileName(argv[2]);
  
  filter->Update();

  std::cout<< "begin writing ...." <<std::endl;
  
  writer->SetFileName(argv[2]);
  writer->SetInput(filter->GetOutputVectorData());
  writer->Update();
  std::cout<< "end writing ...." <<std::endl;

  return EXIT_SUCCESS;
}
