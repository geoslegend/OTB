/*=========================================================================

  Program:   Monteverdi2
  Language:  C++


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See Copyright.txt for details.

  Monteverdi2 is distributed under the CeCILL licence version 2. See
  Licence_CeCILL_V2-en.txt or
  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt for more details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __mvdImageModel_h
#define __mvdImageModel_h

//
// System includes (sorted by alphabetic order)

//
// Qt includes (sorted by alphabetic order)
#include <QObject>

//
// ITK includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)

//
// Monteverdi includes (sorted by alphabetic order)

//
// External classes pre-declaration.
namespace
{
}

namespace mvd
{
//
// Internal classes pre-declaration.

/** \class ImageModel
 *
 */
class ImageModel :
    public QObject
{
  Q_OBJECT;

//
// Public methods.
public:

  /** Constructor */
  ImageModel( QObject* parent =NULL );

  /** Destructor */
  virtual ~ImageModel();

//
// SIGNALS.
signals:

//
// Protected methods.
protected:

//
// Protected attributes.
protected:

//
// Private methods.
private:

//
// Private attributes.
private:

//
// SLOTS.
private slots:
};

} // end namespace 'mvd'

#endif // __mvdImageModel_h
