#ifndef CSPHERERENDER_H
#define CSPHERERENDER_H

#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QVector>
#include <QVector3D>
#include <QVector4D>
#include "VolumeRenderGlobal.h"

class QOpenGLShaderProgram;
class QOpenGLShader;

class VOLUMERENDERSHARED_EXPORT CSphereRender : protected QOpenGLFunctions
{
    public:

        CSphereRender();
        ~CSphereRender();

        void    setWindowInfo(int width, int height, float fov);
        void    setMatrix(QMatrix4x4* pModel, QMatrix4x4* pView, QMatrix4x4* pProjection);
        void    setPosition(float x, float y, float z);
        void    setRadius(float radius);
        void    setColor(const QVector4D& color);

        void    render();

    private:

        void    initGL();

    private:

        QOpenGLShaderProgram*       m_pProgram = nullptr;
        QOpenGLShader*              m_pVShader = nullptr;
        QOpenGLShader*              m_pFShader = nullptr;
        QOpenGLVertexArrayObject    m_vao;
        QOpenGLBuffer               m_vbo;
        QVector<float>              m_position;
        QVector4D                   m_color;
        float                       m_radius = 0;
        float                       m_fov = 0;
        int                         m_wndWidth = 0;
        int                         m_wndHeight = 0;
        int                         m_ptRadiusId = -1;
        int                         m_ptScaleId = -1;
        int                         m_modelViewMatId = -1;
        int                         m_modelViewProjMatId = -1;
        int                         m_projMatId = -1;
        int                         m_vertexId = -1;
        int                         m_colorId = -1;
        const QMatrix4x4*           m_pProjectionMat = nullptr;
        const QMatrix4x4*           m_pViewMat = nullptr;
        const QMatrix4x4*           m_pModelMat = nullptr;
};

#endif // CSPHERERENDER_H
