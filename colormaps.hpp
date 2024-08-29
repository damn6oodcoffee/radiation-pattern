#ifndef COLORMAPS_HPP
#define COLORMAPS_HPP

#include "colormap_data.hpp"

using ColormapFunc = std::function<RGB(double)>;

inline double keepNormalized(double value) { 
    return std::clamp(value, 0.0, 1.0);
}

inline int getColormapIndex(double value) {
    value = keepNormalized(value);
    return static_cast<int>(255*value + 0.5);
}

inline RGB defaultColormap(double value) {
    value = keepNormalized(value);
    return {
        static_cast<float>(value), 
        static_cast<float>(value), 
        static_cast<float>(value),
    };
}

inline RGB plasmaColormap(double value) {
    return ColormapData::plasmaData[getColormapIndex(value)];
}

inline RGB viridisColormap(double value) {
    return ColormapData::viridisData[getColormapIndex(value)];
}

inline RGB infernoColormap(double value) {
    return ColormapData::infernoData[getColormapIndex(value)];
}

inline RGB turboColormap(double value) {
    return ColormapData::turboData[getColormapIndex(value)];
}


#endif
