#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QWidget>
#include <QMainWindow>
#include <QtWidgets>
#include <qvideowidget.h>
#include <qvideosurfaceformat.h>
#include <QKeyEvent>
#include "wifibotmanager.h"
#include "connectiondialog.h"
#include "ui_mainwindow.h"
#include <QtWebEngineWidgets>

#define INTERVAL_SYNC 75

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    Ui::MainWindow *ui;
    WifiBotManager *manager;

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

    // Sync
    QTimer *timer;

    /** UI */
    QProgressBar *battery;
    QRadioButton *forewardSensor;
    QRadioButton *backwardSensor;
    QWebEngineView * cameraView;

    void resetMovesSatements();
    void initComponents();

    /** Others */
    void setInterfaceEnabled(bool b);

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    Ui::MainWindow * getUI();

private slots:


    /** Buttons */

    // Moves
    void on_btnMoveForward_pressed();

    void on_btnMoveForward_released();

    void on_btnMoveBackward_pressed();

    void on_btnMoveBackward_released();

    void on_btnMoveRotateLeft_pressed();

    void on_btnMoveRotateLeft_released();

    void on_btnMoveRotateRight_pressed();

    void on_btnMoveRotateRight_released();

    // Camera
    void on_btnCameraTiltUp_pressed();

    void on_btnCameraTiltUp_released();

    void on_btnCameraTiltDown_pressed();

    void on_btnCameraTiltDown_released();

    void on_btnCameraPanLeft_pressed();

    void on_btnCameraPanLeft_released();

    void on_btnCameraPanRight_pressed();

    void on_btnCameraPanRight_released();

    void on_sliderSpeed_valueChanged(int value);

    /** Menu Actions */
    // Update
    void on_actionConnectTo_triggered();
    void on_actionDisconnect_triggered();

    /** Others */
    // Sync
    void syncData();
};

#endif // MAINWINDOW_H
