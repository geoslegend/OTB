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
#ifndef otbSpot6ImageMetadataInterfaceFactory_h
#define otbSpot6ImageMetadataInterfaceFactory_h

#include "OTBMetadataExport.h"
#include "itkObjectFactoryBase.h"

namespace otb
{
/** \class Spot6ImageMetadataInterfaceFactory
 * \brief Creating an instance of a ImageMetadataInterface object using object factory.
 *
 * \ingroup OTBMetadata
 */
class OTBMetadata_EXPORT Spot6ImageMetadataInterfaceFactory : public itk::ObjectFactoryBase
{
public:
  /** Standard class typedefs. */
  typedef Spot6ImageMetadataInterfaceFactory Self;
  typedef itk::ObjectFactoryBase            Superclass;
  typedef itk::SmartPointer<Self>           Pointer;
  typedef itk::SmartPointer<const Self>     ConstPointer;

  /** Class methods used to interface with the registered factories. */
  const char* GetITKSourceVersion(void) const ITK_OVERRIDE;
  const char* GetDescription(void) const ITK_OVERRIDE;

  /** Method for class instantiation. */
  itkFactorylessNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(Spot6ImageMetadataInterfaceFactory, itk::ObjectFactoryBase);

  /** Register one factory of this type  */
  static void RegisterOneFactory(void)
  {
    Spot6ImageMetadataInterfaceFactory::Pointer Spot6IMIFactory = Spot6ImageMetadataInterfaceFactory::New();
    itk::ObjectFactoryBase::RegisterFactory(Spot6IMIFactory);
  }

protected:
  Spot6ImageMetadataInterfaceFactory();
  ~Spot6ImageMetadataInterfaceFactory() ITK_OVERRIDE;

private:
  Spot6ImageMetadataInterfaceFactory(const Self &); //purposely not implemented
  void operator =(const Self&); //purposely not implemented

};

} // end namespace otb

#endif
