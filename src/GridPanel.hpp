#ifndef GRIDPANEL_HPP
#define GRIDPANEL_HPP

#include <wx/wx.h>
#include <wx/grid.h>


class GridPanel: public wxPanel {
public:
    GridPanel(wxWindow *parent, 
              wxWindowID id=wxID_ANY, 
              const wxPoint &pos=wxDefaultPosition, 
              const wxSize &size=wxDefaultSize);
    void OnSize(wxSizeEvent& event);
private:
    wxGrid* grid;
    wxDECLARE_EVENT_TABLE();
};

class CellRenderer: public wxGridCellBoolRenderer {
public:
    void Draw(wxGrid &grid, wxGridCellAttr &attr, wxDC &dc, 
         const wxRect &rect, int row, int col, bool isSelected) override;
};

#endif
