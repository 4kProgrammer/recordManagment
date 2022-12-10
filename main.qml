import QtQuick 2.14
import QtQuick.Window 2.14
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import com.test.RecorderPageManagment 1.0

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    Connections{
        target: RecorderPageManagment
        function onShowMessageToUser(message){

        }
    }
    Column{
        width: parent.width
        height: 200
        spacing: 5

        Row{
            width: parent.width
            height: 40
            CheckBox{
                id:cb_record
                width: 200
                height: 40
                text: "Mission record?"
                onClicked: {
                    if(cb_record.checkState==2){
                        var suggestRecorderPageList= RecorderPageManagment.recommedPageNumberForRecord(parseInt(text_totalTime.text));
                        if(suggestRecorderPageList.length>0){
                            tf_pageAddress.text=suggestRecorderPageList[0];
                        }
                    }else if(cb_record.checkState==0){
                        var canPageAddressAssighnable= RecorderPageManagment.addPageNumberToUsedList(0,parseInt(text_totalTime.text),true,false,true);
                        tf_pageAddress.text="0";
                    }


                }
                Component.onCompleted: {
                    var defualtPageNUmber=0
                    var canPageAddressAssighnable= RecorderPageManagment.addPageNumberToUsedList(parseInt(defualtPageNUmber),parseInt(text_totalTime.text),true,false,true);
                    tf_pageAddress.text=defualtPageNUmber.toString();
                }
            }

            TextField{
                id:tf_pageAddress
                anchors.verticalCenter: parent.verticalCenter
                width: 100
                height: parent.height
                text: "10"
                onTextEdited:  {
                    console.log("parseInt(tf_pageAddress.text)")
                    console.log("parseInt(tf_pageAddress.text)"+parseInt(tf_pageAddress.text))
                    var canPageAddressAssighnable= RecorderPageManagment.addPageNumberToUsedList(parseInt(tf_pageAddress.text),parseInt(text_totalTime.text),true,false,true);
                }
            }
            Button{
                id:btn_suggestRecorderPage
                anchors.verticalCenter: parent.verticalCenter
                width: 100
                height: parent.height
                text: "suggest Page add"
                onClicked: {
                     var suggestRecorderPageList= RecorderPageManagment.recommedPageNumberForRecord(parseInt(text_totalTime.text));
                     console.log("su"+suggestRecorderPageList)
                    if(suggestRecorderPageList.length>0){
                        tf_pageAddress.text=suggestRecorderPageList[0];
                    }
                }


            }
            Text {
                anchors.verticalCenter: parent.verticalCenter
                width: 150
                wrapMode: Text.Wrap
                height: parent.height
                text: RecorderPageManagment.warningMessage
            }
            TextField{
                id:text_totalTime
                anchors.verticalCenter: parent.verticalCenter
                width: 100
                height: parent.height
                text: "150"
                onTextChanged: {
                    var canPageAddressAssighnable= RecorderPageManagment.addPageNumberToUsedList(parseInt(tf_pageAddress.text),parseInt(text_totalTime.text),true,false,true);

                }

            }

            Button{
                id:startScenario
                anchors.verticalCenter: parent.verticalCenter
                width: 100
                height: parent.height
                text: "start Scenario"
                onClicked: {
                     var canPageAddressAssighnable= RecorderPageManagment.addPageNumberToUsedList(parseInt(tf_pageAddress.text),parseInt(text_totalTime.text),true,true,true);
                }


            }

        }

        Row{
            width: parent.width
            height: 40
            Text {
                anchors.verticalCenter: parent.verticalCenter
                width: 200
                height: parent.height
                text: "actual last Senario time"
            }
            TextField{
                id:tf_actualLastScenarioTime
                anchors.verticalCenter: parent.verticalCenter
                width: 100
                height: parent.height
            }
            Button{
                id:btn_correctLastScenarioRecord
                anchors.verticalCenter: parent.verticalCenter
                text: "correct Last Scenario Record"
                onClicked: {
                    RecorderPageManagment.correctUsedRecordPage(parseInt(tf_actualLastScenarioTime.text));
                }
            }

        }
        Row{
            width: parent.width
            height: 40


            Button{
                id:btn_clearRecoder
                anchors.verticalCenter: parent.verticalCenter
                text: "Clear recorder content"
                onClicked: {
                    RecorderPageManagment.resetUsedPage();
                }
            }

        }

    }
}
