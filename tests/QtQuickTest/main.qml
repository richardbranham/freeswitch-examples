import QtQuick 2.11
import QtQuick.Window 2.11
import QtQuick.Controls 2.1
import QtQuick.Dialogs 1.2

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    Item {
        property int someNumber: 100
    }

    TextField {
        id: textField1
        x: 10
        y: 40
        width: 620
        height: 55
        text: qsTr("Text Field")
    }

    TextArea {
        id: textArea1
        x: 10
        y: 110
        width: 620
        height: 354
        text: qsTr("Text Area")
    }

    MessageDialog {
        id: messageDialog
        title: "Bridge Desktop"
        text: ""
        onAccepted: {
            //console.log("And of course you could only agree.")
            //Qt.quit()
        }
        Component.onCompleted: visible = true
    }

    function messageReceived(messageText) {
        console.log(messageText);
        textField1.text = messageText;
        textArea1.append(messageText);

        var vertoMessage = JSON.parse(messageText);

        console.log(messageText);

        if(vertoMessage.params.msg.caller_id_number.length > 5 ) {
            messageDialog.text = vertoMessage.params.msg.caller_id_number;
            messageDialog.open();
        }
    }
}
