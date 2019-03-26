#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QLabel>

#include "TangramSolver.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:
    void on_pushButton_chooseUnitsImg_clicked();
    
    void on_pushButton_chooseDstImg_clicked();
    
    void on_pushButton_solve_clicked();
    
private:
    Ui::MainWindow *ui;
    
    cv::Mat m_unitsImg;
    cv::Mat m_dstImg;
    cv::Mat m_resultImg;
    
    void showMat(cv::Mat img, QLabel * label);
};

#endif // MAINWINDOW_H
