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

// For the version numbers
#include "Seg3DConfiguration.h"

// Boost include
#include <boost/lexical_cast.hpp>

// Application includes
#include <Application/Application/Application.h>
#include <Application/Interface/Interface.h>

// Interface includes
#include <Interface/QtInterface/QtApplication.h>
#include <Interface/QtInterface/QtPointer.h>
#include <Interface/AppInterface/AppInterface.h>
#include <Interface/AppInterface/SplashStart.h>

namespace Seg3D
{

AppInterface::AppInterface()
{
	// Set the window information and set the version numbers
	setWindowTitle( QString( "Seg3D Version " ) + SEG3D_VERSION + QString( " " ) + SEG3D_BITS
	    + QString( " " ) + SEG3D_DEBUG_VERSION );
	setWindowIconText( QString( "Seg3D" ) );
	setDocumentMode( true );

	// Tell Qt what size to start up in
	resize( 1280, 720 );

	SplashStart* splash_screen_ = new SplashStart();
	splash_screen_->exec();

	// Enable fullscreen on startup via commandline argument
	std::string true_false_ = Seg3D::Application::Instance()->checkCommandLineParameter(
	    "fullscreen" );
	if ( boost::lexical_cast< bool >( true_false_ ) )
	{
		SCI_LOG_MESSAGE("Going full screen based on command line parameter");
		showFullScreen();
	}

	// Tell Qt where to doc the toolbars
	setCorner( Qt::TopLeftCorner, Qt::LeftDockWidgetArea );
	setCorner( Qt::TopRightCorner, Qt::RightDockWidgetArea );
	setCorner( Qt::BottomLeftCorner, Qt::LeftDockWidgetArea );
	setCorner( Qt::BottomRightCorner, Qt::RightDockWidgetArea );

	// Define the main window viewer canvas
	viewer_interface_ = new ViewerInterface( this );

	controller_interface_ = new AppController( this );
	controller_interface_->hide();

	// Setup the history dock widget
	add_windowids();

	show_window( "history" );
	show_window( "project" );
	show_window( "tools" );

	show_window( "measurement" );
	show_window( "layermanager" );

	setCentralWidget( viewer_interface_ );

	application_menu_ = new AppMenu( this );
	status_bar_ = new AppStatusBar( this );

	qpointer_type app_interface( this );

	InterfaceManager::Instance()->show_window_signal_.connect( boost::bind(
	    &AppInterface::HandleShowWindow, app_interface, _1 ) );

	InterfaceManager::Instance()->close_window_signal_.connect( boost::bind(
	    &AppInterface::HandleCloseWindow, app_interface, _1 ) );

	// Connect state and reflect the current state

	{
		// NOTE: State Engine is locked so the application thread cannot make
		// any changes to it
		StateEngine::lock_type lock( StateEngine::GetMutex() );

		// Connect and update full screen state
		set_full_screen( InterfaceManager::Instance()->full_screen_state_->get() );
		InterfaceManager::Instance()->full_screen_state_-> value_changed_signal_.connect(
		    boost::bind( &AppInterface::SetFullScreen, app_interface, _1, _2 ) );
	}
}

void AppInterface::set_full_screen( bool full_screen )
{
	if ( full_screen ) showFullScreen();
	else showNormal();
}

AppInterface::~AppInterface()
{
	//  viewer_interface_->writeSizeSettings();
}

ViewerInterface*
AppInterface::viewer_interface()
{
	return viewer_interface_.data();
}

HistoryDockWidget*
AppInterface::history_dock_widget()
{
	return history_dock_window_.data();
}

ProjectDockWidget*
AppInterface::project_dock_widget()
{
	return project_dock_window_.data();
}

ToolsDockWidget*
AppInterface::tools_dock_widget()
{
	return tools_dock_window_.data();
}

LayerManagerDockWidget*
AppInterface::layer_manager_dock_widget()
{
	return layer_manager_dock_window_.data();
}

MeasurementDockWidget*
AppInterface::measurement_dock_widget()
{
	return measurement_dock_window_.data();
}

void AppInterface::add_windowids()
{
	InterfaceManager::Instance()->add_windowid( "controller" );
	InterfaceManager::Instance()->add_windowid( "project" );
	InterfaceManager::Instance()->add_windowid( "history" );
	InterfaceManager::Instance()->add_windowid( "layermanager" );
	InterfaceManager::Instance()->add_windowid( "tools" );
	InterfaceManager::Instance()->add_windowid( "measurement" );
}

void AppInterface::show_window( const std::string& windowid )
{
	std::string lower_windowid = Utils::string_to_lower( windowid );
	if ( lower_windowid == "controller" )
	{
		if ( controller_interface_.isNull() )
		{
			controller_interface_ = new AppController( this );
			controller_interface_->show();
		}
		else
		{
			controller_interface_->show();
			controller_interface_->raise();
		}
	}
	else if ( lower_windowid == "project" )
	{
		if ( project_dock_window_.isNull() )
		{
			project_dock_window_ = new ProjectDockWidget( this );
			addDockWidget( Qt::LeftDockWidgetArea, project_dock_window_ );
			project_dock_window_->show();
		}
		else
		{
			project_dock_window_->show();
			project_dock_window_->raise();
		}
	}
	else if ( lower_windowid == "layermanager" )
	{
		if ( layer_manager_dock_window_.isNull() )
		{
			layer_manager_dock_window_ = new LayerManagerDockWidget( this );
			addDockWidget( Qt::RightDockWidgetArea, layer_manager_dock_window_ );
			layer_manager_dock_window_->show();
		}
		else
		{
			layer_manager_dock_window_->show();
			layer_manager_dock_window_->raise();
		}
	}
	else if ( lower_windowid == "history" )
	{
		if ( history_dock_window_.isNull() )
		{
			history_dock_window_ = new HistoryDockWidget( this );
			addDockWidget( Qt::LeftDockWidgetArea, history_dock_window_ );
			history_dock_window_->show();
		}
		else
		{
			history_dock_window_->show();
			history_dock_window_->raise();
		}
	}
	else if ( lower_windowid == "tools" )
	{
		if ( tools_dock_window_.isNull() )
		{
			tools_dock_window_ = new ToolsDockWidget( this );
			addDockWidget( Qt::LeftDockWidgetArea, tools_dock_window_ );
			tools_dock_window_->show();
		}
		else
		{
			tools_dock_window_->show();
			tools_dock_window_->raise();
		}
	}
	else if ( lower_windowid == "measurement" )
	{
		if ( measurement_dock_window_.isNull() )
		{
			measurement_dock_window_ = new MeasurementDockWidget( this );
			addDockWidget( Qt::RightDockWidgetArea, measurement_dock_window_ );
			measurement_dock_window_->show();
		}
		else
		{
			measurement_dock_window_->show();
			measurement_dock_window_->raise();
		}
	}
}

void AppInterface::close_window( const std::string& windowid )
{
	std::string lower_windowid = Utils::string_to_lower( windowid );
	if ( lower_windowid == "controller" )
	{
		if ( !( controller_interface_.isNull() ) )
		{
			controller_interface_->close();
		}
	}
	else if ( lower_windowid == "project" )
	{
		if ( !( project_dock_window_.isNull() ) ) project_dock_window_->close();
	}
	else if ( lower_windowid == "layermanager" )
	{
		if ( !( layer_manager_dock_window_.isNull() ) ) layer_manager_dock_window_->close();
	}
	else if ( lower_windowid == "history" )
	{
		if ( !( history_dock_window_.isNull() ) ) history_dock_window_->close();
	}
	else if ( lower_windowid == "tools" )
	{
		if ( !( tools_dock_window_.isNull() ) ) tools_dock_window_->close();
	}
	else if ( lower_windowid == "measurement" )
	{
		if ( !( measurement_dock_window_.isNull() ) ) measurement_dock_window_->close();
	}
}

void AppInterface::addDockWidget( Qt::DockWidgetArea area, QDockWidget* dock_widget )
{
	QMainWindow::addDockWidget( area, dock_widget );

	QList< QDockWidget* > object_list = findChildren< QDockWidget* > ();
	QList< QDockWidget* >::iterator it = object_list.begin();
	QList< QDockWidget* >::iterator it_end = object_list.end();
	while ( it != it_end )
	{
		if ( ( dock_widget != *it ) && ( dockWidgetArea( *it ) == area ) )
		{
			tabifyDockWidget( *it, dock_widget );
			break;
		}
		++it;
	}
}

void AppInterface::HandleShowWindow( qpointer_type app_interface, std::string windowid )
{
	Interface::PostEvent( CheckQtPointer( app_interface, boost::bind( &AppInterface::show_window,
	    app_interface.data(), windowid ) ) );
}

void AppInterface::HandleCloseWindow( qpointer_type app_interface, std::string windowid )
{
	Interface::PostEvent( CheckQtPointer( app_interface, boost::bind( &AppInterface::close_window,
	    app_interface.data(), windowid ) ) );
}

void AppInterface::SetFullScreen( qpointer_type app_interface, bool full_screen,
    ActionSource action_source )
{
	Interface::PostEvent( CheckQtPointer( app_interface, boost::bind(
	    &AppInterface::set_full_screen, app_interface.data(), full_screen ) ) );
}

} // end namespace Seg3D
