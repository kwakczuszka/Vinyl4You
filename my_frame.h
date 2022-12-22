#pragma once

class MyFrame : public wxFrame
{
public:
    MyFrame();

private:
    
    void OnLogout(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
};

enum
{
    ID_Logout = 1
};

