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
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "itkFixedArray.h"
#include "otbTimeSeries.h"

int otbPolynomialTimeSeriesTest(int argc, char* argv[])
{

  typedef double CoefficientPrecisionType;
  const unsigned int Degree = 2;
  typedef otb::PolynomialTimeSeries< Degree, CoefficientPrecisionType > FunctionType;
  typedef FunctionType::CoefficientsType CoefficientsType;

  CoefficientsType coefs;

  coefs[0] = 1;
  coefs[1] = 2;
  coefs[2] = 4;

  FunctionType f;
  f.SetCoefficients( coefs );

  for( int i=0; i<Degree; ++i)
    if(coefs[i] != f.GetCoefficient(i))
      return EXIT_FAILURE;
  
  return EXIT_SUCCESS;
}
