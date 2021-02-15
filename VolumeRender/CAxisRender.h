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

#ifndef CAXISRENDER_H
#define CAXISRENDER_H

#include <memory>
#include <QMatrix4x4>
#include "VolumeRenderGlobal.h"
#include "CSphereRender.h"
#include "CCylinderRender.h"

class VOLUMERENDERSHARED_EXPORT CAxisRender
{
    public:

        CAxisRender();
        CAxisRender(QMatrix4x4* pModel, QMatrix4x4* pView, QMatrix4x4* pProjection, int width, int height, float fov);

        void    render();

    private:

        void    addSphere(float x, float y, float z, float radius, const QVector4D& color);
        void    addCylinder(const QVector3D& start, const QVector3D& end, float radius, const QVector4D& color);

    private:

        using SphereRenderPtr = std::shared_ptr<CSphereRender>;
        using CylinderRenderPtr = std::shared_ptr<CCylinderRender>;

        float                           m_fov = 0;
        int                             m_wndWidth = 0;
        int                             m_wndHeight = 0;
        QMatrix4x4*                     m_pProjectionMat = nullptr;
        QMatrix4x4*                     m_pViewMat = nullptr;
        QMatrix4x4*                     m_pModelMat = nullptr;
        std::vector<SphereRenderPtr>    m_spheres;
        std::vector<CylinderRenderPtr>  m_cylinders;
};

#endif // CAXISRENDER_H
