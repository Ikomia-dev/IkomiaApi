#ifndef CPOINT_H
#define CPOINT_H

template<typename T>
class CPoint
{
    public:

        CPoint()
        {
            m_x = T();
            m_y = T();
        }
        CPoint(T x, T y)
        {
            m_x = x;
            m_y = y;
        }

        void setX(T x)
        {
            m_x = x;
        }

        void setY(T y)
        {
            m_y = y;
        }

        T   getX() const
        {
            return m_x;
        }

        T   getY() const
        {
            return m_y;
        }

    public:

        T m_x;
        T m_y;
};

using CPointI = CPoint<int>;
using CPointF = CPoint<float>;
using PolygonF = std::vector<CPointF>;

#endif // CPOINT_H
