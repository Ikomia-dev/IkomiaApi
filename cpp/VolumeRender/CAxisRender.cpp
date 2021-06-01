// Copyright (C) 2021 Ikomia SAS
// Contact: https://www.ikomia.com
//
// This file is part of the Ikomia API libraries.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

#include "CAxisRender.h"

CAxisRender::CAxisRender()
{
}

CAxisRender::CAxisRender(QMatrix4x4* pModel, QMatrix4x4* pView, QMatrix4x4* pProjection, int width, int height, float fov)
{
    m_pProjectionMat = pProjection;
    m_pViewMat = pView;
    m_pModelMat = pModel;
    m_wndWidth = width;
    m_wndHeight = height;
    m_fov = fov;

    QVector4D red(1.0, 0.0, 0.0, 1.0);
    QVector4D green(0.0, 1.0, 0.0, 1.0);
    QVector4D blue(0.0, 0.0, 1.0, 1.0);

    addSphere(0.0f, 0.0f, 0.0f, 0.2f, QVector4D(1.0, 1.0, 1.0, 1.0));
    addSphere(1.0f, 0.0f, 0.0f, 0.10f, red);
    addSphere(0.0f, 1.0f, 0.0f, 0.10f, green);
    addSphere(0.0f, 0.0f, 1.0f, 0.10f, blue);

    addCylinder(QVector3D(0.0, 0.0, 0.0), QVector3D(1.0, 0.0, 0.0), 0.1f, red);
    addCylinder(QVector3D(0.0, 0.0, 0.0), QVector3D(0.0, 1.0, 0.0), 0.1f, green);
    addCylinder(QVector3D(0.0, 0.0, 0.0), QVector3D(0.0, 0.0, 1.0), 0.1f, blue);
}

void CAxisRender::render()
{
    for(size_t i=0; i<m_spheres.size(); ++i)
        m_spheres[i]->render();

    for(size_t i=0; i<m_cylinders.size(); ++i)
        m_cylinders[i]->render();
}

void CAxisRender::addSphere(float x, float y, float z, float radius, const QVector4D &color)
{
    auto pSphere = std::make_shared<CSphereRender>();
    pSphere->setMatrix(m_pModelMat, m_pViewMat, m_pProjectionMat);
    pSphere->setWindowInfo(m_wndWidth, m_wndHeight, m_fov);
    pSphere->setPosition(x, y, z);
    pSphere->setRadius(radius);
    pSphere->setColor(color);
    m_spheres.push_back(std::move(pSphere));
}

void CAxisRender::addCylinder(const QVector3D &start, const QVector3D &end, float radius, const QVector4D &color)
{
    auto pCylinder = std::make_shared<CCylinderRender>();
    pCylinder->setMatrix(m_pModelMat, m_pViewMat, m_pProjectionMat);
    pCylinder->setPosition(start, end);
    pCylinder->setRadius(radius);
    pCylinder->setColor(color);
    m_cylinders.push_back(std::move(pCylinder));
}
