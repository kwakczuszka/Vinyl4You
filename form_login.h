#ifndef FORMLOGIN_H
#define FORMLOGIN_H
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include "main.h"

class FormLogin : public wxFrame
{
public:
    FormLogin(const wxString& title);

    virtual ~FormLogin();

private:
    wxStaticText* login_label;
    wxStaticText* password_label;
    wxTextCtrl* login_input;
    wxTextCtrl* password_input;
    wxButton* button_login;
    wxButton* button_register;
    wxMessageDialog* dlg;

private:
    void OnQuit(wxCommandEvent& event);
    void OnLogin(wxCommandEvent& event);
    void OnRegister(wxCommandEvent& event);

private:
    DECLARE_EVENT_TABLE()

    enum {
        BUTTON_Login = wxID_HIGHEST + 1
    };
    enum {
        BUTTON_Register = wxID_HIGHEST + 2
    };
};
#endif
