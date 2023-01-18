#pragma once
#include <iostream>
#include <string>
#include <time.h>
#include <fstream>
#include <vector>
#include "objects.h"
#include "wx/wxprec.h"
#include <wx/listctrl.h>


using namespace std;


static inline void rtrim(std::string& s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
        }).base(), s.end());
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
};


Date::Date() = default;

Date::Date(int a, int b, int c) {
    day = a;
    month = b;
    year = c;
}

inline bool operator< (const Date& smol_date, const Date& big_date) {
    if (smol_date.year > big_date.year)
        return false;
    if (smol_date.year < big_date.year)
        return true;
    if (smol_date.month > big_date.month)
        return false;
    if (smol_date.month < big_date.month)
        return true;
    if (smol_date.day > big_date.day)
        return false;
    if (smol_date.day < big_date.day)
        return true;
}
inline bool operator> (const Date& smol_date, const Date& big_date) { return big_date < smol_date; }
inline bool operator<=(const Date& smol_date, const Date& big_date) { return !(smol_date > big_date); }
inline bool operator>=(const Date& smol_date, const Date& big_date) { return !(smol_date < big_date); }

bool Date::three_mnths(int d, int m, int y) {

    Date* query = new Date;
    query->day = d;
    query->month = m;
    query->year = y;

    time_t theTime = time(NULL);
#pragma warning(suppress : 4996)
    struct tm* aTime = localtime(&theTime);
#pragma warning(suppress : 4996)
    int curr_d = aTime->tm_mday;
    int curr_m = aTime->tm_mon + 1;
    int curr_y = aTime->tm_year + 1900;

    aTime->tm_mon += 3;
    aTime->tm_year += aTime->tm_mon / 12;

    Date* _3_mnth_fwd = new Date;
    _3_mnth_fwd->day = curr_d;
    _3_mnth_fwd->month = curr_m;
    _3_mnth_fwd->year = curr_y;

    if (query <= _3_mnth_fwd)
        query->~Date();
    _3_mnth_fwd->~Date();
    return true;
    return false;
}

Rental::Rental(string lmao) {
    login = lmao.substr(0, 12);
    disc_id = lmao.substr(12, 3);
    string x = lmao.substr(15, 2);
    string y = lmao.substr(17, 2);
    string z = lmao.substr(19, 4);
    rtrim(login);
    rtrim(disc_id);
    rtrim(x);
    rtrim(y);
    rtrim(z);
    dedline = new Date(stoi(x), stoi(y), stoi(z));
}

bool Rental::passed() {
    time_t theTime = time(NULL);
#pragma warning(suppress : 4996)
    struct tm* aTime = localtime(&theTime);
#pragma warning(suppress : 4996)
    int curr_d = aTime->tm_mday;
    int curr_m = aTime->tm_mon + 1;
    int curr_y = aTime->tm_year + 1900;

    Date* today = new Date(curr_d, curr_m, curr_y);

    if (today < dedline)
        return false;
    return true;
}

wxBEGIN_EVENT_TABLE(DiscListCtrl, wxListCtrl)
EVT_LIST_ITEM_ACTIVATED(LIST_CTRL, DiscListCtrl::OnActivated)
wxEND_EVENT_TABLE()

void DiscListCtrl::OnActivated(wxListEvent& event) {
    LogEvent(event, wxT("OnActivated"));
}

void DiscListCtrl::LogEvent(const wxListEvent& event, const wxChar* eventName) {
    wxLogMessage(wxT("Item %ld: %s (item text = %s, data = %ld lolol = %ld)"),
        event.GetIndex(), eventName,
        event.GetText(), static_cast<long>(event.GetData()), static_cast<long>(event.GetId()));
}
const wxChar* SMALL_VIRTUAL_VIEW_ITEMS[][2] = {
    { wxT("Cat"), wxT("meow") },
    { wxT("Cow"), wxT("moo") },
    { wxT("Crow"), wxT("caw") },
    { wxT("Dog"), wxT("woof") },
    { wxT("Duck"), wxT("quack") },
    { wxT("Mouse"), wxT("squeak") },
    { wxT("Owl"), wxT("hoo") },
    { wxT("Pig"), wxT("oink") },
    { wxT("Pigeon"), wxT("coo") },
    { wxT("Sheep"), wxT("baaah") },
};
wxString DiscListCtrl::OnGetItemText(long item, long column) const {
    if (GetItemCount() == WXSIZEOF(SMALL_VIRTUAL_VIEW_ITEMS)) {
        return SMALL_VIRTUAL_VIEW_ITEMS[item][column];
    }
    else {
        return wxString::Format(wxT("Column %ld of item %ld"), column, item);
    }
}


