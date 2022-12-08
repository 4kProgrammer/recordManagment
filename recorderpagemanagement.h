#ifndef RECORDERPAGEMANAGEMENT_H
#define RECORDERPAGEMANAGEMENT_H

#include <QObject>
#include <QDebug>
class QQmlEngine;
class QJSEngine;

class RecorderPageManagement : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString warningMessage READ warningMessage WRITE setWarningMessage NOTIFY warningMessageChanged)

public:
    explicit RecorderPageManagement(QObject *parent = nullptr);
    static RecorderPageManagement *instance();
    static QObject *qmlInstance(QQmlEngine *engine, QJSEngine *scriptEngine);

//    static LoginService *instance();
//    static QObject *qmlInstance(QQmlEngine *engine, QJSEngine *scriptEngine);

    QList<quint32> recorderPagesCapacity={200,200,500,1000,1200,350,450,80,4200,1000};
    QList<qint32> recorderPagesUsedTemp={0,0,0,0,0,0,0,0,0,0};
    QList<qint32> recorderPagesUsed={0,0,0,0,0,0,0,0,0,0};

    Q_INVOKABLE bool addPageNumberToUsedList(qint32 currentPageNumber,qint32 totalTime,bool firstRequest,bool startRecord);
    QVariantList recommedPageNumberForRecord(qint32 totalTime);
    void addPageNumberToUsedList_test();

    void showMessageToUserAndEndFunction(QString message);

    QString warningMessage() const
    {
        return m_warningMessage;
    }

public slots:
    void setWarningMessage(QString warningMessage)
    {
        if (m_warningMessage == warningMessage)
            return;

        m_warningMessage = warningMessage;
        emit warningMessageChanged(m_warningMessage);
    }

private:
    static RecorderPageManagement* m_pThis;


    QString m_warningMessage={};

signals:

    void showMessageToUser(QString message);

    void warningMessageChanged(QString warningMessage);
};

#endif // RECORDERPAGEMANAGEMENT_H
