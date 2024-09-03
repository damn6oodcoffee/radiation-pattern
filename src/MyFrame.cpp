
#include "MyFrame.hpp"

MyFrame::MyFrame(const wxString &title)
    : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxDefaultSize)
{
    auto sizer = new wxBoxSizer(wxVERTICAL);
    wxGLAttributes vAttrs;
    vAttrs.PlatformDefaults().Defaults().EndList();
    if (wxGLCanvas::IsDisplaySupported(vAttrs)) {
        glPane = new OGLPane(this, vAttrs);
        //glPane->SetMinSize(FromDIP(wxSize(640, 480)));
        sizer->Add(glPane, 1, wxEXPAND | wxALL);
    }
    SetSizerAndFit(sizer);
}