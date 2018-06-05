#ifndef WIFIBOTMANAGER_H
#define WIFIBOTMANAGER_H

#include <QObject>
#include <QTimer>
#include <QDebug>
#include <QMessageBox>
#include <QInputDialog>
#include <QIcon>
#include <QUrl>
#include <QNetworkAccessManager>
#include <QtNetwork>

#include <string>

using namespace std;

#define PARAM_CAMERA_UP "action=command&dest=0&plugin=0&id=10094853&group=1&value=-200"
#define PARAM_CAMERA_DOWN "action=command&dest=0&plugin=0&id=10094853&group=1&value=200"
#define PARAM_CAMERA_LEFT "action=command&dest=0&plugin=0&id=10094852&group=1&value=200"
#define PARAM_CAMERA_RIGHT "action=command&dest=0&plugin=0&id=10094852&group=1&value=-200"

#define INTERVAL_SYNC 75

class WifiBotManager : public QObject
{
    Q_OBJECT

private :
    // Cameras
    QNetworkRequest request;
    QNetworkAccessManager *manager;

    // Connections
    QByteArray sendBuffer;

    QTimer *timer;


    // Messaging
    void genMessage();
    void sendMessage();
    void recvMessage();

public:
    // Connections
    QTcpSocket tcp;
    bool connected;
    QString ipAddress;
    quint16 portAddress;
    quint16 cameraPortAddress;
    bool cameraConnected;

    // Moves
    bool moveForward;
    bool moveBackward;
    bool moveRotationLeft;
    bool moveRotationRight;
    quint16 speed;

    // Camera Moves
    bool cameraTiltUp;
    bool cameraTiltDown;
    bool cameraPanLeft;
    bool cameraPanRight;

    // Sensors
    quint16 batterySensor;
    int proximitySensor1;
    int proximitySensor2;

    WifiBotManager(QObject *parent=0);
    ~WifiBotManager();

    /**
     * @brief initStatements
     */
    void initStatements();

    /**
     * @brief resetStatements
     */
    void resetStatements();

    /**
     * @brief connectTo
     * @param ipAddress
     * @param portAddres
     * @param cameraPortAddres
     * @return
     */
    bool connectTo(QString ipAddress, quint16 portAddress, quint16 cameraPortAddress);

    /**
     * @brief WifiBotManager::disconnect
     */
    void disconnect();

    /**
     * @brief crc16
     * @param byteArray
     * @param pos
     * @return
     */
    quint16 crc16(QByteArray adresse_tab , unsigned char taille_max);

public slots :
    void tick();
};

#endif // WIFIBOTMANAGER_H
