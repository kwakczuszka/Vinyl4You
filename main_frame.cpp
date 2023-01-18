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
#include <wx/listctrl.h>

using namespace std;


MainFrame::MainFrame(std::string logged_user) : wxFrame(nullptr, wxID_ANY, "Vinyl4You Client", wxPoint(wxID_ANY, wxID_ANY), wxSize(1280, 720),
    wxSYSTEM_MENU | wxMINIMIZE_BOX| wxMAXIMIZE_BOX | wxCLOSE_BOX | wxCAPTION | wxCLIP_CHILDREN) {
    SetIcon(wxICON(vinyl_ico));
    window_menu = new wxWindow(this, ID_Window_menu, wxDefaultPosition, wxSize(1280, 720), NULL, wxEmptyString);
//    panel_menu = new wxPanel(window_menu, wxID_ANY);

  /*  panel_rent = new wxPanel(this, wxID_ANY);
    panel_rent->Hide();
    panel_my_rentals = new wxPanel(this, wxID_ANY);
    panel_my_rentals->Hide();
    panel_all_discs = new wxPanel(this, wxID_ANY);
    panel_all_discs->Hide();
    panel_return_disc = new wxPanel(this, wxID_ANY);
    panel_return_disc->Hide();*/

    wxMenu* menu_options = new wxMenu;

    button_rent = new wxButton(window_menu, BUTTON_rent, wxT("Wypożycz płytę"));
    button_my_rentals = new wxButton(window_menu, BUTTON_my_rentals, wxT("Moje płyty"));
    button_all_discs = new wxButton(window_menu, BUTTON_all_discs, wxT("Poznaj ofertę"));
    button_return_disc = new wxButton(window_menu, BUTTON_return_disc, wxT("Zwróć płytę"));

    wxGridSizer* buttonSizer = new wxGridSizer(2, 2, 1, 1);

    buttonSizer->Add(button_rent, wxALIGN_CENTER);
    buttonSizer->Add(button_my_rentals, wxALIGN_CENTER);
    buttonSizer->Add(button_all_discs, wxALIGN_CENTER);
    buttonSizer->Add(button_return_disc, wxALIGN_CENTER);

    window_menu->SetBackgroundColour(*wxWHITE);
    window_menu->SetSizer(buttonSizer);

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

    menu_options->Append(ID_Logout, "&Wyloguj \tCtrl+L", "Powrót do ekranu logowania");
    menu_options->AppendSeparator();
    menu_options->Append(wxID_EXIT, "&Zamknij program \tCtrl+Q", "Koniec pracy z programem");

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
    window_menu->Hide();
    window_rent = new wxWindow(this, ID_Window_rent, wxDefaultPosition, wxSize(1280, 670), wxVSCROLL, wxEmptyString);
    fstream disc_data;
    fstream rentals_data;
    disc_data.open("disc_data.txt", ios::in);
    rentals_data.open("rentals_data.txt", ios::in||ios::out);
    string a;
    vector<Disc*> disclist;
    vector<Rental*> rentlist;
    while (getline(disc_data, a)) {
        Disc* dsc = new Disc(a);
        disclist.push_back(dsc);
    }
    while (getline(rentals_data, a)) {
        Rental* rent = new Rental(a);
        rentlist.push_back(rent);
    }
    DiscListCtrl* rentlist_list = new DiscListCtrl(window_rent, LIST_CTRL, wxPoint(20, 20), wxSize(1100, 600), wxLC_REPORT);
    rentlist_list->CentreOnParent();
    rentlist_list->InsertColumn(0, wxString("Tytul"), wxLIST_FORMAT_LEFT, 250);
    rentlist_list->InsertColumn(1, wxString("Artysta"), wxLIST_FORMAT_LEFT, 250);
    rentlist_list->InsertColumn(2, wxString("Gatunek"), wxLIST_FORMAT_LEFT, 250);
    rentlist_list->InsertColumn(3, wxString("Czas trwania"), wxLIST_FORMAT_LEFT, 200);

    for (Disc* i : disclist) {
        long index = rentlist_list->InsertItem(0, _(i->title));
        rentlist_list->SetItem(index, 1, _(i->artist));
        rentlist_list->SetItem(index, 2, _(i->genre));
        rentlist_list->SetItem(index, 3, _(i->length));
    }
    

    wxBoxSizer* lolix = new wxBoxSizer(wxVERTICAL);
    lolix->Add(rentlist_list);
    window_rent->SetSizer(lolix);
    window_rent->Show();
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
