#include "wifibotmanager.h"

WifiBotManager::WifiBotManager(QObject *parent):
    QObject(parent)
{
    initStatements();

    timer = new QTimer();
    timer->setInterval(INTERVAL_SYNC);
    connect(timer, SIGNAL(timeout()), this, SLOT(tick()));
}

WifiBotManager::~WifiBotManager(){
    delete camera;
    delete timer;
}

void WifiBotManager::initStatements(){
    this->resetStatements();
}

void WifiBotManager::resetStatements()
{
    connected=false;
    batterySensor=0;
    proximitySensor1 = 0;
    proximitySensor2 = 0;
    cameraConnected=false;
    moveForward=false;
    moveBackward=false;
    moveRotationLeft=false;
    moveRotationRight=false;
    cameraTiltUp=false;
    cameraTiltDown=false;
    cameraPanLeft=false;
    cameraPanRight=false;
    speed=0;

    sendBuffer.clear();
}

bool WifiBotManager::connectTo(QString ipAddress, quint16 portAddress, quint16 cameraPortAddress){
    qDebug()<<"Connection to "<< ipAddress << ':'<< portAddress;
    resetStatements();
    tcp.connectToHost(ipAddress, portAddress);
    if(tcp.waitForConnected(5000)){
        this->connected = true;
        timer->start();
        this->ipAddress = ipAddress;
        this->portAddress = portAddress;
        this->cameraPortAddress =cameraPortAddress;
        return true;
    }
    else{
        this->connected = false;
        return false;
    }
}

void WifiBotManager::disconnect(){
    tcp.disconnectFromHost();
    resetStatements();
}

void WifiBotManager::tick(){
    qDebug("Tick");
    genMessage();
    sendMessage();
    recvMessage();
}

void WifiBotManager::genMessage(){
    /*// If Camera is connected
    if(camera){
        if(cameraTiltUp){
            QUrl url("http://"+ ipAddress +":" +cameraPortAddress+ "?"+PARAM_CAMERA_UP);
            camera->get(QNetworkRequest(url));
        }
        else if(cameraTiltDown){
            QUrl url("http://" + ipAddress +":" +cameraPortAddress +  "?" + PARAM_CAMERA_DOWN);
            camera->get(QNetworkRequest(url));
        }
        else if(cameraPanLeft){
            QUrl url("http://" + ipAddress+":" +cameraPortAddress +  "?" + PARAM_CAMERA_LEFT);
            camera->get(QNetworkRequest(url));
        }
        else if(cameraPanRight){
            QUrl url("http://" + ipAddress+":" +cameraPortAddress +  "?" + PARAM_CAMERA_RIGHT);
            camera->get(QNetworkRequest(url));
        }
    }
    */
    sendBuffer.clear();
    /*
    sendBuffer.append((char)0xff);
    sendBuffer.append((char)0x09);

    if(moveForward){
        if(moveForward&& this->proximitySensor1 > 0){
            if(moveRotationRight){
                sendBuffer.append((char)speed);
                sendBuffer.append((char)0x00);
                sendBuffer.append((char)(speed/2));
                sendBuffer.append((char)0x00);
            }
            else if(moveRotationLeft){
                sendBuffer.append((char)(speed/2));
                sendBuffer.append((char)0x00);
                sendBuffer.append((char)speed);
                sendBuffer.append((char)0x00);
            }
            else{
                sendBuffer.append((char)speed);
                sendBuffer.append((char)0x00);
                sendBuffer.append((char)speed);
                sendBuffer.append((char)0x00);
            }
        }
    }
    else if(moveBackward && this->proximitySensor2 > 0){
        if(moveRotationRight){
            sendBuffer.append((char)speed);
            sendBuffer.append((char)0x00);
            sendBuffer.append((char)(speed/2));
            sendBuffer.append((char)0x00);
        }
        else if(moveRotationLeft){
            sendBuffer.append((char)(speed/2));
            sendBuffer.append((char)0x00);
            sendBuffer.append((char)speed);
            sendBuffer.append((char)0x00);
        }
        else{
            sendBuffer.append((char)speed);
            sendBuffer.append((char)0x00);
            sendBuffer.append((char)speed);
            sendBuffer.append((char)0x00);
        }
    }
    else if(moveRotationLeft || moveRotationRight){
        sendBuffer.append((char)speed);
        sendBuffer.append((char)0x00);
        sendBuffer.append((char)speed);
        sendBuffer.append((char)0x00);
    }
    else{
        sendBuffer.append((char)0x00);
        sendBuffer.append((char)0x00);
        sendBuffer.append((char)0x00);
        sendBuffer.append((char)0x00);
    }
    if(moveForward)
        //avant / avant
        sendBuffer.append((char)0b01010000);
    //arrière / arrière
    else if(moveBackward)
        sendBuffer.append((char)0b00000000);
    //arrière / avant
    else if(moveRotationLeft)
        sendBuffer.append((char)0b00010000);
    //avant / arrière
    else if(moveRotationRight)
        sendBuffer.append((char)0b01000000);
    else
        sendBuffer.append((char)0b01010000);
        */

    sendBuffer.append((char)0x09);
    sendBuffer.append((char)0xff);
    sendBuffer.append((char)0x00);
    sendBuffer.append((char)0xff);
    sendBuffer.append((char)0x00);
    sendBuffer.append((char)0x50);
    quint16 crc = qChecksum(sendBuffer.data(), sendBuffer.length());
    sendBuffer.append((char)crc);
    sendBuffer.append((char)crc>>8);
    sendBuffer.prepend((char)0xff);

}

void WifiBotManager::sendMessage(){
    tcp.write(sendBuffer);
    tcp.flush();
}

void WifiBotManager::recvMessage(){
    char recvBuffer[21];
    tcp.read(recvBuffer, 21);
    this->batterySensor = (((unsigned int)((unsigned char)recvBuffer[2])) * 100.0 / 255.0);
    this->proximitySensor1 = (int) recvBuffer[3];
    this->proximitySensor2 = (int) recvBuffer[4];
}

quint16 WifiBotManager::crc16(QByteArray byteArray, int pos){
    unsigned char *data = (unsigned char* )byteArray.constData();
    quint16 crc = 0xFFFF;
    quint16 Polynome = 0xA001;
    quint16 Parity = 0;
    for(; pos < byteArray.length(); pos++){
        crc ^= *(data+pos);
        for (unsigned int CptBit = 0; CptBit <= 7 ; CptBit++){
            Parity= crc;
            crc >>= 1;
            if (Parity%2 == true) crc ^= Polynome;
        }
    }
    return crc;
}

