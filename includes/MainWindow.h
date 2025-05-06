#pragma once

#include "InventoryManager.h"

// Qt includes
#include <QMainWindow>
#include <QTableWidget>
#include <QLineEdit>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QPushButton>

// Chart includes
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
QT_CHARTS_USE_NAMESPACE

/**
 * @brief Main application window for inventory management
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief Construct the main window
     * @param parent Parent widget, defaults to nullptr
     */
    MainWindow(QWidget *parent = nullptr);

private:
    /**
     * @brief Validate the product input form
     * @return true if all fields are valid, false otherwise
     */
    bool validateForm();

private slots:
    /**
     * @brief Add a new product using the form data
     */
    void addProduct();

    /**
     * @brief Update the selected product with the form data
     */
    void updateProduct();

    /**
     * @brief Remove the selected product from inventory
     */
    void removeProduct();

    /**
     * @brief Clear all form fields and selection
     */
    void clearForm();

    /**
     * @brief Handle product selection in the table
     */
    void selectionChanged();

    /**
     * @brief Refresh the product table with current data
     */
    void updateTable();

    /**
     * @brief Display a chart showing inventory value by category
     */
    void showInventoryValueChart();

    /**
     * @brief Display a chart showing product count distribution by category
     */
    void showCategoryDistributionChart();

    /**
     * @brief Export inventory data to a CSV file
     */
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