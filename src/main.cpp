/**
 * @file main.cpp
 * @brief Main source code file
 */

/*
 * This program source code file is part of EmDebug, a free front-end GUI for GDB + OpenOCD.
 *
 * Copyright (C) 2014 Augusto Fraga Giachero
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License V2
 * as published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, you may find one here:
 * http://www.gnu.org/licenses/old-licenses/gpl-2.0.html
 * or you may search the http://www.gnu.org website for the version 2 license,
 * or you may write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 */

#include <iostream>

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif

#include "gdbmi.h"

class EmDebug: public wxApp
{
	public:
		virtual bool OnInit();
};

class MainFrame: public wxFrame
{
	public:
		MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
	private:
		void OnHello(wxCommandEvent& event);
		void OnExit(wxCommandEvent& event);
		void OnAbout(wxCommandEvent& event);
		wxDECLARE_EVENT_TABLE();
};

enum
{
	ID_Hello = 1
};

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
	EVT_MENU(ID_Hello,   MainFrame::OnHello)
	EVT_MENU(wxID_EXIT,  MainFrame::OnExit)
	EVT_MENU(wxID_ABOUT, MainFrame::OnAbout)
wxEND_EVENT_TABLE()
wxIMPLEMENT_APP(EmDebug);

bool EmDebug::OnInit()
{
	MainFrame *frame = new MainFrame( "EmDebug", wxPoint(50, 50), wxSize(450, 340) );
	frame->Show( true );
	return true;
}

MainFrame::MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
	: wxFrame(NULL, wxID_ANY, title, pos, size)
{
	wxMenu *menuFile = new wxMenu;
	menuFile->Append(ID_Hello, "&Hello...\tCtrl-H",
                     "Help string shown in status bar for this menu item");
	menuFile->AppendSeparator();
	menuFile->Append(wxID_EXIT);
	wxMenu *menuHelp = new wxMenu;
	menuHelp->Append(wxID_ABOUT);
	wxMenuBar *menuBar = new wxMenuBar;
	menuBar->Append( menuFile, "&File" );
	menuBar->Append( menuHelp, "&Help" );
	SetMenuBar( menuBar );
	CreateStatusBar();
	//SetStatusText( "Welcome to wxWidgets!" );
}

void MainFrame::OnExit(wxCommandEvent& event)
{
	Close( true );
}

void MainFrame::OnAbout(wxCommandEvent& event)
{
	wxMessageBox( "This is a wxWidgets' Hello world sample",
                  "About Hello World", wxOK | wxICON_INFORMATION );
}

void MainFrame::OnHello(wxCommandEvent& event)
{
	wxLogMessage("Hello world from wxWidgets!");
}
