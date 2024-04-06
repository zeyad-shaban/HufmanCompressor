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


MainFrame::MainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title) {
	//wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
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

	wxString firstFileName = fileNames[0];
}
