#include "childwindow.h"

#include <QMdiSubWindow>

#include "childwidget.h"

ChildWindow::ChildWindow(QWidget *parent) :
  QMdiSubWindow(parent)
{
    childWidget = new ChildWidget(this);
    this->setWidget(childWidget);
}

ChildWindow::~ChildWindow()
{
    childWidget->~ChildWidget();
}
