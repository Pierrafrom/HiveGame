import QtQuick 2.15
import QtQuick.Controls 2.15

ApplicationWindow {
    visible: true
    width: 800
    height: 600
    title: "Page de jeu avec grille hexagonale"

    Rectangle {
        id: mainPage
        anchors.fill: parent
        color: "lightgray"

        // Bandeau supérieur
        Rectangle {
            id: topBanner
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            height: parent.height * 0.1
            color: "darkblue"

            Button {
                id: backButton
                text: "Retour"
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 10
                onClicked: console.log("Bouton Retour cliqué")
            }

            Text {
                id: titleText
                text: "Titre de la page"
                color: "white"
                font.pixelSize: 18
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
            }

            Row {
                id: actionButtons
                spacing: 10
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.rightMargin: 10

                Button { text: "Settings" }
                Button { text: "Undo" }
                Button { text: "Redo" }
                Button { text: "Save" }
            }
        }

        // Conteneur principal
        Rectangle {
            id: mainContainer
            anchors.top: topBanner.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            color: "white"

            // Menu à droite
            Rectangle {
                id: sideMenu
                width: parent.width * 0.3
                anchors.top: parent.top
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                color: "gray"
                Text {
                    anchors.centerIn: parent
                    text: "Menu"
                    color: "white"
                    font.pixelSize: 20
                }
            }

            // Zone de contenu avec grille hexagonale
            Flickable {
                id: contentArea
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.right: sideMenu.left
                anchors.bottom: parent.bottom
                contentWidth: 2000
                contentHeight: 2000
                clip: true

                Rectangle {
                    id: hexGrid
                    width: contentWidth
                    height: contentHeight
                    color: "transparent"

                    // Repeater pour générer la grille d'hexagones
                    Repeater {
                        id: hexRepeater
                        model: 100 // Nombre total d'hexagones

                        delegate: Canvas {
                            id: hexagonCanvas
                            width: 60 // Largeur de l'hexagone
                            height: 52 // Hauteur effective de l'hexagone

                            // Positionnement en quinconce
                            x: (model.index % 10) * (width + 30) +
                                ((Math.floor(model.index / 10) % 2) * (width + 30) / 2)
                            y: Math.floor(model.index / 10) * (height * 0.5)

                            onPaint: {
                                var ctx = getContext("2d");
                                ctx.clearRect(0, 0, width, height);

                                // Définir les points de l'hexagone
                                var size = width / 2; // Rayon de l'hexagone
                                var centerX = width / 2;
                                var centerY = height / 2;

                                ctx.beginPath();
                                for (var i = 0; i < 6; i++) {
                                    var angle = Math.PI / 3 * i; // Diviser en 6 segments
                                    var x = centerX + size * Math.cos(angle);
                                    var y = centerY + size * Math.sin(angle);
                                    if (i === 0) ctx.moveTo(x, y);
                                    else ctx.lineTo(x, y);
                                }
                                ctx.closePath();

                                // Dessiner l'hexagone
                                ctx.fillStyle = "lightblue"; // Remplissage
                                ctx.fill();
                                ctx.strokeStyle = "black"; // Bordure
                                ctx.lineWidth = 1;
                                ctx.stroke();
                            }

                            MouseArea {
                                anchors.fill: parent
                                onClicked: console.log("Hexagone", model.index, "cliqué");
                            }
                        }
                    }
                }
            }
        }
    }
}
