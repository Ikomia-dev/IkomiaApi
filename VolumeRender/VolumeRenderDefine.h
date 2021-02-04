#ifndef VOLUMERENDERDEFINE_H
#define VOLUMERENDERDEFINE_H

//----------------------------------------------------------------------------------//
//- We place enum here instead of scopped enum in CVolumeRender because of         -//
//- multiple definitions problem if we include VolumeRender.h in several cpp unit. -//
//- This is due to CL/cl2.hpp and MinGW but we don't know the root cause...        -//
//----------------------------------------------------------------------------------//
enum class RenderError : short
{
    SUCCESS,
    INIT_FAILED
};

enum class RenderParamType : size_t
{
    DENSITY,
    BRIGHTNESS,
    OFFSET,
    SCALE,
    CHR_MIN,
    CHR_MAX,
    CHG_MIN,
    CHG_MAX,
    CHB_MIN,
    CHB_MAX,
    ISOVALUE_R,
    ISOVALUE_G,
    ISOVALUE_B,
    ISOVALUE_A
};

enum class RenderMode : size_t
{
    RAYCAST,
    MIP,
    MinIP,
    ISOSURFACE
};

enum class RenderColormap : size_t
{
    CLASSIC,
    SKIN,
    JET,
    GRAYCOLOR
};

#endif // VOLUMERENDERDEFINE_H
