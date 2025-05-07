#pragma once

#include "inventory_manager.h"

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
    bool validate_form();

private slots:
    /**
     * @brief Add a new product using the form data
     */
    void add_product();

    /**
     * @brief Update the selected product with the form data
     */
    void update_product();

    /**
     * @brief Remove the selected product from inventory
     */
    void remove_product();

    /**
     * @brief Clear all form fields and selection
     */
    void clear_form();

    /**
     * @brief Handle product selection in the table
     */
    void selection_changed();

    /**
     * @brief Refresh the product table with current data
     */
    void update_table();

    /**
     * @brief Display a chart showing inventory value by category
     */
    void show_inventory_value_chart();

    /**
     * @brief Display a chart showing product count distribution by category
     */
    void show_category_distribution_chart();

    /**
     * @brief Export inventory data to a CSV file
     */
    void export_to_csv();

    /**
     * @brief Import inventory data from a CSV file
     */
    void import_from_csv();

    /**
     * @brief Search for products by name
     */
    void search_by_name();

    /**
     * @brief Search for products by category
     */
    void search_by_category();

    /**
     * @brief Display low stock products
     * @param threshold The quantity threshold below which products are considered low stock
     */
    void show_low_stock_products(int threshold = 10);

    /**
     * @brief Reset the search filter and show all products
     */
    void reset_search();

private:
    QTableWidget *product_table;
    QLineEdit *name_edit;
    QLineEdit *category_edit;
    QDoubleSpinBox *price_spin_box;
    QSpinBox *quantity_spin_box;
    QLineEdit *description_edit;

    // Search components
    QLineEdit *search_edit;
    QPushButton *search_name_button;
    QPushButton *search_category_button;
    QPushButton *reset_search_button;
    QPushButton *low_stock_button;

    // File operation buttons
    QPushButton *import_button;
    QPushButton *export_button;

    InventoryManager inventory_manager;
};