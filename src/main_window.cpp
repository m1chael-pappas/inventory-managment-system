
#include "includes/main_window.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QMessageBox>
#include <QHeaderView>
#include <QStatusBar>
#include <QDialog>
#include <QFileDialog>
#include <QProgressBar>
#include <QTextStream>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setWindowTitle("Inventory Management System");
    resize(800, 600);

    QWidget *central_widget = new QWidget(this);
    setCentralWidget(central_widget);

    QVBoxLayout *main_layout = new QVBoxLayout(central_widget);

    // Create search area
    QGroupBox *search_group = new QGroupBox("Search Products");
    QHBoxLayout *search_layout = new QHBoxLayout();

    search_edit = new QLineEdit();
    search_name_button = new QPushButton("Search by Name");
    search_category_button = new QPushButton("Search by Category");
    reset_search_button = new QPushButton("Show All");
    low_stock_button = new QPushButton("Show Low Stock");

    search_layout->addWidget(new QLabel("Search:"));
    search_layout->addWidget(search_edit);
    search_layout->addWidget(search_name_button);
    search_layout->addWidget(search_category_button);
    search_layout->addWidget(reset_search_button);
    search_layout->addWidget(low_stock_button);

    search_group->setLayout(search_layout);
    main_layout->addWidget(search_group);

    // Create product table
    product_table = new QTableWidget(0, 6, this);
    product_table->setHorizontalHeaderLabels({"ID", "Name", "Category", "Price", "Quantity", "Total Value"});
    product_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    product_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    product_table->setEditTriggers(QAbstractItemView::NoEditTriggers);

    main_layout->addWidget(product_table);

    // Create form layout for product details
    QGroupBox *product_details_group = new QGroupBox("Product Details");
    QFormLayout *form_layout = new QFormLayout();

    name_edit = new QLineEdit();
    category_edit = new QLineEdit();
    price_spin_box = new QDoubleSpinBox();
    price_spin_box->setRange(0, 9999.99);
    price_spin_box->setPrefix("$");
    quantity_spin_box = new QSpinBox();
    quantity_spin_box->setRange(0, 9999);
    description_edit = new QLineEdit();

    form_layout->addRow("Name:", name_edit);
    form_layout->addRow("Category:", category_edit);
    form_layout->addRow("Price:", price_spin_box);
    form_layout->addRow("Quantity:", quantity_spin_box);
    form_layout->addRow("Description:", description_edit);

    product_details_group->setLayout(form_layout);
    main_layout->addWidget(product_details_group);

    // Create CRUD buttons
    QHBoxLayout *button_layout = new QHBoxLayout();
    QPushButton *add_button = new QPushButton("Add Product");
    QPushButton *update_button = new QPushButton("Update Product");
    QPushButton *remove_button = new QPushButton("Remove Product");
    QPushButton *clear_button = new QPushButton("Clear Form");

    button_layout->addWidget(add_button);
    button_layout->addWidget(update_button);
    button_layout->addWidget(remove_button);
    button_layout->addWidget(clear_button);

    main_layout->addLayout(button_layout);

    // Create file operation and visualization buttons
    QHBoxLayout *utility_button_layout = new QHBoxLayout();

    import_button = new QPushButton("Import from CSV");
    export_button = new QPushButton("Export to CSV");
    QPushButton *value_chart_button = new QPushButton("Show Value Chart");
    QPushButton *distribution_chart_button = new QPushButton("Show Distribution Chart");

    utility_button_layout->addWidget(import_button);
    utility_button_layout->addWidget(export_button);
    utility_button_layout->addWidget(value_chart_button);
    utility_button_layout->addWidget(distribution_chart_button);

    main_layout->addLayout(utility_button_layout);

    // Connect signals and slots
    connect(add_button, &QPushButton::clicked, this, &MainWindow::add_product);
    connect(update_button, &QPushButton::clicked, this, &MainWindow::update_product);
    connect(remove_button, &QPushButton::clicked, this, &MainWindow::remove_product);
    connect(clear_button, &QPushButton::clicked, this, &MainWindow::clear_form);
    connect(product_table, &QTableWidget::itemSelectionChanged, this, &MainWindow::selection_changed);

    connect(search_name_button, &QPushButton::clicked, this, &MainWindow::search_by_name);
    connect(search_category_button, &QPushButton::clicked, this, &MainWindow::search_by_category);
    connect(reset_search_button, &QPushButton::clicked, this, &MainWindow::reset_search);
    connect(low_stock_button, &QPushButton::clicked, [this]()
            { this->show_low_stock_products(); });

    connect(import_button, &QPushButton::clicked, this, &MainWindow::import_from_csv);
    connect(export_button, &QPushButton::clicked, this, &MainWindow::export_to_csv);
    connect(value_chart_button, &QPushButton::clicked, this, &MainWindow::show_inventory_value_chart);
    connect(distribution_chart_button, &QPushButton::clicked, this, &MainWindow::show_category_distribution_chart);

    // Initialize inventory manager and update table
    update_table();
}

bool MainWindow::validate_form()
{
    bool isValid = true;
    QString errorMessage;

    if (name_edit->text().trimmed().isEmpty())
    {
        errorMessage += "- Product name cannot be empty\n";
        isValid = false;
    }

    if (category_edit->text().trimmed().isEmpty())
    {
        errorMessage += "- Category cannot be empty\n";
        isValid = false;
    }

    if (!isValid)
    {
        QMessageBox::warning(this, "Validation Error",
                             "Please correct the following errors:\n\n" + errorMessage);
    }

    return isValid;
}

void MainWindow::add_product()
{

    if (!validate_form())
    {
        return;
    }

    Product product(
        0,
        name_edit->text().toStdString(),
        category_edit->text().toStdString(),
        price_spin_box->value(),
        quantity_spin_box->value(),
        description_edit->text().toStdString());

    try
    {
        inventory_manager.add_product(product);
        update_table();
        clear_form();
        QMessageBox::information(this, "Success", "Product added successfully.");
    }
    catch (const std::exception &e)
    {
        QMessageBox::critical(this, "Error", QString("Failed to add product: %1").arg(e.what()));
    }
}

void MainWindow::update_product()
{
    QList<QTableWidgetItem *> selectedItems = product_table->selectedItems();
    if (selectedItems.isEmpty())
    {
        QMessageBox::warning(this, "Warning", "No product selected.");
        return;
    }

    int row = selectedItems.first()->row();
    int id = product_table->item(row, 0)->text().toInt();

    Product product(
        id,
        name_edit->text().toStdString(),
        category_edit->text().toStdString(),
        price_spin_box->value(),
        quantity_spin_box->value(),
        description_edit->text().toStdString());

    try
    {
        inventory_manager.update_product(id, product);
        update_table();
        clear_form();
        QMessageBox::information(this, "Success", "Product updated successfully.");
    }
    catch (const std::exception &e)
    {
        QMessageBox::critical(this, "Error", QString("Failed to update product: %1").arg(e.what()));
    }
}

void MainWindow::remove_product()
{
    QList<QTableWidgetItem *> selectedItems = product_table->selectedItems();
    if (selectedItems.isEmpty())
    {
        QMessageBox::warning(this, "Warning", "No product selected.");
        return;
    }

    int row = selectedItems.first()->row();
    int id = product_table->item(row, 0)->text().toInt();

    if (QMessageBox::question(this, "Confirm", "Are you sure you want to remove this product?",
                              QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
    {
        try
        {
            inventory_manager.remove_product(id);
            update_table();
            clear_form();
            QMessageBox::information(this, "Success", "Product removed successfully.");
        }
        catch (const std::exception &e)
        {
            QMessageBox::critical(this, "Error", QString("Failed to remove product: %1").arg(e.what()));
        }
    }
}

void MainWindow::clear_form()
{
    name_edit->clear();
    category_edit->clear();
    price_spin_box->setValue(0.0);
    quantity_spin_box->setValue(0);
    description_edit->clear();

    product_table->clearSelection();
}

void MainWindow::selection_changed()
{
    QList<QTableWidgetItem *> selectedItems = product_table->selectedItems();
    if (selectedItems.isEmpty())
    {
        return;
    }

    int row = selectedItems.first()->row();
    int id = product_table->item(row, 0)->text().toInt();

    try
    {
        Product product = inventory_manager.get_product_by_id(id);
        name_edit->setText(QString::fromStdString(product.get_name()));
        category_edit->setText(QString::fromStdString(product.get_category()));
        price_spin_box->setValue(product.get_price());
        quantity_spin_box->setValue(product.get_quantity());
        description_edit->setText(QString::fromStdString(product.get_description()));
    }
    catch (const std::exception &e)
    {
        QMessageBox::critical(this, "Error", QString("Failed to load product details: %1").arg(e.what()));
    }
}

void MainWindow::update_table()
{
    product_table->setRowCount(0);

    const std::vector<Product> &products = inventory_manager.get_all_products();
    double inventoryTotal = 0.0;

    for (const auto &product : products)
    {
        int row = product_table->rowCount();
        product_table->insertRow(row);

        // Calculate total value
        double totalValue = product.get_price() * product.get_quantity();
        inventoryTotal += totalValue;

        QTableWidgetItem *idItem = new QTableWidgetItem(QString::number(product.get_id()));
        idItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);

        QTableWidgetItem *nameItem = new QTableWidgetItem(QString::fromStdString(product.get_name()));

        QTableWidgetItem *categoryItem = new QTableWidgetItem(QString::fromStdString(product.get_category()));

        QTableWidgetItem *priceItem = new QTableWidgetItem(QString("$") + QString::number(product.get_price(), 'f', 2));
        priceItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);

        QTableWidgetItem *quantityItem = new QTableWidgetItem(QString::number(product.get_quantity()));
        quantityItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);

        // Highlight low stock items
        if (product.get_quantity() < 10)
        {
            quantityItem->setBackground(QColor(255, 200, 200)); // Light red background
        }

        QTableWidgetItem *totalValueItem = new QTableWidgetItem(QString("$") + QString::number(totalValue, 'f', 2));
        totalValueItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);

        product_table->setItem(row, 0, idItem);
        product_table->setItem(row, 1, nameItem);
        product_table->setItem(row, 2, categoryItem);
        product_table->setItem(row, 3, priceItem);
        product_table->setItem(row, 4, quantityItem);
        product_table->setItem(row, 5, totalValueItem);
    }

    // Update status bar with total inventory value
    statusBar()->showMessage(QString("Total Inventory Value: $%1").arg(inventoryTotal, 0, 'f', 2));
}

void MainWindow::export_to_csv()
{
    QString filename = QFileDialog::getSaveFileName(this,
                                                    "Export Inventory", "", "CSV Files (*.csv)");

    if (filename.isEmpty())
    {
        return;
    }

    // Ensure filename ends with .csv
    if (!filename.endsWith(".csv", Qt::CaseInsensitive))
    {
        filename += ".csv";
    }

    try
    {
        inventory_manager.save_to_file(filename.toStdString());
        QMessageBox::information(this, "Success", "Inventory exported successfully to " + filename);
    }
    catch (const std::exception &e)
    {
        QMessageBox::critical(this, "Error", QString("Failed to export inventory data: %1").arg(e.what()));
    }
}

void MainWindow::show_inventory_value_chart()
{
    // Create a dialog to display the chart
    QDialog *chartDialog = new QDialog(this);
    chartDialog->setWindowTitle("Inventory Value by Category");
    chartDialog->resize(600, 400);

    // Create a vertical layout for the dialog
    QVBoxLayout *layout = new QVBoxLayout(chartDialog);

    // Group products by category and calculate total value
    std::map<std::string, double> categoryValues;
    const std::vector<Product> &products = inventory_manager.get_all_products();

    for (const auto &product : products)
    {
        double value = product.get_price() * product.get_quantity();
        categoryValues[product.get_category()] += value;
    }

    // Create a single bar set for all categories
    QBarSeries *series = new QBarSeries();

    // Add a bar set for each category
    for (const auto &pair : categoryValues)
    {
        QBarSet *barSet = new QBarSet(QString::fromStdString(pair.first));
        *barSet << pair.second;
        series->append(barSet);
    }

    // Create the chart and add the series
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Inventory Value by Category");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    // Create the axes
    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText("Value ($)");
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    // Add a simple axis with just "Categories" as the label
    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append("Categories");
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    // Hide the x-axis labels since they're redundant
    axisX->setLabelsVisible(false);

    // Make the legend visible to identify categories
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignRight);

    // Create chart view
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    layout->addWidget(chartView);

    chartDialog->exec();

    delete chartDialog;
}

void MainWindow::show_category_distribution_chart()
{
    // Create a dialog to display the chart
    QDialog *chartDialog = new QDialog(this);
    chartDialog->setWindowTitle("Product Distribution by Category");
    chartDialog->resize(600, 400);

    // Create a vertical layout for the dialog
    QVBoxLayout *layout = new QVBoxLayout(chartDialog);

    // Create a pie series for the chart
    QPieSeries *series = new QPieSeries();

    // Group products by category and count
    std::map<std::string, int> categoryCount;
    const std::vector<Product> &products = inventory_manager.get_all_products();

    for (const auto &product : products)
    {
        categoryCount[product.get_category()]++;
    }

    // Add slices for each category
    for (const auto &pair : categoryCount)
    {
        series->append(QString::fromStdString(pair.first), pair.second);
    }

    // Create the chart and add the series
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Product Distribution by Category");
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignRight);

    // Create chart view
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    // Add the chart view to the layout
    layout->addWidget(chartView);

    // Show the dialog
    chartDialog->exec();

    // Clean up
    delete chartDialog;
}

void MainWindow::search_by_name()
{
    QString search_text = search_edit->text().trimmed();
    if (search_text.isEmpty())
    {
        QMessageBox::warning(this, "Search Error", "Please enter a search term.");
        return;
    }

    std::vector<Product> results = inventory_manager.find_products_by_name(search_text.toStdString());
    if (results.empty())
    {
        QMessageBox::information(this, "Search Results", "No products found matching the search term.");
        return;
    }

    // Clear the table and display only search results
    product_table->setRowCount(0);
    double inventory_total = 0.0;

    for (const auto &product : results)
    {
        int row = product_table->rowCount();
        product_table->insertRow(row);

        double total_value = product.get_total_value();
        inventory_total += total_value;

        QTableWidgetItem *id_item = new QTableWidgetItem(QString::number(product.get_id()));
        id_item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);

        QTableWidgetItem *name_item = new QTableWidgetItem(QString::fromStdString(product.get_name()));
        QTableWidgetItem *category_item = new QTableWidgetItem(QString::fromStdString(product.get_category()));

        QTableWidgetItem *price_item = new QTableWidgetItem(QString("$") + QString::number(product.get_price(), 'f', 2));
        price_item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);

        QTableWidgetItem *quantity_item = new QTableWidgetItem(QString::number(product.get_quantity()));
        quantity_item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);

        // Highlight low stock items
        if (product.is_low_stock(10))
        {
            quantity_item->setBackground(QColor(255, 200, 200)); // Light red background
        }

        QTableWidgetItem *total_value_item = new QTableWidgetItem(QString("$") + QString::number(total_value, 'f', 2));
        total_value_item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);

        product_table->setItem(row, 0, id_item);
        product_table->setItem(row, 1, name_item);
        product_table->setItem(row, 2, category_item);
        product_table->setItem(row, 3, price_item);
        product_table->setItem(row, 4, quantity_item);
        product_table->setItem(row, 5, total_value_item);
    }

    statusBar()->showMessage(QString("Found %1 products. Total Value: $%2")
                                 .arg(results.size())
                                 .arg(inventory_total, 0, 'f', 2));
}

void MainWindow::search_by_category()
{
    QString search_text = search_edit->text().trimmed();
    if (search_text.isEmpty())
    {
        QMessageBox::warning(this, "Search Error", "Please enter a category name.");
        return;
    }

    std::vector<Product> results = inventory_manager.find_products_by_category(search_text.toStdString());
    if (results.empty())
    {
        QMessageBox::information(this, "Search Results", "No products found in this category.");
        return;
    }

    // Clear the table and display only search results
    product_table->setRowCount(0);
    double inventory_total = 0.0;

    for (const auto &product : results)
    {
        int row = product_table->rowCount();
        product_table->insertRow(row);

        double total_value = product.get_total_value();
        inventory_total += total_value;

        QTableWidgetItem *id_item = new QTableWidgetItem(QString::number(product.get_id()));
        id_item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);

        QTableWidgetItem *name_item = new QTableWidgetItem(QString::fromStdString(product.get_name()));
        QTableWidgetItem *category_item = new QTableWidgetItem(QString::fromStdString(product.get_category()));

        QTableWidgetItem *price_item = new QTableWidgetItem(QString("$") + QString::number(product.get_price(), 'f', 2));
        price_item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);

        QTableWidgetItem *quantity_item = new QTableWidgetItem(QString::number(product.get_quantity()));
        quantity_item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);

        // Highlight low stock items
        if (product.is_low_stock(10))
        {
            quantity_item->setBackground(QColor(255, 200, 200)); // Light red background
        }

        QTableWidgetItem *total_value_item = new QTableWidgetItem(QString("$") + QString::number(total_value, 'f', 2));
        total_value_item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);

        product_table->setItem(row, 0, id_item);
        product_table->setItem(row, 1, name_item);
        product_table->setItem(row, 2, category_item);
        product_table->setItem(row, 3, price_item);
        product_table->setItem(row, 4, quantity_item);
        product_table->setItem(row, 5, total_value_item);
    }

    statusBar()->showMessage(QString("Found %1 products in category '%2'. Total Value: $%3")
                                 .arg(results.size())
                                 .arg(search_text)
                                 .arg(inventory_total, 0, 'f', 2));
}

void MainWindow::reset_search()
{
    search_edit->clear();
    update_table();
    statusBar()->showMessage("Showing all products");
}

void MainWindow::show_low_stock_products(int threshold)
{
    std::vector<Product> results = inventory_manager.get_low_stock_products(threshold);
    if (results.empty())
    {
        QMessageBox::information(this, "Low Stock", "No products are below the stock threshold.");
        return;
    }

    // Clear the table and display only low stock products
    product_table->setRowCount(0);
    double inventory_total = 0.0;

    for (const auto &product : results)
    {
        int row = product_table->rowCount();
        product_table->insertRow(row);

        double total_value = product.get_total_value();
        inventory_total += total_value;

        QTableWidgetItem *id_item = new QTableWidgetItem(QString::number(product.get_id()));
        id_item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);

        QTableWidgetItem *name_item = new QTableWidgetItem(QString::fromStdString(product.get_name()));
        QTableWidgetItem *category_item = new QTableWidgetItem(QString::fromStdString(product.get_category()));

        QTableWidgetItem *price_item = new QTableWidgetItem(QString("$") + QString::number(product.get_price(), 'f', 2));
        price_item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);

        QTableWidgetItem *quantity_item = new QTableWidgetItem(QString::number(product.get_quantity()));
        quantity_item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        quantity_item->setBackground(QColor(255, 200, 200)); // Highlight all low stock items

        QTableWidgetItem *total_value_item = new QTableWidgetItem(QString("$") + QString::number(total_value, 'f', 2));
        total_value_item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);

        product_table->setItem(row, 0, id_item);
        product_table->setItem(row, 1, name_item);
        product_table->setItem(row, 2, category_item);
        product_table->setItem(row, 3, price_item);
        product_table->setItem(row, 4, quantity_item);
        product_table->setItem(row, 5, total_value_item);
    }

    statusBar()->showMessage(QString("Found %1 products with low stock (below %2). Total Value: $%3")
                                 .arg(results.size())
                                 .arg(threshold)
                                 .arg(inventory_total, 0, 'f', 2));
}

void MainWindow::import_from_csv()
{
    QString filename = QFileDialog::getOpenFileName(this,
                                                    "Import Inventory", "", "CSV Files (*.csv)");

    if (filename.isEmpty())
    {
        return;
    }

    try
    {
        inventory_manager.load_from_file(filename.toStdString());
        update_table();
        QMessageBox::information(this, "Success", "Inventory data imported successfully from " + filename);
    }
    catch (const std::exception &e)
    {
        QMessageBox::critical(this, "Error", QString("Failed to import inventory data: %1").arg(e.what()));
    }
}