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

    Row{
        TextField{
            width: 100
            height: 40

            onTextChanged: {
                var canPageAddressAssighnable= RecorderPageManagment.addPageNumberToUsedList(parseInt(text),parseInt(text_totalTime.text),true,false);
            }
        }
        Text {
            width: 200
            height: 40
            text: RecorderPageManagment.warningMessage
        }
        TextField{
             id:text_totalTime
            width: 100
            height: 40

        }

    }



}
