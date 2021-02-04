#include "CWidgetOutput.h"
#include <QPainter>
#include "Main/CoreTools.hpp"

CWidgetOutput::CWidgetOutput() : CProtocolTaskIO(IODataType::WIDGET)
{
    m_name = "CWidgetOutput";
    m_description = QObject::tr("Generic widget for custom display.\nAny Qt-based window (ex: Matplotlib canvas).").toStdString();
    m_saveFormat = DataFileFormat::JPG;
}

CWidgetOutput::CWidgetOutput(IODataType data): CProtocolTaskIO(data)
{
    m_name = "CWidgetOutput";
    m_description = QObject::tr("Generic widget for custom display.\nAny Qt-based window (ex: Matplotlib canvas).").toStdString();
    m_saveFormat = DataFileFormat::JPG;
}

CWidgetOutput::~CWidgetOutput()
{
    if(m_pWidget)
        delete m_pWidget;
}

void CWidgetOutput::setWidget(QWidget *pWidget)
{
    m_pWidget = pWidget;
}

QWidget *CWidgetOutput::getWidget() const
{
    return m_pWidget;
}

bool CWidgetOutput::isDataAvailable() const
{
    return m_pWidget != nullptr;
}

void CWidgetOutput::clearData()
{
    m_pWidget = nullptr;
}

void CWidgetOutput::save()
{
    std::string path = m_saveFolder + m_saveBaseName + Utils::Data::getFileFormatExtension(m_saveFormat);
    save(path);
}

void CWidgetOutput::save(const std::string &path)
{
    if(!m_pWidget)
        return;

    QImage img(m_pWidget->size(), QImage::Format_RGB32);
    QPainter painter(&img);
    m_pWidget->render(&painter);
    img.save(QString::fromStdString(path));
}

std::shared_ptr<CWidgetOutput> CWidgetOutput::clone() const
{
    return std::static_pointer_cast<CWidgetOutput>(cloneImp());
}

std::shared_ptr<CProtocolTaskIO> CWidgetOutput::cloneImp() const
{
    return std::shared_ptr<CWidgetOutput>(new CWidgetOutput(*this));
}
