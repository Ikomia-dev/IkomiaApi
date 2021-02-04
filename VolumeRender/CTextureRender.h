#ifndef CTEXTURERENDER_H
#define CTEXTURERENDER_H

#include "VolumeRenderGlobal.h"
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QMatrix4x4>
#include <QVector>

class QOpenGLShaderProgram;
class QOpenGLShader;
class QOpenGLTexture;

class VOLUMERENDERSHARED_EXPORT CTextureRender : protected QOpenGLFunctions
{
    public:
        CTextureRender(bool bPboMode = true);
        ~CTextureRender();

        void    render();

        void    setSizes(int width, int height);
        void    setMatrix(QMatrix4x4& model, QMatrix4x4& view, QMatrix4x4& projection);
        void    setPBO(GLuint pbo);
        void    setShaderMode(bool bShader);

        GLuint  getTextureId();

    private:

        void    initGL();
        void    initVBO();
        void    initTexture();

    private:

        QOpenGLShaderProgram*       m_pProgram = nullptr;
        QOpenGLShader*              m_pVShader = nullptr;
        QOpenGLShader*              m_pFShader = nullptr;
        QOpenGLVertexArrayObject    m_vao;
        QOpenGLBuffer               m_vbo;
        QVector<GLfloat>            m_vertexData;
        QOpenGLTexture*             m_pTexture = nullptr;
        GLuint                      m_pbo;
        int                         m_width;
        int                         m_height;
        QMatrix4x4                  m_viewMatrix;
        QMatrix4x4                  m_modelMatrix;
        QMatrix4x4                  m_projMatrix;
        QMatrix4x4                  m_modelViewProj;
        int                         m_modelViewProjMatId;
        int                         m_samplerId;
        int                         m_vertexId;
        int                         m_textureId;
        bool                        m_bPboMode = true;
        bool                        m_bShaderMode = true;
};

#endif // CTEXTURERENDER_H
