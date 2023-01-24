#include <wx/wxprec.h>
#include "main.h"
#include "form_login.h"
#include <time.h>
#include <fstream>
#include "main_frame.h"

using std::to_string;
using std::string;

std::fstream MyApp::log_; 
wxDateTime MyApp::dt;

bool MyApp::OnInit() {
    log_.open("log.txt", std::ios::app);
    FormLogin* logowanko = new FormLogin;
    logowanko -> SetIcon(wxICON(vinyl_ico));
    logowanko->Show(true);
    
    MyApp::dt.SetToCurrent();
    
    log_ << "<" << MyApp::dt.FormatISOCombined(' ') << "> App started\n";
    dt.~wxDateTime();
    return true;
}

MyApp::~MyApp() {
    MyApp::dt.SetToCurrent();
    MyApp::log_ << "<" << MyApp::dt.FormatISOCombined(' ') << "> App terminated\n";
    MyApp::log_.close();
    string new_name_str;
    const char* new_name;
    if (MainFrame::logged_user == "") {
        new_name_str = "log/log_unlogged_" + to_string(MyApp::dt.GetYear()) + '_' + to_string(MyApp::dt.GetMonth() + 1) + '_' + to_string(MyApp::dt.GetDay()) + '-' +
            to_string(MyApp::dt.GetHour()) + '_' + to_string(MyApp::dt.GetMinute()) + '_' + to_string(MyApp::dt.GetSecond()) + ".txt";
        new_name = new_name_str.c_str();
    }
    else {
        new_name_str = "log/log_" + MainFrame::logged_user + '_' + to_string(MyApp::dt.GetYear()) + '_' + to_string(MyApp::dt.GetMonth() + 1) + '_' + to_string(MyApp::dt.GetDay()) + '-' +
            to_string(MyApp::dt.GetHour()) + '_' + to_string(MyApp::dt.GetMinute()) + '_' + to_string(MyApp::dt.GetSecond()) + ".txt";
        new_name = new_name_str.c_str();
    }
    rename("log.txt", new_name);
}

wxIMPLEMENT_APP(MyApp);

