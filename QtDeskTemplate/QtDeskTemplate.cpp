#include "QtDeskTemplate.h"
#include <opencv.hpp>
#include <iostream>
#include <QFileDialog>
#include "cvalgrm.h"

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
    QString fn = QFileDialog::getOpenFileName(this, _STR("选择图片"), "D:/TestSet/",
        _STR("jpg文件(*.jpg);;png文件(*.png);;所有文件(*.*)"));
    ui.label->setText(fn);
    auto fileName = Qstr2str(fn);
    cv::Mat img = cv::imread(fileName, cv::IMREAD_GRAYSCALE);
    cv::imshow("test", img);
    cv::waitKey();
    cv::Mat qimg;
    cv::cvtColor(img, qimg, cv::COLOR_BGR2RGB);
    QImage image = QImage((const unsigned char*)(qimg.data), qimg.cols, qimg.rows, qimg.step, QImage::Format_RGB888);
    QGraphicsScene* scene = new QGraphicsScene;
    scene->addPixmap(QPixmap::fromImage(image));
    ui.graphicsView->rotate(0);
    ui.graphicsView->setScene(scene);

    auto ellipseRotRrec = my::fitEllipseHalcon(img);
    cv::Mat rsltImg = my::drawEllipseOnImage(img, ellipseRotRrec);
    cv::imshow("rslt", rsltImg);
    cv::waitKey();
}

QtDeskTemplate::~QtDeskTemplate()
{
    cv::destroyAllWindows();
}
