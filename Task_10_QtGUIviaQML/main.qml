import QtQuick 2.5
import QtQuick.Window 2.2
import Qt.labs.folderlistmodel 2.1
import QtQuick.Dialogs
import QtQuick.Controls 2.3

ApplicationWindow {
    id: mainWindow
    minimumWidth: 850
    minimumHeight: 800
    visible: true
    title: qsTr("ImageViewer")

    menuBar: MenuBar {
                Menu {
                    title: "File"
                    MenuItem {
                        text: "Open"
                        onTriggered:{
                            folderDialog.open()
                        }
                    }

                }

                Menu {
                    title: "View"
                    MenuItem {
                        text: "List"
                        onClicked: {
                            listView.visible = true
                            tableView.visible = false
                            pathView.visible = false
                    }
                    }
                    MenuItem {
                        text: "Table"
                        onClicked: {
                            listView.visible = false
                            tableView.visible = true
                            pathView.visible = false
                    }
                    }
                    MenuItem {
                        text: "Path"
                        onClicked: {
                            listView.visible = false
                            tableView.visible = false
                            pathView.visible = true
                    }
                    }
                }
    }

    FolderDialog {
        id: folderDialog
        onAccepted: {
            currentFolder: folderDialog.fileUrl
            _imageDirectory.directory_path = folderDialog.currentFolder
        }
    }

    Rectangle{
        id: tableView
        anchors.fill: parent
        visible: false;

        ScrollView {
            width: parent.width
            height: parent.height
            ScrollBar.vertical.width: 20

            Grid {
                width: parent.width - 20
                height: parent.height
                anchors.margins: 10
                spacing: 10

                Repeater{
                   model:  _imageDirectory.image_list

                   Image {
                       source: model.modelData
                       width: 200
                       fillMode: Image.PreserveAspectFit

                       MouseArea{
                           width: parent.width
                           height: parent.height

                           onClicked:  {
                               showImage.source = model.modelData
                               showImage.visible = true
                           }
                       }
                   }
                }
            }
        }
    }

    Rectangle{
        id: listView
        anchors.fill: parent
        visible: false

        ScrollView {
            width: listView.width
            height: listView.height

            Column{
               width: listView.width
               height: listView.height
               spacing: 10

               Repeater{
                  model:  _imageDirectory.image_list

                  Image {
                      source:  model.modelData
                      anchors.horizontalCenter: parent.horizontalCenter
                      width: parent.width / 2
                      fillMode: Image.PreserveAspectFit

                      MouseArea{
                          width: parent.width
                          height: parent.height

                          onClicked:  {
                              showImage.source =  model.modelData
                              showImage.visible = true
                          }
                      }
                  }
               }
           }
        }
    }

    Rectangle{
        id: pathView
        anchors.fill: parent
        visible: true;

        PathView {
            id: pathViewComponent
            anchors.fill: parent

            model: _imageDirectory.image_list
            snapMode: PathView.SnapOneItem
            pathItemCount: 5

            path: Path {
                id: path
                startX: -50
                startY: pathViewComponent.height / 2

                PathLine {
                    id: pathLine
                    x: pathViewComponent.width - 50
                    y: pathViewComponent.height / 2
                    relativeX: x + 100
                }
            }

            delegate: Item {
                id: itemListInfo
                height: 100
                width: 100

                Rectangle {
                    anchors.fill: parent
                    color: "transparent"

                    Image {
                        anchors.fill: parent
                        source: model.modelData
                    }

                    MouseArea {
                        width: parent.width
                        height: parent.height

                        onClicked:  {
                            showImage.source = model.modelData
                            showImage.visible = true
                        }
                    }
                }
            }
        }
    }

    Image {
        id: showImage
        visible: false
        anchors.fill: parent
        MouseArea{
            width: parent.width
            height: parent.height

            onClicked:  {
                showImage.visible = false
            }
        }
    }
}
