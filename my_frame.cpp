#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include "main.h"
#include "form_login.h"
#include "my_frame.h"

MyFrame::MyFrame() : wxFrame(nullptr, wxID_ANY, "Vinyl4You Client")
{
    wxBORDER_THEME;
    wxMenu* menu_options = new wxMenu;
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
    SetStatusText("Vinyl4You Client v. 1.0. All rights reserved.");

    Bind(wxEVT_MENU, &MyFrame::OnLogout, this, ID_Logout);
    Bind(wxEVT_MENU, &MyFrame::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &MyFrame::OnExit, this, wxID_EXIT);
}

void MyFrame::OnExit(wxCommandEvent& event)
{
    Close(true);
}

void MyFrame::OnAbout(wxCommandEvent& event)
{
    wxMessageBox("no siema, chcesz winylka?", "vinyl4you", wxOK | wxICON_INFORMATION);
}

void MyFrame::OnLogout(wxCommandEvent& event)
{
    Close(true);
    FormLogin* formLogin = new FormLogin(wxT("Vinyl4You - Logowanie"));
    formLogin->Show(true);
    wxMessageBox(wxT("Sesja zakończona"), wxT("Zostałeś wylogowany!"), wxICON_INFORMATION);

}