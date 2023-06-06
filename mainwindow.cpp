#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QPieSeries *series = new QPieSeries();
    series->append("1", 11);
    series->append("2", 2);
    series->append("3", 3);
    series->append("4", 9);

    series->setLabelsVisible();
    series->setHoleSize(0.47);

    QPieSlice *slice1 = series->slices().at(0);
    slice1->setBrush(Qt::red);
    slice1->setLabelPosition(QPieSlice::LabelOutside);

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Продажи");
    chart->legend()->hide();

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    chartView->setMaximumWidth(500);
    ui->verticalLayout->addWidget(chartView);
}

MainWindow::~MainWindow()
{
    delete ui;
}
