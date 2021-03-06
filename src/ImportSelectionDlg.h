// -*- C++ -*- generated by wxGlade 0.6.5 on Mon Jun 04 08:30:53 2012 from "C:\dpo\tools\scripts\cta\cta.wxg"

#include <wx/wx.h>
#include <wx/image.h>
#include "EnvVarVect.h"

#ifndef IMPORTSELECTIONDLG_H
#define IMPORTSELECTIONDLG_H

// begin wxGlade: ::dependencies
#include <wx/listctrl.h>
// end wxGlade

// begin wxGlade: ::extracode

// end wxGlade

class ImportSelectionDlg: public wxDialog {
public:
    // begin wxGlade: ImportSelectionDlg::ids
    // end wxGlade

    ImportSelectionDlg(EnvVarVect * user,
                        EnvVarVect * system,
                        wxWindow* parent,
                        int id,
                        const wxString& title,
                        const wxPoint& pos=wxDefaultPosition,
                        const wxSize& size=wxDefaultSize,
                        long style=wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER);

    bool read_file(const std::string &);

private:
    // begin wxGlade: ImportSelectionDlg::methods
    void set_properties();
    void do_layout();
    // end wxGlade

protected:
    // begin wxGlade: ImportSelectionDlg::attributes
    wxStaticBox* sizer_23_staticbox;
    wxStaticText* label_select_env_vars;
    wxStaticText* label_select_env_vars_note;
    wxStaticText* label_user_env_vars_backup;
    wxListCtrl* list_ctrl_env_vars_user_;
    wxStaticText* label_system_env_vars_backup;
    wxListCtrl* list_ctrl_env_vars_system_;
    wxButton* button_select_all;
    wxButton* button_unselect_all;
    wxButton* button_import_variables;
    // end wxGlade

    DECLARE_EVENT_TABLE()

public:
    virtual void on_select_all(wxCommandEvent &event); // wxGlade: <event_handler>
    virtual void on_unselect_all(wxCommandEvent &event); // wxGlade: <event_handler>
    virtual void on_button_import_variables(wxCommandEvent &event); // wxGlade: <event_handler>

protected:

    std::string backup_file_;
    EnvVarVect * env_vars_user_;
    EnvVarVect * env_vars_system_;

    void create_list_controls();
    void set_item_state_to_selected(wxListCtrl *, int flag);
    void import(wxListCtrl *, EnvVarVect *);

}; // wxGlade: end class




#endif // IMPORTSELECTIONDLG_H
