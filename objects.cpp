#include <iostream>
#include <string>
#include <time.h>
#include <fstream>
#include <vector>
#include "objects.h"
#include <wx/listctrl.h>
#include <wx/calctrl.h>
#include "main_frame.h"
#include <wx/font.h>

using namespace std;

static inline void rtrim(std::string& s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
        }).base(), s.end());
}

vector<Disc*> Disc::disclist;
vector<Disc*> Disc::disclist_rent;
vector<Disc*> Disc::disclist_my;
vector<Rental*> Rental::rentlist;

void deleteLine(std::string delLine) {
    std::string line;
    std::ifstream file;
    file.open("rentals_data.txt", std::fstream::in);
    if (file.good()) {
        std::fstream temp("temp.txt", std::fstream::out);
        while (getline(file, line)) {
            if (line.find(delLine) != std::string::npos)
                line.replace(line.find(delLine), delLine.length(), "");
            if (!line.empty()) {
                temp << line << "\n";
            }
        }
        file.close();
        temp.close();
        remove("rentals_data.txt");
        rename("temp.txt", "rentals_data.txt");
    }
    else {
        wxMessageBox(wxT("błond!"), wxT("Zwrócono płytę!"), wxICON_INFORMATION);

    }
}

Disc::Disc(string lol) {
    id = lol.substr(0, 3);
    title = lol.substr(3, 27);
    artist = lol.substr(30, 16);
    genre = lol.substr(46, 12);
    length = lol.substr(58, 5);
    rtrim(id);
    rtrim(title);
    rtrim(artist);
    rtrim(genre);
    rtrim(length);
    MyApp::dt.SetToCurrent();
    MyApp::log_ << "<" << MyApp::dt.FormatISOCombined(' ') << "> Created 'Disc' object at " << &*this << "\n";
};

Disc::~Disc() {
    MyApp::dt.SetToCurrent();
    MyApp::log_ << "<" << MyApp::dt.FormatISOCombined(' ') << "> Destroyed 'Disc' object at " << &*this << "\n";
};

Date::Date() {
    MyApp::dt.SetToCurrent();
    MyApp::log_ << "<" << MyApp::dt.FormatISOCombined(' ') << "> Created 'Date' object at " << &*this << "\n";
};

Date::~Date() {
    MyApp::dt.SetToCurrent();
    MyApp::log_ << "<" << MyApp::dt.FormatISOCombined(' ') << "> Destroyed 'Date' object at " << &*this << "\n";
};

string Date::format_() {
    string a="";
    a += to_string(day);
    a += '/';
    a += to_string(month);
    a += '/';
    a += to_string(year);
    return a;
}

Date::Date(int a, int b, int c) {
    day = a;
    month = b;
    year = c;
    MyApp::dt.SetToCurrent();
    MyApp::log_ << "<" << MyApp::dt.FormatISOCombined(' ') << "> Created 'Date' object at " << &*this << "\n";
}

string Date::pack() {
    string a = "";
    a += to_string(day);
    for (int i = 0; i < 2 - to_string(day).length(); i++) {
        a += ' ';
    }
    a += to_string(month);
    for (int i = 0; i < 2 - to_string(month).length(); i++) {
        a += ' ';
    }
    a += to_string(year);

    return a;
}

Rental::Rental(string lmao) {
    login = lmao.substr(0, 13);
    disc_id = lmao.substr(13, 3);
    string x = lmao.substr(16, 2);
    string y = lmao.substr(18, 2);
    string z = lmao.substr(20, 4);
    rtrim(login);
    rtrim(disc_id);
    rtrim(x);
    rtrim(y);
    rtrim(z);
    dedline = new Date(stoi(x), stoi(y), stoi(z));
    MyApp::dt.SetToCurrent();
    MyApp::log_ << "<" << MyApp::dt.FormatISOCombined(' ') << "> Created 'Rental' object at " << &*this << "\n";
}

Rental::Rental(string log, int d_id, Date* dedl) {
    login = log;
    disc_id = to_string(d_id);
    dedline = dedl;
    MyApp::dt.SetToCurrent();
    MyApp::log_ << "<" << MyApp::dt.FormatISOCombined(' ') << "> Created 'Rental' object at " << &*this << "\n";
}

Rental::Rental(string log, string d_id, Date* dedl) {
    login = log;
    disc_id = d_id;
    dedline = dedl;
    MyApp::dt.SetToCurrent();
    MyApp::log_ << "<" << MyApp::dt.FormatISOCombined(' ') << "> Created 'Rental' object at " << &*this << "\n";
}

Rental::~Rental() {
    this->dedline->~Date();
    MyApp::dt.SetToCurrent();
    MyApp::log_ << "<" << MyApp::dt.FormatISOCombined(' ') << "> Destroyed 'Rental' object at " << &*this << "\n";
}

bool Rental::passed() {
    wxDateTime bruh;
    bruh.SetToCurrent();
    int curr_d = bruh.GetDay();
    int curr_m = bruh.GetMonth();
    int curr_y = bruh.GetYear();
    curr_m++;
    bruh.~wxDateTime();
    Date* today = new Date(curr_d, curr_m, curr_y);

    if (today->year < dedline->year) {
        today->~Date();
        return false;
    }
    else if (today->year > dedline->year) {
        today->~Date();
        return true;
    }
    else if (today->month < dedline->month) {
        today->~Date();
        return false;
    }
    else if (today->month > dedline->month) {
        today->~Date();
        return true;
    }
    else if (today->day < dedline->day) {
        today->~Date();
        return false;
    }
    today->~Date();
    return true;
}

string Rental::pack() {
    string a = "";
    a += login;
    for (int i = 0; i < 13 - login.length(); i++) {
        a += ' ';
    }
    a += disc_id;
    for (int i = 0; i < 3 - disc_id.length(); i++) {
        a += ' ';
    }
    a += dedline->pack();

    return a;
}

wxBEGIN_EVENT_TABLE(DiscListCtrl, wxListCtrl)
EVT_LIST_ITEM_ACTIVATED(LIST_CTRL, DiscListCtrl::OnActivated)
wxEND_EVENT_TABLE()

DiscListCtrl::~DiscListCtrl() {
    MyApp::dt.SetToCurrent();
    MyApp::log_ << "<" << MyApp::dt.FormatISOCombined(' ') << "> Destroyed 'DiscListCtrl' object at " << &*this << "\n";
}

void DiscListCtrl::Format() {
    this->SetFont(wxFont(18, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxEmptyString, wxFONTENCODING_DEFAULT));
    this->InsertColumn(0, wxString("Tytul"), wxLIST_FORMAT_LEFT, 450);
    this->InsertColumn(1, wxString("Artysta"), wxLIST_FORMAT_LEFT, 275);
    this->InsertColumn(2, wxString("Gatunek"), wxLIST_FORMAT_LEFT, 275);
    this->InsertColumn(3, wxString("Czas trwania"), wxLIST_FORMAT_LEFT, 256);
    MyApp::dt.SetToCurrent();
    MyApp::log_ << "<" << MyApp::dt.FormatISOCombined(' ') << "> DiscListCtrl object at " << &*this << " formatted\n";
}

void DiscListCtrl::Format2() {
    this->SetFont(wxFont(18, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxEmptyString, wxFONTENCODING_DEFAULT));
    this->InsertColumn(0, wxString("Tytul"), wxLIST_FORMAT_LEFT, 450);
    this->InsertColumn(1, wxString("Artysta"), wxLIST_FORMAT_LEFT, 275);
    this->InsertColumn(2, wxString("Gatunek"), wxLIST_FORMAT_LEFT, 275);
    this->InsertColumn(3, wxString("Termin zwrotu"), wxLIST_FORMAT_LEFT, 256);
    MyApp::dt.SetToCurrent();
    MyApp::log_ << "<" << MyApp::dt.FormatISOCombined(' ') << "> DiscListCtrl object at " << &*this << " formatted\n";
}

wxString DiscListCtrl::OnGetItemText(long item, long column) const {
    return wxString::Format(wxT("Column %ld of item %ld"), column, item);
}

void DiscListCtrl::Refresh_lists() {
    MyApp::dt.SetToCurrent();
    MyApp::log_ << "<" << MyApp::dt.FormatISOCombined(' ') << " List refresh has begun\n";
    DiscListCtrl::rentlist_list->ClearAll();
    DiscListCtrl::rentlist_my_list->ClearAll();
    DiscListCtrl::all_discs_list->ClearAll();
    DiscListCtrl::rentlist_list->Format();
    DiscListCtrl::rentlist_my_list->Format2();
    DiscListCtrl::all_discs_list->Format();

    vector<Rental*>::iterator itr2;
    vector<Disc*>::iterator itr;

    for (itr = Disc::disclist_rent.begin(); itr < Disc::disclist_rent.end(); itr++) {
        long index = DiscListCtrl::rentlist_list->InsertItem(0, ((*itr)->title));
        DiscListCtrl::rentlist_list->SetItem(index, 1, ((*itr)->artist));
        DiscListCtrl::rentlist_list->SetItem(index, 2, ((*itr)->genre));
        DiscListCtrl::rentlist_list->SetItem(index, 3, ((*itr)->length));
    }

    for (itr = Disc::disclist_my.begin(); itr < Disc::disclist_my.end(); itr++) {
        for (itr2 = Rental::rentlist.begin(); itr2 < Rental::rentlist.end(); itr2++) {
            if (MainFrame::logged_user == (*itr2)->login && (*itr)->id == (*itr2)->disc_id) {
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

    for (itr = Disc::disclist.begin(); itr < Disc::disclist.end(); itr++) {
        long index = DiscListCtrl::all_discs_list->InsertItem(0, ((*itr)->title));
        DiscListCtrl::all_discs_list->SetItem(index, 1, ((*itr)->artist));
        DiscListCtrl::all_discs_list->SetItem(index, 2, ((*itr)->genre));
        DiscListCtrl::all_discs_list->SetItem(index, 3, ((*itr)->length));
    }
    MyApp::dt.SetToCurrent();
    MyApp::log_ << "<" << MyApp::dt.FormatISOCombined(' ') << " List refresh finished\n";
};

void DiscListCtrl::OnActivated(wxListEvent& event) {
    MyApp::dt.SetToCurrent();
    MyApp::log_ << "<" << MyApp::dt.FormatISOCombined(' ') << "> Picked disc " << event.GetItem().GetText() << "\n";
    if (this->GetParent()->GetName() == "rent") {
        wxMessageBox(wxT("Wybierz deklarowaną datę zwrotu płyty (max. 3 miesiące)"), wxT("Informacja"), wxICON_INFORMATION);

        wxDateTime dat = wxDefaultDateTime;
        wxListItem itemek = event.GetItem();
        Calendar* cal = new Calendar(this, CAL_DATE, itemek, dat, wxPoint(328, 50), wxSize(600, 300), NULL, "");
        MyApp::dt.SetToCurrent();
        MyApp::log_ << "<" << MyApp::dt.FormatISOCombined(' ') << "> Date select shown\n";
    }
    else if (this->GetParent()->GetName() == "return") {
        wxListItem item = event.GetItem();
        wxString tittle = item.GetText();
        tittle.ToStdString();
        vector<Rental*>::iterator itr;
        vector<Disc*>::iterator itr2;
        bool pas = false;
        for (itr = Rental::rentlist.begin(); itr < Rental::rentlist.end(); itr++) {
            if ((*itr)->login == MainFrame::logged_user) {
                for (itr2 = Disc::disclist_my.begin(); itr2 < Disc::disclist_my.end(); ) {
                    if ((*itr2)->title == tittle&&(*itr)->disc_id==(*itr2)->id) {
                        MyApp::dt.SetToCurrent();
                        MyApp::log_ << "<" << MyApp::dt.FormatISOCombined(' ') << " > Disc id " << (*itr2)->id << " returned\n";
                        deleteLine((*itr)->pack());
                        itr = Rental::rentlist.erase(itr);
                        MainFrame::DataUpdate(MainFrame::logged_user);
                        DiscListCtrl::Refresh_lists();
                        pas = true;
                        
                        break;
                    }
                    else {
                        itr2++;
                    }
                }
            }
            if (pas)
                break;
        }
        wxMessageBox(wxT("Dziękujemy za skorzystanie z usług, zapraszamy ponownie!"), wxT("Zwrócono płytę!"), wxICON_INFORMATION);
        
        MyApp::dt.SetToCurrent();
        MyApp::log_ << "<" << MyApp::dt.FormatISOCombined(' ') << "> Info message shown (disc returned)\n";
    }
    else {
        wxMessageBox(wxT("W celu wypożyczenia płyty, skorzystaj z zakładki \"Wypożycz płytę\""), wxT("Informacja"), wxICON_INFORMATION);
        MyApp::dt.SetToCurrent();
        MyApp::log_ << "<" << MyApp::dt.FormatISOCombined(' ') << "> Info message shown (wrong list)\n";
    }
}

void Calendar::DateGet(wxCalendarEvent& event) {
    wxDateTime date = event.GetDate();
    MyApp::dt.SetToCurrent();
    MyApp::log_ << "<" << MyApp::dt.FormatISOCombined(' ') << "> Picked date " << date.FormatISODate() << "\n";
    wxListItem item = this->disc;
    this->~Calendar();
    MyApp::dt.SetToCurrent();
    MyApp::log_ << "<" << MyApp::dt.FormatISOCombined(' ') << "> Date select closed\n";
    string id_ = Disc::disclist_rent[Disc::disclist_rent.size() - item.m_itemId - 1]->id;
    int day = date.GetDay();
    int mont = date.GetMonth();
    mont++;
    int yer = date.GetYear();
    wxDateTime today;
    today.SetToCurrent();
    Date* todey = new Date(today.GetDay(), today.GetMonth() + 1, today.GetYear());
    Date* data = new Date(day, mont, yer);

    bool statement;

    if (data->year < todey->year) {
        data->~Date();
        statement= false;
    }
    else if (data->year > todey->year) {
        data->~Date();
        statement= true;
    }
    else if (data->month < todey->month) {
        data->~Date();
        statement= false;
    }
    else if (data->month > todey->month) {
        data->~Date();
        statement= true;
    }
    else if (data->day < todey->day) {
        data->~Date();
        statement= false;
    }
    else {
        data->~Date();
        statement = true;
    }
    if (statement) {
        todey->month += 3;
        if (todey->month > 12) {
            todey->month -= 12;
            todey->year++;
        }
        if (data->year > todey->year) {
            data->~Date();
            statement = false;
        }
        else if (data->year < todey->year) {
            data->~Date();
            statement = true;
        }
        else if (data->month > todey->month) {
            data->~Date();
            statement = false;
        }
        else if (data->month < todey->month) {
            data->~Date();
            statement = true;
        }
        else if (data->day > todey->day) {
            data->~Date();
            statement = false;
        }
        else {
            data->~Date();
            statement = true;
        }
        if (statement) {
            fstream rentals_data;
            rentals_data.open("rentals_data.txt", ios::app);
            Rental* rent = new Rental(MainFrame::logged_user, id_, data);
            rentals_data << rent->pack() << endl;
            MyApp::dt.SetToCurrent();
            MyApp::log_ << "<" << MyApp::dt.FormatISOCombined(' ') << "> Succesfully rent disc with id "<< id_ << " by " << MainFrame::logged_user << " with deadline " << date.FormatISODate() << "\n";
            wxMessageBox(wxT("Właśnie wypożyczyłeś płytę! Życzymy audiofilskich doznań!"), wxT("Dziękujemy z skorzystanie z usług!"), wxICON_INFORMATION);
            MyApp::dt.SetToCurrent();
            MyApp::log_ << "<" << MyApp::dt.FormatISOCombined(' ') << "> Thanks for rent window shown\n";
            today.~wxDateTime();
            todey->~Date();
            data->~Date();
            rent->~Rental();
            rentals_data.close();
            MainFrame::DataUpdate(MainFrame::logged_user);
            DiscListCtrl::Refresh_lists();
        }
        else {
            wxMessageBox(wxT("Data powinna zawierać się w przedziale jutro + 3 miesiące"), wxT("Błąd!"), wxICON_INFORMATION);
            MyApp::dt.SetToCurrent();
            MyApp::log_ << "<" << MyApp::dt.FormatISOCombined(' ') << "> Picked date out of range\n";
        }
            
    }
    else {
        wxMessageBox(wxT("Data powinna zawierać się w przedziale jutro + 3 miesiące"), wxT("Błąd!"), wxICON_INFORMATION);
        MyApp::dt.SetToCurrent();
        MyApp::log_ << "<" << MyApp::dt.FormatISOCombined(' ') << "> Picked date out of range\n";
    }
        
}

Calendar::~Calendar() {
    MyApp::dt.SetToCurrent();
    MyApp::log_ << "<" << MyApp::dt.FormatISOCombined(' ') << "> Destroyed 'Calendar' object at " << &*this << "\n";
}
DiscListCtrl* DiscListCtrl::rentlist_list;
DiscListCtrl* DiscListCtrl::rentlist_my_list;
DiscListCtrl* DiscListCtrl::all_discs_list;

wxBEGIN_EVENT_TABLE(Calendar, wxCalendarCtrl)
EVT_CALENDAR(CAL_DATE, Calendar::DateGet)
wxEND_EVENT_TABLE()