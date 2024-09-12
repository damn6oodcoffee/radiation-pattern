
#include <numbers>
#include "ControlPanel.hpp"

ControlPanel::ControlPanel(wxWindow *parent, wxWindowID winid,
                           const wxPoint &pos, const wxSize &size)
    : wxPanel(parent, winid, pos, size)
    , azPointCount{256}
    , thPointCount{256}
    , antennaGrid(10, 2, 2, 100)
{
    antennaGrid.toggleCell(5,4);
    antennaGrid.toggleCell(5,5);
    antennaGrid.toggleCell(5,3);
    antennaGrid.toggleCell(5,6);

    auto vertSizer = new wxBoxSizer(wxVERTICAL);

    auto sbGrid = new wxStaticBoxSizer(wxVERTICAL, this, "Array");
    sbGrid->SetMinSize(256, 256);
    gridPanel = new GridPanel(sbGrid->GetStaticBox());
    gridPanel->SetSize(wxSize(100,100));
    gridPanel->SetBackgroundColour(wxColour(0xff0000));
    sbGrid->Add(gridPanel, 1, wxSHAPED | wxALIGN_CENTER | wxALL, FromDIP(10));

    auto sbGridParams = new wxStaticBoxSizer(wxVERTICAL, this, "Array Size");
    wxTextCtrl* gridSizeTextCtrl = new wxTextCtrl(sbGridParams->GetStaticBox(), wxID_ANY);
    wxButton* newSizeButton= new wxButton(sbGridParams->GetStaticBox(), wxID_ANY, "Apply New Size");
    wxGridSizer* gridParamsSizer = new wxGridSizer(1, 3, 0, 0);
    //gridParamsSizer->Add(new wxStaticText(sbGridParams->GetStaticBox(), wxID_ANY, "Size:"), 1, wxALL | wxALIGN_BOTTOM | wxALIGN_LEFT);
    gridParamsSizer->Add(gridSizeTextCtrl, 0, wxRIGHT, FromDIP(5));
    gridParamsSizer->Add(newSizeButton, 0, wxALIGN_LEFT | wxLEFT, FromDIP(5));
    gridParamsSizer->AddStretchSpacer(1);
    sbGridParams->Add(gridParamsSizer, 1, wxEXPAND | wxLEFT | wxRIGHT, FromDIP(10));

    auto sbAntennaParams = new wxStaticBoxSizer(wxVERTICAL, this, "Antenna Array Parameters");
    wxTextCtrl* wavelengthTextCtrl = new wxTextCtrl(sbAntennaParams->GetStaticBox(), wxID_ANY);
    wxTextCtrl* intervalTextCtrl = new wxTextCtrl(sbAntennaParams->GetStaticBox(), wxID_ANY);
    wxTextCtrl* distanceTextCtrl = new wxTextCtrl(sbAntennaParams->GetStaticBox(), wxID_ANY);
    wxButton* plotButton = new wxButton(sbAntennaParams->GetStaticBox(), wxID_ANY, "Plot");
    wxGridSizer* antennaParamsSizer = new wxGridSizer(4, 3, 0, 0);
    antennaParamsSizer->Add(new wxStaticText(sbAntennaParams->GetStaticBox(), wxID_ANY, "Wavelength:"), 0, wxALL | wxALIGN_BOTTOM | wxALIGN_LEFT);
    antennaParamsSizer->Add(new wxStaticText(sbAntennaParams->GetStaticBox(), wxID_ANY, "Interval:"), 0, wxALL | wxALIGN_BOTTOM | wxALIGN_LEFT | wxLEFT, FromDIP(5));
    antennaParamsSizer->AddStretchSpacer(1);
    antennaParamsSizer->Add(wavelengthTextCtrl, 0, wxRIGHT, FromDIP(5));
    antennaParamsSizer->Add(intervalTextCtrl, 0, wxLEFT, FromDIP(5));
    antennaParamsSizer->AddStretchSpacer(1);
    antennaParamsSizer->Add(new wxStaticText(sbAntennaParams->GetStaticBox(), wxID_ANY, "Distance:"), 0, wxALL | wxALIGN_BOTTOM | wxALIGN_LEFT);
    antennaParamsSizer->AddStretchSpacer(1);
    antennaParamsSizer->AddStretchSpacer(1);
    antennaParamsSizer->Add(distanceTextCtrl, 0, wxRIGHT, FromDIP(5));
    antennaParamsSizer->Add(plotButton, 0,  wxLEFT, FromDIP(5));
    antennaParamsSizer->AddStretchSpacer(1);
    sbAntennaParams->Add(antennaParamsSizer, 1, wxEXPAND | wxLEFT | wxRIGHT, FromDIP(10));

    auto sbPlotParams = new wxStaticBoxSizer(wxVERTICAL, this, "Plot Options");
    auto plotParamsSizer = new wxBoxSizer(wxHORIZONTAL);
    auto colormapCombo = new wxComboBox(sbPlotParams->GetStaticBox(), wxID_ANY, "colormapCombo");
    auto plotTypeCombo = new wxComboBox(sbPlotParams->GetStaticBox(), wxID_ANY, "plotTypeCombo");
    colormapCombo->Insert("turbo", 0);
    colormapCombo->Insert("default", 1);
    plotParamsSizer->Add(colormapCombo, 0, wxEXPAND | wxRIGHT, FromDIP(5));
    plotParamsSizer->Add(plotTypeCombo, 0, wxEXPAND | wxLEFT, FromDIP(5));
    plotParamsSizer->AddStretchSpacer(1);
    sbPlotParams->Add(plotParamsSizer, 1, wxEXPAND | wxLEFT | wxRIGHT, FromDIP(10));

    // auto dummy = new wxPanel(sbGrid->GetStaticBox());
    // dummy->SetBackgroundColour(wxColour(0xff00ff));
    // sbGrid->Add(dummy, 1, wxSHAPED | wxALIGN_CENTER | wxALL, FromDIP(10));
    
    vertSizer->Add(sbGrid, 2, wxEXPAND | wxALL, FromDIP(5));
    vertSizer->Add(sbGridParams, 0, wxEXPAND | wxALL, FromDIP(5));
    vertSizer->Add(sbAntennaParams, 0, wxEXPAND | wxALL, FromDIP(5));
    vertSizer->Add(sbPlotParams, 0, wxEXPAND | wxALL, FromDIP(5));
    vertSizer->AddSpacer(32);

    SetSizerAndFit(vertSizer);
    
}

void ControlPanel::computeRadiationPattern() {
    auto radPattern = antennaGrid.computeRadiationPattern(azPointCount, 2 * thPointCount);
    std::vector<SphericalPoint> newUpperHemispherePattern{};
    std::copy_if(radPattern.begin(), radPattern.end(), std::back_inserter(newUpperHemispherePattern), [](auto&& elt){
        return elt.theta <= std::numbers::pi / 2;
    });
    upperHemispherePattern = std::move(newUpperHemispherePattern);
}

Surface3D ControlPanel::getPolarSurface() {
    surface.setVertices(getPolarVertices(), 
        { static_cast<unsigned int>(azPointCount),
          static_cast<unsigned int>(thPointCount) });
    return surface;
}

Surface3D ControlPanel::getUVSurface() {
    surface.setVertices(getUVVertices(), 
        { static_cast<unsigned int>(azPointCount),
          static_cast<unsigned int>(thPointCount) });
    return surface;
}

std::vector<double> ControlPanel::getPolarVertices() {
    std::vector<double> vertices{};
    for (auto&& sphPoint: upperHemispherePattern) {
        auto cartPoint{sphericalToCartesian(sphPoint)};
        vertices.insert(vertices.end(), {cartPoint.x, cartPoint.y, cartPoint.z});
    }
    return vertices;
}

std::vector<double> ControlPanel::getUVVertices() {
    std::vector<double> vertices{};
    for (auto&& sphPoint: upperHemispherePattern) {
        auto uvPoint{ sphericalToUV(sphPoint) };
        vertices.insert(vertices.end(), { uvPoint.u, uvPoint.v, uvPoint.z });
    }
    return vertices;
}

