#include "recorderpagemanagement.h"
#include <QQmlEngine>


RecorderPageManagement* RecorderPageManagement::m_pThis = nullptr;

RecorderPageManagement::RecorderPageManagement(QObject *parent) : QObject(parent)
{

}

bool RecorderPageManagement::addPageNumberToUsedList(qint32 currentPageNumber,qint32 totalTime,bool firstRequest,bool startRecord){

    static QString warningMessage={};
    static bool pageAmongPageForRecordFounded=false;
    static bool canAssignThisPageForRecord=false;
    if(firstRequest==true){
        warningMessage.clear();
        pageAmongPageForRecordFounded=false;
        canAssignThisPageForRecord=false;
        recorderPagesUsedTemp=recorderPagesUsed;
    }


    if(currentPageNumber>=recorderPagesCapacity.length() || currentPageNumber<=0 ){//Guard clause
        //
        if(firstRequest==false){
            warningMessage.append("This mission not completely record, totalTime exceed recorderPageCapacity");

        } else{
            if(currentPageNumber==0){
                warningMessage.append("mission not record");
            }else{
                warningMessage.append("This page recoder is not exit");
            }

        }

        if(startRecord==true){
            recorderPagesUsed=recorderPagesUsedTemp;
        }
        qDebug()<<warningMessage;
        showMessageToUserAndEndFunction(warningMessage);
        setWarningMessage(warningMessage);
        return canAssignThisPageForRecord;
    }


    //if totalTime<recorercapaicty.at(currentPage-1) --totoalTime set to zero and avoid to recurcive function continue
    qint32 timeRemider=totalTime-recorderPagesCapacity.at(currentPageNumber-1);
    if(timeRemider<0){
        timeRemider=0;
    }
    if(recorderPagesUsed.at(currentPageNumber-1)==1){
        if(firstRequest==true){
            warningMessage.append("This recode Page Used Before:");
            warningMessage.append(QString::number(currentPageNumber));
            if(startRecord==true){
                recorderPagesUsed=recorderPagesUsedTemp;
            }
           // qDebug()<<warningMessage;
            showMessageToUserAndEndFunction(warningMessage);
            setWarningMessage(warningMessage);
            return canAssignThisPageForRecord;
        } else{
            if(pageAmongPageForRecordFounded==false){
                warningMessage.append("one page among pages that need for record used:");
                warningMessage.append(QString::number(currentPageNumber));
                pageAmongPageForRecordFounded=true;
            }else{
                warningMessage.append(",");
                warningMessage.append(QString::number(currentPageNumber));
            }
        }
    }else{
       recorderPagesUsedTemp.replace(currentPageNumber-1,1);
    }

    if(timeRemider>0){
        qint32 nextPageNumber=currentPageNumber+1;

        firstRequest=false;
        addPageNumberToUsedList(nextPageNumber,timeRemider,false,startRecord);
    }
    //

    if(startRecord==true){
      recorderPagesUsed=recorderPagesUsedTemp;
    }
    qDebug()<<warningMessage;
    showMessageToUserAndEndFunction(warningMessage);
    warningMessage.append(QString::number(canAssignThisPageForRecord));
    setWarningMessage(warningMessage);
    return canAssignThisPageForRecord;
}

QVariantList RecorderPageManagement::recommedPageNumberForRecord(qint32 totalTime){
    QVariantList recommedPages={};
    recorderPagesUsed={0,0,0,1,0,0,0,0,0,0};
    for(int pageNumberIndex=1;pageNumberIndex<11;pageNumberIndex++){
            qDebug()<<pageNumberIndex<<totalTime;
            bool canAssignThisPageForRecord=addPageNumberToUsedList(pageNumberIndex,totalTime,true,false);
            if(canAssignThisPageForRecord==true){
                recommedPages<<pageNumberIndex;
            }
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
            addPageNumberToUsedList(pageNumberIndex,totalTimeIndex,true,false);
        }
    }
}

void RecorderPageManagement::showMessageToUserAndEndFunction(QString message)
{
    emit showMessageToUser(message);
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


