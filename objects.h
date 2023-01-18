#pragma once
#include <iostream>
#include <string>
#include <time.h>
#include "wx/wxprec.h"
#include "wx/listctrl.h"


using namespace std;

class Disc {
public:
    string id;
    string title;
    string artist;
    string genre;
    string length;
public:
    Disc(string lol);
};

class Date {
    int day;
    int month;
    int year;
public:
    Date();
    Date(int a, int b, int c);
    bool three_mnths(int y, int m, int d);

    friend inline bool operator< (const Date& smol_date, const Date& big_date);
    friend inline bool operator> (const Date& smol_date, const Date& big_date);
    friend inline bool operator<=(const Date& smol_date, const Date& big_date);
    friend inline bool operator>=(const Date& smol_date, const Date& big_date);
    friend bool passed();

    Date operator =(const Date& _date) {
        Date _new = Date(0, 0, 0);
        _new.day = _date.day;
        _new.month = _date.month;
        _new.year = _date.year;
        return _new;
    }

};

class Rental {
    string login;
    string disc_id;
    Date* dedline;
    bool passed();
public:
    Rental(string lmao);
};

class DiscListCtrl : public wxListCtrl {
    public:
        DiscListCtrl(wxWindow* parent, const wxWindowID id, const wxPoint& pos, const wxSize& size, long style) : wxListCtrl(parent, id, pos, size, style) {}
        void OnActivated(wxListEvent & event);
        void LogEvent(const wxListEvent& event, const wxChar* eventName);

    public:
        virtual wxString OnGetItemText(long item, long column) const;
        wxDECLARE_NO_COPY_CLASS(DiscListCtrl);
        wxDECLARE_EVENT_TABLE();
};

enum {LIST_CTRL = 1000};