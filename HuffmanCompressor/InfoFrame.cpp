#include <wx/wx.h>
#include "InfoFrame.h"
#include "MainFrame.h"
#include <unordered_map>
#include "Node.h"

void drawNode(wxDC& dc, Node* node, int x, int y) {

}

class TreeControl : public wxScrolledWindow {
public:
	Node* root = nullptr;

	TreeControl(wxWindow* parent, Node* root) : wxScrolledWindow(parent) {
		 SetScrollRate(10, 10);
		 SetVirtualSize(200, 400);
		 this->root = root;
	}

protected:
	void OnDraw(wxDC& dc) override {
		if (!root) return;
		dc.DrawCircle(100, 50, 20);
		dc.DrawText("1", 95, 45);
	}
};



InfoFrame::InfoFrame(const wxString& title, MainFrame* mainFrame, std::unordered_map<std::string, std::string> map, Node* root, std::string text)
	: wxFrame(nullptr, wxID_ANY, title), mainFrame(mainFrame) {
	wxScrolledWindow* scrolledWindow = new wxScrolledWindow(this);
	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
	scrolledWindow->SetSizer(mainSizer);

	// start fonts
	wxFont titleFont = wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT);
	titleFont.SetWeight(wxFONTWEIGHT_BOLD);
	titleFont.SetPointSize(17);

	wxFont labelFont = wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT);
	labelFont.SetPointSize(15);
	// end fonts

	wxButton* backButton = new wxButton(scrolledWindow, wxID_ANY, "Back");
	mainSizer->Add(backButton, 0, wxALL, 5);
	backButton->Bind(wxEVT_BUTTON, &InfoFrame::OnBack, this);

	wxStaticText* titleText = new wxStaticText(scrolledWindow, wxID_ANY, "3A-Z Compressor");
	titleText->SetFont(titleFont);
	titleText->SetBackgroundColour(wxColour(245, 245, 245));
	mainSizer->Add(titleText, 0, wxALL | wxEXPAND, 5);



	// ============================= HUFFMAN TREE ===========================
	wxStaticText* treeLabel = new wxStaticText(scrolledWindow, wxID_ANY, "Huffman Tree");
	treeLabel->SetFont(labelFont);
	mainSizer->Add(treeLabel, 0, wxALL | wxLEFT, 5);
	TreeControl* treeCtrl = new TreeControl(scrolledWindow, root);
	mainSizer->Add(treeCtrl, 1, wxALL | wxEXPAND, 5);

	// ============================= ENCODER ===========================
	wxStaticText* encoderLabel = new wxStaticText(scrolledWindow, wxID_ANY, "Encoder");
	encoderLabel->SetFont(labelFont);
	mainSizer->Add(encoderLabel, 0, wxALL | wxLEFT, 5);

	wxTextCtrl* encoderCtrl = new wxTextCtrl(scrolledWindow, wxID_ANY, "here goes the encoder", wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY);
	mainSizer->Add(encoderCtrl, 1, wxALL | wxEXPAND, 5);

	// ============================= Compressed ===========================
	wxStaticText* compressedLabel = new wxStaticText(scrolledWindow, wxID_ANY, "Compressed");
	compressedLabel->SetFont(labelFont);
	mainSizer->Add(compressedLabel, 0, wxALL | wxLEFT, 5);

	wxTextCtrl* compressedCtrl = new wxTextCtrl(scrolledWindow, wxID_ANY, text, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY);
	mainSizer->Add(compressedCtrl, 1, wxALL | wxEXPAND, 5);

	scrolledWindow->SetScrollRate(10, 10);
}

void InfoFrame::OnBack(wxCommandEvent& event) {
	this->Show(false);
	mainFrame->Show(true);
	this->Destroy();
}