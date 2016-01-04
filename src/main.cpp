/*
 * ====================================================================
 * Copyright (c) 2012 Hubert Talbot.  All rights reserved.
 *
 * CTA is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * CTA is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with CTA.  If not, see <http://www.gnu.org/licenses/>.
 *
 * ====================================================================
 */

#include "CTAFrame.h"


class MyApp: public wxApp
{
public:
    bool OnInit();
    int FilterEvent(wxEvent& event);

protected:
    CTAFrame * frame_;
};


bool MyApp::OnInit()
{
    // use standard command line handling:
    if ( !wxApp::OnInit() )
        return false;

    // parse the cmd line
    int x = 50,
        y = 50;
    if ( argc == 3 )
    {

        wxSscanf(wxString(argv[1]), wxT("%d"), &x);
        wxSscanf(wxString(argv[2]), wxT("%d"), &y);
    }

    // Create the main frame window
    frame_ = new CTAFrame(0, -1, wxT("CTA"));
    frame_->Show(true);

    return true;
}

int MyApp::FilterEvent(wxEvent& event)
{
    if ((event.GetEventType() == wxEVT_KEY_DOWN) &&
        (((wxKeyEvent&)event).GetKeyCode() == WXK_F12) &&
        ((wxKeyEvent&)event).ShiftDown() &&
        ((wxKeyEvent&)event).ControlDown())
    {
        frame_->manual_mode();
    }

    return -1;
}


IMPLEMENT_APP(MyApp)
