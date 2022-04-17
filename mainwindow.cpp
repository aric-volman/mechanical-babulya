#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionImport_triggered()
{
    openedFile.clear();
    ui->textEdit->setText(QString());
    QString fileName = QFileDialog::getOpenFileName(this, tr("Import text file"));
    QFile file(fileName);
    openedFile = fileName;
    if (!file.open(QIODevice::ReadWrite | QFile::Text)) {
        QMessageBox::warning(this, "Warning", "File not found");
    }
    setWindowTitle("Mechanical Babulya - " + fileName);
    QTextStream in(&file);
    QString text = in.readAll(); // Final text output

    ui->textEdit->setText(text);

}


void MainWindow::on_actionExport_triggered()
{

}

