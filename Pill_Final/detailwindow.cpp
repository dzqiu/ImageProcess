#include "detailwindow.h"
#include "ui_detailwindow.h"

detailWindow::detailWindow( vector<MESSAGE_STRUCT> mess,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::detailWindow)
{
    ui->setupUi(this);

    ui->tableWidget->setRowCount(mess.size());
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget->verticalHeader()->setVisible(false);
    for (int i=0;i<mess.size();i++)
    {
        ui->tableWidget->setItem(i,0,new QTableWidgetItem(QString::number(mess.at(i).num)));
        ui->tableWidget->setItem(i,1,new QTableWidgetItem(mess.at(i).shape));
        ui->tableWidget->setItem(i,2,new QTableWidgetItem(mess.at(i).color));
    }

}

detailWindow::~detailWindow()
{
    delete ui;
}
