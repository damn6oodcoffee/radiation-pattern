#ifndef CONTROLPANEL_HPP
#define CONTROLPANEL_HPP

#include <memory>
#include <wx/wx.h>
#include "Antenna.hpp"
#include "surface.hpp"
#include "GridPanel.hpp"

class ControlPanel: public wxPanel {
public:
    ControlPanel(wxWindow *parent, wxWindowID winid = wxID_ANY,
                 const wxPoint &pos = wxDefaultPosition,
                 const wxSize &size = wxDefaultSize);
    void computeRadiationPattern();
    Surface3D getPolarSurface();
    Surface3D getUVSurface();
private:
    GridPanel* gridPanel;
    wxTextCtrl* gridSizeCtrl;
    wxTextCtrl* intervalCtrl;
    wxTextCtrl* wavelengthCtrl;
    wxTextCtrl* distanceCtrl;
    wxButton* applyBtn;
    wxButton* computeBtn;
    int azPointCount;
    int thPointCount;

    AntennaGrid antennaGrid;
    std::vector<SphericalPoint> upperHemispherePattern;
    Surface3D surface;

    std::vector<double> getPolarVertices();
    std::vector<double> getUVVertices();
};




#endif