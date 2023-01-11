#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include "main.h"
#include "form_login.h"
#include "main_frame.h"

MainFrame::MainFrame(std::string logged_user) : wxFrame(nullptr, wxID_ANY, "Vinyl4You Client", wxPoint(wxID_ANY, wxID_ANY), wxSize(640, 480),
    wxSYSTEM_MENU | wxMINIMIZE_BOX| wxMAXIMIZE_BOX | wxCLOSE_BOX | wxCAPTION | wxCLIP_CHILDREN) {
    SetIcon(wxICON(vinyl_ico));
    wxBORDER_THEME;
    wxPanel* panel = new wxPanel(this, wxID_ANY);
    wxMenu* menu_options = new wxMenu;
    wxBoxSizer* vert_box = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* horiz_box1 = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* horiz_box2 = new wxBoxSizer(wxHORIZONTAL);
    
    button_rent = new wxButton(panel, BUTTON_rent, wxT("Wypożycz płytę"));
    horiz_box1->Add(button_rent);
    button_my_rentals = new wxButton(panel, BUTTON_my_rentals, wxT("Moje płyty"));
    horiz_box1->Add(button_my_rentals);

    button_all_discs = new wxButton(panel, BUTTON_all_discs, wxT("Poznaj ofertę"));
    horiz_box2->Add(button_all_discs);
    button_return_disc = new wxButton(panel, BUTTON_return_disc, wxT("Zwróć płytę"));
    horiz_box2->Add(button_return_disc);

    panel->SetBackgroundColour(*wxWHITE);
    vert_box->Add(horiz_box1, 1, wxALIGN_CENTER, 1);
    vert_box->Add(horiz_box2, 1, wxALIGN_CENTER, 1);
    panel->SetSizer(vert_box);
    Centre();

    menu_options->Append(ID_Logout, "&Wyloguj \tCtrl+L",
        "Powrót do ekranu logowania");
    menu_options->AppendSeparator();
    menu_options->Append(wxID_EXIT, "&Zamknij program \tCtrl+Q",
        "Koniec pracy z programem");

    wxMenu* menu_help = new wxMenu;
    menu_help->Append(wxID_ABOUT, "O programie...", "Informacje o programie");

    wxMenuBar* menuBar = new wxMenuBar;
    menuBar->Append(menu_options, "&Opcje");
    menuBar->Append(menu_help, "&Pomoc");

    SetMenuBar(menuBar);

    CreateStatusBar();
    SetStatusText("Vinyl4You Client v. 1.0. All rights reserved. Logged as: "+logged_user);

    Bind(wxEVT_MENU, &MainFrame::OnLogout, this, ID_Logout);
    Bind(wxEVT_MENU, &MainFrame::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &MainFrame::OnExit, this, wxID_EXIT);
}

void MainFrame::OnExit(wxCommandEvent& event) {
    Close(true);
}

void MainFrame::OnAbout(wxCommandEvent& event) {
    wxMessageBox("no siema, chcesz winylka?", "vinyl4you", wxOK | wxICON_INFORMATION);
}

void MainFrame::OnLogout(wxCommandEvent& event) {
    Close(true);
    FormLogin* formLogin = new FormLogin();
    formLogin->Show(true);
    wxMessageBox(wxT("Sesja zakończona"), wxT("Zostałeś wylogowany!"), wxICON_INFORMATION);

}

MainFrame::~MainFrame() = default;

BEGIN_EVENT_TABLE(MainFrame, wxFrame)
EVT_BUTTON(BUTTON_my_rentals, MainFrame::OnAbout)
EVT_BUTTON(BUTTON_rent, MainFrame::OnAbout)
EVT_BUTTON(BUTTON_all_discs, MainFrame::OnAbout)
EVT_BUTTON(BUTTON_return_disc, MainFrame::OnAbout)
END_EVENT_TABLE()
