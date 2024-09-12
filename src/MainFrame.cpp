
#include <wx/splitter.h>
#include "MainFrame.hpp"

MainFrame::MainFrame(const wxString &title)
    : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxDefaultSize)
{

    auto splitter = new wxSplitterWindow(this, wxID_ANY, wxDefaultPosition,
                                         wxDefaultSize, wxSP_BORDER | wxSP_LIVE_UPDATE);
    controlPanel = new ControlPanel(splitter);
    wxGLAttributes vAttrs;
    vAttrs.PlatformDefaults().Defaults().EndList();
    if (wxGLCanvas::IsDisplaySupported(vAttrs)) {
        glPane = new OGLPane(splitter, vAttrs);
        //glPane->SetMinSize(FromDIP(wxSize(640, 480)));
    }


    controlPanel->computeRadiationPattern();
    glPane->plot(controlPanel->getUVSurface(), viridisColormap, defaultColoringRule);
    //glPane1->plot(controlPanel->getUVSurface(), viridisColormap, defaultColoringRule);
    
    splitter->SetMinimumPaneSize(256);
    splitter->SplitVertically(controlPanel, glPane);

    SetMinSize(wxSize(640, 480));
}