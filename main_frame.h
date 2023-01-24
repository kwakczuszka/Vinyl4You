#include "wx/wxprec.h"
#include "wx/listctrl.h"

class MainFrame : public wxFrame {
public:
    MainFrame(std::string logged_user);
    ~MainFrame();

    

private:
    wxBitmapButton* button_rent;
    wxBitmapButton* button_my_rentals;
    wxBitmapButton* button_all_discs;

public:
    wxWindow* window_rent;
    wxWindow* window_menu;

    static std::string logged_user;

    void OnLogout(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);

    void OnRent(wxCommandEvent& event);
    void OnMyRentals(wxCommandEvent& event);
    void OnAllDiscs(wxCommandEvent& event);
    void Helpr(MainFrame* frame, int a);

    static void DataUpdate(std::string login);

private:
    DECLARE_EVENT_TABLE()
    enum {BUTTON_rent = 2};
    enum {BUTTON_my_rentals = 3};
    enum {BUTTON_all_discs = 4};
    enum {BUTTON_return_disc = 5};
    enum {ID_Logout = 1};
    enum {ID_Window_menu = 50};
    enum {ID_Window_rent = 51};
    enum {ID_Window_my_rentals = 52};
    enum {ID_Window_all_discs = 53};
    enum {ID_Window_return_disc = 54};
};

class wxMyBitmapButton : public wxBitmapButton {
public:
    wxMyBitmapButton(wxWindow* parent, wxWindowID id, const wxBitmap& bitmap, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize,
        long style = wxBU_AUTODRAW, const wxValidator& validator = wxDefaultValidator, const wxString& name = wxButtonNameStr) : wxBitmapButton(parent, id, bitmap, pos, size, style, validator, name) {
        MyApp::dt.SetToCurrent();
        MyApp::log_ << "<" << MyApp::dt.FormatISOCombined(' ') << "> Created 'wxMyBitmapButton' object at " << &*this << "\n";
    };
    ~wxMyBitmapButton();
};