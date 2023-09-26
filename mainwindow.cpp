#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QVBoxLayout>
#include "qwt_plot_curve.h"
#include <QwtLegend>
#include<QwtPlotGrid>
#include<QwtPlot>
#include<QwtLegendLabel>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    qwtPlot_show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::qwtPlot_show()
{
    QwtPlot *plotCurve = new QwtPlot();
    plotCurve->detachItems();
    plotCurve->replot();

    /*setAxisScale四个参数的含义分别是：坐标轴->坐标轴最小值->坐标轴最大值->步进*/
    plotCurve->setAxisScale(QwtPlot::xBottom, 0, 100, 10);
    plotCurve->setAxisScale(QwtPlot::yLeft, 0, 10, 2);
    plotCurve->setAxisTitle(QwtPlot::xBottom,
                            QString::fromLocal8Bit("x 轴(data)"));
    plotCurve->setAxisTitle(QwtPlot::yLeft, QString::fromLocal8Bit("y 轴(data)"));

    /*点*/
    QPolygonF point;
    for (int i = 0; i < 100; i++) {
        point << QPointF(i, i * 0.1);
    }

    /*绘制曲线*/
    QwtPlotCurve *curve = new QwtPlotCurve("Curve 1");   //设置曲线
    curve->setSamples(point);
    curve->setPen(QColor(160, 36, 48), 2,
                  Qt::SolidLine); //设置画笔(颜色,粗细,线条样式)
    curve->attach(plotCurve);   //把曲线附加到qwtPlot上
    curve->setCurveAttribute(QwtPlotCurve::Fitted, true);   //曲线光滑

    /*添加网格*/
    QwtPlotGrid *grid = new QwtPlotGrid();
    grid->enableX( true );//设置网格线
    grid->enableY( true );
    grid->setMajorPen( Qt::black, 0, Qt::DotLine );
    grid->attach(plotCurve);

    /*添加图例*/
    QwtLegend *legend = new QwtLegend();
    legend->setDefaultItemMode( QwtLegendData::Checkable );//图例可被点击
    plotCurve->insertLegend( legend, QwtPlot::RightLegend );  //右侧显示图例

    connect( legend, SIGNAL( checked( const QVariant &, bool, int ) ),
             SLOT( showItem( const QVariant &, bool ) ) );  //点击图例操作
    QwtPlotItemList items = plotCurve->itemList(
                                QwtPlotItem::Rtti_PlotCurve );//QwtPlotItem类是画布上所有图元的基类,此处获取画了多少条曲线,如果为获取其他形状，需要改变参数
    for ( int i = 0; i < items.size(); i++ ) {
        const QVariant itemInfo = plotCurve->itemToInfo(
                                      items[i] );    //QVariant可以存储各种数据类型
        QwtLegendLabel *legendLabel = qobject_cast<QwtLegendLabel *>
                                      ( legend->legendWidget( itemInfo ) );
        if ( legendLabel ) {
            legendLabel->setChecked( true );
        }
        items[i]->setVisible( true );

    }

    /*ui界面显示曲线*/
    plotCurve->replot();
    plotCurve->setAutoReplot(true);   //自动更新

    QVBoxLayout *vlayout = new QVBoxLayout();
    vlayout->addWidget(plotCurve);
    this->centralWidget()->setLayout(vlayout);

}
