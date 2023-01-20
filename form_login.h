#include <wx/wxprec.h>

class FormLogin : public wxFrame
{
public:
    FormLogin();

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

class FormRegister : public wxFrame
{
public:
    FormRegister();

    virtual ~FormRegister();

private:
    wxStaticText* login_label;
    wxStaticText* password_label;
    wxStaticText* rpt_password_label;
    wxTextCtrl* login_input;
    wxTextCtrl* password_input;
    wxTextCtrl* rpt_password_input;
    wxButton* button_register;
    wxButton* button_ret_login;
    wxMessageDialog* dlg;

private:
    void OnQuit(wxCommandEvent& event);
    void OnRegister(wxCommandEvent& event);
    void OnRetLogin(wxCommandEvent& event);

private:
    DECLARE_EVENT_TABLE()

    enum {
        BUTTON_Register = wxID_HIGHEST + 3
    };
    enum {
        BUTTON_Ret_Login = wxID_HIGHEST + 4
    };


};