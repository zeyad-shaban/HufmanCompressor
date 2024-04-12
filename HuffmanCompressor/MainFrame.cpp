#pragma once

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

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
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	wxStaticText* titleText = new wxStaticText(this, wxID_ANY, "3A-Z Compressor");
	sizer->Add(titleText, 0, wxALL | wxCENTER, 5);

	wxButton* huffmanButton = new wxButton(this, wxID_ANY, "Huffman");
	wxButton* testButton = new wxButton(this, wxID_ANY, "Test");
	sizer->Add(huffmanButton, 0, wxALL, 5);
	sizer->Add(testButton, 0, wxALL, 5);



	dropTarget = new wxTextCtrl(this, wxID_ANY, _("Drop files onto me!"), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY);
	dropTarget->DragAcceptFiles(true);
	sizer->Add(dropTarget, 1, wxEXPAND, 0);

	SetSizer(sizer);
	Layout();
	Centre();

	dropTarget->Connect(wxEVT_DROP_FILES, wxDropFilesEventHandler(MainFrame::OnDropFiles), NULL, this);


	wxButton* submintBtn = new wxButton(this, wxID_ANY, "Submit");
	sizer->Add(submintBtn, 0, wxALL, 5);
	submintBtn->Bind(wxEVT_BUTTON, &MainFrame::onFileSubmit, this);


	SetSizer(sizer);
	this->Centre();
}

void MainFrame::OnDropFiles(wxDropFilesEvent& event) {
	if (event.GetNumberOfFiles() > 0) {

		wxString* dropped = event.GetFiles();
		wxASSERT(dropped);

		wxBusyCursor busyCursor;
		wxWindowDisabler disabler;
		wxBusyInfo busyInfo(_("Adding files, wait please..."));

		wxString name;
		wxArrayString files;

		for (int i = 0; i < event.GetNumberOfFiles(); i++) {
			name = dropped[i];
			if (wxFileExists(name))
				files.push_back(name);
			else if (wxDirExists(name))
				wxDir::GetAllFiles(name, &files);
		}

		wxTextCtrl* textCtrl = dynamic_cast<wxTextCtrl*>(event.GetEventObject());
		wxASSERT(textCtrl);
		textCtrl->Clear();
		for (size_t i = 0; i < files.size(); i++) {
			*textCtrl << files[i] << wxT('\n');
		}
	}
}

void MainFrame::onFileSubmit(wxCommandEvent& event) {
	wxString fileName = dropTarget->GetValue();

	wxArrayString fileNames = wxStringTokenize(fileName, wxT("\n"));

	if (fileNames.IsEmpty()) return;

	std::string filePath = std::string(fileNames[0].mb_str());

	std::string base_filename = filePath.substr(filePath.find_last_of("/\\") + 1);
	std::string::size_type const p(base_filename.find_last_of('.'));
	std::string file_without_extension = base_filename.substr(0, p);

	// TOOD DISPLAY A LITTLE LOAD BAR

	int freqTable[128] = { 0 };
	int size = 0;
	genFreqTable(filePath, freqTable, &size);


	MinHeap* heap = new MinHeap(size);
	bool overflow = false;

	for (int i = 0; i < 128; i++)
		if (freqTable[i])
			heap->insertValues(std::string(1, char(i)), freqTable[i]);


	Node* root = tregen(heap);
	// TODO DISPLAY THE TREE
	delete heap;

	compress compressor = compress();

	compressor.createMaps(root, "");
	// TODO DISPALY THE MAP CREATED
	string codedtext = compressor.compressing(filePath, file_without_extension);
	string original = compressor.decompressing(codedtext, file_without_extension);

	std::cout << "the program has ended" << std::endl;
}
