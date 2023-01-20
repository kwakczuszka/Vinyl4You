#include <fstream>
#include "main.h"
#include "form_login.h"
#include "main_frame.h"
#include <vector>
#include <wx/listctrl.h>
#include <wx/notebook.h>
#include "objects.h"

using namespace std;


MainFrame::MainFrame(std::string logged_user) : wxFrame(nullptr, wxID_ANY, "Vinyl4You Client", wxPoint(wxID_ANY, wxID_ANY), wxSize(1280, 720),
    wxSYSTEM_MENU | wxMINIMIZE_BOX| wxMAXIMIZE_BOX | wxCLOSE_BOX | wxCAPTION | wxCLIP_CHILDREN) {
    SetIcon(wxICON(vinyl_ico));
    window_menu = new wxWindow(this, ID_Window_menu, wxDefaultPosition, wxSize(1280, 720), NULL, wxEmptyString);
    wxMenu* menu_options = new wxMenu;
    fstream disc_data;
    fstream rentals_data;
    disc_data.open("disc_data.txt", ios::in);
    rentals_data.open("rentals_data.txt", ios::in || ios::out);
    string a;
    while (getline(disc_data, a)) {
        Disc* dsc = new Disc(a);
        Disc::disclist.push_back(dsc);
    }

    Disc::disclist_rent = Disc::disclist;

    while (getline(rentals_data, a)) {
        Rental* rent = new Rental(a);
        Rental::rentlist.push_back(rent);
    }
    int x = 0;

    vector<Disc*>::iterator itd;
    vector<Rental*>::iterator itr;

    for (itr = Rental::rentlist.begin(); itr < Rental::rentlist.end(); itr++) {
        if ((*itr)->login == logged_user) {
            for (itd = Disc::disclist.begin(); itd < Disc::disclist.end(); itd++) {
                if ((*itd)->id == (*itr)->disc_id) {
                    Disc::disclist_my.push_back((*itd));
                }
            }
        }
    }

    for (itr = Rental::rentlist.begin(); itr < Rental::rentlist.end(); itr++) {
        for (itd = Disc::disclist_rent.begin(); itd < Disc::disclist_rent.end(); ) {
            if ((*itd)->id == (*itr)->disc_id) {
                itd = Disc::disclist_rent.erase(itd);
            }
            else
                itd++;
        }
    }
    
    button_rent =           new wxButton(window_menu, BUTTON_rent, wxT("Wypożycz płytę"));
    button_my_rentals =     new wxButton(window_menu, BUTTON_my_rentals, wxT("Moje płyty"));
    button_all_discs =      new wxButton(window_menu, BUTTON_all_discs, wxT("Poznaj ofertę"));
    button_return_disc =    new wxButton(window_menu, BUTTON_return_disc, wxT("Zwróć płytę"));

    wxGridSizer* buttonSizer = new wxGridSizer(2, 2, 1, 1);

    buttonSizer->Add(button_rent,           wxALIGN_CENTER);
    buttonSizer->Add(button_my_rentals,     wxALIGN_CENTER);
    buttonSizer->Add(button_all_discs,      wxALIGN_CENTER);
    buttonSizer->Add(button_return_disc,    wxALIGN_CENTER);

    window_menu->SetBackgroundColour(*wxWHITE);
    window_menu->SetSizer(buttonSizer);

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
    window_my_rentals = new wxWindow(this, ID_Window_rent, wxDefaultPosition, wxSize(1280, 670), wxVSCROLL, wxEmptyString);
    wxNotebook* sheesh = new wxNotebook(window_rent, ID_Window_rent, wxDefaultPosition, wxSize(1280, 760), 0L, "Plytki");
    wxWindow* sub_1 = new wxWindow(sheesh, wxID_ANY, wxDefaultPosition, wxSize(1200, 600), NULL, wxEmptyString);

    DiscListCtrl* rentlist_list = new DiscListCtrl(sub_1, LIST_CTRL, wxPoint(20, 20), wxSize(1100, 600), wxLC_REPORT);
    rentlist_list->Format();

    vector<Disc*>::iterator itr;
    for (itr = Disc::disclist_rent.begin(); itr < Disc::disclist_rent.end(); itr++) {
        long index = rentlist_list->InsertItem(0, ((*itr)->title));
        rentlist_list->SetItem(index, 1, ((*itr)->artist));
        rentlist_list->SetItem(index, 2, ((*itr)->genre));
        rentlist_list->SetItem(index, 3, ((*itr)->length));
    }
    
    wxBoxSizer* listSizer = new wxBoxSizer(wxVERTICAL);
    listSizer->Add(rentlist_list);
    
    sub_1->SetSizer(listSizer);
   
    sheesh->AddPage(sub_1, "Wypozycz plyte", true, -1);

    wxWindow* sub_2 = new wxWindow(sheesh, wxID_ANY, wxDefaultPosition, wxSize(1200, 600), NULL, wxEmptyString);

    DiscListCtrl* rentlist_my_list = new DiscListCtrl(sub_2, LIST_CTRL, wxPoint(20, 20), wxSize(1100, 600), wxLC_REPORT);
    rentlist_my_list->Format();

    for (itr = Disc::disclist_my.begin(); itr < Disc::disclist_my.end(); itr++) {
        long index = rentlist_my_list->InsertItem(0, ((*itr)->title));
        rentlist_my_list->SetItem(index, 1, ((*itr)->artist));
        rentlist_my_list->SetItem(index, 2, ((*itr)->genre));
        rentlist_my_list->SetItem(index, 3, ((*itr)->length));
    }

    wxBoxSizer* listSizer2 = new wxBoxSizer(wxVERTICAL);
    listSizer2->Add(rentlist_my_list);
    sub_2->SetSizer(listSizer2);

    sheesh->AddPage(sub_2, "Moje plyty", false, -1);
    sheesh->Show();
}

void MainFrame::list_OnRent(wxEvent& event, int index) {
    string id_ = Disc::disclist[index]->id;
}

void MainFrame::OnMyRentals(wxCommandEvent& event) {
    window_menu->Hide();
    window_rent = new wxWindow(this, ID_Window_rent, wxDefaultPosition, wxSize(1280, 670), wxVSCROLL, wxEmptyString);
    window_my_rentals = new wxWindow(this, ID_Window_rent, wxDefaultPosition, wxSize(1280, 670), wxVSCROLL, wxEmptyString);

    DiscListCtrl* rentlist_list = new DiscListCtrl(window_rent, LIST_CTRL, wxPoint(20, 20), wxSize(1100, 600), wxLC_REPORT);
    rentlist_list->Format();

    vector<Disc*>::iterator itr;
    for (itr = Disc::disclist_rent.begin(); itr < Disc::disclist_rent.end(); itr++) {
        long index = rentlist_list->InsertItem(0, ((*itr)->title));
        rentlist_list->SetItem(index, 1, ((*itr)->artist));
        rentlist_list->SetItem(index, 2, ((*itr)->genre));
        rentlist_list->SetItem(index, 3, ((*itr)->length));
    }

    wxBoxSizer* listSizer = new wxBoxSizer(wxVERTICAL);
    listSizer->Add(rentlist_list);
    wxNotebook* sheesh = new wxNotebook(window_rent, ID_Window_rent, wxDefaultPosition, wxSize(1280, 760), 0L, "Plytki");
    wxWindow* sub_1 = new wxWindow(sheesh, wxID_ANY, wxDefaultPosition, wxSize(1200, 600), NULL, wxEmptyString);
    sub_1->SetSizer(listSizer);

    sheesh->AddPage(sub_1, "Wypozycz plyte", false, -1);
     
    DiscListCtrl* rentlist_my_list = new DiscListCtrl(window_my_rentals, LIST_CTRL, wxPoint(20, 20), wxSize(1100, 600), wxLC_REPORT);
    rentlist_my_list->Format();

    for (itr = Disc::disclist_my.begin(); itr < Disc::disclist_my.end(); itr++) {
        long index = rentlist_my_list->InsertItem(0, ((*itr)->title));
        rentlist_my_list->SetItem(index, 1, ((*itr)->artist));
        rentlist_my_list->SetItem(index, 2, ((*itr)->genre));
        rentlist_my_list->SetItem(index, 3, ((*itr)->length));
    }

    wxBoxSizer* listSizer2 = new wxBoxSizer(wxVERTICAL);
    wxWindow* sub_2 = new wxWindow(sheesh, wxID_ANY, wxDefaultPosition, wxSize(1200, 600), NULL, wxEmptyString);
    listSizer2->Add(rentlist_my_list);

    sub_2->SetSizer(listSizer2);

    sheesh->AddPage(sub_2, "Moje plyty", true, -1);
    sheesh->Show();
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

