#pragma once
#include <wx/wx.h>
#include "TreeControl.h"

class MainFrame;

class InfoFrame : public wxFrame {
private:
	void OnBack(wxCommandEvent& event);
public:
	MainFrame* mainFrame = NULL;

	InfoFrame(const wxString& title, MainFrame* mainFrame, std::unordered_map<std::string, std::string> map, Node* root, std::string text, bool compressing);
};