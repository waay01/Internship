#include "mainwindow.h"
#include "./ui_mainwindow.h"

int permission = 0;
QMap<int, int> st;
std::map<QString, int> pie, stats, position = {{"Консультант", 0}, {"Кассир", 1}, {"Менеджер", 2}, {"Директор", 3}};
QString EmployeeCode;

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

int rowIndex(int row) {
    return st[row];
}

void MainWindow::addInTable() {
    database databaseQuery;

    dialog dialogInsert;
    dialogInsert.setName("Добавление");
    dialogInsert.setModal(true);

    const int currentIndex = ui->tabWidget->currentIndex();
    switch (currentIndex) {
        case 1: {
            QStringList SupplierCode, EmployeeAcceptedCode, CategoryCode;
            QVector<QStringList> itemList;
            int countRow = 0;

            QSqlQueryModel* SupplierCodeQuery = databaseQuery.execSelectQuery("select SupplierCode from Suppliers");
            countRow = SupplierCodeQuery->rowCount();
            for (int row = 0; row < countRow; ++row)
                SupplierCode += SupplierCodeQuery->data(SupplierCodeQuery->index(row, 0)).toString();
            itemList.emplace_back(SupplierCode);
            delete SupplierCodeQuery;

            QSqlQueryModel* EmployeeAcceptedCodeQuery = databaseQuery.execSelectQuery("select EmployeeCode from Employees");
            countRow = EmployeeAcceptedCodeQuery->rowCount();
            for (int row = 0; row < countRow; ++row)
                EmployeeAcceptedCode += EmployeeAcceptedCodeQuery->data(EmployeeAcceptedCodeQuery->index(row, 0)).toString();
            itemList.emplace_back(EmployeeAcceptedCode);
            delete EmployeeAcceptedCodeQuery;

            QSqlQueryModel* CategoryCodeQuery = databaseQuery.execSelectQuery("select CategoryCode from Categories");
            countRow = CategoryCodeQuery->rowCount();
            for (int row = 0; row < countRow; ++row)
                CategoryCode += CategoryCodeQuery->data(CategoryCodeQuery->index(row, 0)).toString();
            itemList.emplace_back(CategoryCode);
            delete CategoryCodeQuery;

            dialogInsert.sendData(currentIndex, itemList);
            dialogInsert.getWindow(0);
            dialogInsert.exec();
            const QStringList& data = dialogInsert.getData(1);
            if (dialogInsert.transaction() && !data.contains("")) {
                QString strQuery = "insert into Deliveries(ProductCode, Name, DateReceived, Quantity, PurchasePrice, SupplierCode, EmployeeAcceptedCode, CategoryCode, Quality) values(%1, '%2', '%3', %4, %5, %6, %7, %8, %9);";
                databaseQuery.execQuery(QString(strQuery).arg(data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7], data[8]));

                strQuery = "insert into Logs(Date, \nTable\n, EmployeeCode, ActionCode) values(datetime('now','localtime'), 'Deliveries', %1, 1);";
                databaseQuery.execQuery(QString(strQuery).arg(EmployeeCode));
            }
            break;
        }

        case 2: {
            QStringList ProductCode, CustomerCode, EmployeeCode;
            QVector<QStringList> itemList;
            int countRow = 0;

            QSqlQueryModel* ProductCodeQuery = databaseQuery.execSelectQuery("select ProductCode from Products");
            countRow = ProductCodeQuery->rowCount();
            for (int row = 0; row < countRow; ++row)
                ProductCode += ProductCodeQuery->data(ProductCodeQuery->index(row, 0)).toString();
            itemList.emplace_back(ProductCode);
            delete ProductCodeQuery;

            QSqlQueryModel* CustomerCodeQuery = databaseQuery.execSelectQuery("select CustomerCode from Customers");
            countRow = CustomerCodeQuery->rowCount();
            for (int row = 0; row < countRow; ++row)
                CustomerCode += CustomerCodeQuery->data(CustomerCodeQuery->index(row, 0)).toString();
            itemList.emplace_back(CustomerCode);
            delete CustomerCodeQuery;

            QSqlQueryModel* EmployeeCodeQuery = databaseQuery.execSelectQuery("select EmployeeCode from Employees");
            countRow = EmployeeCodeQuery->rowCount();
            for (int row = 0; row < countRow; ++row)
                EmployeeCode += EmployeeCodeQuery->data(EmployeeCodeQuery->index(row, 0)).toString();
            itemList.emplace_back(EmployeeCode);
            delete EmployeeCodeQuery;

            dialogInsert.sendData(currentIndex, itemList);
            dialogInsert.getWindow(1);
            dialogInsert.exec();
            const QStringList& data = dialogInsert.getData(2);
            if (dialogInsert.transaction() && !data.contains("")) {
                QString strQuery = "insert into Sales(ProductCode, SaleDate, QuantitySold, RetailPrice, CustomerCode, EmployeeCode) values(%1, '%2', %3, %4, %5, %6);";
                databaseQuery.execQuery(QString(strQuery).arg(data[0], data[1], data[2], data[3], data[4], data[5]));

                strQuery = "insert into Logs(Date, \"Table\", EmployeeCode, ActionCode) values(datetime('now','localtime'), 'Sales', %1, 1);";
                databaseQuery.execQuery(QString(strQuery).arg(::EmployeeCode));
            }
            break;
        }

        case 3: {
            dialogInsert.getWindow(2);
            dialogInsert.exec();
            const QStringList& data = dialogInsert.getData(3);
            if (dialogInsert.transaction() && !data.contains("")) {
                QString strQuery = "insert into Products(Name, Quantity) values('%1',%2);";
                databaseQuery.execQuery(QString(strQuery).arg(data[0], data[1]));

                strQuery = "insert into Logs(Date, \"Table\", EmployeeCode, ActionCode) values(datetime('now','localtime'), 'Products', %1, 1);";
                databaseQuery.execQuery(QString(strQuery).arg(EmployeeCode));
            }
            break;
        }
    }
}

void MainWindow::editInTable(QTableWidget* table) {
    database databaseQuery;

    dialog dialogUpdate;
    dialogUpdate.setName("Редактирование");
    dialogUpdate.setModal(true);

    const int currentIndex = ui->tabWidget->currentIndex(), currentRow = table->currentItem()->row()+1;
    switch (currentIndex) {
        case 1: {
            QStringList SupplierCode, EmployeeAcceptedCode, CategoryCode;
            QVector<QStringList> itemList;
            int countRow = 0;

            QSqlQueryModel* SupplierCodeQuery = databaseQuery.execSelectQuery("select SupplierCode from Suppliers");
            countRow = SupplierCodeQuery->rowCount();
            for (int row = 0; row < countRow; ++row)
                SupplierCode += SupplierCodeQuery->data(SupplierCodeQuery->index(row, 0)).toString();
            itemList.emplace_back(SupplierCode);
            delete SupplierCodeQuery;

            QSqlQueryModel* EmployeeAcceptedCodeQuery = databaseQuery.execSelectQuery("select EmployeeCode from Employees");
            countRow = EmployeeAcceptedCodeQuery->rowCount();
            for (int row = 0; row < countRow; ++row)
                EmployeeAcceptedCode += EmployeeAcceptedCodeQuery->data(EmployeeAcceptedCodeQuery->index(row, 0)).toString();
            itemList.emplace_back(EmployeeAcceptedCode);
            delete EmployeeAcceptedCodeQuery;

            QSqlQueryModel* CategoryCodeQuery = databaseQuery.execSelectQuery("select CategoryCode from Categories");
            countRow = CategoryCodeQuery->rowCount();
            for (int row = 0; row < countRow; ++row)
                CategoryCode += CategoryCodeQuery->data(CategoryCodeQuery->index(row, 0)).toString();
            itemList.emplace_back(CategoryCode);
            delete CategoryCodeQuery;

            dialogUpdate.sendData(currentIndex, itemList);
            dialogUpdate.getWindow(0);
            dialogUpdate.preInput(0, currentRow-1);
            dialogUpdate.exec();
            const QStringList& data = dialogUpdate.getData(1);
            if (dialogUpdate.transaction() && !data.contains("")) {
                QString strQuery = "update Deliveries set ProductCode = %1, Name = '%2', DateReceived = '%3', Quantity = %4, PurchasePrice = %5, SupplierCode = %6, EmployeeAcceptedCode = %7, CategoryCode = %8, Quality = %9 where DeliveriesCode = %10;";
                databaseQuery.execQuery(QString(strQuery).arg(data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7], data[8], QString::number(rowIndex(currentRow))));

                strQuery = "insert into Logs(Date, \"Table\", EmployeeCode, ActionCode) values(datetime('now','localtime'), 'Deliveries', %1, 2);";
                databaseQuery.execQuery(QString(strQuery).arg(EmployeeCode));
            }
            break;
        }

        case 2: {
            QStringList ProductCode, CustomerCode, EmployeeCode;
            QVector<QStringList> itemList;
            int countRow = 0;

            QSqlQueryModel* ProductCodeQuery = databaseQuery.execSelectQuery("select ProductCode from Products");
            countRow = ProductCodeQuery->rowCount();
            for (int row = 0; row < countRow; ++row)
                ProductCode += ProductCodeQuery->data(ProductCodeQuery->index(row, 0)).toString();
            itemList.emplace_back(ProductCode);
            delete ProductCodeQuery;

            QSqlQueryModel* CustomerCodeQuery = databaseQuery.execSelectQuery("select CustomerCode from Customers");
            countRow = CustomerCodeQuery->rowCount();
            for (int row = 0; row < countRow; ++row)
                CustomerCode += CustomerCodeQuery->data(CustomerCodeQuery->index(row, 0)).toString();
            itemList.emplace_back(CustomerCode);
            delete CustomerCodeQuery;

            QSqlQueryModel* EmployeeCodeQuery = databaseQuery.execSelectQuery("select EmployeeCode from Employees");
            countRow = EmployeeCodeQuery->rowCount();
            for (int row = 0; row < countRow; ++row)
                EmployeeCode += EmployeeCodeQuery->data(EmployeeCodeQuery->index(row, 0)).toString();
            itemList.emplace_back(EmployeeCode);
            delete EmployeeCodeQuery;

            dialogUpdate.sendData(currentIndex, itemList);
            dialogUpdate.getWindow(1);
            dialogUpdate.preInput(1, currentRow-1);
            dialogUpdate.exec();
            const QStringList& data = dialogUpdate.getData(2);
            if (dialogUpdate.transaction() && !data.contains("")) {
                QString strQuery = "update Sales set ProductCode = %1, SaleDate = '%2', QuantitySold = %3, RetailPrice = %4, CustomerCode = %5, EmployeeCode = %6 where SaleCode = %7;";
                databaseQuery.execQuery(QString(strQuery).arg(data[0], data[1], data[2], data[3], data[4], data[5], QString::number(rowIndex(currentRow))));

                strQuery = "insert into Logs(Date, \"Table\", EmployeeCode, ActionCode) values(datetime('now','localtime'), 'Sales', %1, 2);";
                databaseQuery.execQuery(QString(strQuery).arg(::EmployeeCode));
            }
            break;
        }

        case 3: {
            dialogUpdate.getWindow(2);
            dialogUpdate.preInput(2, currentRow-1);
            dialogUpdate.exec();
            const QStringList& data = dialogUpdate.getData(3);
            if (dialogUpdate.transaction() && !data.contains("")) {
                QString strQuery = "update Products set Name = '%1', Quantity = %2 where ProductCode = %3;";
                databaseQuery.execQuery(QString(strQuery).arg(data[0], data[1], QString::number(rowIndex(currentRow))));

                strQuery = "insert into Logs(Date, \"Table\", EmployeeCode, ActionCode) values(datetime('now','localtime'), 'Products', %1, 2);";
                databaseQuery.execQuery(QString(strQuery).arg(EmployeeCode));
            }
            break;
        }
    }
}

void MainWindow::deleteInTable(QTableWidget* table) {
    database databaseQuery;
    QStringList tableNames = {"Deliveries", "Sales", "Products"};

    const int currentIndex = ui->tabWidget->currentIndex()-1, currentRow = table->currentItem()->row()+1;
    if (0 <= currentIndex && currentIndex < tableNames.size()) {
        QString strQuery = "delete from %1 where ProductCode = %2";
        databaseQuery.execQuery(QString(strQuery).arg(tableNames[currentIndex]).arg(rowIndex(currentRow)));

        strQuery = "insert into Logs(Date, \"Table\", EmployeeCode, ActionCode) values(datetime('now','localtime'), '%1', %2, 3);";
        databaseQuery.execQuery(QString(strQuery).arg(tableNames[currentIndex], EmployeeCode));
    }
}

QString simpleHash(const QString& input) {
    int hash = 0;

    for (auto &ch : input)
        hash ^= ch.unicode();

    return QString::number(hash+110000);
}

void MainWindow::activateContextMenu(QTableWidget *table) {
    table->setContextMenuPolicy(Qt::CustomContextMenu);

    switch(permission) {
    case 1:
        connect(table, &QTableWidget::doubleClicked, this, &MainWindow::addInTable);
        break;
    case 2:
        connect(table, &QTableWidget::doubleClicked, this, &MainWindow::addInTable);
        connect(table, &QTableWidget::doubleClicked, [=]() { editInTable(table); });
        break;
    case 3:
        connect(table, &QTableWidget::doubleClicked, this, &MainWindow::addInTable);
        connect(table, &QTableWidget::doubleClicked, [=]() { editInTable(table); });
        connect(table, &QTableWidget::doubleClicked, [=]() { deleteInTable(table); });
        break;
    }
    connect(table, &QTableWidget::customContextMenuRequested, [=](const QPoint &pos) { slotContextMenu(pos, table); });
}

void MainWindow::slotContextMenu(QPoint pos, QTableWidget *table) {
    QMenu *menu = new QMenu(this);

    QAction *addSlot = new QAction("Добавить", this);
    connect(addSlot, &QAction::triggered, this, &MainWindow::addInTable);

    QAction *editSlot = new QAction("Редактировать", this);
    connect(editSlot, &QAction::triggered, this, [table, this]() { editInTable(table); });

    QAction *deleteSlot = new QAction("Удалить", this);
    connect(deleteSlot, &QAction::triggered, this, [table, this]() { deleteInTable(table); });

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
        qDebug() << item;
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

void MainWindow::on_tabWidget_2_currentChanged(int index)
{
    database databaseQuery;

    switch(index) {
        case 0 : {
            showDB("Logs", ui->tableWidget);
        }
        case 2: {
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
    }
}

void MainWindow::on_pushButton_clicked()
{
    if (!ui->lineEdit->text().isEmpty() && !ui->lineEdit_2->text().isEmpty()) {
        database databaseQuery;

        QSqlQueryModel *query = databaseQuery.execSelectQuery(QString("select count(*) from Employees where EmployeeCode = %1 and Password = '%2'").arg(ui->lineEdit->text(), simpleHash(ui->lineEdit_2->text())));
        if (query->data(query->index(0, 0)).toString() == "1") {
            query = databaseQuery.execSelectQuery(QString("select EmployeeName, Position from Employees where EmployeeCode = %1 and Password = '%2'").arg(ui->lineEdit->text(), simpleHash(ui->lineEdit_2->text())));
            ui->label_6->setText(query->data(query->index(0, 0)).toString());
            ui->label_7->setText(query->data(query->index(0, 1)).toString());

            ui->stackedWidget->setCurrentIndex(1);

            permission = position[query->data(query->index(0, 1)).toString()];

            EmployeeCode = ui->lineEdit->text();

            ui->lineEdit->clear();
            ui->lineEdit_2->clear();
        }
    }
}

void MainWindow::on_pushButton_3_clicked()
{
    ui->label_6->clear();
    ui->label_7->clear();

    permission = 0;

    ui->stackedWidget->setCurrentIndex(0);
}
