#include "QtDeskTemplate.h"
#include <opencv.hpp>
#include <iostream>
#include <QFileDialog>

#define _STR(x) QString::fromLocal8Bit(x)

std::string Qstr2str(QString str)
{
    QByteArray byteArray = str.toLocal8Bit();
    const char* cString = byteArray.constData();

    return std::string(cString);
}

QtDeskTemplate::QtDeskTemplate(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
}

void QtDeskTemplate::LoadImage(void)
{
    auto fileName = Qstr2str(QFileDialog::getOpenFileName(this, _STR("选择图片"), "D:",
        _STR("jpg文件(*.jpg);;png文件(*.jpg);;所有文件(*.*)")));
    cv::Mat img = cv::imread(fileName);
    cv::imshow("test", img);
    cv::cvtColor(img, img, cv::COLOR_BGR2RGB);
    QImage image = QImage((const unsigned char*)(img.data), img.cols, img.rows, img.step, QImage::Format_RGB888);
    QGraphicsScene* scene = new QGraphicsScene;
    scene->addPixmap(QPixmap::fromImage(image));
    ui.graphicsView->rotate(0);
    ui.graphicsView->setScene(scene);
}

QtDeskTemplate::~QtDeskTemplate()
{}
