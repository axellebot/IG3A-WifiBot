#ifndef CONNECTIONDIALOG_H
#define CONNECTIONDIALOG_H

#include <QDialog>
#include "wifibotmanager.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui_connectiondialog.h"

namespace Ui {
class ConnectionDialog;
}

class ConnectionDialog : public QDialog
{
    Q_OBJECT

private:
    Ui::ConnectionDialog *ui;
    WifiBotManager *manager;
    QWidget *parent;

public:
    explicit ConnectionDialog(QWidget *parent, WifiBotManager *manager);
    ~ConnectionDialog();

private slots:
    void on_buttonBox_accepted();
};

#endif // CONNECTIONDIALOG_H
