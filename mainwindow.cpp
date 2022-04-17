#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <cmath>
#include <iostream>
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

void MainWindow::setGridText(std::vector<std::vector<int>> inputGrid) {
   QList<QLineEdit *> lineEdits = ui->gridLayout->findChildren<QLineEdit *>();
   int total = 0;
   for (int y = 0; y < 9; y++) {
       for (int x = 0; x < 9; x++) {
           QLineEdit* current = lineEdits.at(0);
           current->setText(tr("1"));
           total++;
       }
   }
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
    QString text = in.read(81); // Final text output
    std::string textStdRaw = text.toStdString();
    std::cout << textStdRaw << std::endl;
    // Converts entire string to 2D vector
    int total = 0;
    /*
    for (int y = 0; y < std::sqrt(textStdRaw.size()); y++) {
        for (int x = 0; x < std::sqrt(textStdRaw.size()); x++) {
            grid.at(y).at(x) = (textStdRaw.at(total)-'0');
            if (!std::isdigit(textStdRaw.at(total))) {
                QMessageBox::critical(this, "Warning", "Contents of file are not numbers");
            }
            total++;
        }
    }*/
    setGridText(grid);
    ui->textEdit->setText(text);

}


void MainWindow::on_actionExport_triggered()
{

}

