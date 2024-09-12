#include "wx/graphics.h"
#include "../marked.xpm"
#include "GridPanel.hpp"

wxBEGIN_EVENT_TABLE(GridPanel, wxPanel)
    EVT_SIZE(GridPanel::OnSize)
wxEND_EVENT_TABLE()

GridPanel::GridPanel(wxWindow *parent, wxWindowID id, 
                     const wxPoint &pos, const wxSize &size)
    : wxPanel(parent, id, pos, size)
{
    auto sizer = new wxBoxSizer(wxHORIZONTAL);
    grid = new wxGrid(this, wxID_ANY);
    grid->CreateGrid(10, 10);
    // grid->SetSelectionMode(wxGrid::wxGridSelectionModes::wxGridSelectNone);

    for (int i{0}, size{grid->GetNumberCols()}; i < size; i++)
        grid->SetColFormatBool(i);

    for (int i{0}, size{grid->GetNumberRows()}; i < size; ++i)
        for (int j{0}, size{grid->GetNumberCols()}; j < size; ++j)
            grid->SetCellRenderer(i, j, new CellRenderer());

    grid->HideColLabels();
    grid->HideRowLabels();
    sizer->Add(grid, 1, wxEXPAND);
    // wxPanel* panel = new wxPanel(this);
    // panel->SetBackgroundColour(wxColour(0x00ff00));
    // sizer->Add(panel);
    SetSizer(sizer);
   
}

void GridPanel::OnSize(wxSizeEvent& event) {
    auto [width, height] = event.GetSize();

    grid->SetRowMinimalAcceptableHeight(height / grid->GetNumberRows());
    for (int i{0}, size{grid->GetNumberRows()}; i < size; ++i) {
        grid->SetRowSize(i, height / size);
        grid->SetRowMinimalHeight(i, height / size);
    }
    grid->SetColMinimalAcceptableWidth(width / grid->GetNumberCols());
    for (int i{0}, size{grid->GetNumberCols()}; i < size; ++i) {
        grid->SetColSize(i, width / size);
        grid->SetColMinimalWidth(i, width / size);
    }
    grid->SetMaxSize(event.GetSize());
    grid->SetMinSize(wxSize(1,1));
    event.Skip();
}


// wxImage* pimg = nullptr;
void CellRenderer::Draw(wxGrid &grid, wxGridCellAttr &attr, wxDC &dc, 
                        const wxRect &rect, int row, int col, bool isSelected) 
{   
   
    
    // if (!pimg)
    //     pimg = new wxImage("K:/wxgrid_test/marked.png");
    // dc.SetPen(*wxBLACK_PEN);
    dc.SetBrush(*wxWHITE_BRUSH);
    dc.DrawRectangle(rect);
    // dc.SetPen(*wxRED_PEN);
    // dc.DrawEllipse(rect);
    //attr.Get
    #if 1
    if (wxGridCellBoolEditor::IsTrueValue(grid.GetCellValue(row, col))) {
        int cellWidth{rect.GetWidth()};
        int cellHeight{rect.GetHeight()};
        // wxBitmap bmp(pimg->Scale(cellWidth ? cellWidth : 1, cellHeight ? cellHeight : 1));
        // static wxBitmap bmp(_6b6b376298c47d1eca8aba187699eae5stDtWUdmQODtuKv);
        // wxBitmap::Rescale(bmp, rect.GetSize());
        static auto img = wxImage(_6b6b376298c47d1eca8aba187699eae5stDtWUdmQODtuKv);
        auto resizedImg = img.Scale(cellWidth, cellHeight);
        wxBitmap bmp(resizedImg);
        dc.DrawBitmap(bmp, rect.GetTopLeft());
    }
    #endif
    
}