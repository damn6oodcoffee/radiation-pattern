#ifndef MYFRAME_HPP
#define MYFRAME_HPP

#include "OGLPane.hpp"
#include "ControlPanel.hpp"

class MainFrame : public wxFrame
{
public:
    MainFrame(const wxString &title);

private:
    OGLPane* glPane;
    OGLPane* glPane1;
    ControlPanel* controlPanel;
};

#endif