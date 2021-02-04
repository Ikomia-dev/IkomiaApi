#ifndef RAYCASTCOLOR_H
#define RAYCASTCOLOR_H
#include "raycastColorTools.h"
#include "raycastColorF2B.h"
#include "raycastColorIso.h"
#include "raycastColorMIP.h"
#include "raycastColorMinIP.h"

#include <string>

std::string kernelRaycastColor = raycastColorTools
+ raycastColorF2B
+ raycastColorIso
+ raycastColorMIP
+ raycastColorMinIP;

#endif // RAYCASTCOLOR_H
