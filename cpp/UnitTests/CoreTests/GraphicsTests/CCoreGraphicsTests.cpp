#define NO_IMPORT_ARRAY
#include "CCoreGraphicsTests.h"
#include <QString>
#include <QtTest>
#include <QGraphicsItem>
#include "Graphics/CGraphicsPolygon.h"
#include "Graphics/CGraphicsComplexPolygon.h"
#include "Graphics/CGraphicsPolyline.h"
#include "Graphics/CGraphicsEllipse.h"
#include "Graphics/CGraphicsRectangle.h"
#include "Graphics/CGraphicsPoint.h"
#include "Graphics/CGraphicsText.h"
#include "Graphics/CGraphicsJSON.h"
#include "Graphics/CGraphicsLayer.h"

CCoreGraphicsTests::CCoreGraphicsTests()
{
}

QList<QGraphicsItem*> CCoreGraphicsTests::createGraphicsScene()
{
    QList<QGraphicsItem*> items;

    //Ellipse
    auto pElli = new CGraphicsEllipse;
    pElli->setRect(QRectF(10, 10, 50, 50));
    items.push_back(pElli);

    //Rectangle
    items.push_back(new CGraphicsRectangle(cv::Rect(120, 10, 50, 50)));

    //Polyline
    CGraphicsPolylineProperty lineProp;
    lineProp.m_penColor = {255,255,255,255};
    auto pPolyline = new CGraphicsPolyline(lineProp);
    pPolyline->addPoint(QPointF(300, 30));
    pPolyline->addPoint(QPointF(500, 600));
    pPolyline->addPoint(QPointF(700, 100));
    items.push_back(pPolyline);

    //Polygon
    auto pPoly = new CGraphicsPolygon;
    pPoly->addPoint(QPointF(50, 350));
    pPoly->addPoint(QPointF(150, 450));
    pPoly->addPoint(QPointF(200, 700));
    pPoly->addPoint(QPointF(50, 750));
    pPoly->addPoint(QPointF(30, 500));
    items.push_back(pPoly);

    //Complex polygon
    auto pCplxPoly = new CGraphicsComplexPolygon;
    QPolygonF outerPoly;
    outerPoly.append(QPointF(750.0f, 350.0f));
    outerPoly.append(QPointF(850.0f, 450.0f));
    outerPoly.append(QPointF(900.0f, 700.0f));
    outerPoly.append(QPointF(800.0f, 750.0f));
    outerPoly.append(QPointF(730.0f, 450.0f));
    pCplxPoly->setOuterPolygon(outerPoly, false);
    QPolygonF innerPoly;
    innerPoly.append(QPointF(750.0f, 400.0f));
    innerPoly.append(QPointF(800.0f, 700.0f));
    innerPoly.append(QPointF(810.0f, 450.0f));
    pCplxPoly->addInnerPolygon(innerPoly, false);
    pCplxPoly->buildPainterPath();
    items.push_back(pCplxPoly);

    //Point
    CGraphicsPointProperty ptProp;
    ptProp.m_penColor = {255,255,255,255};
    ptProp.m_brushColor = {255,255,255,255};
    ptProp.m_size = 5;
    ptProp.m_category = "Test";
    items.push_back(new CGraphicsPoint(QPointF(512,512), ptProp));

    //Text
    CGraphicsTextProperty txtProp;
    txtProp.m_color = {255,255,255,255};
    txtProp.m_fontName = "Arial";
    txtProp.m_fontSize = 14;
    txtProp.m_bBold = false;
    txtProp.m_bItalic = true;
    txtProp.m_bUnderline = false;
    txtProp.m_bStrikeOut = true;
    items.push_back(new CGraphicsText("MyText", txtProp, QPoint(512, 800)));
    return items;
}

bool CCoreGraphicsTests::colorMatch(const QColor &qcolor, const std::vector<int> &ccolor) const
{
    return (qcolor.red() == ccolor[0] && qcolor.green() == ccolor[1] && qcolor.blue() == ccolor[2]);
}

void CCoreGraphicsTests::polygonTest()
{
    //Constructors
    CGraphicsPolygon poly1(nullptr);
    QVERIFY(poly1.type() == CGraphicsPolygon::Type);

    CGraphicsPolygonProperty prop;
    prop.m_penColor = {0,0,255,255};
    prop.m_brushColor = {0,0,255,255};
    prop.m_lineSize = 2;
    prop.m_category = "Test";

    CGraphicsPolygon poly2(prop, &poly1);
    QVERIFY(poly2.type() == CGraphicsPolygon::Type);
    QVERIFY(colorMatch(poly2.pen().color(), prop.m_penColor));
    QVERIFY(poly2.pen().width() == prop.m_lineSize);
    QVERIFY(colorMatch(poly2.brush().color(), prop.m_brushColor));
    QVERIFY(poly2.getCategory() == QString::fromStdString(prop.m_category));
    QVERIFY(poly2.parentItem() == static_cast<QGraphicsItem*>(&poly1));

    CGraphicsPolygon poly3(poly2);
    QVERIFY(poly3.type() == CGraphicsPolygon::Type);
    QVERIFY(poly3.pen().color() == poly2.pen().color());
    QVERIFY(poly3.pen().width() == poly2.pen().width());
    QVERIFY(poly3.brush().color() == poly2.brush().color());
    QVERIFY(poly3.getCategory() == poly2.getCategory());
    QVERIFY(poly3.parentItem() == static_cast<QGraphicsItem*>(&poly1));
    QVERIFY(poly3.polygon().size() == poly2.polygon().size());

    //Point operations
    poly3.addPoint(QPoint(0, 0)); //First point is added twice to close the polygon
    poly3.addPoint(QPoint(10, 10));
    poly3.addPoint(QPoint(0, 10));
    QVERIFY(poly3.polygon().size() == 4);

    poly3.updatePoint(0, QPoint(1, 1));
    QVERIFY(poly3.polygon().first() == QPoint(1,1));

    poly3.updatePoint(12, QPoint(1,1)); //Not possible but should not crash

    poly3.updateLastPoint(QPoint(8, 15));
    QVERIFY(poly3.polygon().last() == QPoint(8,15));

    //Assignement
    CGraphicsPolygon poly4;
    poly4 = poly3;
    QVERIFY(poly4.type() == CGraphicsPolygon::Type);
    QVERIFY(poly4.pen().color() == poly3.pen().color());
    QVERIFY(poly4.pen().width() == poly3.pen().width());
    QVERIFY(poly4.brush().color() == poly3.brush().color());
    QVERIFY(poly4.getCategory() == poly3.getCategory());
    QVERIFY(poly4.parentItem() == poly3.parentItem());
    QVERIFY(poly4.polygon().size() == poly3.polygon().size());
    QVERIFY(poly4.polygon().last() == QPoint(8,15));

    //JSON
    CGraphicsJSON jsonMgr;
    auto jsonData = poly4.getJsonData(jsonMgr);
    CGraphicsPolygon poly5;

    QVERIFY_EXCEPTION_THROWN(poly5.buildFromJsonData(jsonMgr, QByteArray()), CException);

    poly5.buildFromJsonData(jsonMgr, jsonData);
    QVERIFY(poly5.type() == CGraphicsPolygon::Type);
    QVERIFY(poly5.pen().color() == poly4.pen().color());
    QVERIFY(poly5.pen().width() == poly4.pen().width());
    QVERIFY(poly5.brush().color() == poly4.brush().color());
    QVERIFY(poly5.getCategory() == poly4.getCategory());
    QVERIFY(poly5.polygon().size() == poly4.polygon().size());
    QVERIFY(poly5.polygon().last() == QPoint(8,15));
}

void CCoreGraphicsTests::complexPolygonTest()
{
    //Constructors
    CGraphicsComplexPolygon poly1(nullptr);
    QVERIFY(poly1.type() == CGraphicsComplexPolygon::Type);

    CGraphicsPolygonProperty prop;
    prop.m_penColor = {0,0,255,255};
    prop.m_brushColor = {0,0,255,255};
    prop.m_lineSize = 2;
    prop.m_category = "Test";

    CGraphicsComplexPolygon poly2(prop, &poly1);
    QVERIFY(poly2.type() == CGraphicsComplexPolygon::Type);
    QVERIFY(colorMatch(poly2.pen().color(), prop.m_penColor));
    QVERIFY(poly2.pen().width() == prop.m_lineSize);
    QVERIFY(colorMatch(poly2.brush().color(), prop.m_brushColor));
    QVERIFY(poly2.getCategory() == QString::fromStdString(prop.m_category));
    QVERIFY(poly2.parentItem() == static_cast<QGraphicsItem*>(&poly1));
    QVERIFY(poly2.getOuterPolygon().size() == 0);
    QVERIFY(poly2.getInnerPolygons().size() == 0);

    CGraphicsComplexPolygon poly3(poly2);
    QVERIFY(poly3.type() == CGraphicsComplexPolygon::Type);
    QVERIFY(poly3.pen().color() == poly2.pen().color());
    QVERIFY(poly3.pen().width() == poly2.pen().width());
    QVERIFY(poly3.brush().color() == poly2.brush().color());
    QVERIFY(poly3.getCategory() == poly2.getCategory());
    QVERIFY(poly3.parentItem() == static_cast<QGraphicsItem*>(&poly1));
    QVERIFY(poly3.getOuterPolygon().size() == 0);
    QVERIFY(poly3.getInnerPolygons().size() == 0);

    //Set outer polygon
    QPolygonF outerPoly;
    outerPoly.append(QPointF(0.0f, 0.0f));
    outerPoly.append(QPointF(200.0f, 0.0f));
    outerPoly.append(QPointF(300.0f, 150.0f));
    outerPoly.append(QPointF(100.0f, 200.0f));
    outerPoly.append(QPointF(0.0f, 150.0f));
    poly3.setOuterPolygon(outerPoly, false);
    auto outerTmp = poly3.getOuterPolygon();
    QVERIFY(outerTmp == outerPoly);

    //Add inner polygons
    QPolygonF innerPoly1;
    innerPoly1.append(QPointF(10.0f, 10.0f));
    innerPoly1.append(QPointF(50.0f, 10.0f));
    innerPoly1.append(QPointF(30.0f, 50.0f));
    poly3.addInnerPolygon(innerPoly1, false);

    QPolygonF innerPoly2;
    innerPoly2.append(QPointF(50.0f, 10.0f));
    innerPoly2.append(QPointF(150.0f, 10.0f));
    innerPoly2.append(QPointF(30.0f, 100.0f));
    poly3.addInnerPolygon(innerPoly2, false);

    auto inners3 = poly3.getInnerPolygons();
    QVERIFY(inners3.size() == 2);
    QVERIFY(inners3[0] == innerPoly1);
    QVERIFY(inners3[1] == innerPoly2);

    //Assignement
    CGraphicsComplexPolygon poly4;
    poly4 = poly3;
    QVERIFY(poly4.type() == CGraphicsComplexPolygon::Type);
    QVERIFY(poly4.pen().color() == poly3.pen().color());
    QVERIFY(poly4.pen().width() == poly3.pen().width());
    QVERIFY(poly4.brush().color() == poly3.brush().color());
    QVERIFY(poly4.getCategory() == poly3.getCategory());
    QVERIFY(poly4.parentItem() == poly3.parentItem());
    QVERIFY(poly4.getOuterPolygon() == poly3.getOuterPolygon());
    auto inners4 = poly4.getInnerPolygons();
    QVERIFY(inners4.size() == inners3.size());
    QVERIFY(inners4[0] == inners3[0]);
    QVERIFY(inners4[1] == inners3[1]);

    //JSON
    CGraphicsJSON jsonMgr;
    auto jsonData = poly4.getJsonData(jsonMgr);
    CGraphicsComplexPolygon poly5;

    QVERIFY_EXCEPTION_THROWN(poly5.buildFromJsonData(jsonMgr, QByteArray()), CException);

    poly5.buildFromJsonData(jsonMgr, jsonData);
    QVERIFY(poly5.type() == CGraphicsComplexPolygon::Type);
    QVERIFY(poly5.pen().color() == poly4.pen().color());
    QVERIFY(poly5.pen().width() == poly4.pen().width());
    QVERIFY(poly5.brush().color() == poly4.brush().color());
    QVERIFY(poly5.getCategory() == poly4.getCategory());
    QVERIFY(poly5.getOuterPolygon() == poly4.getOuterPolygon());
    auto inners5 = poly5.getInnerPolygons();
    QVERIFY(inners5.size() == inners4.size());
    QVERIFY(inners5[0] == inners4[0]);
    QVERIFY(inners5[1] == inners4[1]);

    //Clear
    poly5.clear();
    QVERIFY(poly5.getOuterPolygon().size() == 0);
    QVERIFY(poly5.getInnerPolygons().size() == 0);
}

void CCoreGraphicsTests::polylineTest()
{
    //Constructors
    CGraphicsPolyline poly1(nullptr);
    QVERIFY(poly1.type() == CGraphicsPolyline::Type);

    CGraphicsPolylineProperty prop;
    prop.m_penColor = {0,0,255,255};
    prop.m_lineSize = 2;
    prop.m_category = "Test";

    CGraphicsPolyline poly2(prop, &poly1);
    QVERIFY(poly2.type() == CGraphicsPolyline::Type);
    QVERIFY(colorMatch(poly2.pen().color(), prop.m_penColor));
    QVERIFY(poly2.pen().width() == prop.m_lineSize);
    QVERIFY(poly2.getCategory() == QString::fromStdString(prop.m_category));
    QVERIFY(poly2.parentItem() == static_cast<QGraphicsItem*>(&poly1));

    CGraphicsPolyline poly3(poly2);
    QVERIFY(poly3.type() == CGraphicsPolyline::Type);
    QVERIFY(poly3.pen().color() == poly2.pen().color());
    QVERIFY(poly3.pen().width() == poly2.pen().width());
    QVERIFY(poly3.getCategory() == poly2.getCategory());
    QVERIFY(poly3.parentItem() == static_cast<QGraphicsItem*>(&poly1));
    QVERIFY(poly3.getPolygon().size() == poly2.getPolygon().size());

    //Point operations
    poly3.addPoint(QPoint(0, 0)); //First point is added twice for display
    poly3.addPoint(QPoint(10, 10));
    poly3.addPoint(QPoint(0, 10));
    QVERIFY(poly3.getPolygon().size() == 4);

    poly3.updatePoint(0, QPoint(1, 1));
    QVERIFY(poly3.getPolygon().first() == QPoint(1,1));

    poly3.updatePoint(12, QPoint(1,1)); //Not possible but should not crash

    poly3.updateLastPoint(QPoint(8, 15));
    QVERIFY(poly3.getPolygon().last() == QPoint(8,15));

    //Assignement
    CGraphicsPolyline poly4;
    poly4 = poly3;
    QVERIFY(poly4.type() == CGraphicsPolyline::Type);
    QVERIFY(poly4.pen().color() == poly3.pen().color());
    QVERIFY(poly4.pen().width() == poly3.pen().width());
    QVERIFY(poly4.getCategory() == poly3.getCategory());
    QVERIFY(poly4.parentItem() == poly3.parentItem());
    QVERIFY(poly4.getPolygon().size() == poly3.getPolygon().size());
    QVERIFY(poly4.getPolygon().last() == QPoint(8,15));

    //JSON
    CGraphicsJSON jsonMgr;
    auto jsonData = poly4.getJsonData(jsonMgr);
    CGraphicsPolyline poly5;

    QVERIFY_EXCEPTION_THROWN(poly5.buildFromJsonData(jsonMgr, QByteArray()), CException);

    poly5.buildFromJsonData(jsonMgr, jsonData);
    QVERIFY(poly5.type() == CGraphicsPolyline::Type);
    QVERIFY(poly5.pen().color() == poly4.pen().color());
    QVERIFY(poly5.pen().width() == poly4.pen().width());
    QVERIFY(poly5.getCategory() == poly4.getCategory());
    QVERIFY(poly5.getPolygon().size() == poly4.getPolygon().size());
    QVERIFY(poly5.getPolygon().last() == QPoint(8,15));
}

void CCoreGraphicsTests::ellipseTest()
{
    //Constructors
    CGraphicsEllipse elli1(nullptr);
    QVERIFY(elli1.type() == CGraphicsEllipse::Type);

    CGraphicsEllipseProperty prop;
    prop.m_penColor = {0,0,255,255};
    prop.m_brushColor = {0,0,255,255};
    prop.m_lineSize = 2;
    prop.m_category = "Test";

    CGraphicsEllipse elli2(prop, &elli1);
    QVERIFY(elli2.type() == CGraphicsEllipse::Type);
    QVERIFY(colorMatch(elli2.pen().color(), prop.m_penColor));
    QVERIFY(elli2.pen().width() == prop.m_lineSize);
    QVERIFY(colorMatch(elli2.brush().color(), prop.m_brushColor));
    QVERIFY(elli2.getCategory() == QString::fromStdString(prop.m_category));
    QVERIFY(elli2.parentItem() == static_cast<QGraphicsItem*>(&elli1));

    CGraphicsEllipse elli3(elli2);
    QVERIFY(elli3.type() == CGraphicsEllipse::Type);
    QVERIFY(elli3.pen().color() == elli2.pen().color());
    QVERIFY(elli3.pen().width() == elli2.pen().width());
    QVERIFY(elli3.brush().color() == elli2.brush().color());
    QVERIFY(elli3.getCategory() == elli2.getCategory());
    QVERIFY(elli3.parentItem() == static_cast<QGraphicsItem*>(&elli1));

    //Set rect
    QRect rc(0, 0, 100, 200);
    elli3.setRect(rc);

    //Assignement
    CGraphicsEllipse elli4;
    elli4 = elli3;
    QVERIFY(elli4.type() == CGraphicsEllipse::Type);
    QVERIFY(elli4.pen().color() == elli3.pen().color());
    QVERIFY(elli4.pen().width() == elli3.pen().width());
    QVERIFY(elli4.brush().color() == elli3.brush().color());
    QVERIFY(elli4.getCategory() == elli3.getCategory());
    QVERIFY(elli4.parentItem() == elli3.parentItem());
    QVERIFY(elli4.rect() == elli3.rect());
    QVERIFY(elli4.rect() == rc);

    //JSON
    CGraphicsJSON jsonMgr;
    auto jsonData = elli4.getJsonData(jsonMgr);
    CGraphicsEllipse elli5;

    QVERIFY_EXCEPTION_THROWN(elli5.buildFromJsonData(jsonMgr, QByteArray()), CException);

    elli5.buildFromJsonData(jsonMgr, jsonData);
    QVERIFY(elli5.type() == CGraphicsEllipse::Type);
    QVERIFY(elli5.pen().color() == elli4.pen().color());
    QVERIFY(elli5.pen().width() == elli4.pen().width());
    QVERIFY(elli5.brush().color() == elli4.brush().color());
    QVERIFY(elli5.getCategory() == elli4.getCategory());
    QVERIFY(elli4.rect() == elli4.rect());
}

void CCoreGraphicsTests::rectangleTest()
{
    //Constructors
    CGraphicsRectangle rect1(nullptr);
    QVERIFY(rect1.type() == CGraphicsRectangle::Type);

    CGraphicsRectProperty prop;
    prop.m_penColor = {0,0,255,255};
    prop.m_brushColor = {0,0,255,255};
    prop.m_lineSize = 2;
    prop.m_category = "Test";

    CGraphicsRectangle rect2(prop, &rect1);
    QVERIFY(rect2.type() == CGraphicsRectangle::Type);
    QVERIFY(colorMatch(rect2.pen().color(), prop.m_penColor));
    QVERIFY(rect2.pen().width() == prop.m_lineSize);
    QVERIFY(colorMatch(rect2.brush().color(), prop.m_brushColor));
    QVERIFY(rect2.getCategory() == QString::fromStdString(prop.m_category));
    QVERIFY(rect2.parentItem() == static_cast<QGraphicsItem*>(&rect1));

    CGraphicsRectangle rect3(rect2);
    QVERIFY(rect3.type() == CGraphicsRectangle::Type);
    QVERIFY(rect3.pen().color() == rect2.pen().color());
    QVERIFY(rect3.pen().width() == rect2.pen().width());
    QVERIFY(rect3.brush().color() == rect2.brush().color());
    QVERIFY(rect3.getCategory() == rect2.getCategory());
    QVERIFY(rect3.parentItem() == static_cast<QGraphicsItem*>(&rect1));
    QRect rc(0, 0, 100, 200);
    rect3.setRect(rc);

    cv::Rect cvRc(0, 0, 150, 320);
    CGraphicsRectangle rect4(cvRc);
    QVERIFY(rect4.type() == CGraphicsRectangle::Type);
    QVERIFY(rect4.rect().x() == cvRc.x);
    QVERIFY(rect4.rect().y() == cvRc.y);
    QVERIFY(rect4.rect().width() == cvRc.width);
    QVERIFY(rect4.rect().height() == cvRc.height);

    //Assignement
    CGraphicsRectangle rect5;
    rect5 = rect3;
    QVERIFY(rect5.type() == CGraphicsRectangle::Type);
    QVERIFY(rect5.pen().color() == rect3.pen().color());
    QVERIFY(rect5.pen().width() == rect3.pen().width());
    QVERIFY(rect5.brush().color() == rect3.brush().color());
    QVERIFY(rect5.getCategory() == rect3.getCategory());
    QVERIFY(rect5.parentItem() == rect3.parentItem());
    QVERIFY(rect5.rect() == rect3.rect());
    QVERIFY(rect5.rect() == rc);

    //JSON
    CGraphicsJSON jsonMgr;
    auto jsonData = rect5.getJsonData(jsonMgr);
    CGraphicsRectangle rect6;

    QVERIFY_EXCEPTION_THROWN(rect6.buildFromJsonData(jsonMgr, QByteArray()), CException);

    rect6.buildFromJsonData(jsonMgr, jsonData);
    QVERIFY(rect6.type() == CGraphicsRectangle::Type);
    QVERIFY(rect6.pen().color() == rect5.pen().color());
    QVERIFY(rect6.pen().width() == rect5.pen().width());
    QVERIFY(rect6.brush().color() == rect5.brush().color());
    QVERIFY(rect6.getCategory() == rect5.getCategory());
    QVERIFY(rect6.rect() == rect5.rect());
}

void CCoreGraphicsTests::pointTest()
{
    //Constructors
    CGraphicsPoint pt1(nullptr);
    QVERIFY(pt1.type() == CGraphicsPoint::Type);

    CGraphicsPointProperty prop;
    prop.m_penColor = {0,0,255,255};
    prop.m_brushColor = {0,0,255,255};
    prop.m_size = 5;
    prop.m_category = "Test";

    auto pos = QPointF(20.0f, 20.0f);
    CGraphicsPoint pt2(pos, prop, nullptr);
    QVERIFY(pt2.type() == CGraphicsPoint::Type);
    QVERIFY(colorMatch(pt2.pen().color(), prop.m_penColor));
    QVERIFY(colorMatch(pt2.brush().color(), prop.m_brushColor));
    QVERIFY(pt2.getCategory() == QString::fromStdString(prop.m_category));
    QVERIFY(pt2.rect().center() == pos);
    QVERIFY(pt2.rect().width() == pt2.rect().height());
    QVERIFY(pt2.rect().width() == prop.m_size);

    CGraphicsPoint pt3(pt2);
    QVERIFY(pt3.type() == CGraphicsPoint::Type);
    QVERIFY(pt3.pen().color() == pt2.pen().color());
    QVERIFY(pt3.brush().color() == pt2.brush().color());
    QVERIFY(pt3.getCategory() == pt2.getCategory());
    QVERIFY(pt3.rect() == pt2.rect());

    //Assignement
    CGraphicsPoint pt4;
    pt4 = pt3;
    QVERIFY(pt4.type() == CGraphicsPoint::Type);
    QVERIFY(pt4.pen().color() == pt3.pen().color());
    QVERIFY(pt4.brush().color() == pt3.brush().color());
    QVERIFY(pt4.getCategory() == pt3.getCategory());
    QVERIFY(pt4.rect() == pt3.rect());

    //JSON
    CGraphicsJSON jsonMgr;
    auto jsonData = pt4.getJsonData(jsonMgr);
    CGraphicsPoint pt5;

    QVERIFY_EXCEPTION_THROWN(pt5.buildFromJsonData(jsonMgr, QByteArray()), CException);

    pt5.buildFromJsonData(jsonMgr, jsonData);
    QVERIFY(pt5.type() == CGraphicsPoint::Type);
    QVERIFY(pt5.pen().color() == pt4.pen().color());
    QVERIFY(pt5.brush().color() == pt4.brush().color());
    QVERIFY(pt5.getCategory() == pt4.getCategory());
    QVERIFY(pt5.rect() == pt4.rect());
}

void CCoreGraphicsTests::textTest()
{
    //Constructors
    CGraphicsText txt1(nullptr);
    QVERIFY(txt1.type() == CGraphicsText::Type);

    CGraphicsTextProperty prop;
    prop.m_color = {0,0,255,255};
    prop.m_fontName = "Arial";
    prop.m_fontSize = 8;
    prop.m_bBold = false;
    prop.m_bItalic = true;
    prop.m_bUnderline = false;
    prop.m_bStrikeOut = true;

    CGraphicsText txt2("MyText", prop, nullptr);
    QVERIFY(txt2.type() == CGraphicsText::Type);
    QVERIFY(colorMatch(txt2.defaultTextColor(), prop.m_color));
    auto font2 = txt2.font();
    QVERIFY(font2.family() == QString::fromStdString(prop.m_fontName));
    QVERIFY(font2.pointSize() == prop.m_fontSize);
    QVERIFY(font2.bold() == prop.m_bBold);
    QVERIFY(font2.italic() == prop.m_bItalic);
    QVERIFY(font2.underline() == prop.m_bUnderline);
    QVERIFY(font2.strikeOut() == prop.m_bStrikeOut);

    auto pos = QPointF(20.0f, 20.0f);
    CGraphicsText txt3("MyText", prop, pos, nullptr);
    QVERIFY(txt3.type() == CGraphicsText::Type);
    QVERIFY(colorMatch(txt3.defaultTextColor(), prop.m_color));
    QVERIFY(txt3.pos() == pos);
    auto font3 = txt3.font();
    QVERIFY(font3.family() == QString::fromStdString(prop.m_fontName));
    QVERIFY(font3.pointSize() == prop.m_fontSize);
    QVERIFY(font3.bold() == prop.m_bBold);
    QVERIFY(font3.italic() == prop.m_bItalic);
    QVERIFY(font3.underline() == prop.m_bUnderline);
    QVERIFY(font3.strikeOut() == prop.m_bStrikeOut);

    CGraphicsText txt4(txt3);
    QVERIFY(txt4.type() == CGraphicsText::Type);
    QVERIFY(txt4.defaultTextColor() == txt3.defaultTextColor());
    QVERIFY(txt4.pos() == txt3.pos());
    auto font4 = txt4.font();
    QVERIFY(font4.family() == font3.family());
    QVERIFY(font4.pointSize() == font3.pointSize());
    QVERIFY(font4.bold() == font3.bold());
    QVERIFY(font4.italic() == font3.italic());
    QVERIFY(font4.underline() == font3.underline());
    QVERIFY(font4.strikeOut() == font3.strikeOut());

    //Assignement
    CGraphicsText txt5;
    txt5 = txt3;
    QVERIFY(txt5.type() == CGraphicsText::Type);
    QVERIFY(txt5.defaultTextColor() == txt3.defaultTextColor());
    QVERIFY(txt5.pos() == txt3.pos());
    auto font5 = txt5.font();
    QVERIFY(font5.family() == font3.family());
    QVERIFY(font5.pointSize() == font3.pointSize());
    QVERIFY(font5.bold() == font3.bold());
    QVERIFY(font5.italic() == font3.italic());
    QVERIFY(font5.underline() == font3.underline());
    QVERIFY(font5.strikeOut() == font3.strikeOut());

    //JSON
    CGraphicsJSON jsonMgr;
    auto jsonData = txt5.getJsonData(jsonMgr);
    CGraphicsText txt6;

    QVERIFY_EXCEPTION_THROWN(txt6.buildFromJsonData(jsonMgr, QByteArray()), CException);

    txt6.buildFromJsonData(jsonMgr, jsonData);
    QVERIFY(txt6.type() == CGraphicsText::Type);
    QVERIFY(txt6.defaultTextColor() == txt5.defaultTextColor());
    QVERIFY(txt6.pos() == txt5.pos());
    auto font6 = txt5.font();
    QVERIFY(font6.family() == font5.family());
    QVERIFY(font6.pointSize() == font5.pointSize());
    QVERIFY(font6.bold() == font5.bold());
    QVERIFY(font6.italic() == font5.italic());
    QVERIFY(font6.underline() == font5.underline());
    QVERIFY(font6.strikeOut() == font5.strikeOut());
}

void CCoreGraphicsTests::layerTest()
{
    //Constructors
    CGraphicsLayer layer1("One", nullptr);
    QVERIFY(layer1.type() == CGraphicsLayer::Type);
    QVERIFY(layer1.getName() ==  "One");

    CGraphicsLayer layer2("Two", &layer1);
    QVERIFY(layer2.type() == CGraphicsLayer::Type);
    QVERIFY(layer2.getName() == "Two");
    QVERIFY(layer2.parentItem() == &layer1);

    CGraphicsLayer layer3("Three", &layer1);

    CGraphicsLayer layer4(layer3);
    QVERIFY(layer4.type() == CGraphicsLayer::Type);
    QVERIFY(layer4.getName() == layer3.getName());
    //Parent relation is not copied
    QVERIFY(layer4.parentItem() == nullptr);

    //Setters
    layer1.setRoot(true);
    QVERIFY(layer1.isRoot() == true);
    layer1.setName("OneRoot");
    QVERIFY(layer1.getName() ==  "OneRoot");

    //Getters
    QVERIFY(layer1.getChildLayerCount() == 2);
    auto childLayers = layer1.getChildLayers();
    QVERIFY(childLayers.size() == 2);
    QVERIFY(childLayers[0] == &layer2);
    QVERIFY(childLayers[1] == &layer3);

    //Add child items
    size_t nb = 20;
    std::vector<QGraphicsItem*> items;

    for(size_t i=0; i<nb; ++i)
        items.push_back(new CGraphicsEllipse(&layer3));

    QVERIFY(layer3.getChildItemCount() == nb);

    auto childItems = layer3.getChildItems();
    QVERIFY(childItems.size() == (int)items.size());

    for(size_t i=0; i<items.size(); ++i)
        QVERIFY(childItems[i] == items[i]);

    //Assignement
    CGraphicsLayer layer5 = layer3;
    QVERIFY(layer5.getChildLayerCount() == layer3.getChildLayerCount());
    QVERIFY(layer5.getChildItemCount() == layer3.getChildItemCount());

    //Delete child items
    layer5.deleteChildItems();
    QVERIFY(layer5.getChildItemCount() == 0);
}

void CCoreGraphicsTests::conversionTest()
{
    auto items = createGraphicsScene();

    CGraphicsConversion conv(m_width, m_height);
    auto binary = conv.graphicsToBinaryMask(items);
    QVERIFY(binary.empty() == false);
    QVERIFY(binary.getNbCols() == (size_t)m_width);
    QVERIFY(binary.getNbRows() == (size_t)m_height);

    auto items2 = conv.binaryMaskToGraphics(binary, nullptr, {255,0,0}, {255,0,0}, 1);
    //Each letter of text count as object
    QVERIFY(items2.size() == 11);

    //All objects are CGraphicsPolygon or CComplexGraphicsPolygon
    int nbComplexPoly = 0;
    for(int i=0; i<items2.size(); ++i)
    {
        if(items2[i]->type() == CGraphicsComplexPolygon::Type)
            nbComplexPoly++;
    }
    QVERIFY(nbComplexPoly == 2);
}

//Need a valid QApplication to test CGraphicsText
QTEST_MAIN(CCoreGraphicsTests)
//QTEST_GUILESS_MAIN(CCoreGraphicsTests)
