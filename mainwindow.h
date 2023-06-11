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
#include "dialog.h"

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

    void editInTable(QTableWidget* table);

    void deleteInTable(QTableWidget* table);

    void slotContextMenu(QPoint pos, QTableWidget *table);

    void on_tabWidget_currentChanged(int index);

    void on_calendarWidget_clicked(const QDate &date);

    void on_tabWidget_2_currentChanged(int index);

    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::MainWindow *ui;

    void showPie(const std::map<QString, int> &data, QLayout *layout);

    void showDB(const QString &strTable, QTableWidget *tableWidget, const int& index);

    std::map<QString, int> getData(const QString &strTable);

    void showStats(const std::map<QString, int> &data, QLayout *layout);

    void activateContextMenu(QTableWidget *table);
};
#endif // MAINWINDOW_H
