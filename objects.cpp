#include <iostream>
#include <string>
#include <time.h>
#include <fstream>
#include <vector>
#include "objects.h"
#include <wx/listctrl.h>
#include <wx/calctrl.h>
#include "main_frame.h"

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
};

Date::Date() = default;

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
}

bool Date::three_mnths() {
    wxDateTime bruh;
    bruh.SetToCurrent();
    int curr_d = bruh.GetDay();
    int curr_m = bruh.GetMonth();
    int curr_y = bruh.GetYear();

    curr_m += 4;
    curr_y += curr_m / 12;

    Date* _3_mnth_fwd = new Date;
    _3_mnth_fwd->day = curr_d;
    _3_mnth_fwd->month = curr_m;
    _3_mnth_fwd->year = curr_y;

    if (this <= _3_mnth_fwd) {
        _3_mnth_fwd->~Date();
        bruh.~wxDateTime();
        return true;
    }
    _3_mnth_fwd->~Date();
    bruh.~wxDateTime();
    return false;
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
}

Rental::Rental(string log, int d_id, Date* dedl) {
    login = log;
    disc_id = to_string(d_id);
    dedline = dedl;
}

Rental::Rental(string log, string d_id, Date* dedl) {
    login = log;
    disc_id = d_id;
    dedline = dedl;
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

void DiscListCtrl::Format() {
    this->InsertColumn(0, wxString("Tytul"), wxLIST_FORMAT_LEFT, 350);
    this->InsertColumn(1, wxString("Artysta"), wxLIST_FORMAT_LEFT, 350);
    this->InsertColumn(2, wxString("Gatunek"), wxLIST_FORMAT_LEFT, 300);
    this->InsertColumn(3, wxString("Czas trwania"), wxLIST_FORMAT_LEFT, 156);
}

void DiscListCtrl::Format2() {
    this->InsertColumn(0, wxString("Tytul"), wxLIST_FORMAT_LEFT, 350);
    this->InsertColumn(1, wxString("Artysta"), wxLIST_FORMAT_LEFT, 350);
    this->InsertColumn(2, wxString("Gatunek"), wxLIST_FORMAT_LEFT, 300);
    this->InsertColumn(3, wxString("Termin zwrotu"), wxLIST_FORMAT_LEFT, 156);
}

const wxChar* SMALL_VIRTUAL_VIEW_ITEMS[][4] = {
    {}
};

wxString DiscListCtrl::OnGetItemText(long item, long column) const {
    if (GetItemCount() == WXSIZEOF(SMALL_VIRTUAL_VIEW_ITEMS)) {
        return SMALL_VIRTUAL_VIEW_ITEMS[item][column];
    }
    else {
        return wxString::Format(wxT("Column %ld of item %ld"), column, item);
    }
}

void DiscListCtrl::Refresh_lists() {
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
};

void DiscListCtrl::OnActivated(wxListEvent& event) {
    if (this->GetParent()->GetName() == "rent") {
        wxDateTime dt = wxDefaultDateTime;
        wxListItem itemek = event.GetItem();
        Calendar* cal = new Calendar(this, CAL_DATE, itemek, dt, wxPoint(150, 150), wxDefaultSize, NULL, "");
    }
    else if (this->GetParent()->GetName() == "return") {

    }
    else {
        wxMessageBox(wxT("xddddddddddddd!"), wxT("Błąd logowania!"), wxICON_ERROR);
    }

}

void Calendar::DateGet(wxCalendarEvent& event) {
    wxDateTime date = event.GetDate();
    wxListItem item = this->disc;
    this->~Calendar();
    string id_ = Disc::disclist_rent[Disc::disclist_rent.size() - item.m_itemId - 1]->id;
    int day = date.GetDay();
    int mont = date.GetMonth();
    mont++;
    int yer = date.GetYear();
    wxDateTime today;
    today.SetToCurrent();
    Date* todey = new Date(today.GetDay(), today.GetMonth()+1, today.GetYear());
    Date* data = new Date(day, mont, yer);
    if (data > todey) {
        fstream rentals_data;
        rentals_data.open("rentals_data.txt", ios::app);
        Rental* rent = new Rental(MainFrame::logged_user, id_, data);
        rentals_data << rent->pack() << endl;
        wxMessageBox(wxT("Właśnie wypożyczyłeś płytę! Życzymy audiofilskich doznań!"), wxT("Dziękujemy z skorzystanie z usług!"), wxICON_INFORMATION);
        today.~wxDateTime();
        todey->~Date();
        data->~Date();
        rent->~Rental();
        rentals_data.close();
        MainFrame::DataUpdate(MainFrame::logged_user);
        DiscListCtrl::Refresh_lists();
    }
    else
        wxMessageBox(wxT("Data powinna zawierać się w przedziale jutro + 3 miesiące"), wxT("Błąd!"), wxICON_INFORMATION); 
}

DiscListCtrl* DiscListCtrl::rentlist_list;
DiscListCtrl* DiscListCtrl::rentlist_my_list;
DiscListCtrl* DiscListCtrl::all_discs_list;

wxBEGIN_EVENT_TABLE(Calendar, wxCalendarCtrl)
EVT_CALENDAR(CAL_DATE, Calendar::DateGet)
wxEND_EVENT_TABLE()



