#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    manager = new WifiBotManager(this);
    timer = new QTimer();
    timer->setInterval(INTERVAL_SYNC);
    connect(timer, SIGNAL(timeout()), this, SLOT(syncData()));
    timer->start();
    resetMovesSatements();
    initComponents();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete manager;
    delete timer;
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
    //cam = new Camera(this->ui);
    //ui->dockWidgetCamera->setVisible(false);
    //ui->dockWidgetCamera->setMinimumHeight(325);

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
    qDebug( "Camera Tilt Up" );
}

void MainWindow::on_btnCameraTiltDown_pressed()
{
    qDebug( "Camera Tilt Down" );
}

void MainWindow::on_btnCameraPanLeft_pressed()
{
    qDebug( "Camera Rotate Left" );
}

void MainWindow::on_btnCameraPanRight_pressed()
{
    qDebug( "Camera Rotate Right" );
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

    if(manager->connected){
        battery->setValue(manager->batterySensor);
        forewardSensor->setChecked(manager->proximitySensor1<0);
        backwardSensor->setChecked(manager->proximitySensor2<0);
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

