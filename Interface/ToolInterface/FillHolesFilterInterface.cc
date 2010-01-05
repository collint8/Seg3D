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



#include "FillHolesFilterInterface.h"
#include "ui_FillHolesFilterInterface.h"


namespace Seg3D {
  
  SCI_REGISTER_TOOLINTERFACE(FillHolesFilterInterface)
  
  class FillHolesFilterInterfacePrivate {
  public:
    Ui::FillHolesFilterInterface ui_;
  };
  
  FillHolesFilterInterface::FillHolesFilterInterface() :
  private_(new FillHolesFilterInterfacePrivate)
  {
    
  }
  
  FillHolesFilterInterface::~FillHolesFilterInterface()
  {
  }
  
  
  bool
  FillHolesFilterInterface::build_widget(QFrame* frame)
  {
    private_->ui_.setupUi(frame);
    
    SCI_LOG_DEBUG("Finished building a Fill Holes Filter Interface");
    return (true);
    
  }
  
  //  --- Function for making signal slots connections ---  //
  void 
  FillHolesFilterInterface::makeConnections()
  {
    connect(private_->ui_.activeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(senseActiveChanged(int)));
    
  }
  
  
  //  --- Private slots for custom signals ---  //
  void FillHolesFilterInterface::senseActiveChanged(int active)
  {
    Q_EMIT activeChanged(active);
  }
  
  
  //  --- Public slots for setting widget values ---  //
  void FillHolesFilterInterface::setActive(int active)
  {
    private_->ui_.activeComboBox->setCurrentIndex(active);
  }
  
  void FillHolesFilterInterface::addToActive(QStringList &items)
  {
    private_->ui_.activeComboBox->addItems(items);
  }
  
  
} // namespace Seg3D