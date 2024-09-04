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
    ControlPanel* controlPanel;
};

#endif