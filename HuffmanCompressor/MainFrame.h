#pragma once
#include <wx/wx.h>


class MainFrame : public wxFrame {
public:
	MainFrame(const wxString& title);

private:
	void OnBrowseCompressFile(wxCommandEvent& event);
	void OnBrowseOutputDir(wxCommandEvent& event);
	void onCompress(wxCommandEvent& event);
	wxTextCtrl* filePathCtrl;
	wxTextCtrl* dirPathCtrl;
};