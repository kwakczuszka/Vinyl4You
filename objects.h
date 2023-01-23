#include "wx/wxprec.h"
#include <wx/listctrl.h>
#include <wx/calctrl.h>
#include "wx/datectrl.h"

using namespace std;

class Disc {
public:
    string id;
    string title;
    string artist;
    string genre;
    string length;
    static vector <Disc*> disclist;
    static vector <Disc*> disclist_my;
    static vector <Disc*> disclist_rent;
public:
    Disc(string lol);
};

class Date {
public:
    int day;
    int month;
    int year;
public:
    Date();
    Date(int a, int b, int c);
    string format_();
    string pack();
    
    Date operator =(const Date& _date) {
        Date _new = Date(0, 0, 0);
        _new.day = _date.day;
        _new.month = _date.month;
        _new.year = _date.year;
        return _new;
    }
};

class Rental {
public:
    string login;
    string disc_id;
    Date* dedline;
    static vector <Rental*> rentlist;
    bool passed();
public:
    Rental(string lmao);
    Rental(string log, int d_id, Date* dedl);
    Rental(string log, string d_id, Date* dedl);
    string pack();
};

class DiscListCtrl : public wxListCtrl {
    public:
        DiscListCtrl(wxWindow* parent, const wxWindowID id, const wxPoint& pos, const wxSize& size, long style) : wxListCtrl(parent, id, pos, size, style) {}
        void OnActivated(wxListEvent & event);
        static void Refresh_lists();
        void Format();
        void Format2();

        static DiscListCtrl* rentlist_list;
        static DiscListCtrl* rentlist_my_list;
        static DiscListCtrl* all_discs_list;

    public:
        virtual wxString OnGetItemText(long item, long column) const;
        wxDECLARE_EVENT_TABLE();
};

enum {LIST_CTRL = 1000};

class Calendar : wxCalendarCtrl {
public:
    wxListItem disc;
    Calendar(wxWindow* parent, wxWindowID id, const wxDateTime& date = wxDefaultDateTime, const wxPoint& pos = wxDefaultPosition, 
        const wxSize& size = wxDefaultSize, long style = wxCAL_SHOW_HOLIDAYS, const wxString& name = "") : wxCalendarCtrl(parent, id, date, pos, size, style, name) {}
    Calendar(wxWindow* parent, wxWindowID id, wxListItem carry, const wxDateTime& date = wxDefaultDateTime, const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize, long style = wxCAL_SHOW_HOLIDAYS, const wxString& name = "") : wxCalendarCtrl(parent, id, date, pos, size, style, name) {
        disc = carry;
    };
    void DateGet(wxCalendarEvent& event);
    wxDECLARE_EVENT_TABLE();
};

enum {CAL_DATE = 2137};

void deleteLine(std::string delLine);

