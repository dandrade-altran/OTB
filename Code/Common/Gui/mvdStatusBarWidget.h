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
#ifndef __mvdStatusBarWidget_h
#define __mvdStatusBarWidget_h

//
// Configuration include.
//// Included at first position before any other ones.
#include "ConfigureMonteverdi2.h"


/*****************************************************************************/
/* INCLUDE SECTION                                                           */

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.
#include <QtGui>

//
// System includes (sorted by alphabetic order)

//
// ITK includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)

//
// Monteverdi includes (sorted by alphabetic order)
#include "Core/mvdTypes.h"


/*****************************************************************************/
/* PRE-DECLARATION SECTION                                                   */

//
// External classes pre-declaration.
namespace
{
}

namespace mvd
{
//
// Internal classes pre-declaration.
namespace Ui
{
class StatusBarWidget;
};


/*****************************************************************************/
/* CLASS DEFINITION SECTION                                                  */

/**
 * \class StatusBarWidget
 *
 * \brief Status Bar widget
 * widget class.
 */
class Monteverdi2_EXPORT StatusBarWidget :
    public QWidget
{

  /*-[ QOBJECT SECTION ]-----------------------------------------------------*/

  Q_OBJECT;

  /*-[ PUBLIC SECTION ]------------------------------------------------------*/

//
// Public methods.
public:

  /** \brief Constructor. */
  StatusBarWidget( QWidget* parent =NULL, Qt::WindowFlags flags =0 );

  /** \brief Destructor. */
  virtual ~StatusBarWidget();

  /** 
    * accessors to statusBar widgets 
    */
#if USE_OLD_IMAGE_VIEW
  QLineEdit * GetCurrentPixelIndexWidget();
  QLabel *    GetCurrentPixelRadioWidget();
  QLineEdit * GetCurrentScaleWidget();
#endif

  /*-[ PUBLIC SLOTS SECTION ]------------------------------------------------*/

//
// Public SLOTS.
public slots:

  /**
   */
  void SetPixelIndex( const IndexType&, bool isInside );

  /**
   */
  void SetScale( double sx, double sy );

  /*-[ SIGNALS SECTION ]-----------------------------------------------------*/

//
// Signals.
signals:
  /**
   */
  void ScaleChanged( double );
  /**
   */
  void PixelIndexChanged( const IndexType& );

  /*-[ PROTECTED SECTION ]---------------------------------------------------*/

//
// Protected methods.
protected:

  /*-[ PRIVATE SECTION ]-----------------------------------------------------*/

//
// Protected attributes.
protected:

//
// Private methods.
private:
  /**
   */
  inline static QString ZoomLevel( double scale );

//
// Private attributes.
private:

  /**
   * \brief uic generated.
   */
  Ui::StatusBarWidget* m_UI;

  /*-[ PRIVATE SLOTS SECTION ]-----------------------------------------------*/

//
// Slots.
private slots:
  /**
   */
  // void SetPixelIndexText( const QString& );
  /**
   */
  void SetPixelRadiometryText( const QString& );
  /**
   */
  void on_scaleLineEdit_editingFinished();
  /**
   */
  void on_pixelIndexLineEdit_returnPressed();
};

} // end namespace 'mvd'

/*****************************************************************************/
/* INLINE SECTION                                                            */

//
// Qt includes (sorted by alphabetic order)
//// Must be included before system/custom includes.

//
// System includes (sorted by alphabetic order)

//
// ITK includes (sorted by alphabetic order)

//
// OTB includes (sorted by alphabetic order)

//
// Monteverdi includes (sorted by alphabetic order)
#include "Core/mvdAlgorithm.h"

namespace mvd
{

/*****************************************************************************/
inline
QString
StatusBarWidget
::ZoomLevel( double scale )
{
  if( scale>1.0 )
    {
    // return ToQString( scale ) + ":1";
    return QString( "%1:1" ).arg( scale );
    }
  else if( scale<1.0 )
    {
    // return "1:" + ToQString( 1.0 / scale );
    return QString( "1:%1" ).arg( 1.0 / scale );
    }
  else
    {
    return "1:1";
    }

  return QString( ":" );
}

} // end namespace 'mvd'

#endif // __mvdStatusBarWidget_h