#ifndef PLOTRAWWIDGET_H
#define PLOTRAWWIDGET_H
#include <qwt_plot.h>

class QwtPlotCurve;

class PlotRawWidget : public QwtPlot
{
    Q_OBJECT
public:
    explicit PlotRawWidget(QWidget *parent = 0);
    void plotCurve();

private:
    QwtPlotCurve *sin_crv;

signals:

public slots:

};
#endif // PLOTRAWWIDGET_H
