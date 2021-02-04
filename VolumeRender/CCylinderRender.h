#ifndef CCYLINDERRENDER_H
#define CCYLINDERRENDER_H

#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QVector3D>
#include <QVector4D>
#include <QVector>
#include "VolumeRenderGlobal.h"

class QOpenGLShaderProgram;
class QOpenGLShader;

class VOLUMERENDERSHARED_EXPORT CCylinderRender : protected QOpenGLFunctions
{
    public:

        CCylinderRender();
        ~CCylinderRender();

        void    setMatrix(QMatrix4x4* pModel, QMatrix4x4* pView, QMatrix4x4* pProjection);
        void    setPosition(QVector3D startPt, QVector3D endPt);
        void    setRadius(float radius);
        void    setColor(const QVector4D &color);

        void    render();

    private:

        void    initGL();
        void    initVbo();

    private:

        QOpenGLShaderProgram*       m_pProgram = nullptr;
        QOpenGLShader*              m_pVShader = nullptr;
        QOpenGLShader*              m_pFShader = nullptr;
        const QMatrix4x4*           m_pProjectionMat = nullptr;
        const QMatrix4x4*           m_pViewMat = nullptr;
        const QMatrix4x4*           m_pModelMat = nullptr;
        QOpenGLVertexArrayObject    m_vao;
        QOpenGLBuffer               m_vbo;
        QVector3D                   m_startPt;
        QVector3D                   m_axis;
        QVector4D                   m_color;
        GLfloat*                    m_pBase = nullptr;
        float                       m_radius = 0.0;
        int                         m_modelViewMatId = -1;
        int                         m_modelViewProjMatId = -1;
        int                         m_projMatId = -1;
        int                         m_modelViewInverseMatId = -1;
        int                         m_startPtId = -1;
        int                         m_axisId = -1;
        int                         m_radiusId = -1;
        int                         m_localCoordId = -1;
        int                         m_colorId;
};

#endif // CCYLINDERRENDER_H
