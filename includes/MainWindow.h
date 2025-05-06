#pragma once

#include "InventoryManager.h"

#include <QMainWindow>
#include <QTableWidget>
#include <QLineEdit>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QPushButton>

// Include charts if using them
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
QT_CHARTS_USE_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private:
    bool validateForm();

private slots:
    void addProduct();
    void updateProduct();
    void removeProduct();
    void clearForm();
    void selectionChanged();
    void updateTable();
    void showInventoryValueChart();
    void showCategoryDistributionChart();
    void exportToCSV();

private:
    QTableWidget *productTable;
    QLineEdit *nameEdit;
    QLineEdit *categoryEdit;
    QDoubleSpinBox *priceSpinBox;
    QSpinBox *quantitySpinBox;
    QLineEdit *descriptionEdit;

    InventoryManager inventoryManager;
};