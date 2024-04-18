#pragma once
#include <wx/wx.h>
#include "TreeControl.hpp"

class MainFrame;

class InfoFrame : public wxFrame {
private:
	void OnBack(wxCommandEvent& event);
public:
	//MainFrame* mainFrame;

	InfoFrame(const wxString& title, std::unordered_map<std::string, std::string> map, Node* root, std::string text, bool compressing);
};