import QtQuick 2.0
import Ubuntu.Components 1.1
import PowaFi 1.0
/*!
    \brief MainView with a Label and Button elements.
*/

MainView {
    // objectName for functional testing purposes (autopilot-qt5)
    objectName: "mainView"

    // Note! applicationName needs to match the "name" field of the click manifest
    applicationName: "powafi.kamikaze"

    /*
     This property enables the application to change orientation
     when the device is rotated. The default is false.
    */
    //automaticOrientation: true

    // Removes the old toolbar and enables new features of the new header.
    useDeprecatedToolbar: false

    width: units.gu(100)
    height: units.gu(75)

    Page {
        title: i18n.tr("PowaFi")

        MyType {
            id: myType

            Component.onCompleted: {
                myType.helloWorld = i18n.tr("Hello world..")
            }
        }

        Column {
            spacing: units.gu(1)
            anchors {
                margins: units.gu(2)
                fill: parent
            }

            Row {
                spacing: units.gu(1)

                Label {
                    id: labelMac
                    objectName: "labelMac"

                    text: "MAC"
                }

                TextField {
                    id: inputMac
                    objectName: "inputMac"
                    errorHighlight: false
                    //validator: DoubleValidator {notation: DoubleValidator.StandardNotation}
                    width: pageLayout.width - 2 * root.margins - root.buttonWidth
                    height: units.gu(5)
                    font.pixelSize: FontUtils.sizeToPixels("medium")
                    text: 'ac:cf:23:34:96:24'
                    //onTextChanged: {
                    //    if (activeFocus) {
                    //        inputTo.text = convert(inputFrom.text, selectorFrom.currencyIndex, selectorTo.currencyIndex)
                    //    }
                    //}
                    //function update() {
                    //    text = convert(inputTo.text, selectorTo.currencyIndex, selectorFrom.currencyIndex)
                    //}
                }
            }

            Row {
                spacing: units.gu(1)

                Label {
                    id: labelIp
                    objectName: "labelIp"

                    text: "IP"
                }

                TextField {
                    id: inputIP
                    objectName: "inputIP"
                    errorHighlight: false
                    //validator: DoubleValidator {notation: DoubleValidator.StandardNotation}
                    width: pageLayout.width - 2 * root.margins - root.buttonWidth
                    height: units.gu(5)
                    font.pixelSize: FontUtils.sizeToPixels("medium")
                    text: '192.168.4.108'
                    //onTextChanged: {
                    //    if (activeFocus) {
                    //        inputTo.text = convert(inputFrom.text, selectorFrom.currencyIndex, selectorTo.currencyIndex)
                    //    }
                    //}
                    //function update() {
                    //    text = convert(inputTo.text, selectorTo.currencyIndex, selectorFrom.currencyIndex)
                    //}
                }
            }

            Row {
                spacing: units.gu(1)
                Button {
                    objectName: "button"
                    width: pageLayout.width - 2 * root.margins - root.buttonWidth

                    text: i18n.tr("Power On")

                    onClicked: {
                        myType.subscribeUDP(inputIP.text, 10000, inputMac.text);
                        myType.switchOnUDP(inputIP.text, 10000, inputMac.text);
                        //myType.helloWorld = i18n.tr("..from Cpp Backend")
                    }
                }

                Button {
                    objectName: "button"
                    width: pageLayout.width - 2 * root.margins - root.buttonWidth

                    text: i18n.tr("Power Off")

                    onClicked: {
                        myType.subscribeUDP(inputIP.text, 10000, inputMac.text);
                        myType.switchOffUDP(inputIP.text, 10000, inputMac.text);
                        //myType.helloWorld = i18n.tr("..from Cpp Backend")
                    }
                }
            }
        }
    }
}

