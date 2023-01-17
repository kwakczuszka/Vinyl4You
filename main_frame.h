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
    wxPanel* panel_menu;
    wxPanel* panel_rent;
    wxPanel* panel_my_rentals;
    wxPanel* panel_all_discs;
    wxPanel* panel_return_disc;
    
    void OnLogout(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);

    void OnRent(wxCommandEvent& event);
    void OnMyRentals(wxCommandEvent& event);
    void OnAllDiscs(wxCommandEvent& event);
    void OnReturnDisc(wxCommandEvent& event);

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



