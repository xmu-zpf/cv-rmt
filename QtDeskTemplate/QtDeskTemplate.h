#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QtDeskTemplate.h"

class QtDeskTemplate : public QMainWindow
{
    Q_OBJECT

public:
    QtDeskTemplate(QWidget *parent = nullptr);
    ~QtDeskTemplate();

    //static void

public slots:
    void LoadImage(void);

private:
    Ui::QtDeskTemplateClass ui;
};
