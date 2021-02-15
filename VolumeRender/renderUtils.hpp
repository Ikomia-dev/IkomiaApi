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

#ifndef RENDERUTILS_HPP
#define RENDERUTILS_HPP

#include <QtOpenGL>

namespace Ikomia
{
    namespace Utils
    {
        namespace GL
        {
            inline const char * getGLErrorStr(GLenum err)
            {
                switch (err)
                {
                    case GL_NO_ERROR:          return "No error";
                    case GL_INVALID_ENUM:      return "Invalid enum";
                    case GL_INVALID_VALUE:     return "Invalid value";
                    case GL_INVALID_OPERATION: return "Invalid operation";
                    case GL_STACK_OVERFLOW:    return "Stack overflow";
                    case GL_STACK_UNDERFLOW:   return "Stack underflow";
                    case GL_OUT_OF_MEMORY:     return "Out of memory";
                    default:                   return "Unknown error";
                }
            }

            inline void checkGLError()
            {
                while (true)
                {
                    const GLenum err = glGetError();
                    if (GL_NO_ERROR == err)
                        break;

                    qCritical().noquote() << "GL Error: " << getGLErrorStr(err);
                }
            }
        }
    }
}

#endif // RENDERUTILS_HPP
