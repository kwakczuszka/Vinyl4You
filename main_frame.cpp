#include <fstream>
#include "main.h"
#include "form_login.h"
#include "main_frame.h"
#include <vector>
#include <wx/listctrl.h>
#include <wx/notebook.h>
#include "objects.h"
#include <wx/calctrl.h>
#include <sstream>
#include <wx/bmpbuttn.h>
#include <wx/image.h>
#include <wx/wx.h>
#include <wx/dc.h>

using namespace std;

MainFrame::~MainFrame() {
    wxWindowList list = this->GetChildren();
    for (int i = 0; i < list.size(); i++) {
        list[i]->DestroyChildren();
    }
    this->DestroyChildren();
}

void MainFrame::DataUpdate(string login) {
    vector<Disc*>::iterator itd;
    vector<Rental*>::iterator itr;

    for (itr = Rental::rentlist.begin(); itr < Rental::rentlist.end(); itr++) {
        (*itr)->~Rental();
    }
    for (itd = Disc::disclist.begin(); itd < Disc::disclist.end(); itd++) {
        (*itd)->~Disc();
    }
    for (itd = Disc::disclist_my.begin(); itd < Disc::disclist_my.end(); itd++) {
        (*itd)->~Disc();
    }
    for (itd = Disc::disclist_rent.begin(); itd < Disc::disclist_rent.end(); itd++) {
        (*itd)->~Disc();
    }
    Disc::disclist.clear();
    Disc::disclist_my.clear();
    Disc::disclist_rent.clear();
    Rental::rentlist.clear();

    fstream disc_data;
    fstream rentals_data;
    disc_data.open("disc_data.txt", ios::in);
    rentals_data.open("rentals_data.txt", ios::in);
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

    for (itr = Rental::rentlist.begin(); itr < Rental::rentlist.end(); itr++) {
        if ((*itr)->login == login) {
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
    disc_data.close();
    rentals_data.close();
}

string MainFrame::logged_user = "";

MainFrame::MainFrame(std::string logged_) : wxFrame(nullptr, wxID_ANY, "Vinyl4You Client", wxPoint(wxID_ANY, wxID_ANY), wxSize(1280, 720),
    wxSYSTEM_MENU | wxMINIMIZE_BOX| wxMAXIMIZE_BOX | wxCLOSE_BOX | wxCAPTION | wxCLIP_CHILDREN) {
    SetIcon(wxICON(vinyl_ico));
    MainFrame::logged_user = logged_;
    window_menu = new wxWindow(this, ID_Window_menu, wxDefaultPosition, wxSize(1280, 720), NULL, wxEmptyString);
    wxMenu* menu_options = new wxMenu;
    MainFrame::DataUpdate(logged_user);
        
    wxInitAllImageHandlers();
    wxImage img_1("rent.jpg", wxBITMAP_TYPE_JPEG, -1);
    wxImage img_2("myrent.jpg", wxBITMAP_TYPE_JPEG, -1);
    wxImage img_3("all.jpg", wxBITMAP_TYPE_JPEG, -1);
    wxBitmap bmp_1(img_1, wxBITMAP_SCREEN_DEPTH);
    wxBitmap bmp_2(img_2, wxBITMAP_SCREEN_DEPTH);
    wxBitmap bmp_3(img_3, wxBITMAP_SCREEN_DEPTH);

    button_rent =           new wxBitmapButton(window_menu, BUTTON_rent, bmp_1, wxDefaultPosition, wxSize(640, 360), 0L, wxDefaultValidator, wxString("rencig"));
    button_my_rentals =     new wxBitmapButton(window_menu, BUTTON_my_rentals, bmp_2, wxDefaultPosition, wxSize(640, 360), 0, wxDefaultValidator, wxString("bruh"));
    button_all_discs =      new wxBitmapButton(window_menu, BUTTON_all_discs, bmp_3, wxDefaultPosition, wxSize(1280, 360), wxALIGN_CENTER, wxDefaultValidator, wxString("bruh"));
//    button_return_disc =    new wxButton(window_menu, BUTTON_return_disc, wxT("Zwróć płytę"));

    wxGridSizer* buttonSizer = new wxGridSizer(2, 2, 1, 1);
    buttonSizer->Add(button_rent);
    buttonSizer->Add(button_my_rentals);
    buttonSizer->Add(button_all_discs);

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

    vector<Rental*>::iterator itr;
    for (itr = Rental::rentlist.begin(); itr < Rental::rentlist.end(); itr++) {
        if (logged_user == (*itr)->login && (*itr)->passed()) {
            wxMessageBox(wxT("Minął zadeklarowany czas zwrotu niektórych pozycji z Twojej audioteki!"), wxT("Ostrzeżenie!"), wxICON_WARNING);
        }
    }

    Bind(wxEVT_MENU, &MainFrame::OnLogout, this, ID_Logout);
    Bind(wxEVT_MENU, &MainFrame::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &MainFrame::OnExit, this, wxID_EXIT);
}

void MainFrame::OnExit(wxCommandEvent& event) {
    this->~MainFrame();
    Close(true);
}

void MainFrame::OnAbout(wxCommandEvent& event) {
    wxMessageBox("Brought to you by K. Kwak, M. Libera and K. Peszko", "Vinyl4You", wxOK | wxICON_INFORMATION);
}

void MainFrame::OnLogout(wxCommandEvent& event) {
    FormLogin* formLogin = new FormLogin();
    Close(true);
    this->~MainFrame();
    wxMessageBox(wxT("Sesja zakończona"), wxT("Zostałeś wylogowany!"), wxICON_INFORMATION);
    logged_user = "";
    formLogin->Show(true);
}

void MainFrame::Helpr(MainFrame* frame, int a) {
    window_menu->~wxWindow();
    window_rent = new wxWindow(frame, ID_Window_rent, wxDefaultPosition, wxSize(1280, 670), wxCENTER, wxEmptyString);

    wxNotebook* sheesh = new wxNotebook(window_rent, ID_Window_rent, wxDefaultPosition, wxSize(1280, 760), wxCENTER, "Plytki");
    wxWindow* sub_1 = new wxWindow(sheesh, wxID_ANY, wxPoint(40, 20), wxSize(1200, 600), wxCENTER, wxString("rent"));
    wxWindow* sub_2 = new wxWindow(sheesh, wxID_ANY, wxPoint(40, 20), wxSize(1200, 600), wxCENTER, wxString("return"));
    wxWindow* sub_3 = new wxWindow(sheesh, wxID_ANY, wxPoint(40, 20), wxSize(1200, 600), wxCENTER, wxString("nope"));

    vector<Disc*>::iterator itr;
    vector<Rental*>::iterator itr2;

    DiscListCtrl::rentlist_list = new DiscListCtrl(sub_1, LIST_CTRL, wxPoint(50, 50), wxSize(1256, 610), wxLC_REPORT);
    DiscListCtrl::rentlist_list->Format();

    for (itr = Disc::disclist_rent.begin(); itr < Disc::disclist_rent.end(); itr++) {
        long index = DiscListCtrl::rentlist_list->InsertItem(0, ((*itr)->title));
        DiscListCtrl::rentlist_list->SetItem(index, 1, ((*itr)->artist));
        DiscListCtrl::rentlist_list->SetItem(index, 2, ((*itr)->genre));
        DiscListCtrl::rentlist_list->SetItem(index, 3, ((*itr)->length));
    }

    DiscListCtrl::rentlist_my_list = new DiscListCtrl(sub_2, LIST_CTRL, wxPoint(50, 50), wxSize(1256, 610), wxLC_REPORT);
    DiscListCtrl::rentlist_my_list->Format2();

    for (itr = Disc::disclist_my.begin(); itr < Disc::disclist_my.end(); itr++) {
        for (itr2 = Rental::rentlist.begin(); itr2 < Rental::rentlist.end(); itr2++) {
            if (MainFrame::logged_user == (*itr2)->login&&(*itr)->id==(*itr2)->disc_id) {
                long index = DiscListCtrl::rentlist_my_list->InsertItem(0, ((*itr)->title));
                DiscListCtrl::rentlist_my_list->SetItem(index, 1, ((*itr)->artist));
                DiscListCtrl::rentlist_my_list->SetItem(index, 2, ((*itr)->genre));
                DiscListCtrl::rentlist_my_list->SetItem(index, 3, ((*itr2)->dedline->format_()));
                if ((*itr2)->passed()) {
                    DiscListCtrl::rentlist_my_list->SetItemBackgroundColour(index, "red");
                }
            }
        }
    }

    DiscListCtrl::all_discs_list = new DiscListCtrl(sub_3, LIST_CTRL, wxPoint(50, 50), wxSize(1256, 610), wxLC_REPORT);
    DiscListCtrl::all_discs_list->Format();

    for (itr = Disc::disclist.begin(); itr < Disc::disclist.end(); itr++) {
        long index = DiscListCtrl::all_discs_list->InsertItem(0, ((*itr)->title));
        DiscListCtrl::all_discs_list->SetItem(index, 1, ((*itr)->artist));
        DiscListCtrl::all_discs_list->SetItem(index, 2, ((*itr)->genre));
        DiscListCtrl::all_discs_list->SetItem(index, 3, ((*itr)->length));
    }

    wxBoxSizer* listSizer1 = new wxBoxSizer(wxHORIZONTAL);
    listSizer1->Add(DiscListCtrl::rentlist_list);
    sub_1->SetSizer(listSizer1);
    
    wxBoxSizer* listSizer2 = new wxBoxSizer(wxHORIZONTAL);
    listSizer2->Add(DiscListCtrl::rentlist_my_list);
    sub_2->SetSizer(listSizer2);

    wxBoxSizer* listSizer3 = new wxBoxSizer(wxHORIZONTAL);
    listSizer3->Add(DiscListCtrl::all_discs_list);
    sub_3->SetSizer(listSizer3);

    switch (a) {
    case(1) : {
        sheesh->AddPage(sub_1, "Wypozycz plyte", true, -1);
        sheesh->AddPage(sub_2, "Moje plyty", false, -1);
        sheesh->AddPage(sub_3, "Wszystkie plyty", false, -1);
        break;
        }
    case(2) : {
        sheesh->AddPage(sub_1, "Wypozycz plyte", false, -1);
        sheesh->AddPage(sub_2, "Moje plyty", true, -1);
        sheesh->AddPage(sub_3, "Wszystkie plyty", false, -1);
        break;
        }
    case(3): {
        sheesh->AddPage(sub_1, "Wypozycz plyte", false, -1);
        sheesh->AddPage(sub_2, "Moje plyty", false, -1);
        sheesh->AddPage(sub_3, "Wszystkie plyty", true, -1);
        break;
        }
    }
    wxBoxSizer* lol = new wxBoxSizer(wxVERTICAL);
    lol->Add(sheesh);
    window_rent->SetSizer(lol);
    window_rent->Show();
}

void MainFrame::OnRent(wxCommandEvent& event) {
    MainFrame::Helpr(this, 1);
}

void MainFrame::OnMyRentals(wxCommandEvent& event) {
    MainFrame::Helpr(this, 2);
}

void MainFrame::OnAllDiscs(wxCommandEvent& event) {
    MainFrame::Helpr(this, 3);
}

BEGIN_EVENT_TABLE(MainFrame, wxFrame)
EVT_BUTTON(BUTTON_my_rentals, MainFrame::OnMyRentals)
EVT_BUTTON(BUTTON_rent, MainFrame::OnRent)
EVT_BUTTON(BUTTON_all_discs, MainFrame::OnAllDiscs)
END_EVENT_TABLE()

