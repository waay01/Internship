#include "dialog.h"
#include "ui_dialog.h"

bool status = false;

dialog::dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dialog)
{
    ui->setupUi(this);
}

dialog::~dialog()
{
    delete ui;
}

void dialog::getWindow(int index) {
    ui->stackedWidget->setCurrentIndex(index);
}

void dialog::preInput(int indexTable, int row) {
    database databaseQuery;
    QStringList listTable = {"Deliveries", "Sales", "Products"}, itemList, nameColumn;
    int countRow = 0, countColumn = 0;
    nameColumn = databaseQuery.execQueryPRAGMA(listTable[indexTable]);
    countColumn = nameColumn.size();

    QSqlQueryModel* query = databaseQuery.execSelectQuery(QString("select * from %1").arg(listTable[indexTable]));

    for (int column = 1; column < countColumn; ++column)
        itemList += query->data(query->index(row, column)).toString();

    qDebug() << itemList[2];

    switch(indexTable) {
        case 0:
            ui->lineEdit_6->setText(itemList[0]);
            ui->lineEdit_7->setText(itemList[1]);
            ui->dateEdit->setDate(QDate().fromString(itemList[2], "yyyy-MM-dd"));
            ui->lineEdit_8->setText(itemList[3]);
            ui->lineEdit_9->setText(itemList[4]);
            ui->comboBox_2->setCurrentText(itemList[5]);
            ui->comboBox_3->setCurrentText(itemList[6]);
            ui->comboBox_4->setCurrentText(itemList[7]);
            ui->lineEdit_10->setText(itemList[8]);
            break;
        case 1:
            ui->comboBox->setCurrentText(itemList[0]);
            ui->dateEdit_2->setDate(QDate().fromString(itemList[1], "yyyy-MM-dd"));
            ui->lineEdit_2->setText(itemList[2]);
            ui->lineEdit_17->setText(itemList[3]);
            ui->comboBox_9->setCurrentText(itemList[4]);
            ui->comboBox_10->setCurrentText(itemList[5]);
            break;
        case 2:
            ui->lineEdit->setText(itemList[0]);
            ui->lineEdit_3->setText(itemList[1]);
            break;
    }

    delete query;
}


void dialog::setName(const QString& name) {
    ui->label->setText(name + ':');
}

void dialog::sendData(int index, const QVector<QStringList>& itemList) {
    if (index == 1) {
        ui->comboBox_2->addItems(itemList[0]);
        ui->comboBox_3->addItems(itemList[1]);
        ui->comboBox_4->addItems(itemList[2]);
    } else if (index == 2) {
        ui->comboBox->addItems(itemList[0]);
        ui->comboBox_9->addItems(itemList[1]);
        ui->comboBox_10->addItems(itemList[2]);
    }
}

QStringList dialog::getData(int index) {
    QStringList dataList = {};

    switch(index) {
    case 1:
        dataList += ui->lineEdit_6->text();
        dataList += ui->lineEdit_7->text();
        dataList += ui->dateEdit->text();
        dataList += ui->lineEdit_8->text();
        dataList += ui->lineEdit_9->text();
        dataList += ui->comboBox_2->currentText();
        dataList += ui->comboBox_3->currentText();
        dataList += ui->comboBox_4->currentText();
        dataList += ui->lineEdit_10->text();
        break;
    case 2:
        dataList += ui->comboBox->currentText();
        dataList += ui->dateEdit_2->text();
        dataList += ui->lineEdit_2->text();
        dataList += ui->lineEdit_17->text();
        dataList += ui->comboBox_9->currentText();
        dataList += ui->comboBox_10->currentText();
        break;
    case 3:
        dataList += ui->lineEdit->text();
        dataList += ui->lineEdit_3->text();
        break;
    }

    return dataList;
}

bool dialog::transaction() {
    return status;
}

void dialog::on_pushButton_clicked()
{
    status = true;
    transaction();
    close();
}
