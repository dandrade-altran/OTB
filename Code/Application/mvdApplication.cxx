/*=========================================================================

  Program:   Monteverdi2
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved .
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include "mvdApplication.h"

//
// System includes (sorted by alphabetic order)
#include <iostream>

//
// Qt includes (sorted by alphabetic order)
#include <QApplication>
#include <QDir>
#include <QLibraryInfo>
#include <QLocale>
#include <QMessageBox>
#include <QTextCodec>
#include <QTranslator>

//
// OTB includes (sorted by alphabetic order)

//
// Monteverdi includes (sorted by alphabetic order)
#include "ConfigureMonteverdi2.h"

//
// Class implementation.
namespace mvd
{
/*
  TRANSLATOR mvd::Application

  Necessary for lupdate to be aware of C++ namespaces.

  Context comment for translator.
*/

/*******************************************************************************/
Application
::Application( int& argc, char** argv ) :
  I18nApplication( argc, argv )
{
  InitializeCore();

  QObject::connect(
    this, SIGNAL( aboutToQuit() ),
    this, SLOT( OnAboutToQuit() )
    );
}

/*******************************************************************************/
Application
::~Application()
{
}

/*******************************************************************************/
void
Application
::InitializeCore()
{
  //
  // Setup application tags.
  QCoreApplication::setApplicationName(
    PROJECT_NAME
  );
  QCoreApplication::setApplicationVersion(
    Monteverdi2_VERSION_STRING
  );

  //
  // Setup organization tags.
  QCoreApplication::setOrganizationName(
    tr( "Centre National d'Etudes Spatiales (CNES)" )
  );
  QCoreApplication::setOrganizationDomain(
    "orfeo-toolbox.org"
  );
}

/*******************************************************************************/
/* SLOTS                                                                       */
/*******************************************************************************/
void
Application
::OnAboutToQuit()
{
  qDebug() << tr( ">DEBUG> mvd::Application::OnAboutToQuit()." );
}

} // end namespace 'mvd'
