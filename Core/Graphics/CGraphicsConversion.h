#ifndef CGRAPHICSCONVERSION_H
#define CGRAPHICSCONVERSION_H

#include "Main/CoreGlobal.hpp"
#include "Main/CoreDefine.hpp"
#include "Data/CMat.hpp"
#include <QGraphicsItem>

/** @cond INTERNAL */

class CProxyGraphicsItem;
class CProxyGraphicsPoint;
class CProxyGraphicsRect;
class CProxyGraphicsEllipse;
class CProxyGraphicsPolygon;
class CProxyGraphicsPolyline;
class CProxyGraphicsText;
class CProxyGraphicsComplexPoly;

class CGraphicsPoint;
class CGraphicsRectangle;
class CGraphicsEllipse;
class CGraphicsPolygon;
class CGraphicsPolyline;
class CGraphicsText;
class CGraphicsComplexPolygon;
struct GraphicsPolygonProperty;

class CORESHARED_EXPORT CGraphicsConversion
{
    public:

        CGraphicsConversion();
        CGraphicsConversion(int width, int height);

        QList<QGraphicsItem*>   binaryMaskToGraphics(const CMat& mask, QGraphicsItem *pParent,
                                                     const CColor& penColor, const CColor& brushColor, int lineSize);

        QList<QGraphicsItem*>   blobsToGraphics(const std::vector<std::vector<cv::Point>>& polygonArray,
                                                const std::vector<cv::Vec4i>& hierarchy, QGraphicsItem *pParent,
                                                const CColor &penColor, const CColor &brushColor, int lineSize);

        std::vector<std::shared_ptr<CProxyGraphicsItem>> blobsToProxyGraphics(const std::vector<std::vector<cv::Point>>& polygonArray,
                                                                              const std::vector<cv::Vec4i>& hierarchy,
                                                                              const CColor& penColor, const CColor& brushColor, int lineSize);

        CMat                    graphicsToBinaryMask(const QList<QGraphicsItem*>& graphics);
        CMat                    graphicsToBinaryMask(const std::vector<std::shared_ptr<CProxyGraphicsItem>>& graphics);

        void                    insertToImage(CMat& image, const CGraphicsPoint* pItem, bool bForceFill, bool bBinary);
        void                    insertToImage(CMat& image, const CGraphicsRectangle* pItem, bool bForceFill, bool bBinary);
        void                    insertToImage(CMat& image, const CGraphicsEllipse* pItem, bool bForceFill, bool bBinary);
        void                    insertToImage(CMat& image, const CGraphicsPolygon* pItem, bool bForceFill, bool bBinary);
        void                    insertToImage(CMat& image, const CGraphicsPolyline* pItem, bool bForceFill, bool bBinary);
        void                    insertToImage(CMat& image, const CGraphicsText* pItem, bool bForceFill, bool bBinary);
        void                    insertToImage(CMat& image, const CGraphicsComplexPolygon* pItem, bool bForceFill, bool bBinary);

        void                    insertToImage(CMat& image, const CProxyGraphicsPoint* pItem, bool bForceFill, bool bBinary);
        void                    insertToImage(CMat& image, const CProxyGraphicsRect* pItem, bool bForceFill, bool bBinary);
        void                    insertToImage(CMat& image, const CProxyGraphicsEllipse* pItem, bool bForceFill, bool bBinary);
        void                    insertToImage(CMat& image, const CProxyGraphicsPolygon* pItem, bool bForceFill, bool bBinary);
        void                    insertToImage(CMat& image, const CProxyGraphicsPolyline* pItem, bool bForceFill, bool bBinary);
        void                    insertToImage(CMat& image, const CProxyGraphicsText* pItem, bool bForceFill, bool bBinary);
        void                    insertToImage(CMat& image, const CProxyGraphicsComplexPoly* pItem, bool bForceFill, bool bBinary);

    private:

        std::vector<CPointF>    convertToPolygon(const std::vector<cv::Point>& cvPoly);
        QPolygonF               convertToQPolygon(const std::vector<cv::Point>& cvPoly);
        QPolygonF               convertToQPolygon(const std::vector<CPointF>& poly);
        std::vector<cv::Point>  convertToCvPolygon(const QGraphicsItem *pItem, const QPolygonF& qtPoly);
        std::vector<cv::Point>  convertToCvPolygon(const std::vector<CPointF>& poly);

        void                    clipPoint(cv::Point& pt);

    private:

        int m_width = 0;
        int m_height = 0;
};

/** @endcond */

#endif // CGRAPHICSCONVERSION_H
