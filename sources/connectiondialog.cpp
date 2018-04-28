#include "connectiondialog.h"

ConnectionDialog::ConnectionDialog(QWidget *parent,
                                   WifiBotManager *manager):
    QDialog(parent),
    ui(new Ui::ConnectionDialog)
{
    ui->setupUi(this);

    this->parent = parent;
    this->manager = manager;
}

ConnectionDialog::~ConnectionDialog()
{
    delete ui;
}

void ConnectionDialog::on_buttonBox_accepted()
{
    QString address = ui->lineEdit_address->text();
    quint16 port = quint16(ui->lineEdit_port->text().toInt());
    quint16 cameraPort = quint16(ui->lineEdit_cameraPort->text().toInt());

    Ui::MainWindow* ui = ((MainWindow*) parentWidget())->getUI();

    ui->statusBar->showMessage(tr("Connecting..."));

    if(manager->connectTo(address, port,cameraPort))
    {
        ui->statusBar->showMessage(tr("Connected:") + address + tr(":") + QString::number(port));
        this->accept();
    }
    else
    {
        ui->statusBar->showMessage(tr("/!\\ ") + manager->tcp.errorString());
        this->reject();
    }
}
