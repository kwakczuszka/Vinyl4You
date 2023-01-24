#pragma once
#include <wx/wxprec.h>

class MyApp : public wxApp {
public:
    virtual bool OnInit();
    static std::fstream log_;
    static wxDateTime dt;
    ~MyApp();
};




