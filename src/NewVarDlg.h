// -*- C++ -*- generated by wxGlade 0.6.5 on Mon Jun 04 08:30:53 2012 from "C:\dpo\tools\scripts\cta\cta.wxg"

#include "wx_ids.h"

#include <wx/wx.h>
#include <wx/image.h>

#ifndef NEWVARDLG_H
#define NEWVARDLG_H

// begin wxGlade: ::dependencies
// end wxGlade

// begin wxGlade: ::extracode

// end wxGlade


class NewVarDlg: public wxDialog {
public:
    // begin wxGlade: NewVarDlg::ids
    // end wxGlade

    NewVarDlg(wxWindow* parent,
                int id,
                const wxString& title,
                const wxPoint& pos=wxDefaultPosition,
                const wxSize& size=wxDefaultSize,
                long style=wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER);

private:
    // begin wxGlade: NewVarDlg::methods
    void set_properties();
    void do_layout();
    // end wxGlade

protected:
    // begin wxGlade: NewVarDlg::attributes
    wxStaticText* label_new_env_var_;
    wxTextCtrl* text_ctrl_new_env_var_;
    wxStaticText* label_value_;
    wxTextCtrl* text_ctrl_value_;
    wxButton* button_ok_;
    wxButton* button_cancel_;
    // end wxGlade

    DECLARE_EVENT_TABLE();

public:
    virtual void on_ok(wxCommandEvent &event); // wxGlade: <event_handler>
    virtual void on_cancel(wxCommandEvent &event); // wxGlade: <event_handler>

    wxString get_new_env_var_id() { return text_ctrl_new_env_var_->GetValue(); }
    void set_new_env_var_id(const wxString & id) { text_ctrl_new_env_var_->SetValue(id.c_str()); }
    wxString get_new_value() { return text_ctrl_value_->GetValue(); }
    void disable_new_env_var_id() { text_ctrl_new_env_var_->Disable(); }
    void set_default_button_ok() { button_ok_->SetDefault(); }
}; // wxGlade: end class




#endif // NEWVARDLG_H