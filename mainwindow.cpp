#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <cmath>
#include <iostream>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <qcustomplot.h>
#include "lib/solvers/solver.cpp"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    initGrid();
    ui->setupUi(this);
    QFile f(":qdarkstyle/dark/style.qss");

    if (!f.exists())   {
        printf("Unable to set stylesheet, file not found\n");
    }
    else   {
        f.open(QFile::ReadOnly | QFile::Text);
        QTextStream ts(&f);
        qApp->setStyleSheet(ts.readAll());
    }
    setWindowTitle(tr("Mechanical Babulya"));
    ui->widget->addGraph();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resetGrid() {
    for (int y = 0; y < 9; y++) {
        for (int x = 0; x < 9; x++) {
            grid.at(y).at(x) = 0;
        }
    }
    setGridText();
}

void MainWindow::setGridText() {
   std::vector<std::vector<int>> tempGrid = grid;
   // Reset grid
   for (int y = 0; y < 9; y++) {
       for (int x = 0; x < 9; x++) {
           grid.at(y).at(x) = 0;
       }
   }

   for (int y = 0; y < 9; y++) {
       for (int x = 0; x < 9; x++) {
           QLineEdit* current = qobject_cast<QLineEdit *>(ui->gridLayout->itemAtPosition(y, x)->widget());
           if (grid.at(y).at(x) != 0) {
                current->setText(QString::fromStdString(std::to_string(grid.at(y).at(x))));
           } else if (grid.at(y).at(x) == 0) {
                current->setText(tr(""));
           }
       }
   }
   grid = tempGrid;
   for (int y = 0; y < 9; y++) {
       for (int x = 0; x < 9; x++) {
           QLineEdit* current = qobject_cast<QLineEdit *>(ui->gridLayout->itemAtPosition(y, x)->widget());
           if (grid.at(y).at(x) != 0) {
                current->setText(QString::fromStdString(std::to_string(grid.at(y).at(x))));
           } else if (grid.at(y).at(x) == 0) {
                current->setText(tr(""));
           }
       }
   }
}


void MainWindow::getGridText() {
   for (int y = 0; y < 9; y++) {
       for (int x = 0; x < 9; x++) {
           QLineEdit* current = qobject_cast<QLineEdit *>(ui->gridLayout->itemAtPosition(y, x)->widget());
           if (current->text().toInt() == 0){
                grid.at(y).at(x) = 0;
           } else if (!(current->text() == "") && current->text().toInt() > 0 && current->text().toInt() <= 9) {
                grid.at(y).at(x) = current->text().toInt();
           } else {
                grid.at(y).at(x) = 0;
           }
       }
   }
}


void MainWindow::initGrid() {
    grid.resize(9);
    for(int y = 0; y < 9; y++) {
        for(int x = 0; x < 9; x++) {
            grid[y].push_back(0);
            // std::cout << "here" << std::endl;
        }
    }
}


void MainWindow::on_actionImport_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Import text file"));
    QFile file(fileName);
    if (!file.open(QIODevice::ReadWrite | QFile::Text)) {
        QMessageBox::warning(this, "Warning", "File not found or readable");
    }
    setWindowTitle("Mechanical Babulya - " + fileName);
    QTextStream in(&file);
    QString text = in.read(81); // Final text output
    std::string textStdRaw = text.toStdString();
    std::cout << textStdRaw << std::endl;
    // Converts entire string to 2D vector
    int total = 0;
    bool notADigit = false;
    for (int y = 0; y < std::sqrt(textStdRaw.size()); y++) {
        for (int x = 0; x < std::sqrt(textStdRaw.size()); x++) {
            grid.at(y).at(x) = (textStdRaw.at(total)-'0');
            notADigit = !std::isdigit(textStdRaw.at(total));
            total++;
        }
    }
    if (notADigit) {
        QMessageBox::warning(this, "Warning", "Contents of file are not numbers");
    }
    for (int y = 0; y < 9; y++) {
        for (int x = 0; x < 9; x++) {
            QLineEdit* current = qobject_cast<QLineEdit *>(ui->gridLayout->itemAtPosition(y, x)->widget());
            if (grid.at(y).at(x) != 0) {
                 current->setText(QString::fromStdString(std::to_string(grid.at(y).at(x))));
            }
        }
    }
    setGridText();
    ui->textEdit->setText(text);

}


void MainWindow::on_actionExport_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Sudoku"), ".", tr("Text files (*.txt)"));
    QFile file(fileName);
    if (!file.open(QIODevice::ReadWrite | QFile::Text)) {
        QMessageBox::warning(this, "Warning", "File not found or readable");
    }
    QTextStream out(&file);
    getGridText();
    QString line;
    for (int y = 0; y < 9; y++) {
        for (int x = 0; x < 9; x++) {
            line += QString::number(grid.at(y).at(x));
        }
    }
    out << line;
}


void MainWindow::on_pushButton_clicked()
{
    getGridText();

    solver *sudokuSolver = new solver();
    sudokuSolver->setGrid(grid);
    int warning = sudokuSolver->solve();

    grid = sudokuSolver->getGrid();

    setGridText();

    if (warning == 1) {
        std::list<double> rawdata = sudokuSolver->getGridCompletionVector();

        auto rawDataFront = rawdata.begin();

        QVector<double> data(rawdata.size());

        QVector<double> x(rawdata.size());

        std::cout << "Size of Data: " << data.size() << std::endl;

        for (int i = 1; i <= data.size(); i++) {

            x[i] = (double)i;
            data[i] = (double)*rawDataFront;
            // x.replace(i,  (double)i);
            // data.replace(i, *rawDataFront);
            std::cout << data[i] << std::endl;
            std::cout << *rawDataFront << std::endl;
            std::advance(rawDataFront, 1);
        }

        ui->widget->graph(0)->setData(x, data);
        ui->widget->rescaleAxes(true);
        ui->widget->replot();

        QMessageBox::information(this, tr("Mechanical Babulya"), tr("Sudoku is solved!"));

        delete sudokuSolver;

    } else if (warning == 2) {
        QMessageBox::warning(this, tr("Error"), tr("Duplicate numbers found!"));
    } else if (warning == 3) {
        QMessageBox::warning(this, tr("Error"), tr("Impossible puzzle!"));
    }

}


void MainWindow::on_pushButton_2_clicked()
{
    for (int y = 0; y < 9; y++) {
        for (int x = 0; x < 9; x++) {
            grid.at(y).at(x) = 0;
        }
    }
    setGridText();
}

