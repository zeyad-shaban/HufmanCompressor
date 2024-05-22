//#pragma once
//#include <wx/wx.h>
//#include <wx/tglbtn.h>
//#include "StartCompressing.h"
//#include "InfoFrame.h"
//#include <unordered_map>
//#include "StartDecompressing.h"
//
//
//class MainFrame : public wxFrame {
//private:
//	wxButton* startBtn;
//	wxTextCtrl* filePathCtrl;
//	wxTextCtrl* dirPathCtrl;
//	wxTextCtrl*  orderInputCtrl;
//	int order = 1;
//
//	wxTextCtrl* decoderPathCtrl;
//	wxButton* decoderBrowseBtn;
//	wxStaticText* decoderLabel;
//	wxStaticText* fileLabel;
//	wxStaticText* orderLabel;
//
//	void onToggleBtn(wxCommandEvent& event);
//
//
//	void onCompress(wxCommandEvent& event);
//	void onDecompress(wxCommandEvent& event);
//	void onBrowseCompressFile(wxCommandEvent& event);
//	void onBrowseOutputDir(wxCommandEvent& event);
//	void onBrowseDecoderFile(wxCommandEvent& event);
//
//public:
//	MainFrame(const wxString& title);
//};