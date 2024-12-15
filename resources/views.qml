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
                interactive: true

                // Empêcher le Flickable de réagir à la molette
                WheelHandler {
                    id: zoomWheelHandler
                    acceptedDevices: PointerDevice.Mouse

                    onWheel: (event) => {
                        event.accepted = true; // Bloquer le comportement par défaut
                        var scaleFactor = 0.1; // Facteur de zoom

                        if (event.angleDelta.y > 0) {
                            gridScale.xScale = Math.min(gridScale.xScale + scaleFactor, 3.0);
                            gridScale.yScale = Math.min(gridScale.yScale + scaleFactor, 3.0);
                        } else {
                            gridScale.xScale = Math.max(gridScale.xScale - scaleFactor, 0.5);
                            gridScale.yScale = Math.max(gridScale.yScale - scaleFactor, 0.5);
                        }
                    }
                }

                property int hexWidth: 80  // Largeur de l'hexagone
                property int hexHeight: 70 // Hauteur effective de l'hexagone

                // Centrer la vue initiale sur le milieu des 2000 hexagones
                Component.onCompleted: {
                    var index = 2000 /2 + 20 / 2; // Index de l'hexagone central
                    var row = Math.floor(index / 20);
                    var col = index % 20;

                    var hexWidth = contentArea.hexWidth;
                    var hexHeight = contentArea.hexHeight;

                    var x = col * (hexWidth + hexWidth / 2) + ((row % 2) * (hexWidth + hexWidth / 2) / 2);
                    var y = row * (hexHeight * 0.5);

                    contentX = x - (width / 2);
                    contentY = y - (height / 2);
                }

                Rectangle {
                    id: hexGrid
                    width: contentWidth
                    height: contentHeight
                    color: "transparent"
                    transform: Scale {
                        id: gridScale
                        xScale: 1.0
                        yScale: 1.0
                        origin.x: contentArea.width / 2
                        origin.y: contentArea.height / 2
                    }

                    // Repeater pour générer la grille d'hexagones
                    Repeater {
                        id: hexRepeater
                        model: 2000 // Nombre total d'hexagones

                        delegate: Canvas {
                            id: hexagonCanvas
                            width: contentArea.hexWidth // Largeur de l'hexagone
                            height: contentArea.hexHeight // Hauteur effective de l'hexagone

                            // Positionnement en quinconce
                            x: (model.index % 20) * (width + contentArea.hexWidth / 2) +
                                ((Math.floor(model.index / 20) % 2) * (width + contentArea.hexWidth / 2) / 2)
                            y: Math.floor(model.index / 20) * (height * 0.5)

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
