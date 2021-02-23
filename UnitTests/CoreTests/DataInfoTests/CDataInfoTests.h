#ifndef CDATAINFOTESTS_H
#define CDATAINFOTESTS_H

#include <QObject>

class CDataInfoTests : public QObject
{
    Q_OBJECT

    public:

        CDataInfoTests(QObject* parent = nullptr);

    private slots:

        void arrayTest();
};

#endif // CDATAINFOTESTS_H
