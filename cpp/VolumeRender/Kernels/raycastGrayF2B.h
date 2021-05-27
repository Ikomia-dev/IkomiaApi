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

#include <string>

std::string raycastGrayF2B = R"(
/////////////////////////////////////////////////////////////////////////
// Front to Back Raycasting rendering for grayscale image              //
/////////////////////////////////////////////////////////////////////////
__kernel void renderGray_F2B(
		__global uint *d_output, 
		__constant float4* invViewMatrix,
		__read_only image3d_t volume,
		__read_only image2d_t transferFunc,
		sampler_t volumeSampler,
		sampler_t transferFuncSampler,
		__global float* d_depth,
		__constant float4* modelViewProjMatrixOriginal,
		__constant rayParam* myParam
		 )
{	
	uint x = get_global_id(0);
	uint y = get_global_id(1);

	uint lid = get_local_id(0) + get_local_id(1) * get_local_size(0);

	__local float random;
	__local float4 scale;
	__local float4 eyeRay_o;
	__local float4 modelViewProjMatrix[4];
	__local float4 lightPosition;

	if(lid == 0)
	{
		// Dithering process (stochastic jittering)
		random = native_sin(x * 12.9898f + y * 78.233f) * 43758.5453f;
		random = random - floor(random);
		random = fmin(random, 0x1.fffffep-1f);

        scale = (float4)((float)alpha, -(float)beta, -(float)delta, 1.0f);
		/* eyeRay_o = Camera Position (Origin)*/
		// Ray origin is in (m_tx, m_ty, -m_eyeZ + m_tz)
		eyeRay_o = (float4)(invViewMatrix[0].w, invViewMatrix[1].w, invViewMatrix[2].w, 1.0f);   

		modelViewProjMatrix[0] = modelViewProjMatrixOriginal[0];
		modelViewProjMatrix[1] = modelViewProjMatrixOriginal[1];
		modelViewProjMatrix[2] = modelViewProjMatrixOriginal[2];
		modelViewProjMatrix[3] = modelViewProjMatrixOriginal[3];

		lightPosition = native_divide(eyeRay_o, scale);
		lightPosition = lightPosition * 0.5f + 0.5f;
	}

	barrier(CLK_LOCAL_MEM_FENCE);

    bool bIsInside = (x < myParam->wndWidth) && (y < myParam->wndHeight);

	// Bounding box size
    float4 boxMin = (float4)(-alpha, -beta, -delta, 1.0f)*1.1f;
    float4 boxMax = (float4)( alpha,  beta,  delta, 1.0f)*1.1f;

	// Resize coordinates (x,y) from [0, wndWidth]x[0, wndHeight] in [-ratioX ratioX]x[-ratioY ratioY] C [-1, 1]x[-1, 1]
	float u =  myParam->K*x  - myParam->ratioX;
	float v =  myParam->K*y  - myParam->ratioY;

	// camEyeRay_d = ray direction in camera space
	float4 rayDir = fast_normalize(((float4)(u, v, -myParam->focalLength, 0.0f)));

	// eyeRay_d = Current direction (Destination)
	float4 eyeRay_d;
	eyeRay_d.x = dot(rayDir, invViewMatrix[0]);
	eyeRay_d.y = dot(rayDir, invViewMatrix[1]);
	eyeRay_d.z = dot(rayDir, invViewMatrix[2]);
	eyeRay_d.w = 0.0f;

		float tnear;
	float tfar;
	int hit = intersectBox(eyeRay_o, eyeRay_d, boxMin, boxMax, &tnear, &tfar);
	if (!hit) 
	{
		if (bIsInside) 
		{
			// write output color
			uint i = x + (y * myParam->wndWidth);
			d_output[i] = 0;
			d_depth[i] = 0.0f;
		}
		return;
	}

	if (tnear < 0.0f) 
		tnear = 0.0f;     // clamp to near plane

	/*****************************************************************/
	// march along ray from front to back, accumulating color
	/*****************************************************************/

	// init ray limits in object space
	float4 rayStart = eyeRay_o + eyeRay_d*tnear;
	float4 rayStop = eyeRay_o + eyeRay_d*tfar;

	// Get depth value from position in projetive space
	float4 depthVec;
	depthVec.x = dot(rayStop, modelViewProjMatrix[0]);
	depthVec.y = dot(rayStop, modelViewProjMatrix[1]);
	depthVec.z = dot(rayStop, modelViewProjMatrix[2]);
	depthVec.w = dot(rayStop, modelViewProjMatrix[3]);

	/* transform from object space to coordinate space*/
	// in order to map screen coordinate (0,0) to object coordinate (-1,1,1)
	/* map x from [-alpha,alpha] to [-1,1]*/
	// map y from [-beta,beta] to [-1,1]
	// map z from [-delta,delta] to [-1,1]

	rayStart = native_divide(rayStart, scale);
	rayStop = native_divide(rayStop, scale);

	// map position from [-1,1]x[-1,1]x[-1,1] to [0,1]x[0,1]x[0,1] coordinates
	rayStart = rayStart * 0.5f + 0.5f;
	rayStop = rayStop * 0.5f + 0.5f;

	// Init ray step
	float4 dir = fast_normalize(rayStop - rayStart);
	float4 step = dir * (float4)TSTEP;
	float travel = fast_distance(rayStop, rayStart);

	// Init first position with dithering
	float4 pos = rayStart;
	pos += step*random;

	bool bFirstElt = true;

	float4 temp = (float4)(0.0f,0.0f,0.0f,0.0f);

	for(uint i=0; i<10000 && travel>0.0f; i++, pos+=step, travel-=(float)TSTEP) // nb iteration depends on stepsize, should be large enough
	{	
		 bool bCutPlane = pos.x>myParam->clipPlaneCenter[0] && pos.x<myParam->clipPlaneCenter[1] && 
			 pos.y>myParam->clipPlaneCenter[2] && pos.y<myParam->clipPlaneCenter[3] && 
			 pos.z>myParam->clipPlaneCenter[4] && pos.z<myParam->clipPlaneCenter[5];

		if(bCutPlane)
		{
				/* read from 3D texture        */
			// return bgra values in [0,1]^4 due to CL_BGRA and CL_UNORM_INT8
			float4 sample = read_imagef(volume, samplerLinear, pos); 
			float4 color = (float4)(0.0f,0.0f,0.0f,0.0f);
			if( sample.w>=myParam->chR[0] && sample.w<=myParam->chR[1] )
			{
				if (sample.x > 0.5f && myParam->isoValue[3] > 0)
				{
					float4 isoVal = (float4)(myParam->isoValue[0], myParam->isoValue[1], myParam->isoValue[2], myParam->isoValue[3]);					
					float4 normal = calcGradientX(pos, volume);
					float mag = fast_length(normal);
					// Add a Blinn-Phong shading.  
					if(mag>0.0f)
						temp = temp + (1.0f - temp.w)*addBlinnPhongShading(isoVal, pos, lightPosition, -dir, fast_normalize(normal));
					else
						temp = temp + (1.0f - temp.w)*isoVal;
				}

				/*****************************************************************/
				// lookup in transfer function texture
				/*****************************************************************/ 
				float2 transfer_pos = (float2)((sample.w-myParam->transferOffset)*myParam->transferScale, 0.5f);
				color = read_imagef(transferFunc, transferFuncSampler, transfer_pos);
				color.w *= myParam->density*10;
	
				/*****************************************************************/
				// accumulate result
				/*****************************************************************/
				//calculate ray integral
				color.xyz *= color.w;
				temp = temp + (1.0f - temp.w) * color;

				// Fill depth position of first hit object
				if(temp.w > 0.1f && bFirstElt)
				{
					float4 newPos = pos*2.0f - 1.0f;
					newPos *= scale;
					depthVec.x = dot(newPos, modelViewProjMatrix[0]);
					depthVec.y = dot(newPos, modelViewProjMatrix[1]);
					depthVec.z = dot(newPos, modelViewProjMatrix[2]);
					depthVec.w = dot(newPos, modelViewProjMatrix[3]);
					bFirstElt = false;
				}

				// early ray termination
				if(temp.w > 0.95f)
				{
					break;
				}
			}
		}
	}
	temp *= myParam->brightness;

	if (bIsInside) 
	{
		// write output color
		uint i = x + (y * myParam->wndWidth);
		d_output[i] = rgbaFloatToInt(temp);
		d_depth[i] = (depthVec.z/depthVec.w + 1.0f) * 0.5f;
	}

}
)";
