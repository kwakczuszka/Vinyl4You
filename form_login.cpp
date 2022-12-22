#include "form_login.h"
#include <wx/wx.h>
#include "main.h"
#include "md5.h"
#include <fstream>
#include "my_frame.h"

using namespace std; 

FormLogin::FormLogin(const wxString& title) : wxFrame((wxFrame*)NULL, wxID_ANY, title, wxPoint(wxID_ANY, wxID_ANY), wxSize(340, 200),
        wxSYSTEM_MENU | wxMINIMIZE_BOX | wxCLOSE_BOX | wxCAPTION | wxCLIP_CHILDREN) {
    wxPanel* panel = new wxPanel(this, wxID_ANY);

    wxBoxSizer* vbox = new wxBoxSizer(wxVERTICAL);

    wxBoxSizer* hbox1 = new wxBoxSizer(wxHORIZONTAL);
    login_label = new wxStaticText(panel, wxID_ANY, wxT("Login: "), wxDefaultPosition, wxSize(70, -1));
    hbox1->Add(login_label, 0);

    login_input = new wxTextCtrl(panel, wxID_ANY);
    hbox1->Add(login_input, 1);
    vbox->Add(hbox1, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, 10);

    wxBoxSizer* hbox2 = new wxBoxSizer(wxHORIZONTAL);
    password_label = new wxStaticText(panel, wxID_ANY, wxT("Hasło: "), wxDefaultPosition, wxSize(70, -1));
    hbox2->Add(password_label, 0);

    password_input = new wxTextCtrl(panel, wxID_ANY, wxString(""),
        wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD);
    hbox2->Add(password_input, 1);
    vbox->Add(hbox2, 0, wxEXPAND | wxLEFT | wxTOP | wxRIGHT, 10);

    wxBoxSizer* hbox3 = new wxBoxSizer(wxHORIZONTAL);

    button_login = new wxButton(panel, BUTTON_Login, wxT("Zaloguj się"));
    hbox3->Add(button_login);
    vbox->Add(hbox3, 0, wxALIGN_RIGHT | wxTOP | wxRIGHT | wxBOTTOM, 10);


    button_register = new wxButton(panel, BUTTON_Register, wxT("Zarejestruj się"));
    hbox3->Add(button_register);
    vbox->Add(hbox3, 0, wxALIGN_RIGHT | wxTOP | wxRIGHT | wxBOTTOM, 10);

    panel->SetSizer(vbox);
    Centre();
}

void FormLogin::OnQuit(wxCommandEvent& event) {
    Close(true);
}

void FormLogin::OnLogin(wxCommandEvent& event) {
    fstream user_pass;
    fstream user_log;
    user_pass.open("pass_data.txt", ios::in);
    user_log.open("log_data.txt", ios::in);
    if (user_pass.good() && user_log.good()) {
        wxString username = login_input->GetValue();
        wxString password = password_input->GetValue();

        if (username.size() < 3 || password.size() < 3) {
            wxMessageBox(wxT("Login i/lub hasło muszą składać się z co najmniej 3 znaków!"), wxT("Ostrzeżenie!"), wxICON_WARNING);
        } 
        else {
            string a;
            string b;
            bool x = true;
            while (getline(user_log, a) && getline(user_pass, b)) {
                if (a == string(username) && md5(string(password)) == b) {
                    x = false;
                    Close(true);
                    MyFrame* frame = new MyFrame();
                    frame->Show(true);
                    wxMessageBox(wxT("Witaj w Vinyl4You!"), wxT("Logowanie pomyślne!"));
                }
            }
            if (x) {
                wxMessageBox(wxT("Wprowadzony login i/lub hasło są niepoprawne!"), wxT("Błąd logowania!"), wxICON_ERROR);
            }
        }
    }
    user_pass.close();
    user_log.close();
}

void FormLogin::OnRegister(wxCommandEvent& event) {
    wxString username = login_input->GetValue();
    wxString password = password_input->GetValue();

    if (username.size() < 3 || password.size() < 3) {
        wxMessageBox(wxT("Login i/lub hasło muszą składać się z co najmniej 3 znaków!"), wxT("Ostrzeżenie!"), wxICON_WARNING);
    }
    else {
        fstream user_pass;
        fstream user_log;
        user_pass.open("pass_data.txt", ios::in | ios::app);
        user_log.open("log_data.txt", ios::in | ios::app);
        bool success = false;

        if (user_pass.good()&&user_log.good()) {
            wxString username = login_input->GetValue();
            wxString password = password_input->GetValue();
            string a;
            bool x = true;
            while (getline(user_log, a)) {
                string j = string(username);
                if (j == a) {
                    wxMessageBox(wxT("Login zajęty!"), wxT("Ostrzeżenie!"), wxICON_WARNING);
                    x = false;
               }
            }
            if (x) {
                user_log.close();
                user_log.open("log_data.txt", ios::in | ios::app);
                string h = md5(string(password.mb_str()));
                string g = string(username);
                h += "\n";
                g += "\n";
                user_pass<<h;
                user_log<<g;
                success = true;
            }  
        }
        if (success) {
            Close(true);
            FormLogin* okno = new FormLogin("Vinyl4You - Logowanie");
            okno->Show(true);
            wxMessageBox(wxT("Teraz możesz sie zalogować!"), wxT("Rejestracja pomyślna!"));
        }
    }
}

FormLogin::~FormLogin() {}

BEGIN_EVENT_TABLE(FormLogin, wxFrame)
EVT_BUTTON(BUTTON_Login, FormLogin::OnLogin)
EVT_BUTTON(BUTTON_Register, FormLogin::OnRegister)
END_EVENT_TABLE()