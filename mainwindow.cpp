#include "mainwindow.h"
#include "./ui_mainwindow.h"

int permission = 1;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tabWidget->setCurrentIndex(0);

    activateContextMenu(ui->tableWidget_2);
    activateContextMenu(ui->tableWidget_3);
    activateContextMenu(ui->tableWidget_4);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addInTable() {

}

void MainWindow::editInTable() {

}

void MainWindow::deleteInTable() {

}

void MainWindow::activateContextMenu(QTableWidget *table) {
    table->setContextMenuPolicy(Qt::CustomContextMenu);

    switch(permission) {
        case 1:
            connect(table, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(addInTable()));
            break;
        case 2:
            connect(table, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(addInTable()));
            connect(table, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(editInTable()));
            break;
        case 3:
            connect(table, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(addInTable()));
            connect(table, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(editInTable()));
            connect(table, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(deleteInTable()));
            break;
    }
    connect(table, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(slotContextMenu(QPoint)));
}

void MainWindow::slotContextMenu(QPoint pos) {
    QMenu *menu = new QMenu(this);

    QAction *addSlot = new QAction("Добавить", this);
    connect(addSlot, SIGNAL(triggered()), this, SLOT(addInTable()));

    QAction *editSlot = new QAction("Редактировать", this);
    connect(editSlot, SIGNAL(triggered()), this, SLOT(editInTable()));

    QAction *deleteSlot = new QAction("Удалить", this);
    connect(deleteSlot, SIGNAL(triggered()), this, SLOT(deleteInTable()));

    switch(permission) {
        case 1:
            menu->addAction(addSlot);
            break;
        case 2:
            menu->addAction(addSlot);
            menu->addAction(editSlot);
            break;
        case 3:
            menu->addAction(addSlot);
            menu->addAction(editSlot);
            menu->addAction(deleteSlot);
            break;
    }

    menu->popup(ui->tableWidget_2->viewport()->mapToGlobal(pos));
    menu->popup(ui->tableWidget_3->viewport()->mapToGlobal(pos));
    menu->popup(ui->tableWidget_4->viewport()->mapToGlobal(pos));
}

void MainWindow::showPie(const std::map<QString, int> &data, QLayout *layout) {
    QLayoutItem *child;
    while ((child = layout->takeAt(0)) != nullptr) {
        delete child->widget();
        delete child;
    }

    QPieSeries *series = new QPieSeries();
    for (const auto &item : data)
        series->append(item.first, item.second);
    series->setLabelsVisible(true);
    series->setHoleSize(0.47);

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Продажи");
    chart->legend()->hide();

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    chartView->setMaximumWidth(600);
    layout->addWidget(chartView);
}

void MainWindow::showStats(const std::map<QString, int> &data, QLayout *layout) {
    QLayoutItem *child;
    while ((child = layout->takeAt(0)) != nullptr) {
        delete child->widget();
        delete child;
    }

    QChart *chart = new QChart();
    chart->setTitle("Сотрудники");

    QHorizontalStackedBarSeries *series = new QHorizontalStackedBarSeries();
    QBarCategoryAxis *axisY = new QBarCategoryAxis();

    axisY->append({"Январь", "Февраль", "Март", "Апрель", "Май", "Июнь", "Июль", "Август", "Сентябрь", "Октябрь", "Ноябрь", "Декабрь"});
    chart->addAxis(axisY, Qt::AlignLeft);

    for (const auto &item : data) {
        QBarSet *barSet = new QBarSet(item.first);
        barSet->append(item.second);
        series->append(barSet);

    }

    chart->addSeries(series);
    series->attachAxis(axisY);
    chart->legend()->setAlignment(Qt::AlignBottom);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    layout->addWidget(chartView);
}

QMap<int, int> st;
void MainWindow::showDB(const QString &strTable, QTableWidget *tableWidget, const int& index = 0) {
    int countRow = 0, countColumn = 0;
    database databaseQuery;
    QStringList nameColumn, itemList;

    nameColumn = databaseQuery.execQueryPRAGMA(strTable);
    countColumn = nameColumn.size();

    QSqlQueryModel* query = databaseQuery.execSelectQuery(QString("select * from %1").arg(strTable));
    switch (index) {
        case 1: {
            query = databaseQuery.execSelectQuery(QString("select DeliveriesCode, ProductCode, Name, DateReceived, Quantity, PurchasePrice, SupplierName, EmployeeName, CategoryText, Quality from %1"
                                                          " left join Suppliers on Deliveries.SupplierCode = Suppliers.SupplierCode"
                                                          " left join Employees on Deliveries.EmployeeAcceptedCode = Employees.EmployeeCode"
                                                          " left join Categories on Deliveries.CategoryCode = Categories.CategoryCode").arg(strTable));
            break;
        }
        case 2: {
            query = databaseQuery.execSelectQuery(QString("select SaleCode, Name, SaleDate, QuantitySold, RetailPrice, CustomerName, EmployeeName from %1"
                                                          " left join Products on Sales.ProductCode = Products.ProductCode"
                                                          " left join Customers on Sales.CustomerCode = Customers.CustomerCode"
                                                          " left join Employees on Sales.EmployeeCode = Employees.EmployeeCode").arg(strTable));
            break;
        }

    }
    countRow = query->rowCount();
    int i = 1;
    for (int row = 0; row < countRow; ++row) {
        for (int column = 0; column < countColumn; ++column)
            itemList += query->data(query->index(row, column)).toString();
        st[i] = query->data(query->index(row, 0)).toInt();
        ++i;
    }
    delete query;

    tableWidget->setColumnCount(countColumn);
    tableWidget->setRowCount(countRow);
    tableWidget->setHorizontalHeaderLabels(nameColumn);

    int countResultQuery = 0;
    for(int i = 0; i < countRow; ++i){
        for(int j = 0; j < countColumn; ++j) {
            QTableWidgetItem *item = new QTableWidgetItem(itemList[j+countResultQuery]);
            tableWidget->setItem(i,j,item);
        }
        countResultQuery += countColumn;
    }
}

std::map<QString, int> pie;
void MainWindow::on_tabWidget_currentChanged(int index)
{
    switch (index) {
        case 0: { // main
            database databaseQuery;

            QSqlQueryModel* query = databaseQuery.execSelectQuery("select Products.Name, QuantitySold from Sales left join Products on sales.ProductCode = Products.ProductCode");
            int countRow = query->rowCount();
            for (int row = 0; row < countRow; ++row)
                pie[query->data(query->index(row, 0)).toString()] = query->data(query->index(row, 1)).toInt();
            delete query;

            showPie(pie , ui->verticalLayout);
            break;
        }
        case 1: { // deliveries
            showDB("Deliveries", ui->tableWidget_2, 1);
            break;
        }
        case 2: { // sales
            showDB("Sales", ui->tableWidget_3, 2);
            break;
        }
        case 3: { // products
            showDB("Products", ui->tableWidget_4);
            break;
        }
        case 4: { // auth

            break;
        }
        case 5: { // admin panel

            break;
        }
    }
}


void MainWindow::on_calendarWidget_clicked(const QDate &date)
{
    qDebug() << date;
}

std::map<QString, int> stats;
void MainWindow::on_tabWidget_2_currentChanged(int index)
{
    database databaseQuery;

    QSqlQueryModel* query = databaseQuery.execSelectQuery("select EmployeeName from Sales left join Employees on Sales.EmployeeCode = Employees.EmployeeCode");
    int countRow = query->rowCount();
    for (int row = 0; row < countRow; ++row) {
        QString employeeName = query->data(query->index(row, 0)).toString();
        if (stats.contains(employeeName))
            stats[employeeName]++;
        else
            stats[employeeName] = 1;
    }
    delete query;
    showStats(stats, ui->verticalLayout_2);
}

