#ifndef MYAPP_HPP
#define MYAPP_HPP

#include "wx\wx.h"
//#include "MyFrame.h"
#include "OGLPane.hpp"
#include "MyFrame.hpp"

class MyApp : public wxApp
{
private:
	virtual bool OnInit();
};

#endif