#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setGridText();
    void getGridText();
    void resetGrid();
private slots:
    void on_actionImport_triggered();

    void on_actionExport_triggered();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;
    QString openedFile = "";
    std::vector<std::vector<int> > grid;

    void initGrid();
};
#endif // MAINWINDOW_H
