#include "form_login.h"
#include <wx/wx.h>
#include "main.h"
#include "md5.h"
#include <fstream>
#include "main_frame.h"

using namespace std; 

FormRegister::FormRegister() : wxFrame(nullptr, wxID_ANY, "Vinyl4You - Rejestracja", wxPoint(wxID_ANY, wxID_ANY), wxSize(340, 180),
    wxSYSTEM_MENU | wxMINIMIZE_BOX | wxCLOSE_BOX | wxCAPTION | wxCLIP_CHILDREN) {

    SetIcon(wxICON(vinyl_ico));

    wxPanel* panel = new wxPanel(this, wxID_ANY);
    wxBoxSizer* vert_box = new wxBoxSizer(wxVERTICAL);

    wxBoxSizer* horiz_box1 = new wxBoxSizer(wxHORIZONTAL);
    login_label = new wxStaticText(panel, wxID_ANY, wxT("Login: "), wxDefaultPosition, wxSize(100, -1));
    horiz_box1->Add(login_label, 0);

    login_input = new wxTextCtrl(panel, wxID_ANY, wxString(""), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    horiz_box1->Add(login_input, 1);

    wxBoxSizer* horiz_box2 = new wxBoxSizer(wxHORIZONTAL);
    password_label = new wxStaticText(panel, wxID_ANY, wxT("Hasło: "), wxDefaultPosition, wxSize(100, -1));
    horiz_box2->Add(password_label, 0);

    password_input = new wxTextCtrl(panel, wxID_ANY, wxString(""), wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD | wxTE_PROCESS_ENTER);
    horiz_box2->Add(password_input, 1);

    wxBoxSizer* horiz_box3 = new wxBoxSizer(wxHORIZONTAL);
    rpt_password_label = new wxStaticText(panel, wxID_ANY, wxT("Powtórz hasło: "), wxDefaultPosition, wxSize(100, -1));
    horiz_box3->Add(rpt_password_label, 0);

    rpt_password_input = new wxTextCtrl(panel, wxID_ANY, wxString(""), wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD | wxTE_PROCESS_ENTER);
    horiz_box3->Add(rpt_password_input, 1);

    wxBoxSizer* horiz_box4 = new wxBoxSizer(wxHORIZONTAL);

    button_ret_login = new wxButton(panel, BUTTON_Ret_Login, wxT("Powrót do logowania"));
    horiz_box4->Add(button_ret_login);

    button_register = new wxButton(panel, BUTTON_Register, wxT("Zarejestruj się"));
    horiz_box4->Add(button_register);


    vert_box->Add(horiz_box1, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, 10);
    vert_box->Add(horiz_box2, 0, wxEXPAND | wxLEFT | wxTOP | wxRIGHT, 10);
    vert_box->Add(horiz_box3, 0, wxEXPAND | wxLEFT | wxTOP | wxRIGHT, 10);
    vert_box->Add(horiz_box4, 0, wxALIGN_RIGHT | wxTOP | wxRIGHT | wxBOTTOM, 10);

    panel->SetSizer(vert_box);
    
    login_input->Bind(wxEVT_TEXT_ENTER, &FormRegister::OnRegister, this);
    password_input->Bind(wxEVT_TEXT_ENTER, &FormRegister::OnRegister, this);
    rpt_password_input->Bind(wxEVT_TEXT_ENTER, &FormRegister::OnRegister, this);

    Centre();
}

void FormRegister::OnRegister(wxCommandEvent &event) {
    wxString username = login_input->GetValue();
    wxString password = password_input->GetValue();
    wxString rpt_password = rpt_password_input->GetValue();

    if (username.size() < 3 || password.size() < 3 || rpt_password.size() < 3) {
        if (username.size() > 13) {
            wxMessageBox(wxT("Login może składać się z maksymalnie 13 znaków!"), wxT("Ostrzeżenie!"), wxICON_WARNING);
        }
        wxMessageBox(wxT("Login i/lub hasło muszą składać się z co najmniej 3 znaków!"), wxT("Ostrzeżenie!"), wxICON_WARNING);
    }
    

    else if(password != rpt_password)
        wxMessageBox(wxT("Hasła muszą być identyczne!"), wxT("Ostrzeżenie!"), wxICON_WARNING);

    else {
        fstream user_pass;
        fstream user_log;
        user_pass.open("pass_data.txt", ios::in | ios::app);
        user_log.open("log_data.txt", ios::in | ios::app);

        bool success = false;

        if (user_pass.good() && user_log.good()) {
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
                user_pass << h;
                user_log << g;
                success = true;
            }
        }
        if (success) {
            Close(true);
            FormLogin* log_ = new FormLogin();
            log_->Show(true);
            this->~FormRegister();
            wxMessageBox(wxT("Teraz możesz sie zalogować!"), wxT("Rejestracja pomyślna!"));
        }
    }
}

void FormRegister::OnRetLogin(wxCommandEvent& event) {
    Close(true);
    FormLogin* log_ = new FormLogin();
    log_->Show(true);
    this->~FormRegister();
}

FormRegister::~FormRegister() {
};

BEGIN_EVENT_TABLE(FormRegister, wxFrame)
EVT_BUTTON(BUTTON_Register, FormRegister::OnRegister)
EVT_BUTTON(BUTTON_Ret_Login, FormRegister::OnRetLogin)
END_EVENT_TABLE()

FormLogin::FormLogin() : wxFrame(nullptr, wxID_ANY, "Vinyl4You - Logowanie", wxPoint(wxID_ANY, wxID_ANY), wxSize(340, 150),
        wxSYSTEM_MENU | wxMINIMIZE_BOX | wxCLOSE_BOX | wxCAPTION | wxCLIP_CHILDREN) {

    SetIcon(wxICON(vinyl_ico));
    wxPanel* panel = new wxPanel(this, wxID_ANY);
    wxBoxSizer* vert_box = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* horiz_box1 = new wxBoxSizer(wxHORIZONTAL);
    
    login_label = new wxStaticText(panel, wxID_ANY, wxT("Login: "), wxDefaultPosition, wxSize(70, -1));
    horiz_box1->Add(login_label, 0);

    login_input = new wxTextCtrl(panel, wxID_ANY, wxString(""), wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    horiz_box1->Add(login_input, 1);

    vert_box->Add(horiz_box1, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, 10);

    wxBoxSizer* horiz_box2 = new wxBoxSizer(wxHORIZONTAL);
    password_label = new wxStaticText(panel, wxID_ANY, wxT("Hasło: "), wxDefaultPosition, wxSize(70, -1));
    horiz_box2->Add(password_label, 0);

    password_input = new wxTextCtrl(panel, wxID_ANY, wxString(""), wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD | wxTE_PROCESS_ENTER);
    horiz_box2->Add(password_input, 1);

    vert_box->Add(horiz_box2, 0, wxEXPAND | wxLEFT | wxTOP | wxRIGHT, 10);

    wxBoxSizer* horiz_box3 = new wxBoxSizer(wxHORIZONTAL);

    button_login = new wxButton(panel, BUTTON_Login, wxT("Zaloguj się"));
    horiz_box3->Add(button_login);

    button_register = new wxButton(panel, BUTTON_Register, wxT("Zarejestruj się"));
    horiz_box3->Add(button_register);

    vert_box->Add(horiz_box3, 0, wxALIGN_RIGHT | wxTOP | wxRIGHT | wxBOTTOM, 10);

    panel->SetSizer(vert_box);

    login_input->Bind(wxEVT_TEXT_ENTER, &FormLogin::OnLogin, this);
    password_input->Bind(wxEVT_TEXT_ENTER, &FormLogin::OnLogin, this);


    Centre();
}

void FormLogin::OnQuit(wxCommandEvent& event) {
    Close(true);
    this->~FormLogin();
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
            if (username.size() > 13) {
                wxMessageBox(wxT("Login może składać się z maksymalnie 13 znaków!"), wxT("Ostrzeżenie!"), wxICON_WARNING);

            }
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

                    wxMessageBox(wxT("Witaj w Vinyl4You!"), wxT("Logowanie pomyślne!"));
                    MainFrame* frame = new MainFrame(a);
                    frame->Show(true);
                    this->~FormLogin();
                    user_pass.close();
                    user_log.close();

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
    FormRegister* regist_ = new FormRegister;
    regist_->Show(true);
    this->~FormLogin();
}

FormLogin::~FormLogin() {
};

BEGIN_EVENT_TABLE(FormLogin, wxFrame)
EVT_BUTTON(BUTTON_Login, FormLogin::OnLogin)
EVT_BUTTON(BUTTON_Register, FormLogin::OnRegister)
END_EVENT_TABLE()