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

#ifndef APPLICATION_INTERFACEMANAGER_ACTIONS_ACTIONSHOWTOOL_H
#define APPLICATION_INTERFACEMANAGER_ACTIONS_ACTIONSHOWTOOL_H

#include <Core/Action/Actions.h>

namespace Seg3D
{

// TODO:
// This action should be merged into a state variable

class ActionShowWindow : public Core::Action
{

CORE_ACTION( 
  CORE_ACTION_TYPE( "ShowWindow", "Show a window in the interface." )
  CORE_ACTION_ARGUMENT( "windowid", "Name of the window to show." )
)
  // -- Constructor/Destructor --
public:
  ActionShowWindow()
  {
    add_argument( windowid_ );
  }

  virtual ~ActionShowWindow()
  {
  }

  // -- Functions that describe action --
  virtual bool validate( Core::ActionContextHandle& context );
  virtual bool run( Core::ActionContextHandle& context, Core::ActionResultHandle& result );

  // -- Action parameters --
private:
  Core::ActionParameter< std::string > windowid_;

  // -- Dispatcher for the GUI --
public:

  // CREATE:
  // Create the action
  static Core::ActionHandle Create( const std::string& windowid );

  // DISPATCH:
  // Create the action and dispatch it
  static void Dispatch( Core::ActionContextHandle context, const std::string& windowid );

};

} // end namespace Seg3D

#endif