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
            onPong: pongLabel.text = msg
        }

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
            Item {
               Layout.fillHeight: true
            }
        }

        GroupBox {
            id: groupBox
            Layout.rightMargin: 20
            Layout.leftMargin: 20
            Layout.topMargin: 0
            Layout.fillHeight: true
            Layout.fillWidth: true
            title: qsTr("GPS logging without service")

            ColumnLayout{
                anchors.fill: parent

                Button {
                    id: button
                    text: src.active ? qsTr("Stop service") : qsTr("Start service")
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                    font.bold: true
                    onClicked: src.active ? src.stop() : src.start()
                }

                GridLayout{
                    columnSpacing: 20
                    rows: 2
                    columns: 3
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter

                    Label{
                        text: qsTr("Northing: ")
                        font.bold: true
                    }

                    Label{
                        id: northing
                        text: "-"
                    }

                    Label{
                        text: "[m]"
                    }

                    Label{
                        text: qsTr("Easting: ")
                        font.bold: true
                    }

                    Label{
                        id: easting
                        text: "-"
                    }

                    Label{
                        text: "[m]"
                    }
                }

                Label{
                    id: labelError
                    text: "No error"
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                }
            }
        }
    }

    PositionSource {
        id: src
        updateInterval: 2000
        active: false
        onPositionChanged: {
            var coord = src.position.coordinate;
            var utm = Logic.fromLatLon(coord.latitude,coord.longitude);

            northing.text = utm.northing;
            easting.text =utm.easting;
        }
        onSourceErrorChanged: {
            labelError.text = sourceError
        }
    }


}

/*##^##
Designer {
    D{i:0;formeditorZoom:0.8999999761581421}
}
##^##*/
