#ifndef MYFRAME_HPP
#define MYFRAME_HPP

#include "OGLPane.hpp"

class MyFrame : public wxFrame
{
public:
    MyFrame(const wxString &title);

private:
    OGLPane* glPane{ nullptr };
};

#endif