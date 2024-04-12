#include "App.h"
#include <wx/wx.h>
#include "MainFrame.h"
#include <direct.h>


wxIMPLEMENT_APP(App);

bool App::OnInit() {
	_mkdir("./data");

	MainFrame * mainFrame = new MainFrame("3A-Z");
	mainFrame->SetClientSize(800, 600);
	mainFrame->Center();
	mainFrame->Show();
	return true;
}