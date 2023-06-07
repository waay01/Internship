#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPieSeries>
#include <QHorizontalStackedBarSeries>
#include <QChartView>
#include <QSql>
#include <QSqlQuery>
#include <QFile>
#include <QMessageBox>
#include <QList>
#include <QBarSet>
#include <QBarCategoryAxis>

#include "database.h"
#include "qtablewidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

  private slots:

    void addInTable();

    void editInTable();

    void deleteInTable();

    void slotContextMenu(QPoint pos);

    void on_tabWidget_currentChanged(int index);

    void on_calendarWidget_clicked(const QDate &date);

    void on_tabWidget_2_currentChanged(int index);

private:
    Ui::MainWindow *ui;

    void showPie(const std::map<QString, int> &data, QLayout *layout);

    void showDB(const QString &strTable, QTableWidget *tableWidget, const int& index);

    std::map<QString, int> getData(const QString &strTable);

    void showStats(const std::map<QString, int> &data, QLayout *layout);

    void activateContextMenu(QTableWidget *table);
};
#endif // MAINWINDOW_H
