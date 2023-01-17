#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <wx/wxprec.h>
#include <fstream>
#include "main.h"
#include "form_login.h"
#include "main_frame.h"
#include "objects.h"
#include <vector>


using namespace std;


MainFrame::MainFrame(std::string logged_user) : wxFrame(nullptr, wxID_ANY, "Vinyl4You Client", wxPoint(wxID_ANY, wxID_ANY), wxSize(640, 480),
    wxSYSTEM_MENU | wxMINIMIZE_BOX| wxMAXIMIZE_BOX | wxCLOSE_BOX | wxCAPTION | wxCLIP_CHILDREN) {
    SetIcon(wxICON(vinyl_ico));
    wxBORDER_THEME;
    panel_menu = new wxPanel(this, wxID_ANY);
    wxMenu* menu_options = new wxMenu;

    button_rent = new wxButton(panel_menu, BUTTON_rent, wxT("Wypożycz płytę"));
    button_my_rentals = new wxButton(panel_menu, BUTTON_my_rentals, wxT("Moje płyty"));
    button_all_discs = new wxButton(panel_menu, BUTTON_all_discs, wxT("Poznaj ofertę"));
    button_return_disc = new wxButton(panel_menu, BUTTON_return_disc, wxT("Zwróć płytę"));

    wxGridSizer* buttonSizer = new wxGridSizer(2, 2, 1, 1);

    buttonSizer->Add(button_rent, wxALIGN_CENTER);
    buttonSizer->Add(button_my_rentals, wxALIGN_CENTER);
    buttonSizer->Add(button_all_discs, wxALIGN_CENTER);
    buttonSizer->Add(button_return_disc, wxALIGN_CENTER);

    panel_menu->SetBackgroundColour(*wxWHITE);

    panel_menu->SetSizer(buttonSizer);

/*    wxBoxSizer* vert_box = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* horiz_box1 = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* horiz_box2 = new wxBoxSizer(wxHORIZONTAL);
    
    horiz_box1->Add(button_rent, wxLEFT|wxTOP);
    horiz_box1->Add(button_my_rentals, wxRIGHT|wxTOP);

    horiz_box2->Add(button_all_discs, wxLEFT|wxBOTTOM);
    horiz_box2->Add(button_return_disc, wxRIGHT|wxBOTTOM);

    vert_box->Add(horiz_box1, wxTOP);
    vert_box->Add(horiz_box2, wxBOTTOM);
    panel_menu->SetSizer(vert_box);*/

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
    logged_user = "";

}

MainFrame::~MainFrame() = default;

void MainFrame::OnRent(wxCommandEvent& event) {
    fstream disc_data();
    disc_data().open("disc_data.txt", ios::in);
    string a;
    vector<Disc*> disclist;
    while (getline(disc_data(), a)) {
        Disc* dsc = new Disc(a);
        disclist.push_back(dsc);
    }
}

void MainFrame::OnMyRentals(wxCommandEvent& event) {
}

void MainFrame::OnAllDiscs(wxCommandEvent& event) {
}

void MainFrame::OnReturnDisc(wxCommandEvent& event) {
}

BEGIN_EVENT_TABLE(MainFrame, wxFrame)
EVT_BUTTON(BUTTON_my_rentals, MainFrame::OnMyRentals)
EVT_BUTTON(BUTTON_rent, MainFrame::OnRent)
EVT_BUTTON(BUTTON_all_discs, MainFrame::OnAllDiscs)
EVT_BUTTON(BUTTON_return_disc, MainFrame::OnReturnDisc)
END_EVENT_TABLE()
