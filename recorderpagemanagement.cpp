#include "recorderpagemanagement.h"
#include <QQmlEngine>
#include <QFile>


RecorderPageManagement* RecorderPageManagement::m_pThis = nullptr;

RecorderPageManagement::RecorderPageManagement(QObject *parent) : QObject(parent)
{

}

bool RecorderPageManagement::addPageNumberToUsedList(qint32 currentPageNumber,qint32 totalTime,bool firstRequest,bool startRecord,bool showMessageEnabled){

    if(firstRequest==true){
        messageString.clear();
        pageAmongPageForRecordFounded=false;
        canAssignThisPageForRecord=false;
        recorderPagesUsedTemp=recorderPagesUsed;
        lastUsedRecordPagesInTestTemp={};
    }


    if(currentPageNumber>=recorderPagesCapacity.length() || currentPageNumber<=0 ){//Guard clause
        //
        if(firstRequest==false){
            messageString.append("This mission not completely record, totalTime exceed recorderPageCapacity");

        } else{
            if(currentPageNumber==0){
                messageString.append("Mission Not Record");
            }else{
                messageString.append("This page recoder is not exit");
            }

        }

        showMessageToUserAndEndFunction(messageString);
        setWarningMessage(messageString);
        return canAssignThisPageForRecord;
    }

    if(recorderPagesUsed.at(currentPageNumber-1)==1){
        if(firstRequest==true){
            messageString.append("This recode Page Used Before:");
            messageString.append(QString::number(currentPageNumber));
            if(startRecord==true){
                recorderPagesUsed=recorderPagesUsedTemp;
                lastUsedRecordPagesInTest=lastUsedRecordPagesInTestTemp;
                saveRecorderPageInfoInFile();
            }
            showMessageToUserAndEndFunction(messageString);
            setWarningMessage(messageString);
            return canAssignThisPageForRecord;
        } else{
            if(pageAmongPageForRecordFounded==false){
                messageString.append("one page among pages that need for record used:");
                messageString.append(QString::number(currentPageNumber));
                pageAmongPageForRecordFounded=true;
            }else{
                messageString.append(",");
                messageString.append(QString::number(currentPageNumber));
            }
        }
    }else{
        recorderPagesUsedTemp.replace(currentPageNumber-1,1);
        lastUsedRecordPagesInTestTemp<<currentPageNumber;
        if(startRecord==true){
            recorderPagesUsed=recorderPagesUsedTemp;
            lastUsedRecordPagesInTest=lastUsedRecordPagesInTestTemp;
            saveRecorderPageInfoInFile();
        }
    }

    //if totalTime<recorercapaicty.at(currentPage-1) --totoalTime set to zero and avoid to recurcive function continue
    qint32 timeRemider=totalTime-recorderPagesCapacity.at(currentPageNumber-1);
    if(timeRemider<0){
        timeRemider=0;
    }

    if(timeRemider>0){
        qint32 nextPageNumber=currentPageNumber+1;

        firstRequest=false;
        addPageNumberToUsedList(nextPageNumber,timeRemider,false,startRecord,true);
    }else{
        showMessageToUserAndEndFunction(messageString);
        messageString.append("True");
        setWarningMessage(messageString);
        canAssignThisPageForRecord=true;
        return canAssignThisPageForRecord;

    }
    return canAssignThisPageForRecord;

}

QVariantList RecorderPageManagement::recommedPageNumberForRecord(qint32 totalTime){
    QVariantList recommedPages={};
    for(int pageNumberIndex=1;pageNumberIndex<=recorderPagesCapacity.length();pageNumberIndex++){
        bool canAssignThisPageForRecord=addPageNumberToUsedList(pageNumberIndex,totalTime,true,false,false);
        if(canAssignThisPageForRecord==true){
            recommedPages<<pageNumberIndex;
        }
    }
    if(recommedPages.length()>0){
        setWarningMessage("");
    }else{
        setWarningMessage("Not found record page. Maybe recoder is full");

   }
    return recommedPages;
}

void RecorderPageManagement::addPageNumberToUsedList_test(){
    recorderPagesUsed={0,0,0,1,0,0,0,0,0,0};
    for(int pageNumberIndex=-1;pageNumberIndex<12;pageNumberIndex++){
        //        for(int recorderPagesUsedIndex=0;recorderPagesUsedIndex<10;recorderPagesUsedIndex++){
        //          recorderPagesUsed.replace(recorderPagesUsedIndex,1);
        //        }
        for(int totalTimeIndex=1;totalTimeIndex<50000;totalTimeIndex++){
            qDebug()<<pageNumberIndex<<totalTimeIndex;
            addPageNumberToUsedList(pageNumberIndex,totalTimeIndex,true,false,true);
        }
    }
}

void RecorderPageManagement::showMessageToUserAndEndFunction(QString message)
{
    emit showMessageToUser(message);
}

void RecorderPageManagement::correctUsedRecordPage(qint32 correctTotolTime)
{
    for(int i=0;i<lastUsedRecordPagesInTest.length();i++){
        correctTotolTime=correctTotolTime-recorderPagesCapacity.at(lastUsedRecordPagesInTest.at(i)-1);
        if(correctTotolTime<-(qint32)recorderPagesCapacity.at(lastUsedRecordPagesInTest.at(i)-1)){
            recorderPagesUsed.replace(lastUsedRecordPagesInTest.at(i)-1,0);
        }
    }
}

bool RecorderPageManagement::resetUsedPage()
{
    for(int i=0;i<recorderPagesUsed.length();i++){
        recorderPagesUsedTemp.replace(i,0);
        recorderPagesUsed.replace(i,0);
        recordPageUseInTestNumber.replace(i,0);
        corruptedRecordPage.replace(i,0);
    }
    lastUsedRecordPagesInTestTemp={};
    lastUsedRecordPagesInTest={};
    return  true;
}

bool RecorderPageManagement::readRecorderPageInfoFronFile()
{
    QString url="recorderStructure.csv";
    QFile file(url);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    QTextStream in(&file);

    recorderPagesCapacity={};
    recorderPagesUsed={};
    corruptedRecordPage={};
    recordPageUseInTestNumber={};

    bool isHeaderFile=true;
    while (!in.atEnd()) {
        if(isHeaderFile){
            isHeaderFile=false;
        }else{
            QString rowContent=in.readLine(); // reads line from file
            QStringList  columnContent = rowContent.split(',');
            recorderPagesCapacity<<columnContent.at(1).toInt();
            recorderPagesUsed<<columnContent.at(2).toInt();
            corruptedRecordPage<<columnContent.at(4).toInt();
            recordPageUseInTestNumber<<columnContent.at(3).toInt();
        }

    }
    file.close();
    return true;

}

bool RecorderPageManagement::saveRecorderPageInfoInFile()
{
    QString url="recorderStructure.csv";
    QFile file(url);
    if (!file.open(QIODevice::ReadWrite |QIODevice::Truncate | QIODevice::Text))
        return false;

    QTextStream output(&file);

    //header file
    output<<"PageAddress,";
    output<<"Capacity,";
    output<<"used,";
    output<<"Corrupted,";
    output<<"TestNumber\n";


    for(int i=0;i<recorderPagesCapacity.length();i++){
        output<<QString::number(i+1)<<",";
        output<<QString::number(recorderPagesCapacity.at(i))<<",";
        output<<QString::number(recorderPagesUsed.at(i))<<",";
        output<<QString::number(corruptedRecordPage.at(i))<<",";
        output<<QString::number(recordPageUseInTestNumber.at(i))<<",\n";
    }
    file.close();
    return true;

}

void RecorderPageManagement::generateMokingRecorderStructureFile()
{
    QList<quint32> recorderPagesCapacityMuck={200,200,500,1000,1200,350,450,80,4200,1000};
    QString url="recorderStructure.csv";
    QFile file(url);
    if (!file.open(QIODevice::WriteOnly |QIODevice::Truncate | QIODevice::Text))
        return;

    QTextStream in(&file);
    //header file
    in<<"PageAddress,";
    in<<"Capacity,";
    in<<"used,";
    in<<"Corrupted,";
    in<<"TestNumber\n";
    //

    for(int i=0;i<recorderPagesCapacityMuck.length();i++){
        in<<i+1;
        in<<",";
        in<<recorderPagesCapacityMuck.at(i);
        in<<",";
        in<<"0,";
        in<<"0,";
        in<<"0\n";
    }

    file.close();

}

RecorderPageManagement *RecorderPageManagement::instance()
{
    if (m_pThis == nullptr) // avoid creation of new instances
        m_pThis = new RecorderPageManagement;
    return m_pThis;
}

QObject *RecorderPageManagement::qmlInstance(QQmlEngine *engine, QJSEngine *scriptEngine) {
    Q_UNUSED(engine);
    Q_UNUSED(scriptEngine);
    // C++ and QML instance they are the same instance
    return RecorderPageManagement::instance();
}


