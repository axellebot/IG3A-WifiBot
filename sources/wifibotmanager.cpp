#include "wifibotmanager.h"

WifiBotManager::WifiBotManager(QObject *parent):
    QObject(parent)
{
    initStatements();

    timer = new QTimer();
    timer->setInterval(INTERVAL_SYNC);
    connect(timer, SIGNAL(timeout()), this, SLOT(tick()));

    manager = new QNetworkAccessManager();

}

WifiBotManager::~WifiBotManager(){
    delete manager;
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
    if(cameraConnected){
        if(cameraTiltUp){
            QUrl url("http://"+ ipAddress +":" +QString::number(cameraPortAddress)+ "?"+PARAM_CAMERA_UP);
            request.setUrl(url);
            manager->get(request);
        }
        else if(cameraTiltDown){
            QUrl url("http://" + ipAddress +":" +QString::number(cameraPortAddress) +  "?" + PARAM_CAMERA_DOWN);
            request.setUrl(url);
            manager->get(request);
        }
        else if(cameraPanLeft){
            QUrl url("http://" + ipAddress+":" +QString::number(cameraPortAddress) +  "?" + PARAM_CAMERA_LEFT);
            request.setUrl(url);
            manager->get(request);
        }
        else if(cameraPanRight){
            QUrl url("http://" + ipAddress+":" +QString::number(cameraPortAddress) +  "?" + PARAM_CAMERA_RIGHT);
            request.setUrl(url);
            manager->get(request);
        }
    }

    sendBuffer.clear();

    sendBuffer.append((char)0xff);
    sendBuffer.append((char)0x07);

    if(moveForward){
        if(moveForward){
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
    else if(moveBackward){
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

    quint16 crc = crc16(sendBuffer ,sendBuffer.size());
    sendBuffer.append((char)crc);
    sendBuffer.append((char)(crc>>8));

}

void WifiBotManager::sendMessage(){
    tcp.write(sendBuffer);
    tcp.flush();
}

void WifiBotManager::recvMessage(){
    char recvBuffer[21];
    tcp.read(recvBuffer, 21);

    this->speedSensorL = (recvBuffer[1]<<8) +recvBuffer[0];
    this->speedSensorR = ((((quint16)recvBuffer[10])<<8) + (quint16)recvBuffer[9]);
    if (this->speedSensorL > 32767) this->speedSensorL -= 65536;
    if (this->speedSensorR > 32767) this->speedSensorR -= 65536;

    this->odomL = ((long)(recvBuffer[8])<<24) + ((long)(recvBuffer[7])<<16) + ((long)(recvBuffer[6])<<8) + ((long)(recvBuffer[5]));
    this->odomR = ((long)(recvBuffer[16])<<24) + ((long)(recvBuffer[15])<<16) + ((long)(recvBuffer[14])<<8) + ((long)(recvBuffer[13]));
    this->batterySensor = (((unsigned int)(unsigned char)(recvBuffer[2])) * 100.0 / 255.0);
    this->proximitySensor1 = (int) recvBuffer[3];
    this->proximitySensor2 = (int) recvBuffer[4];

}

quint16 WifiBotManager::crc16(QByteArray adresse_tab , unsigned char taille_max) {
    unsigned int crc = 0xFFFF;
    unsigned int polynome = 0xA001;
    unsigned int cptOctet = 0;
    unsigned int cptBit = 0;
    unsigned int parity= 0;
    crc = 0xFFFF;
    polynome = 0xA001;
    for( cptOctet= 1 ; cptOctet < taille_max ; cptOctet++) {
        crc ^= (unsigned char)( adresse_tab[cptOctet]);
        for( cptBit = 0; cptBit <= 7 ; cptBit++) {
            parity= crc;
            crc >>= 1;
            if(parity%2 == true) crc ^= polynome;
        }
    }
    return (crc);
}
