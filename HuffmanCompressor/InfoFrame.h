#pragma once
#include <wx/wx.h>
#include "MainFrame.h"
#include <unordered_map>
#include "Node.h"

class InfoFrame : public wxFrame {
public:
	MainFrame* mainFrame;
	InfoFrame(const wxString& title, MainFrame* mainFrame, std::unordered_map<std::string, std::string> map, Node* root, std::string text);
	void OnBack(wxCommandEvent& event);
};