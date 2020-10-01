/**
    PowaFi - Controls power state of WiFi controlled power switches on your local network
    Copyright (C) 2015  Michael Saunders

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
    
    This program comes with ABSOLUTELY NO WARRANTY
    This is free software, and you are welcome to redistribute it
    under certain conditions
*/

import QtQuick 2.0
import Ubuntu.Components 1.1
import Ubuntu.Components.Popups 1.1
import Ubuntu.Components.ListItems 1.1
import PowaFi 1.0
/*!
    \brief MainView with a Label and Button elements.
*/

MainView {
    id: root
    // objectName for functional testing purposes (autopilot-qt5)
    objectName: "mainView"

    // Note! applicationName needs to match the "name" field of the click manifest
    applicationName: "powafi.org.awiki.kamikaze"

    /*
     This property enables the application to change orientation
     when the device is rotated. The default is false.
    */
    //automaticOrientation: true

    // Removes the old toolbar and enables new features of the new header.
    useDeprecatedToolbar: false

    width: units.gu(100)
    height: units.gu(75)

    property real margins: units.gu(2)
    property real buttonWidth: units.gu(20)

    Page {
        function startupComplete() {
            myType.startDiscover();
        }

        Component.onCompleted: startupComplete();

        title: i18n.tr("PowaFi")

        MyType {
            id: myType
        }

        Column {
            id: pageLayout
            spacing: units.gu(1)
            anchors {
                margins: root.margins
                fill: parent
            }

            Label {
                id: labelInfo
                objectName: "labelInfo"

                text: "PowaFi will allow you to toggle\nthe power state of ARLINK or Orvibo\ndevices on your local network.\n\nTo use it, select a device\nand use the power buttons below.\n"
                //width: units.gu(5)
            }

            Row {

                Button {
                    id: rescanButton
                    objectName: "rescanButton"
                    width: pageLayout.width - 2 * root.margins - root.buttonWidth

                    text: "Rescan"

                    onClicked: {
                        myType.startDiscover();
                        update();
                    }
                }
            }

            Row {

                Button {
                    id: selectButton
                    objectName: "selectButton"
                    property int deviceIndex: 0
                    property TextField inputMac: inputMac
                    property TextField inputIP: inputIP
                    width: pageLayout.width - 2 * root.margins - root.buttonWidth

                    text: " -- Select Device -- "

                    onClicked: {
                        listDevices.updateDeviceList();
                        update();
                        PopupUtils.open(deviceSelector, selectButton)
                    }
                }

                ListModel {
                    id: listDevices
                    ListElement {
                        mac: '--Select Device--'
                        ip: ''
                    }

                    function updateDeviceList() {
                        listDevices.clear();
                        for(var i = 0; i < myType.getNumberDevicesFound(); i++) {
                            console.debug(myType.getDeviceMac(i));
                            listDevices.append({"mac": myType.getDeviceMac(i), "ip": myType.getDeviceIp(i) });
                        }
                        if(listDevices.count == 0) {
                            listDevices.append({"mac": "No Devices On LAN", "ip": '' });
                        }
                    }



                    function getMac(idx) {
                        return (idx >=0 && idx < count) ? get(idx).mac : "";
                    }

                    function getIP(idx) {
                        return (idx >=0 && idx < count) ? get(idx).ip : "";
                    }
                }

                Component {
                    id: deviceSelector

                    Popover {
                        Column {
                            anchors {
                                top: parent.top
                                left: parent.left
                                right: parent.right
                            }
                            height: pageLayout.height
                            Header {
                                id: header
                                text: i18n.tr("Select device by MAC address")
                            }
                            ListView {
                                clip: true
                                width: parent.width
                                height: parent.height - header.height
                                model: listDevices
                                delegate: Standard {
                                    objectName: "popoverDeviceSelector"
                                    text: mac
                                    onClicked: {
                                        caller.deviceIndex = index
                                        caller.inputMac.update()
                                        caller.inputIP.update()
                                        hide()
                                    }
                                }
                            }
                        }
                    }
                }

            }

            Row {
                spacing: units.gu(1)

                Label {
                    id: labelMac
                    objectName: "labelMac"

                    text: "MAC"
                    width: units.gu(5)
                }

                TextField {
                    id: inputMac
                    objectName: "inputMac"
                    errorHighlight: false
                    width: pageLayout.width - 2 * root.margins - root.buttonWidth
                    height: units.gu(5)
                    font.pixelSize: FontUtils.sizeToPixels("medium")
                    text: ''

                    function update() {
                        text = listDevices.getMac(selectButton.deviceIndex)
                    }
                }
            }

            Row {
                spacing: units.gu(1)

                Label {
                    id: labelIp
                    objectName: "labelIp"

                    text: "IP"
                    width: units.gu(5)
                }

                TextField {
                    id: inputIP
                    objectName: "inputIP"
                    errorHighlight: false
                    width: pageLayout.width - 2 * root.margins - root.buttonWidth
                    height: units.gu(5)
                    font.pixelSize: FontUtils.sizeToPixels("medium")
                    text: ''

                    function update() {
                        text = listDevices.getIP(selectButton.deviceIndex)
                    }
                }
            }

            Row {
                spacing: units.gu(1)
                Button {
                    objectName: "buttonOn"
                    width: pageLayout.width - 2 * root.margins - root.buttonWidth

                    text: i18n.tr("Power On")

                    onClicked: {
                        myType.subscribeUDP(inputIP.text, 10000, inputMac.text);
                        myType.switchOnUDP(inputIP.text, 10000, inputMac.text);
                    }
                }

                Button {
                    objectName: "buttonOff"
                    width: pageLayout.width - 2 * root.margins - root.buttonWidth

                    text: i18n.tr("Power Off")

                    onClicked: {
                        myType.subscribeUDP(inputIP.text, 10000, inputMac.text);
                        myType.switchOffUDP(inputIP.text, 10000, inputMac.text);
                    }
                }
            }


            Label {
                x: units.gu(2)
                y: pageLayout.height - units.gu(10)

                id: labelTM
                objectName: "labelTM"
                property string fontSize: "smaller"
                font.pixelSize: FontUtils.sizeToPixels(fontSize)
                text: "This application is not endorsed by Arlec or Orvibo\n\nARLINK and Orvibo are trademarked\nby their respective companies."
            }
            /*
            // Never figured out how to get this working properly, although
            // after configuring the Popup, I think I just didn't realise it needed to be
            // called to be 'opened' and wasn't an item that displayed on the screen by default.
            UbuntuListView {
                id: inputDevice
                objectName: "inputDevice"
                //anchors.fill: parent
                anchors {
                    left: parent.left
                    right: parent.right
                }
                //Layout.fillHeight: true

                //clip: true
                width: pageLayout.width - 2 * root.margins - root.buttonWidth
                height: units.gu(5)

                model: listDevices

                delegate: ListItem.Expandable {
                    id: expandingItem
                    divider.visible: true
                    expandedHeight: units.gu(20)
                    Label {
                        id: labelDiscover
                        text: "Click to see nearby devices"
                    }

                    onClicked: {
                        listDevices.clear();
                        var deviceList = myType.getDiscoveredDevices();
                        inputDevice.expandedIndex = index; // deviceList.length;
                        for(var i = 0; i < deviceList.length; i++) {
                            listDevices.append(deviceList)
                        }
                        inputDevice.update();
                        //caller.input.update();
                    }
                }
            }
            */
        }
    }
}

