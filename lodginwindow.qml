import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material

Window {
    id: root
    width: 900
    height: 675
    visible: true
    title: qsTr("Hello World")

    flags: Qt.FramelessWindowHint
    color: "#00000000"

    property int dragX: 0 //初始为0
    property int dragY: 0
    property bool dragging: false

    // 外矩形
    Rectangle{
        width: parent.width
        height: parent.height
        opacity: 0.8

        gradient: Gradient{
            GradientStop{ position: 0.2; color: "#aabdde"}
            GradientStop{ position: 1.0; color: "#f870b8"}
            // GradientStop{ position: 0.2; color: "#f8f7ae"}
            // GradientStop{ position: 1.0; color: "#a3da62"}
        }
        radius: 20

        MouseArea
        {
            anchors.fill: parent
            onPressed:
            {
                root.dragX = mouseX
                root.dragY = mouseY
                root.dragging = true
            }
            onReleased: root.dragging = false
            onPositionChanged:
            {
                if(root.dragging)
                {
                    root.x += mouseX - root.dragX
                    root.y += mouseY - root.dragY
                }
            }
        }
        // }
    }

    // 内矩形
    Rectangle{
        id: rect2
        width: 700
        height: 525
        radius: 10
        anchors.centerIn: parent
        color: "#f4f4f4"
        opacity: 0.95

        // gradient: Gradient
        // {
        //     GradientStop{ position: 0.8; color: "#ffffff"}
        //     GradientStop{ position: 1.0; color: "#dee2ed"}
        // }

        Text{
            id: ttt
            text: qsTr("欢迎登录")
            font.family: "font/font.ttf"
            font.pixelSize: 30
            font.bold: true
            anchors.right: parent.right
            anchors.rightMargin: 165
            y: 120
            color:"#ff3192"
        }

        // 账号输入框
        TextField{
            id: tf1
            width: 250
            height: 50
            placeholderText: qsTr("账号")
            font.family: "font/font.ttf"
            font.pixelSize: 18
            anchors.horizontalCenter: ttt.horizontalCenter
            y: ttt.y + tf1.height + 40
        }

        // 密码输入框
        TextField {
            id: tf2
            width: 250
            height: 50
            placeholderText: qsTr("密码")
            anchors.horizontalCenter: ttt.horizontalCenter
            y: ttt.y + tf1.height + tf2.height + 70
            font.family: "font/font.ttf"
            font.pixelSize: 18
            echoMode: TextInput.Password  // 设置为密码模式，输入时显示为星号或点
        }


        Button{
            id: b1
            width: 250
            height: 60
            text: qsTr("登录")
            font.family: "font/font.ttf"
            font.pixelSize: 18
            anchors.horizontalCenter: ttt.horizontalCenter
            y: ttt.y + tf1.height + tf2.height + 90 + b1.height

            onClicked: root.close()

            // 按钮的3种状态
            // background: Rectangle{
            //     radius: 25
            //     color: {
            //         if(b1.down)
            //             return "#e91e63"
            //         if(b1.hovered)
            //             return "#ffa744"
            //         return "#d6d7d7"
            //     }
            // }
        }

        Image {
            id: i1
            source: "gui/images/bk2.png"
            width: 348.3
            height: 525
            x: -35
            y: 2
            opacity: 1
        }
    }
}
