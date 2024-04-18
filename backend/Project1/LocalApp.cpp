#include "LocalApp.h"


bool LocalApp::OnInit() {
    _mkdir("./data");

    MainFrame* mainFrame = new MainFrame("3A-Z");
    mainFrame->SetClientSize(800, 600);
    mainFrame->Center();
    mainFrame->Show();
    return true;
}

IMPLEMENT_APP(LocalApp);