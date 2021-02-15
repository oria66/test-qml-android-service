import QtQuick 2.14
import QtPositioning 5.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14

import "qrc:/3dparty/ll2utm.js" as Logic

ApplicationWindow {
    id: applicationWindow
    width: 640
    height: 480
    visible: true
    title: qsTr("Scroll")

    ColumnLayout{
        anchors.fill: parent

        Connections {
            target : pingPong
            onLoggingData: pongLabel.text = msg
        }

        GroupBox {
            Layout.rightMargin: 20
            Layout.leftMargin: 20
            Layout.topMargin: 0
            Layout.fillHeight: true
            Layout.fillWidth: true
            title: qsTr("GPS logging with background service")

            ColumnLayout {
                Label {
                    id: pongLabel
                    Layout.fillWidth: true
                }
                TextField {
                    Layout.fillWidth: true
                    placeholderText : "Type a message"
                    onTextChanged: pingPong.ping(text)
                }

                Button {
                    text: pingPong.isRunningService ? qsTr("Stop service") : qsTr("Start service")
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                    font.bold: true
                    onClicked: {
                        if(pingPong.isRunningService)
                        {
                            pingPong.stopLogging()
                        }else
                        {
                            permissionsDialog.open()

                        }
                    }
                }

                Rectangle{
                    width: 20; height: 20
                    color: pingPong.isRunningService ? "red" : "green"
                }

                Item {
                    Layout.fillHeight: true
                }
            }
        }
    }

    Connections{
        target: androidHandler
        function onPermissionsAccepted(){
            pingPong.startLogging()
        }
    }

    Dialog {
        id: permissionsDialog
        x: (parent.width - width) / 2
        y: (parent.height - height) / 2
        title: "Permissions"
        standardButtons: Dialog.Ok | Dialog.Cancel

        Label{
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            text: "User, can you allow the access?"
        }

        onAccepted:  androidHandler.requestAndroidPermissions()
    }
}

/*##^##
Designer {
    D{i:0;formeditorZoom:0.8999999761581421}
}
##^##*/
