#ifndef RECORDERPAGEMANAGEMENT_H
#define RECORDERPAGEMANAGEMENT_H

#include <QObject>
#include <QDebug>
#include <QVariant>
#include <QVariantList>
class QQmlEngine;
class QJSEngine;

class RecorderPageManagement : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString warningMessage READ warningMessage WRITE setWarningMessage NOTIFY warningMessageChanged)
    Q_PROPERTY(QVariantList recoderLayoutData READ recoderLayoutData WRITE setRecoderLayoutData NOTIFY recoderLayoutDataChanged)

public:
    explicit RecorderPageManagement(QObject *parent = nullptr);
    static RecorderPageManagement *instance();
    static QObject *qmlInstance(QQmlEngine *engine, QJSEngine *scriptEngine);


    Q_INVOKABLE bool addPageNumberToUsedList(qint32 currentPageNumber,qint32 totalTime,bool firstRequest,bool startRecord,bool showMessageEnabled);
    Q_INVOKABLE qint32 recommedPageNumberForRecord(qint32 totalTime);
    void addPageNumberToUsedList_test();

    void showMessageToUserAndEndFunction(QString message);


    Q_INVOKABLE void correctUsedRecordPage(qint32 correctTotolTime);
    Q_INVOKABLE bool resetUsedPage();
    Q_INVOKABLE bool readRecorderPageInfoFromFile();
    Q_INVOKABLE bool saveRecorderPageInfoInFile();
    void determineCorruptedRecordPage(void);

    void generateMokingRecorderStructureFile();


    QString warningMessage() const
    {
        return m_warningMessage;
    }

    QVariantList recoderLayoutData() const
    {
        return m_recoderLayoutData;
    }

public slots:

    void prepareDataToShowAsRecorderLayout();
    void setWarningMessage(QString warningMessage)
    {
        if (m_warningMessage == warningMessage)
            return;

        m_warningMessage = warningMessage;
        emit warningMessageChanged(m_warningMessage);
    }

    void setRecoderLayoutData(QVariantList recoderLayoutData)
    {
        if (m_recoderLayoutData == recoderLayoutData)
            return;

        m_recoderLayoutData = recoderLayoutData;
        emit recoderLayoutDataChanged(m_recoderLayoutData);
    }

private:
    static RecorderPageManagement* m_pThis;
    bool pageAmongPageForRecordFounded=false;
    bool canAssignThisPageForRecord=false;
    QString messageString={};

    QList<quint32> recorderPagesCapacity={};
    QList<qint32> recorderPagesUsedTemp={};
    QList<qint32> recorderPagesUsed={};
    QList<qint32> recordPageUseInTestNumber={};
    QList<qint32> corruptedRecordPage={};
    QList<qint32> lastUsedRecordPagesInTestTemp={};
    QList<qint32> lastUsedRecordPagesInTest={};


    QString m_warningMessage={};

    QVariantList m_recoderLayoutData;

signals:

    void showMessageToUser(QString message);

    void warningMessageChanged(QString warningMessage);
    void recoderLayoutDataChanged(QVariantList recoderLayoutData);
};

#endif // RECORDERPAGEMANAGEMENT_H
