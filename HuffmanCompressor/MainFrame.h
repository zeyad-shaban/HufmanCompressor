#pragma once
#include <wx/wx.h>

class MainFrame : public wxFrame {
public:
	wxTextCtrl* dropTarget;
	MainFrame(const wxString& title);
	void OnDropFiles(wxDropFilesEvent& event);
	void onFileSubmit(wxCommandEvent& event);
};