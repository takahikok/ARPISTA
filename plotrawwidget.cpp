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

	// 曲線の描画
//	PlotCurve();
}

void PlotRawWidget::PlotCurve()
{
	// 曲線の設定
	thisCurve.push_back(new QwtPlotCurve("sin"));
	thisCurve[0]->setPen(QPen(Qt::red));
	thisCurve[0]->attach(this);

	thisCurve[0]->setSamples(data[0].x->data(), data[0].y->data(), static_cast<int>(data[0].y->size()));

	return;
	const int kArraySize = 10000;

	double x[kArraySize] = {}; // x
	double y_sin[kArraySize] = {}; // y

	for (int i = 0; i < kArraySize; ++i) {
		x[i] = i / (kArraySize - 1.0);
		y_sin[i] = sin(2.0*M_PI*(x[i]));
	}

//	thisCurve[0]->setSamples(x, y_sin, kArraySize);
	return;
}
