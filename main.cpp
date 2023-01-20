#include <wx/wxprec.h>
#include "main.h"
#include "form_login.h"

bool MyApp::OnInit() {

    FormLogin* logowanko = new FormLogin;
    logowanko -> SetIcon(wxICON(vinyl_ico));
    logowanko->Show(true);
    return true;
}

wxIMPLEMENT_APP(MyApp);

