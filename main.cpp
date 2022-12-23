#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include "main.h"
#include "form_login.h"


bool MyApp::OnInit()
{

    FormLogin* logowanko = new FormLogin;
    logowanko->Show(true);
    return true;
}

wxIMPLEMENT_APP(MyApp);

