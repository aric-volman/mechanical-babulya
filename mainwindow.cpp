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

void MainWindow::setGridText() {
   int total = 0;
   for (int y = 0; y < 9; y++) {
       for (int x = 0; x < 9; x++) {
           QLineEdit* current = qobject_cast<QLineEdit *>(ui->gridLayout->itemAtPosition(y, x)->widget());
           if (grid.at(y).at(x) != 0) {
                current->setText(QString::fromStdString(std::to_string(grid.at(y).at(x))));
           }
           total++;
       }
   }
}

void MainWindow::initGrid() {
    grid.resize(9);
    for(int y = 0; y < 9; y++) {
        for(int x = 0; x < 9; x++) {
            grid[y].push_back(x);
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
    initGrid();
    // Converts entire string to 2D vector
    int total = 0;

    for (int y = 0; y < std::sqrt(textStdRaw.size()); y++) {
        for (int x = 0; x < std::sqrt(textStdRaw.size()); x++) {
            grid.at(y).at(x) = (textStdRaw.at(total)-'0');
            if (!std::isdigit(textStdRaw.at(total))) {
                QMessageBox::warning(this, "Warning", "Contents of file are not numbers");
            }
            total++;
        }
    }
    setGridText();
    // QList<QLineEdit *> lineEdits = ui->gridLayout->findChildren<QLineEdit *>();
    //edit->setText("a");
    //QLineEdit* btn = qobject_cast<QLineEdit *>(ui->gridLayout->itemAtPosition(0, 0)->widget());
    //btn->setText("hello");
    // setGridText();
    //int x = 0;
    //QLineEdit* current = qobject_cast<QLineEdit *>(ui->gridLayout->itemAtPosition(0, x)->widget());
    //current->setText(tr("1"));

    ui->textEdit->setText(text);

}


void MainWindow::on_actionExport_triggered()
{

}

