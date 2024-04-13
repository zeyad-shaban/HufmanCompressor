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

	Bind(wxEVT_BUTTON, &MainFrame::OnStart, this, startButton->GetId());


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

void MainFrame::OnStart(wxCommandEvent& event) {
	printf("yo");
}





// void MainFrame::OnDropFiles(wxDropFilesEvent& event) {
// 	if (event.GetNumberOfFiles() > 0) {
// 
// 		wxString* dropped = event.GetFiles();
// 		wxASSERT(dropped);
// 
// 		wxBusyCursor busyCursor;
// 		wxWindowDisabler disabler;
// 		wxBusyInfo busyInfo(_("Adding files, wait please..."));
// 
// 		wxString name;
// 		wxArrayString files;
// 
// 		for (int i = 0; i < event.GetNumberOfFiles(); i++) {
// 			name = dropped[i];
// 			if (wxFileExists(name))
// 				files.push_back(name);
// 			else if (wxDirExists(name))
// 				wxDir::GetAllFiles(name, &files);
// 		}
// 
// 		wxTextCtrl* textCtrl = dynamic_cast<wxTextCtrl*>(event.GetEventObject());
// 		wxASSERT(textCtrl);
// 		textCtrl->Clear();
// 		for (size_t i = 0; i < files.size(); i++) {
// 			*textCtrl << files[i] << wxT('\n');
// 		}
// 	}
// }
// 
// void MainFrame::onFileSubmit(wxCommandEvent& event) {
// 	wxString fileName = dropTarget->GetValue();
// 
// 	wxArrayString fileNames = wxStringTokenize(fileName, wxT("\n"));
// 
// 	if (fileNames.IsEmpty()) return;
// 
// 	std::string filePath = std::string(fileNames[0].mb_str());
// 
// 	std::string base_filename = filePath.substr(filePath.find_last_of("/\\") + 1);
// 	std::string::size_type const p(base_filename.find_last_of('.'));
// 	std::string file_without_extension = base_filename.substr(0, p);
// 
// 	// TOOD DISPLAY A LITTLE LOAD BAR
// 
// 	int freqTable[128] = { 0 };
// 	int size = 0;
// 	genFreqTable(filePath, freqTable, &size);
// 
// 
// 	MinHeap* heap = new MinHeap(size);
// 	bool overflow = false;
// 
// 	for (int i = 0; i < 128; i++)
// 		if (freqTable[i])
// 			heap->insertValues(std::string(1, char(i)), freqTable[i]);
// 
// 
// 	Node* root = tregen(heap);
// 	// TODO DISPLAY THE TREE
// 	delete heap;
// 
// 	compress compressor = compress();
// 
// 	compressor.createMaps(root, "");
// 	// TODO DISPALY THE MAP CREATED
// 	string codedtext = compressor.compressing(filePath, file_without_extension);
// 	string original = compressor.decompressing(codedtext, file_without_extension);
// 
// 	std::cout << "the program has ended" << std::endl;
// }
// 