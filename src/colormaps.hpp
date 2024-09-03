#ifndef COLORMAPS_HPP
#define COLORMAPS_HPP

#include <functional>
#include "colormap_data.hpp"

using ColormapFunc = std::function<Colormap::rgb(double)>;

inline double keepNormalized(double value) { 
    return std::clamp(value, 0.0, 1.0);
}

inline int getColormapIndex(double value) {
    value = keepNormalized(value);
    return static_cast<int>(255*value + 0.5);
}

inline Colormap::rgb defaultColormap(double value) {
    value = keepNormalized(value);
    return {
        static_cast<float>(value), 
        static_cast<float>(value), 
        static_cast<float>(value),
    };
}

inline Colormap::rgb plasmaColormap(double value) {
    return ColormapData::plasmaData[getColormapIndex(value)];
}

inline Colormap::rgb viridisColormap(double value) {
    return ColormapData::viridisData[getColormapIndex(value)];
}

inline Colormap::rgb infernoColormap(double value) {
    return ColormapData::infernoData[getColormapIndex(value)];
}

inline Colormap::rgb turboColormap(double value) {
    return ColormapData::turboData[getColormapIndex(value)];
}


#endif
