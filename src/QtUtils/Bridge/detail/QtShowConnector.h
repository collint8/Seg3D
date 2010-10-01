/*
 For more information, please see: http://software.sci.utah.edu

 The MIT License

 Copyright (c) 2009 Scientific Computing and Imaging Institute,
 University of Utah.


 Permission is hereby granted, free of charge, to any person obtaining a
 copy of this software and associated documentation files (the "Software"),
 to deal in the Software without restriction, including without limitation
 the rights to use, copy, modify, merge, publish, distribute, sublicense,
 and/or sell copies of the Software, and to permit persons to whom the
 Software is furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included
 in all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 DEALINGS IN THE SOFTWARE.
 */

#ifndef QTUTILS_BRIDGE_DETAIL_QTSHOWCONNECTOR_H
#define QTUTILS_BRIDGE_DETAIL_QTSHOWCONNECTOR_H

#include <QObject>
#include <QPointer>

#include <Core/Utils/ConnectionHandler.h>
#include <Core/State/StateValue.h>

namespace QtUtils
{

class QtShowConnector : public QObject, protected Core::ConnectionHandler
{
  Q_OBJECT
public:
  QtShowConnector( QWidget* parent, Core::StateBoolHandle& state, bool opposite_logic );
  QtShowConnector( QWidget* parent, Core::StateBaseHandle state, 
    boost::function< bool () > condition );
  QtShowConnector( QWidget* parent, std::vector< Core::StateBaseHandle >& states,
    boost::function< bool () > condition );
  virtual ~QtShowConnector();

  // -- slot functions for boost signals --
private:
  static void ShowWidget( QPointer< QtShowConnector > qpointer, bool visible );
  static void ShowWidget( QPointer< QtShowConnector > qpointer );

  // -- internal variables --
private:
  QWidget* parent_;
  bool opposite_logic_;
  boost::function< bool () > condition_;
};

} // end namespace QtUtils

#endif