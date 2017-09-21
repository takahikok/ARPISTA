#include "plotrawwidget.h"
#include <qwt_plot_curve.h>
#include <qwt_legend.h>
#include <qwt_plot_grid.h>

PlotRawWidget::PlotRawWidget(QWidget *parent) :QwtPlot(parent)
{
    setCanvasBackground(QColor(Qt::white)); // 背景白
    insertLegend(new QwtLegend(), QwtPlot::BottomLegend);   // 凡例

    // グリッド
    QwtPlotGrid *grid1 = new QwtPlotGrid;
    grid1->enableXMin(true);
    grid1->setMajorPen(QPen(Qt::black, 0, Qt::DashDotLine));
    grid1->setMinorPen(QPen(Qt::gray, 0, Qt::DotLine));
    grid1->attach(this);

    // 曲線の設定
    sin_crv = new QwtPlotCurve("sin");
    sin_crv->setPen(QPen(Qt::red));
    sin_crv->attach(this);

    // 曲線の描画
    plotCurve();
}

void PlotRawWidget::plotCurve()
{
    const int kArraySize = 1000;

    double x[kArraySize] = {}; // x
    double y_sin[kArraySize] = {}; // y

    for (int i = 0; i < kArraySize; ++i) {
	x[i] = i / (kArraySize - 1.0);
	y_sin[i] = sin(2.0*M_PI*(x[i]));
    }

    sin_crv->setSamples(x, y_sin, kArraySize);
}
