#pragma once

class MainFrame : public wxFrame {
public:
    MainFrame(std::string logged_user);
    ~MainFrame();

private:
    std::string logged_user;
    wxButton* button_rent;
    wxButton* button_my_rentals;
    wxButton* button_all_discs;
    wxButton* button_return_disc;
    
    void OnLogout(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);

private:
    DECLARE_EVENT_TABLE()
    enum {
        BUTTON_rent = 2
    };
    enum {
        BUTTON_my_rentals = 3
    };
    enum {
        BUTTON_all_discs = 4
    };
    enum {
        BUTTON_return_disc = 5
    };
    enum {
        ID_Logout = 1
    };
};



