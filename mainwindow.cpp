#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showMat(cv::Mat img, QLabel * label)
{
    if(img.channels() == 1)
        cv::cvtColor(img,img,cv::COLOR_GRAY2BGR);
    
    cv::resize(img,img,cv::Size(label->width(),label->height()));//改变图像大小使适应窗口

    cv::cvtColor(img,img,CV_BGR2RGB);//转为RGB格式
    QImage qimg = QImage((const uchar*)img.data,
                  img.cols,img.rows,
                  img.cols*img.channels(), //bytesPerLine,即每行的数据量,此处用处是使数据对齐
                  QImage::Format_RGB888); //简单地转换一下为QImage对象

    label->setPixmap(QPixmap::fromImage(qimg));
}

void MainWindow::on_pushButton_chooseUnitsImg_clicked()
{
    //获取文件名
    QString qFileName;
    qFileName = QFileDialog::getOpenFileName(this,tr("Choose an image"),".",tr("Image File(*.jpg *.png);;All Files(*)"));
    if(qFileName.isEmpty()) return;
    std::string fileName = qFileName.toStdString();
    
    //读入
    m_unitsImg = cv::imread(fileName,0);
    
    if(m_unitsImg.empty())
        return;
    
    //显示
    showMat(m_unitsImg,ui->label_unitsImg);
}

void MainWindow::on_pushButton_chooseDstImg_clicked()
{
    //获取文件名
    QString qFileName;
    qFileName = QFileDialog::getOpenFileName(this,tr("Choose an image"),".",tr("Image File(*.jpg *.png *.bmp *.jpeg);;All Files(*)"));
    if(qFileName.isEmpty()) return;
    std::string fileName = qFileName.toStdString();
    
    //读入
    m_dstImg = cv::imread(fileName,0);
    
    if(m_dstImg.empty())
        return;
    
    //显示
    showMat(m_dstImg,ui->label_dstImg);
}

void MainWindow::on_pushButton_solve_clicked()
{
    TangramSolver solver;
    solver.setFlipEnable(false);
    
    std::vector<std::vector<cv::Point>> resultPos;
    bool isSolved = solver.solve(m_unitsImg,m_dstImg,resultPos);
    
    //test
    if(isSolved)
    {
        cv::Mat testImg = m_dstImg.clone();
        cv::cvtColor(testImg,testImg,cv::COLOR_GRAY2BGR);
        
        cv::drawContours(testImg,resultPos,-1,cv::Scalar(255,100,30),2);
        
        testImg.copyTo(m_resultImg);
        showMat(m_resultImg,ui->label_resultImg);
    }
    else
    {
        m_resultImg.create(500,500,CV_8U);
        m_resultImg = cv::Scalar(0);
        cv::putText(m_resultImg,"failed!",cv::Point(100,280),2,3,255,2);
        showMat(m_resultImg,ui->label_resultImg);
    }
}
