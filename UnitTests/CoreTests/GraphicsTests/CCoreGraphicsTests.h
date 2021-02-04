#ifndef CCOREGRAPHICSTESTS_H
#define CCOREGRAPHICSTESTS_H

#include <QObject>

class QGraphicsItem;

class CCoreGraphicsTests : public QObject
{
    Q_OBJECT

    public:

        CCoreGraphicsTests();

    private:

        QList<QGraphicsItem*>   createGraphicsScene();
        bool                    colorMatch(const QColor& qcolor, const std::vector<int>& ccolor) const;

    private slots:

        void    polygonTest();
        void    complexPolygonTest();
        void    polylineTest();
        void    ellipseTest();
        void    rectangleTest();
        void    pointTest();
        void    textTest();
        void    layerTest();
        void    conversionTest();

    private:

        int     m_width = 1024;
        int     m_height = 1024;
};


#endif // CCOREGRAPHICSTESTS_H
