
#include <wx/splitter.h>
#include "MainFrame.hpp"

MainFrame::MainFrame(const wxString &title)
    : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxDefaultSize)
{

    auto splitter = new wxSplitterWindow(this, wxID_ANY, wxDefaultPosition,
                                         wxDefaultSize, wxSP_BORDER | wxSP_LIVE_UPDATE);

    auto horSplitter = new wxSplitterWindow(splitter, wxID_ANY, wxDefaultPosition,
                                             wxDefaultSize, wxSP_BORDER | wxSP_LIVE_UPDATE);
    controlPanel = new ControlPanel(splitter);
    wxGLAttributes vAttrs;
    vAttrs.PlatformDefaults().Defaults().EndList();
    if (wxGLCanvas::IsDisplaySupported(vAttrs)) {
        glPane1 = new OGLPane(horSplitter, vAttrs, nullptr, false);
        glPane = new OGLPane(horSplitter, vAttrs, glPane1->getContext());
        //glPane = new OGLPane(horSplitter, vAttrs, nullptr);
        //glPane1 = new OGLPane(horSplitter, vAttrs, nullptr);
        //glPane1->plotScene = std::unique_ptr<PlotScene>(glPane->plotScene.get());
        glPane1->setUpOpenGL();
        glPane1->SetCurrent(*glPane1->getContext());
        glPane1->plotScene = std::make_unique<PlotScene>();
        //glPane->SetMinSize(FromDIP(wxSize(640, 480)));
        glPane->id = 0;
        glPane1->id = 999;
    }


    controlPanel->computeRadiationPattern();
    glPane->plot(controlPanel->getUVSurface(), viridisColormap, defaultColoringRule);
    glPane1->plot(controlPanel->getUVSurface(), viridisColormap, defaultColoringRule);
    
    splitter->SetMinimumPaneSize(256);
    splitter->SplitVertically(controlPanel, horSplitter);
    horSplitter->SetMinimumPaneSize(256);
    horSplitter->SplitHorizontally(glPane, glPane1);

    SetMinSize(wxSize(640, 480));
}