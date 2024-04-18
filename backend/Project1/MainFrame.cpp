#include "MainFrame.h"


void MainFrame::onCompress(wxCommandEvent& event) {
	string filePath = string(filePathCtrl->GetValue().mb_str());
	string dirPath = string(dirPathCtrl->GetValue().mb_str());

	unordered_map<string, string> encoder;
	Node* root = NULL;
	string textPrev;

	startCompressing(filePath, dirPath, &encoder, root, &textPrev);

	InfoFrame* infoFrame = new InfoFrame("Info", this, encoder, root, textPrev, true);
	this->Show(false);
	infoFrame->Show(true);
	infoFrame->SetSize(this->GetSize());
	infoFrame->Move(this->GetPosition());
}

void MainFrame::onDecompress(wxCommandEvent& event) {
	//string compressedFilePath = string(filePathCtrl->GetValue().mb_str());
	//string dirPath = string(dirPathCtrl->GetValue().mb_str());
	//string decoderPath = string(decoderPathCtrl->GetValue().mb_str());

	//string base_filename = compressedFilePath.substr(compressedFilePath.find_last_of("/\\") + 1);
	//string::size_type const p(base_filename.find_last_of('.'));
	//string file_without_extension = base_filename.substr(0, p);

	//string fileNamePath = dirPath + "\\" + file_without_extension;


	//compress* compressor = new compress();
	//ifstream decoderFile(decoderPath);

	//if (!decoderFile.is_open()) {
	//	// TODO HANDLE INVALID DECODER PATH
	//}

	//nlohmann::json jsonDecoder = nlohmann::json::parse(decoderFile);
	//for (auto& element : jsonDecoder.items())
	//	compressor->decoder[element.key()] = element.value();


	//bool validPath = false;
	//string decodedTextPrev = compressor->decompressing(compressedFilePath, fileNamePath + "_decompressed.txt", &validPath);

	//if (!validPath) {
	//	// TODO HANDLE INVALID PATH HERE
	//}

	//Node* root = nullptr;
	//InfoFrame* infoFrame = new InfoFrame("Info", this, compressor->decoder, root, decodedTextPrev, false);
	//this->Show(false);
	//infoFrame->Show(true);
	//infoFrame->SetSize(this->GetSize());
	//infoFrame->Move(this->GetPosition());
}
void MainFrame::onBrowseCompressFile(wxCommandEvent& event) {
	wxFileDialog openFileDialog(this, _("Open file"), "", "", "All files (*.*)|*.*", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

	if (openFileDialog.ShowModal() == wxID_OK)
		filePathCtrl->SetValue(openFileDialog.GetPath());
}
void MainFrame::onBrowseOutputDir(wxCommandEvent& event) {
	wxDirDialog openDirDialog(this, _("Open directory"), "", wxDD_DEFAULT_STYLE);

	if (openDirDialog.ShowModal() == wxID_OK)
		dirPathCtrl->SetValue(openDirDialog.GetPath());
}
void MainFrame::onBrowseDecoderFile(wxCommandEvent& event) {
	wxFileDialog openFileDialog(this, _("Open file"), "", "", "JSON files (*.json)|*.json", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

	if (openFileDialog.ShowModal() == wxID_OK)
		decoderPathCtrl->SetValue(openFileDialog.GetPath());
}
void MainFrame::onToggleBtn(wxCommandEvent& event) {
	wxToggleButton* toggleButton = (wxToggleButton*)event.GetEventObject();

	if (toggleButton->GetValue()) {
		toggleButton->SetLabel("Decompress");
		fileLabel->SetLabel("File to Decompress");

		decoderLabel->Show();
		decoderPathCtrl->Show();
		decoderBrowseBtn->Show();
		toggleButton->SetBackgroundColour(wxColour(255, 100, 100));

		startBtn->SetLabel("Start Decompressing");
		startBtn->Unbind(wxEVT_BUTTON, &MainFrame::onCompress, this);
		Bind(wxEVT_BUTTON, &MainFrame::onDecompress, this, startBtn->GetId());
	}
	else {
		toggleButton->SetLabel("Compress");
		fileLabel->SetLabel("File to Compress");

		decoderLabel->Hide();
		decoderPathCtrl->Hide();
		decoderBrowseBtn->Hide();
		toggleButton->SetBackgroundColour(wxColour(145, 196, 131));

		startBtn->SetLabel("Start Compressing");
		startBtn->Unbind(wxEVT_BUTTON, &MainFrame::onDecompress, this);
		Bind(wxEVT_BUTTON, &MainFrame::onCompress, this, startBtn->GetId());
	}

	this->Layout();
}


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

	// ========================== PAGE HEADER ============================

	wxToggleButton* toggleButton = new wxToggleButton(this, wxID_ANY, "Compress");
	toggleButton->SetMinSize(wxSize(-1, btnsHeight));
	toggleButton->SetBackgroundColour(wxColour(145, 196, 131));
	mainSizer->Add(toggleButton, 0, wxALL | wxEXPAND, 5);

	Bind(wxEVT_TOGGLEBUTTON, &MainFrame::onToggleBtn, this, toggleButton->GetId());


	wxStaticText* titleText = new wxStaticText(this, wxID_ANY, "3A-Z Compressor");
	titleText->SetFont(titleFont);
	titleText->SetBackgroundColour(wxColour(245, 245, 245));

	mainSizer->Add(titleText, 0, wxALL | wxEXPAND, 5);

	// ========================  FILE AREA  ===========================
	fileLabel = new wxStaticText(this, wxID_ANY, "File to Compress");
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

	Bind(wxEVT_BUTTON, &MainFrame::onBrowseCompressFile, this, fileBrowseBtn->GetId());

	// ========================  DECODER FILE AREA  ===========================

	decoderLabel = new wxStaticText(this, wxID_ANY, "Decoder File");
	decoderLabel->SetFont(labelFont);
	mainSizer->Add(decoderLabel, 0, wxALL | wxLEFT, 5);

	decoderPathCtrl = new wxTextCtrl(this, wxID_ANY);
	decoderBrowseBtn = new wxButton(this, wxID_ANY, "Browse");
	decoderPathCtrl->SetMinSize(wxSize(-1, btnsHeight));
	decoderBrowseBtn->SetMinSize(wxSize(-1, btnsHeight));

	wxBoxSizer* hboxDecompressFile = new wxBoxSizer(wxHORIZONTAL);
	hboxDecompressFile->Add(decoderPathCtrl, 1, wxALL | wxEXPAND, 5);
	hboxDecompressFile->Add(decoderBrowseBtn, 0, wxALL, 5);

	mainSizer->Add(hboxDecompressFile, 0, wxEXPAND);


	decoderLabel->Hide();
	decoderPathCtrl->Hide();
	decoderBrowseBtn->Hide();

	Bind(wxEVT_BUTTON, &MainFrame::onBrowseDecoderFile, this, decoderBrowseBtn->GetId());


	mainSizer->AddStretchSpacer();

	// ========================  FOLDER AREA  ===========================

	wxStaticText* outDirLabel = new wxStaticText(this, wxID_ANY, "Output Directory");
	outDirLabel->SetFont(labelFont);
	mainSizer->Add(outDirLabel, 0, wxALL | wxLEFT, 5);

	dirPathCtrl = new wxTextCtrl(this, wxID_ANY);
	wxButton* dirBrowseBtn = new wxButton(this, wxID_ANY, "Browse");
	dirPathCtrl->SetMinSize(wxSize(-1, btnsHeight));
	dirBrowseBtn->SetMinSize(wxSize(-1, btnsHeight));

	wxBoxSizer* hboxDir = new wxBoxSizer(wxHORIZONTAL);
	hboxDir->Add(dirPathCtrl, 1, wxALL | wxEXPAND, 5);
	hboxDir->Add(dirBrowseBtn, 0, wxALL, 5);

	mainSizer->Add(hboxDir, 0, wxEXPAND);

	Bind(wxEVT_BUTTON, &MainFrame::onBrowseOutputDir, this, dirBrowseBtn->GetId());

	// ==================  START AREA  =========================

	startBtn = new wxButton(this, wxID_ANY, "Start Compressing");
	startBtn->SetMinSize(wxSize(-1, btnsHeight));
	mainSizer->Add(startBtn, 0, wxALL | wxEXPAND, 5);

	Bind(wxEVT_BUTTON, &MainFrame::onCompress, this, startBtn->GetId());


	this->SetSizerAndFit(mainSizer);
}