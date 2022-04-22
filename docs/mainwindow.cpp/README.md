# Methods

### Constructor
```
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{ ... }
```
Initializes a grid array, the UI, and loads in the stylesheet

### Deconstructor
```
MainWindow::~MainWindow() { ... }
```
Deletes the ui object

### resetGrid
```
void MainWindow::resetGrid() { ... }
```
Sets everything in the grid to 0, then calls the setGridText method

### setGridText
```
void MainWindow::setGridText() { ... }
```
First, it initializes the tempGrid variable as equivalent to the array. Then, it zeros the grid. In a loop, the text of every QLineEdit is set to zero.
For the loops, this line is used to get the QLineEdit of the QGridLayout.
```
QLineEdit* current = qobject_cast<QLineEdit *>(ui->gridLayout->itemAtPosition(y, x)->widget());
```
The grid is now equivalent to the tempGrid, and the text is set to the values at every position on the grid. If a number on the grid is zero, then it is blank in the UI. 
TODO - Simplify this logic

### getGridText
```
void MainWindow::getGridText() { ... }
```
This method gets the number from every corresponding QLineEdit, and assigns the corresponding grid position to that number. If the space is blank, the grid's number is 0. It uses the .toInt() method of the QLineEdit to filter the numerical input.

### initGrid
```
void MainWindow::initGrid() { ... }
```
Initializes the grid with zeros, called at the constructor.

### Import file button action
```
void MainWindow::on_actionImport_triggered() { ... }
```

### Export file button action
```
void MainWindow::on_actionExport_triggered() { ... }
```

### Solve button
```
void MainWindow::on_pushButton_clicked() { ... }
```
TODO - Rename this method

### Reset button
```
void MainWindow::on_pushButton_2_clicked() { ... }
```
TODO - Rename this method
