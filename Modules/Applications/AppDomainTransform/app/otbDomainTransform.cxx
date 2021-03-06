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

#include "otbWrapperApplication.h"
#include "otbWrapperApplicationFactory.h"
#include "otbWaveletImageFilter.h"
#include "otbWaveletInverseImageFilter.h"
#include "otbWaveletGenerator.h"

#include <itkConfigure.h>
#include <itkForwardFFTImageFilter.h>
#include <itkInverseFFTImageFilter.h>
#include <itkUnaryFunctorImageFilter.h>
#include <itkFFTShiftImageFilter.h>

#include "otbComplexToVectorImageCastFilter.h"

namespace otb
{
namespace Wrapper
{
template< class TInput, class TOutput>
class ToComplexPixel
{
public:
  ToComplexPixel ( ) { };

  ~ToComplexPixel( ) { };

  bool operator!=( const ToComplexPixel & ) const
    {
    return false;
    }
  bool operator==( const ToComplexPixel & other ) const
    {
    return !(*this != other);
    }
  inline TOutput operator( )( const TInput & A ) const
    {
      return TOutput( static_cast<typename TOutput::value_type> ( A[0] ),
		      static_cast<typename TOutput::value_type> ( A[1] ) );
    }
};

class DomainTransform : public Application
{
public:
  /** Standard class typedefs. */
  typedef DomainTransform     Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;

  typedef itk::SmartPointer<const Self> ConstPointer;
  typedef float InputPixelType;
  typedef float OutputPixelType;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(Self, otb::Application);

private:
  DomainTransform() {}

  ~DomainTransform() ITK_OVERRIDE
    {
    // This is a trick to make sure fftw will cleanup its threads when application
    // shared lib is released.
    #if defined(ITK_USE_FFTWF)
      fftwf_cleanup_threads();
      fftwf_cleanup();
    #endif
    #if defined(ITK_USE_FFTWD)
      fftw_cleanup_threads();
      fftw_cleanup();
    #endif
    }

  void DoInit() ITK_OVERRIDE
    {
    SetName("DomainTransform");
    SetDescription("Domain Transform application for wavelet and fourier");

    // Documentation
    SetDocName("DomainTransform");
    SetDocLongDescription("Domain Transform application for wavelet and fourier");
    SetDocLimitations("This application is not streamed, check your system resources when processing large images");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso("otbWaveletImageFilter, otbWaveletInverseImageFilter, otbWaveletTransform");
    AddDocTag(Tags::Filter);

    AddParameter(ParameterType_InputImage, "in",  "Input Image");
    SetParameterDescription("in", "This will take an input image to be transformed"
      " image. For FFT inverse transform, it expects a complex image as two-band"
      " image in which first band represent real part and second band represent"
      " imaginary part.");

    AddRAMParameter();

    AddParameter(ParameterType_Choice, "mode", "mode");
    SetParameterDescription("mode", "This parameter allows one to select between"
      " fft(fourier) and wavelet");
    AddChoice("mode.fft", "FFT transform");
    SetParameterDescription("mode.fft", "FFT transform");
    AddParameter(ParameterType_Empty, "mode.fft.shift", "false");
    SetParameterDescription("mode.fft.shift", "Shift transform of fft filter");
    AddChoice("mode.wavelet", "wavelet");
    SetParameterDescription("mode.wavelet", "Wavelet transform");
    AddParameter(ParameterType_Choice, "mode.wavelet.form", "Select wavelet form");

    AddChoice("mode.wavelet.form.haar", "HAAR");
    AddChoice("mode.wavelet.form.db4", "DAUBECHIES4");
    AddChoice("mode.wavelet.form.db6", "DAUBECHIES6");
    AddChoice("mode.wavelet.form.db8", "DAUBECHIES8");
    AddChoice("mode.wavelet.form.db12", "DAUBECHIES12");
    AddChoice("mode.wavelet.form.db20", "DAUBECHIES20");
    AddChoice("mode.wavelet.form.sb24", "SPLINE_BIORTHOGONAL_2_4");
    AddChoice("mode.wavelet.form.sb44", "SPLINE_BIORTHOGONAL_4_4");
    AddChoice("mode.wavelet.form.sym8", "SYMLET8");

    //Default value
    SetParameterString("mode", "wavelet");
    SetParameterString("mode.wavelet.form", "haar");

    AddParameter(ParameterType_Int,"mode.wavelet.nlevels","Number of decomposition levels");
    SetParameterDescription("mode.wavelet.nlevels","Number of decomposition levels");
    SetDefaultParameterInt("mode.wavelet.nlevels",2);
    SetMinimumParameterIntValue("mode.wavelet.nlevels",2);

    AddParameter(ParameterType_Choice,"dir", "Direction");

    AddChoice("dir.fwd", "Forward");
    AddChoice("dir.inv", "Inverse");

    AddParameter(ParameterType_OutputImage, "out", "Output Image");
    SetParameterDescription("out", "This parameter holds the output file name to"
      " which transformed image will be written. This has a slightly different"
      " behaviour depending on transform type. \n For Wavelet, output is a"
      " single band image for both forward and inverse transform. \n For FFT"
      " forward transform, output is two band image where first band represents"
      " real part and second band represents imaginary part of a complex image.");

    SetDocExampleParameterValue("in", "input.tif");
    SetDocExampleParameterValue("mode.wavelet.form", "haar");
    SetDocExampleParameterValue("out", "output_wavelet_haar.tif");
    }

  void DoUpdateParameters() ITK_OVERRIDE
    {

    }

  void DoExecute() ITK_OVERRIDE
    {
    int dir = GetParameterInt("dir");
    int mode = GetParameterInt("mode");

    if( dir != 0 && dir != 1)
      {
      itkExceptionMacro(<< "-dir is '"
         << dir << "'."
         << "It must be either 'fwd' or 'inv'");
      }

    if( mode != 0 && mode != 1)
      {
      itkExceptionMacro( << "mode is '"
         << mode << "'."
         << "It must must be either 'fft' or 'wavelet'");
      }

    if ( mode == 1)
      {
      int wavelet_type = GetParameterInt("mode.wavelet.form");
      unsigned int nlevels = GetParameterInt("mode.wavelet.nlevels");
      switch (wavelet_type)
        {
        case 0:
          {
          DoWaveletTransform<otb::Wavelet::HAAR>(dir, nlevels);
          break;
          }
        case 1:
          {
          DoWaveletTransform<otb::Wavelet::DB4>(dir, nlevels);
          break;
          }
        case 2:
          {
          DoWaveletTransform<otb::Wavelet::DB4>(dir, nlevels);
          break;
          }
        case 3:
          {
          DoWaveletTransform<otb::Wavelet::DB6>(dir, nlevels);
          break;
          }
        case 4:
          {
          DoWaveletTransform<otb::Wavelet::DB8>(dir, nlevels);
          break;
          }
        case 5:
          {
          DoWaveletTransform<otb::Wavelet::DB12>(dir, nlevels);
          break;
          }
        case 6:
          {
          DoWaveletTransform<otb::Wavelet::DB20>(dir, nlevels);
          break;
          }
        case 7:
          {
          DoWaveletTransform<otb::Wavelet::SPLINE_BIORTHOGONAL_2_4 >(dir, nlevels);
          break;
          }
        case 8:
          {
          DoWaveletTransform<otb::Wavelet::SPLINE_BIORTHOGONAL_4_4>(dir, nlevels);
          break;
          }
        case 9:
          {
          DoWaveletTransform<otb::Wavelet::SYMLET8>(dir, nlevels);
          break;
          }
        default:
          {
          itkExceptionMacro( << "Invalid wavelet type: '" <<  wavelet_type << "'");
          break;
          }
        }
      }
    else
      {
      // fft ttransform
      bool shift = IsParameterEnabled( "mode.fft.shift");
      typedef otb::Image< std::complex<OutputPixelType> >          ComplexOutputImageType;
      
      if (dir == 0 )
        {
        //forward fft
        typedef otb::Image<InputPixelType>          TInputImage;
        typedef TInputImage::Pointer TInputImagePointer;

        //get input paramter as otb::Image<InputPixelType>
        TInputImagePointer inImage = GetParameterImage<TInputImage>("in");

        //typedef itk::::ForwardFFTImageFilter over otbImage< InputPixelType >

        typedef itk::ForwardFFTImageFilter < TInputImage, ComplexOutputImageType > FFTFilter;
        FFTFilter::Pointer fwdFilter = FFTFilter::New();
        fwdFilter->SetInput( inImage );

	
        //typedef VectorImage for output of UnaryFunctorImageFilter
        typedef otb::VectorImage<OutputPixelType>          TOutputImage;

	typedef otb::ComplexToVectorImageCastFilter<
	  ComplexOutputImageType,
	  TOutputImage > ComplexToVectorImageCastFilter;
	ComplexToVectorImageCastFilter::Pointer unaryFunctorImageFilter = ComplexToVectorImageCastFilter::New();
	
        if( shift)
          {
          otbAppLogINFO( << "Applying Shift image filter" );
          typedef itk::FFTShiftImageFilter<
            typename FFTFilter::OutputImageType,
            typename FFTFilter::OutputImageType > FFTShiftFilterType;

          FFTShiftFilterType::Pointer
            fftShiftFilter = FFTShiftFilterType::New();

          fftShiftFilter->SetInput( fwdFilter->GetOutput() );
          fftShiftFilter->Update();
          unaryFunctorImageFilter->SetInput(fftShiftFilter->GetOutput() );
          }
        else
          {
          unaryFunctorImageFilter->SetInput(fwdFilter->GetOutput());
          }

        unaryFunctorImageFilter->Update();

        //set output image
        SetParameterOutputImage<TOutputImage>("out",
          unaryFunctorImageFilter->GetOutput());
        }
      else
        {
        //inverse fft
        typedef otb::VectorImage<InputPixelType>          TInputImage;
        typedef TInputImage::Pointer TInputImagePointer;

        TInputImagePointer inImage = GetParameterImage("in");

        // typedef TComplexImage for InverseFFTImageFilter input
        // This a image type of std::complex<InputPixelType>
        typedef otb::Image<
          std::complex<InputPixelType>, 2 > TComplexImage;
        //typedef TOutputImage for InverseFFTImageFilter output
        typedef otb::Image< OutputPixelType >  TOutputImage;
	
        // a unary functor to convert vectorimage to complex image
        typedef itk::UnaryFunctorImageFilter
          <TInputImage,
           TComplexImage,
           ToComplexPixel
           <TInputImage::PixelType,
            TComplexImage::PixelType> > UnaryFunctorImageFilter;

        UnaryFunctorImageFilter::Pointer
          unaryFunctorImageFilter = UnaryFunctorImageFilter::New();

        if( shift)
          {
          typedef itk::FFTShiftImageFilter<
            TInputImage,
            TInputImage > FFTShiftFilterType;

          FFTShiftFilterType::Pointer
            fftShiftFilter = FFTShiftFilterType::New();
          fftShiftFilter->SetInput( inImage );

          fftShiftFilter->Update();

          unaryFunctorImageFilter->SetInput(fftShiftFilter->GetOutput() );
          }
        else
          {
          unaryFunctorImageFilter->SetInput(inImage);
          }

        unaryFunctorImageFilter->Update();

        //typedef itk::::InverseFFTImageFilter over TComplexImage
        typedef itk::InverseFFTImageFilter
          < TComplexImage,
            TOutputImage > FFTFilter;
        FFTFilter::Pointer invFilter = FFTFilter::New();
        invFilter->SetInput( unaryFunctorImageFilter->GetOutput() );
        invFilter->Update();

        //set output image
        SetParameterOutputImage<TOutputImage>("out", invFilter->GetOutput());
        }
      }
    }

  template<otb::Wavelet::Wavelet TWaveletOperator>
  void DoWaveletTransform(const int dir,
                          const unsigned int nlevels,
                          const std::string inkey = "in",
                          const std::string outkey = "out")
    {
    typedef otb::Image< InputPixelType >  TInputImage;
    typedef otb::Image< OutputPixelType >  TOutputImage;
    typedef typename TInputImage::Pointer TInputImagePointer;

    TInputImagePointer inImage = GetParameterImage<TInputImage>(inkey);

    if( dir == 0)
      {
      typedef otb::WaveletImageFilter<
        TInputImage,
        TOutputImage,
        TWaveletOperator> TWaveletImageFilter;

      typedef typename
        TWaveletImageFilter::Pointer
        TWaveletImageFilterPointer;
      TWaveletImageFilterPointer waveletImageFilter =
        TWaveletImageFilter::New();

      waveletImageFilter->SetInput(inImage);
      waveletImageFilter->SetNumberOfDecompositions(nlevels);
      waveletImageFilter->Update();
      SetParameterOutputImage<TOutputImage>(outkey, waveletImageFilter->GetOutput() );
      }
    else
      {
      typedef otb::WaveletInverseImageFilter<
        TInputImage,
        TOutputImage,
        TWaveletOperator > TWaveletImageFilter;

      typedef typename
        TWaveletImageFilter::Pointer
        TWaveletImageFilterPointer;

      TWaveletImageFilterPointer waveletImageFilter =
        TWaveletImageFilter::New();

      waveletImageFilter->SetInput(inImage);
      waveletImageFilter->SetNumberOfDecompositions(nlevels);
      waveletImageFilter->Update();

      SetParameterOutputImage<TOutputImage>( outkey, waveletImageFilter->GetOutput() );
      }
    }
};

} // end of namespace Wrapper
} // end of namespace otb

OTB_APPLICATION_EXPORT(otb::Wrapper::DomainTransform)
