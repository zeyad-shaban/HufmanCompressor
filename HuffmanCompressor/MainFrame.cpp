#include <wx/wxprec.h>
#include <wx/tokenzr.h>
#include <wx/arrstr.h>
#include <wx/dir.h>
#include <wx/busyinfo.h>
#include "MainFrame.h"
#include "utils.h"
#include "MinHeap.h"
#include "Node.h"
#include "Compress.h"
#include <filesystem>

using namespace std;

MainFrame::MainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title) {
	int btnsHeight = 30;
	this->SetBackgroundColour(wxColour(240, 240, 240));

	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

	// start fonts
	wxFont titleFont = wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT);
	titleFont.SetWeight(wxFONTWEIGHT_BOLD);
	titleFont.SetPointSize(17);

	wxFont labelFont = wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT);
	labelFont.SetPointSize(15);
	// end fonts

	// start title area
	wxStaticText* titleText = new wxStaticText(this, wxID_ANY, "3A-Z Compressor");
	titleText->SetFont(titleFont);
	titleText->SetBackgroundColour(wxColour(245, 245, 245));

	mainSizer->Add(titleText, 0, wxALL | wxEXPAND, 5);
	// end title area

	// ========================  FILE AREA  ===========================

	wxStaticText* fileLabel = new wxStaticText(this, wxID_ANY, "File to Compress");
	fileLabel->SetFont(labelFont);
	mainSizer->Add(fileLabel, 0, wxALL | wxLEFT, 5);

	filePathCtrl = new wxTextCtrl(this, wxID_ANY);
	wxButton* fileBrowseBtn = new wxButton(this, wxID_ANY, "Browse");
	filePathCtrl->SetMinSize(wxSize(-1, btnsHeight));
	fileBrowseBtn->SetMinSize(wxSize(-1, btnsHeight));

	wxBoxSizer* hboxFile = new wxBoxSizer(wxHORIZONTAL);
	hboxFile->Add(filePathCtrl, 1, wxALL | wxEXPAND, 5);
	hboxFile->Add(fileBrowseBtn, 0, wxALL, 5);

	mainSizer->Add(hboxFile, 0, wxEXPAND);

	Bind(wxEVT_BUTTON, &MainFrame::OnBrowseCompressFile, this, fileBrowseBtn->GetId());

	mainSizer->AddStretchSpacer();

	// ========================  FOLDER AREA  ===========================

	wxStaticText* outDirLabel = new wxStaticText(this, wxID_ANY, "Output Directory");
	outDirLabel->SetFont(labelFont);
	mainSizer->Add(outDirLabel, 0, wxALL | wxLEFT, 5);

	dirPathCtrl = new wxTextCtrl(this, wxID_ANY); // THIS LINE CAUSES HEAP CORRUPTION
	wxButton* dirBrowseBtn = new wxButton(this, wxID_ANY, "Browse");
	dirPathCtrl->SetMinSize(wxSize(-1, btnsHeight));
	dirBrowseBtn->SetMinSize(wxSize(-1, btnsHeight));

	wxBoxSizer* hboxDir = new wxBoxSizer(wxHORIZONTAL);
	hboxDir->Add(dirPathCtrl, 1, wxALL | wxEXPAND, 5);
	hboxDir->Add(dirBrowseBtn, 0, wxALL, 5);

	mainSizer->Add(hboxDir, 0, wxEXPAND);

	Bind(wxEVT_BUTTON, &MainFrame::OnBrowseOutputDir, this, dirBrowseBtn->GetId());

	// ==================  START AREA  =========================
	wxButton* startButton = new wxButton(this, wxID_ANY, "Start");
	startButton->SetMinSize(wxSize(-1, btnsHeight));
	mainSizer->Add(startButton, 0, wxALL | wxEXPAND, 5);

	Bind(wxEVT_BUTTON, &MainFrame::onCompress, this, startButton->GetId());


	this->SetSizerAndFit(mainSizer);
}

void MainFrame::OnBrowseCompressFile(wxCommandEvent& event) {
	wxFileDialog openFileDialog(this, _("Open file"), "", "", "All files (*.*)|*.*", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

	if (openFileDialog.ShowModal() == wxID_OK) {
		wxString fileName = openFileDialog.GetPath();
		filePathCtrl->SetValue(fileName);
	}
}

void MainFrame::OnBrowseOutputDir(wxCommandEvent& event) {
	wxDirDialog openDirDialog(this, _("Open directory"), "", wxDD_DEFAULT_STYLE);

	if (openDirDialog.ShowModal() == wxID_OK) {
		wxString dirPath = openDirDialog.GetPath();
		dirPathCtrl->SetValue(dirPath);
	}
}

void MainFrame::onCompress(wxCommandEvent& event) {
	string filePath = string(filePathCtrl->GetValue().mb_str());
	string dirPath = string(dirPathCtrl->GetValue().mb_str());

	string base_filename = filePath.substr(filePath.find_last_of("/\\") + 1);
	string::size_type const p(base_filename.find_last_of('.'));
	string file_without_extension = base_filename.substr(0, p);

	string fileNamePath = dirPath + "\\" + file_without_extension;



	int freqTable[128] = { 0 };
	int size = 0;
	genFreqTable(filePath, freqTable, &size);


	MinHeap* heap = new MinHeap(size);
	bool overflow = false;

	for (int i = 0; i < 128; i++)
		if (freqTable[i])
			heap->insertValues(std::string(1, char(i)), freqTable[i]);


	Node* root = tregen(heap);
	delete heap;

	compress compressor = compress();

	bool validPath = false;
	compressor.createMaps(root);
	string codedText = compressor.compressing(filePath, &validPath);

	if (!validPath) {
		// TODO display warning path not valid
	}

	if (codedText == "") {
		// TODO display warning file content empty
	}

	bool success = saveStringToFile(fileNamePath + string(".com"), codedText);
	saveMapToFile(fileNamePath + string(".json"), compressor.decoder);

	if (!success) {
		// TODO display warning dir path not valid
	}


	//string original = compressor.decompressing(codedtext);
}