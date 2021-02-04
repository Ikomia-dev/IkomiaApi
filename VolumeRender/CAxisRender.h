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
