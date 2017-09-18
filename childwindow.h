#ifndef CHILDWINDOW_H
#define CHILDWINDOW_H

#include <QtGui>
#include <QWidget>
#include <QMdiSubWindow>

#include "childwidget.h"

namespace Ui {
    class ChildWindow;
}

class ChildWindow : public QMdiSubWindow
{
    Q_OBJECT

public:
    explicit ChildWindow(QWidget *parent = 0);
    ~ChildWindow();

private:
    ChildWidget *childWidget;
};

#endif // CHILDWINDOW_H
