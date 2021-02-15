/*
 * Copyright (C) 2021 Ikomia SAS
 * Contact: https://www.ikomia.com
 *
 * This file is part of the Ikomia API libraries.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef RAYCASTCOLORF2B_H
#define RAYCASTCOLORF2B_H

#include <string>

std::string raycastColorF2B = R"(
/////////////////////////////////////////////////////////////////////////
// Front to Back Raycasting rendering for color image              //
/////////////////////////////////////////////////////////////////////////
__kernel void renderColor_F2B(
        __global uint *d_output,
         __constant float* invViewMatrix,
          __read_only image3d_t volume,
          __read_only image2d_t transferFunc,
          sampler_t volumeSampler,
          sampler_t transferFuncSampler,
          __global float* d_depth,
          __constant float* modelViewProjMatrix,
          __constant rayParam* myParam
         )

{
    uint x = get_global_id(0);
    uint y = get_global_id(1);

    // Resize coordinates (x,y) in [-1, 1]x[-1, 1]
    float u =  myParam->K*x  - myParam->ratioX;
    float v =  myParam->K*y  - myParam->ratioY;

    // camEyeRay_d = ray direction in camera space
    float4 rayDir = fast_normalize(((float4)(u, v, -myParam->focalLength, 0.0f)));

    // Bounding box size
    float4 boxMin = (float4)(-alpha, -beta, -delta, 1.0f)*1.1f;
    float4 boxMax = (float4)( alpha,  beta,  delta, 1.0f)*1.1f;

    // calculate eye ray in world space
    // eyeRay_o = Camera Position (Origin)
    float4 eyeRay_o = (float4)(invViewMatrix[3], invViewMatrix[7], invViewMatrix[11], 1.0f);

    // 1/2 * borderLength / tan(FOV/2)

    // eyeRay_d = Current Position (Destination)
    float4 eyeRay_d;
    eyeRay_d.x = dot(rayDir, ((float4)(invViewMatrix[0],invViewMatrix[1],invViewMatrix[2],invViewMatrix[3])));
    eyeRay_d.y = dot(rayDir, ((float4)(invViewMatrix[4],invViewMatrix[5],invViewMatrix[6],invViewMatrix[7])));
    eyeRay_d.z = dot(rayDir, ((float4)(invViewMatrix[8],invViewMatrix[9],invViewMatrix[10],invViewMatrix[11])));
    eyeRay_d.w = 0.0f;

    float tnear;
    float tfar;
    int hit = intersectBox(eyeRay_o, eyeRay_d, boxMin, boxMax, &tnear, &tfar);
    if (!hit)
    {
        if ((x < myParam->wndWidth) && (y < myParam->wndHeight))
        {
            // write output color
            uint i =(y * myParam->wndWidth) + x;
            d_output[i] = 0;
            d_depth[i] = 0;
        }
        return;
    }
    if (tnear < 0.0f)
        tnear = 0.0f;     // clamp to near plane

    // *****************************************************************
    // march along ray from back to front, accumulating color
    // *****************************************************************

    // Trouver valeur de lumi�re invariant dans object space...
    float4 lightPosition[1];
    /*
    float4 worldLight = (float4)(-10.0f,-10.0f,-10.0f,0.0f);
    lightPosition[0].x = dot(worldLight, ((float4)(invViewMatrix[0],invViewMatrix[1],invViewMatrix[2],invViewMatrix[3])));
    lightPosition[0].y = dot(worldLight, ((float4)(invViewMatrix[4],invViewMatrix[5],invViewMatrix[6],invViewMatrix[7])));
    lightPosition[0].z = dot(worldLight, ((float4)(invViewMatrix[8],invViewMatrix[9],invViewMatrix[10],invViewMatrix[11])));
    lightPosition[0].w = 0.0f;

    float4 worldLight2 = (float4)(10.0f,-10.0f,-10.0f,0.0f);
    lightPosition[1].x = dot(worldLight2, ((float4)(invViewMatrix[0],invViewMatrix[1],invViewMatrix[2],invViewMatrix[3])));
    lightPosition[1].y = dot(worldLight2, ((float4)(invViewMatrix[4],invViewMatrix[5],invViewMatrix[6],invViewMatrix[7])));
    lightPosition[1].z = dot(worldLight2, ((float4)(invViewMatrix[8],invViewMatrix[9],invViewMatrix[10],invViewMatrix[11])));
    lightPosition[1].w = 0.0f;

    float4 worldLight3 = (float4)(-10.0f,10.0f,-10.0f,0.0f);
    lightPosition[2].x = dot(worldLight3, ((float4)(invViewMatrix[0],invViewMatrix[1],invViewMatrix[2],invViewMatrix[3])));
    lightPosition[2].y = dot(worldLight3, ((float4)(invViewMatrix[4],invViewMatrix[5],invViewMatrix[6],invViewMatrix[7])));
    lightPosition[2].z = dot(worldLight3, ((float4)(invViewMatrix[8],invViewMatrix[9],invViewMatrix[10],invViewMatrix[11])));
    lightPosition[2].w = 0.0f;

    float4 worldLight4 = (float4)(-10.0f,-10.0f,10.0f,0.0f);
    lightPosition[3].x = dot(worldLight4, ((float4)(invViewMatrix[0],invViewMatrix[1],invViewMatrix[2],invViewMatrix[3])));
    lightPosition[3].y = dot(worldLight4, ((float4)(invViewMatrix[4],invViewMatrix[5],invViewMatrix[6],invViewMatrix[7])));
    lightPosition[3].z = dot(worldLight4, ((float4)(invViewMatrix[8],invViewMatrix[9],invViewMatrix[10],invViewMatrix[11])));
    lightPosition[3].w = 0.0f;
    */
    lightPosition[0] = eyeRay_o;
    lightPosition[0].x = native_divide(lightPosition[0].x, (float)alpha);
    lightPosition[0].y = -native_divide(lightPosition[0].y, (float)beta);
    lightPosition[0].z = -native_divide(lightPosition[0].z, (float)delta);
    lightPosition[0] = lightPosition[0] * 0.5f + 0.5f;

    float4 eyePosition = lightPosition[0];

    float4 temp = (float4)(0.0f,0.0f,0.0f,0.0f);
    float4 transparency = (float4)(0.0f,0.0f,0.0f,0.0f);

      // init ray limits
    float4 rayStart = eyeRay_o + eyeRay_d*tnear;
    float4 rayStop = eyeRay_o + eyeRay_d*tfar;

    // Get depth value from position in projetive space
    float4 depthVec;
    depthVec.x = dot(rayStop, ((float4)(modelViewProjMatrix[0],modelViewProjMatrix[1],modelViewProjMatrix[2],modelViewProjMatrix[3])));
    depthVec.y = dot(rayStop, ((float4)(modelViewProjMatrix[4],modelViewProjMatrix[5],modelViewProjMatrix[6],modelViewProjMatrix[7])));
    depthVec.z = dot(rayStop, ((float4)(modelViewProjMatrix[8],modelViewProjMatrix[9],modelViewProjMatrix[10],modelViewProjMatrix[11])));
    depthVec.w = dot(rayStop, ((float4)(modelViewProjMatrix[12],modelViewProjMatrix[13],modelViewProjMatrix[14],modelViewProjMatrix[15])));

    // transform from object space to coordinate space
    // map x from [-alpha,alpha] to [-1,1]
    rayStart.x = native_divide(rayStart.x, (float)alpha);
    rayStop.x  = native_divide(rayStop.x, (float)alpha);
    // map y from [-beta,beta] to [-1,1]
    rayStart.y = -native_divide(rayStart.y, (float)beta);
    rayStop.y  = -native_divide(rayStop.y, (float)beta);
    // map z from [-delta,delta] to [-1,1]
    rayStart.z = -native_divide(rayStart.z, (float)delta);
    rayStop.z  = -native_divide(rayStop.z, (float)delta);
    // map position from [-1,1]x[-1,1]x[-1,1] to [0,1]x[0,1]x[0,1] coordinates
    rayStart = rayStart * 0.5f + 0.5f;
    rayStop = rayStop * 0.5f + 0.5f;

    // Init first position
    float4 pos = rayStart;
    // Init ray step
    float4 dir = fast_normalize(rayStop - rayStart);
    float4 step = dir * (float4)TSTEP;
    float travel = fast_distance(rayStop, rayStart);
    bool bFirstElt = true;
    for(uint i=0; i<2000 && travel>0.0f; i++, pos+=step, travel-=(float)TSTEP) // nb iteration depends on stepsize, should be large enough
    {
        if( pos.x>myParam->clipPlaneCenter[0] && pos.x<myParam->clipPlaneCenter[1] &&
            pos.y>myParam->clipPlaneCenter[2] && pos.y<myParam->clipPlaneCenter[3] &&
            pos.z>myParam->clipPlaneCenter[4] && pos.z<myParam->clipPlaneCenter[5])
        {
            // read from 3D texture
            // return bgra values in [0,1]^4 due to CL_BGRA and CL_UNORM_INT8
            float4 sample = read_imagef(volume, volumeSampler, pos);
            float4 lutR = (float4)(0.0f,0.0f,0.0f,0.0f);
            float4 lutG = (float4)(0.0f,0.0f,0.0f,0.0f);
            float4 lutB = (float4)(0.0f,0.0f,0.0f,0.0f);
            float4 color = (float4)(0.0f,0.0f,0.0f,0.0f);

            // *****************************************************************
            // lookup in transfer function texture
            // *****************************************************************
            if(sample.x>=myParam->chR[0] && sample.x<=myParam->chR[1])
            {
                float2 transfer_pos_r = (float2)((sample.x-myParam->transferOffset)*myParam->transferScale, 0.5f);
                lutR = read_imagef(transferFunc, transferFuncSampler, transfer_pos_r);
            }

            if(sample.y>=myParam->chG[0] && sample.y<=myParam->chG[1])
            {
                float2 transfer_pos_g = (float2)((sample.y-myParam->transferOffset)*myParam->transferScale, 0.5f);
                lutG = read_imagef(transferFunc, transferFuncSampler, transfer_pos_g);
            }
            if(sample.z>=myParam->chB[0] && sample.z<=myParam->chB[1])
            {
                float2 transfer_pos_b = (float2)((sample.z-myParam->transferOffset)*myParam->transferScale, 0.5f);
                lutB = read_imagef(transferFunc, transferFuncSampler, transfer_pos_b);
            }

            lutR.w *= myParam->density*10;
            lutG.w *= myParam->density*10;
            lutB.w *= myParam->density*10;

            color.x = lutB.z;
            color.y = lutG.y;
            color.z = lutR.x;
            float4 colorTransp = (float4)(lutB.w, lutG.w, lutR.w, 0.0);

            // *****************************************************************
            // accumulate result
            // *****************************************************************
            //calculate ray integral
            temp.xyz = temp.xyz + (1.0f - transparency.xyz) * colorTransp.xyz * color.xyz;
            transparency.xyz = transparency.xyz + (1.0f - transparency.xyz) * colorTransp.xyz;

            float colorNorm = sqrt(dot(color, color));
            // early ray termination
            if(colorNorm>0.5f && bFirstElt)
            {
                float4 newPos = pos*2.0f - 1.0f;
                newPos.x *= alpha;
                newPos.y *= -beta;
                newPos.z *= -delta;
                depthVec.x = dot(newPos, ((float4)(modelViewProjMatrix[0],modelViewProjMatrix[1],modelViewProjMatrix[2],modelViewProjMatrix[3])));
                depthVec.y = dot(newPos, ((float4)(modelViewProjMatrix[4],modelViewProjMatrix[5],modelViewProjMatrix[6],modelViewProjMatrix[7])));
                depthVec.z = dot(newPos, ((float4)(modelViewProjMatrix[8],modelViewProjMatrix[9],modelViewProjMatrix[10],modelViewProjMatrix[11])));
                depthVec.w = dot(newPos, ((float4)(modelViewProjMatrix[12],modelViewProjMatrix[13],modelViewProjMatrix[14],modelViewProjMatrix[15])));
                bFirstElt = false;
            }

            if(sample.w*myParam->isoValue[3] > 0.5f)
            {
                float4 isoVal = (float4)(myParam->isoValue[2], myParam->isoValue[1], myParam->isoValue[0], myParam->isoValue[3]);
                temp = temp + (1.0f - transparency)*addBlinnPhongShadingX(isoVal, volume, pos, lightPosition, eyePosition)*isoVal.w;
                transparency = fmin(transparency + 0.96f*isoVal.w, 0.96f);

                float4 newPos = pos*2.0f - 1.0f;
                newPos.x *= alpha;
                newPos.y *= -beta;
                newPos.z *= -delta;
                depthVec.x = dot(newPos, ((float4)(modelViewProjMatrix[0],modelViewProjMatrix[1],modelViewProjMatrix[2],modelViewProjMatrix[3])));
                depthVec.y = dot(newPos, ((float4)(modelViewProjMatrix[4],modelViewProjMatrix[5],modelViewProjMatrix[6],modelViewProjMatrix[7])));
                depthVec.z = dot(newPos, ((float4)(modelViewProjMatrix[8],modelViewProjMatrix[9],modelViewProjMatrix[10],modelViewProjMatrix[11])));
                depthVec.w = dot(newPos, ((float4)(modelViewProjMatrix[12],modelViewProjMatrix[13],modelViewProjMatrix[14],modelViewProjMatrix[15])));
                bFirstElt = false;
            }

            if(transparency.x>0.95 && transparency.y>0.95 && transparency.z>0.95)
            {
                break;
            }
        }
    }
    temp.xyz *= myParam->brightness;
    temp.w = fmax(fmax(transparency.x, transparency.y), transparency.z);

    if ((x < myParam->wndWidth) && (y < myParam->wndHeight)) {
        // write output color
        uint i =(y * myParam->wndWidth) + x;
        d_output[i] = rgbaFloatToInt(temp);
        d_depth[i] = (depthVec.z/depthVec.w + 1.0f) * 0.5f;
    }
}
)";

#endif // RAYCASTCOLORF2B_H
