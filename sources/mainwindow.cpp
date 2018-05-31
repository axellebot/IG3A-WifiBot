#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    manager = new WifiBotManager(this);

    resetMovesSatements();
    initComponents();

    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(syncData()));
    timer->setInterval(INTERVAL_SYNC);
    timer->start();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete manager;
    delete timer;
    delete cameraView;
    delete backwardSensor;
    delete forewardSensor;
    delete battery;
}

void MainWindow::resetMovesSatements(){
    moveForward=false;
    moveBackward=false;
    moveRotationLeft=false;
    moveRotationRight=false;
    cameraTiltUp=false;
    cameraTiltDown=false;
    cameraPanLeft=false;
    cameraPanRight=false;
}

// Initialize the interface
void MainWindow::initComponents()
{
    // mainToolBar
    ui->toolBar->addWidget(new QLabel("Battery"));
    battery = new QProgressBar();
    battery->setFormat("%p%");

    ui->toolBar->addWidget(battery);
    ui->toolBar->addSeparator();

    forewardSensor = new QRadioButton();
    forewardSensor->setText("Avant");
    backwardSensor = new QRadioButton();
    backwardSensor->setText("Arrière");
    ui->toolBar->addWidget(forewardSensor);
    ui->toolBar->addWidget(backwardSensor);

    ui->toolBar->addSeparator();

    // Camera
    cameraView= new QWebEngineView();

    ui->frameLayout->addWidget(cameraView);

    setInterfaceEnabled(false);
}

/** BUTTON CLICKED **/
void MainWindow::on_btnMoveForward_pressed()
{
    qDebug("Move Forward Pressed");
    moveForward=true;
}

void MainWindow::on_btnMoveForward_released()
{
    qDebug("Move Forward Released");
    moveForward=false;
}

void MainWindow::on_btnMoveBackward_pressed()
{
    qDebug("Move Backward Pressed");
    moveBackward=true;
}

void MainWindow::on_btnMoveBackward_released()
{
    qDebug("Move Backward Released");
    moveBackward=false;
}

void MainWindow::on_btnMoveRotateLeft_pressed()
{
    qDebug( "Move Rotate Left Pressed" );
    moveRotationLeft=true;
}

void MainWindow::on_btnMoveRotateLeft_released()
{
    qDebug("Move Rotate Left Released");
    moveRotationLeft=false;
}

void MainWindow::on_btnMoveRotateRight_pressed()
{
    qDebug( "Move Rotate Right Pressed" );
    moveRotationRight=true;
}

void MainWindow::on_btnMoveRotateRight_released()
{
    qDebug( "Move Rotate Right Released" );
    moveRotationRight=false;
}

void MainWindow::on_btnCameraTiltUp_pressed()
{
    qDebug( "Camera Tilt Up Pressed" );
    cameraTiltUp = true;
}

void MainWindow::on_btnCameraTiltUp_released()
 {
     qDebug( "Camera Tilt Up Released");
     cameraTiltUp = false;
 }

void MainWindow::on_btnCameraTiltDown_pressed()
{
    qDebug( "Camera Tilt Down Pressed");
    cameraTiltDown = true;
}

void MainWindow::on_btnCameraTiltDown_released(){
    qDebug( "Camera Tilt Down Released");
    cameraTiltDown = false;
}

void MainWindow::on_btnCameraPanLeft_pressed()
{
    qDebug( "Camera Rotate Left Pressed");
    cameraPanLeft = true;
}

void MainWindow::on_btnCameraPanLeft_released(){
    qDebug( "Camera Rotate Left Released" );
    cameraPanLeft = false;
}

void MainWindow::on_btnCameraPanRight_pressed(){
    qDebug( "Camera Rotate Right Pressed" );
    cameraPanRight=true;
}

void MainWindow::on_btnCameraPanRight_released(){
    qDebug( "Camera Rotate Right Released" );
    cameraPanRight=false;
}

void MainWindow::on_sliderSpeed_valueChanged(int value)
{
    qDebug( "Moves sensitivity at : %d",value );
    ui->lblMoveSpeed->setText(QString::number(value)); // update slider value
}

/** MENU ACTIONS **/
void MainWindow::on_actionConnectTo_triggered()
{
    qDebug("Connection");
    ConnectionDialog * dlg = new ConnectionDialog(this, manager);
    // Accepted
    if (dlg->exec() == 1)
    {
        QMessageBox::information(this, "Connection","Connection succeed", QMessageBox::Ok);
        setInterfaceEnabled(true);
    }
    // Refused
    else
    {
        QMessageBox::warning(this, "Connexion","Connection failed", QMessageBox::Ok);
        setInterfaceEnabled(false);
    }
}

void MainWindow::on_actionDisconnect_triggered()
{
    manager->disconnect();
    QMessageBox::information(this, "Deconnexion","Disconnect succeed", QMessageBox::Ok);
    setInterfaceEnabled(false);
}

/** OTHER **/
// Get the user interface
Ui::MainWindow *MainWindow::getUI()
{
    return this->ui;
}

void MainWindow::syncData(){
    /*
     * Push Datas
     */
    // Moves
    manager->speed=ui->sliderSpeed->value();
    manager->moveForward=this->moveForward;
    manager->moveBackward=this->moveBackward;
    manager->moveRotationLeft=this->moveRotationLeft;
    manager->moveRotationRight=this->moveRotationRight;
    manager->speed=this->speed;

    // Camera
    manager->cameraTiltUp=this->cameraTiltUp;
    manager->cameraTiltDown=this->cameraTiltDown;
    manager->cameraPanLeft=this->cameraPanLeft;
    manager->cameraPanRight=this->cameraPanRight;


    /*
     * Pull Datas
     */
    if(manager->connected){
        battery->setValue(manager->batterySensor);
        forewardSensor->setChecked(manager->proximitySensor1<0);
        backwardSensor->setChecked(manager->proximitySensor2<0);

        if(!manager->cameraConnected){
            QUrl url = QUrl("http://"+ this->manager->ipAddress + ":"+QString::number(this->manager->cameraPortAddress)+"/?action=stream");
            cameraView->load(url);
            cameraView->show();
            manager->cameraConnected = true;
        }
    }
}


void MainWindow::setInterfaceEnabled(bool b){
    ui->actionConnectTo->setEnabled(!b);
    ui->actionDisconnect->setEnabled(b);

    // Moves UI
    ui->btnMoveForward->setEnabled(b);
    ui->btnMoveBackward->setEnabled(b);
    ui->btnMoveRotateLeft->setEnabled(b);
    ui->btnMoveRotateRight->setEnabled(b);
    ui->sliderSpeed->setEnabled(b);

    // Camera UI
    ui->btnCameraTiltUp->setEnabled(b);
    ui->btnCameraTiltDown->setEnabled(b);
    ui->btnCameraPanLeft->setEnabled(b);
    ui->btnCameraPanRight->setEnabled(b);

    ui->toolBar->setVisible(b);
}

