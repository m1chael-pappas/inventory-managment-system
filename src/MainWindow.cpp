
#include "includes/MainWindow.h"

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

    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    // Create product table
    productTable = new QTableWidget(0, 6, this);
    productTable->setHorizontalHeaderLabels({"ID", "Name", "Category", "Price", "Quantity", "Total Value"});
    productTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    productTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    productTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    mainLayout->addWidget(productTable);

    // Create form layout for product details
    QGroupBox *productDetailsGroup = new QGroupBox("Product Details");
    QFormLayout *formLayout = new QFormLayout();

    nameEdit = new QLineEdit();
    categoryEdit = new QLineEdit();
    priceSpinBox = new QDoubleSpinBox();
    priceSpinBox->setRange(0, 9999.99);
    priceSpinBox->setPrefix("$");
    quantitySpinBox = new QSpinBox();
    quantitySpinBox->setRange(0, 9999);
    descriptionEdit = new QLineEdit();

    formLayout->addRow("Name:", nameEdit);
    formLayout->addRow("Category:", categoryEdit);
    formLayout->addRow("Price:", priceSpinBox);
    formLayout->addRow("Quantity:", quantitySpinBox);
    formLayout->addRow("Description:", descriptionEdit);

    productDetailsGroup->setLayout(formLayout);
    mainLayout->addWidget(productDetailsGroup);

    // Create buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *addButton = new QPushButton("Add Product");
    QPushButton *updateButton = new QPushButton("Update Product");
    QPushButton *removeButton = new QPushButton("Remove Product");
    QPushButton *clearButton = new QPushButton("Clear Form");

    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(updateButton);
    buttonLayout->addWidget(removeButton);
    buttonLayout->addWidget(clearButton);

    mainLayout->addLayout(buttonLayout);

    // Create visualization buttons
    QHBoxLayout *chartButtonLayout = new QHBoxLayout();
    QPushButton *valueChartButton = new QPushButton("Show Value Chart");
    QPushButton *distributionChartButton = new QPushButton("Show Distribution Chart");

    chartButtonLayout->addWidget(valueChartButton);
    chartButtonLayout->addWidget(distributionChartButton);

    mainLayout->addLayout(chartButtonLayout);

    // Create export button
    QHBoxLayout *vizButtonLayout = new QHBoxLayout();
    QPushButton *exportButton = new QPushButton("Export to CSV");

    vizButtonLayout->addWidget(exportButton);

    mainLayout->addLayout(vizButtonLayout);

    // Connect signals and slots
    connect(addButton, &QPushButton::clicked, this, &MainWindow::addProduct);
    connect(updateButton, &QPushButton::clicked, this, &MainWindow::updateProduct);
    connect(removeButton, &QPushButton::clicked, this, &MainWindow::removeProduct);
    connect(clearButton, &QPushButton::clicked, this, &MainWindow::clearForm);
    connect(productTable, &QTableWidget::itemSelectionChanged, this, &MainWindow::selectionChanged);
    connect(valueChartButton, &QPushButton::clicked, this, &MainWindow::showInventoryValueChart);
    connect(distributionChartButton, &QPushButton::clicked, this, &MainWindow::showCategoryDistributionChart);
    connect(exportButton, &QPushButton::clicked, this, &MainWindow::exportToCSV);

    // Initialize inventory manager and update table
    updateTable();
}

bool MainWindow::validateForm()
{
    bool isValid = true;
    QString errorMessage;

    if (nameEdit->text().trimmed().isEmpty())
    {
        errorMessage += "- Product name cannot be empty\n";
        isValid = false;
    }

    if (categoryEdit->text().trimmed().isEmpty())
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

void MainWindow::addProduct()
{

    if (!validateForm())
    {
        return;
    }

    Product product(
        0,
        nameEdit->text().toStdString(),
        categoryEdit->text().toStdString(),
        priceSpinBox->value(),
        quantitySpinBox->value(),
        descriptionEdit->text().toStdString());

    try
    {
        inventoryManager.addProduct(product);
        updateTable();
        clearForm();
        QMessageBox::information(this, "Success", "Product added successfully.");
    }
    catch (const std::exception &e)
    {
        QMessageBox::critical(this, "Error", QString("Failed to add product: %1").arg(e.what()));
    }
}

void MainWindow::updateProduct()
{
    QList<QTableWidgetItem *> selectedItems = productTable->selectedItems();
    if (selectedItems.isEmpty())
    {
        QMessageBox::warning(this, "Warning", "No product selected.");
        return;
    }

    int row = selectedItems.first()->row();
    int id = productTable->item(row, 0)->text().toInt();

    Product product(
        id,
        nameEdit->text().toStdString(),
        categoryEdit->text().toStdString(),
        priceSpinBox->value(),
        quantitySpinBox->value(),
        descriptionEdit->text().toStdString());

    try
    {
        inventoryManager.updateProduct(id, product);
        updateTable();
        clearForm();
        QMessageBox::information(this, "Success", "Product updated successfully.");
    }
    catch (const std::exception &e)
    {
        QMessageBox::critical(this, "Error", QString("Failed to update product: %1").arg(e.what()));
    }
}

void MainWindow::removeProduct()
{
    QList<QTableWidgetItem *> selectedItems = productTable->selectedItems();
    if (selectedItems.isEmpty())
    {
        QMessageBox::warning(this, "Warning", "No product selected.");
        return;
    }

    int row = selectedItems.first()->row();
    int id = productTable->item(row, 0)->text().toInt();

    if (QMessageBox::question(this, "Confirm", "Are you sure you want to remove this product?",
                              QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
    {
        try
        {
            inventoryManager.removeProduct(id);
            updateTable();
            clearForm();
            QMessageBox::information(this, "Success", "Product removed successfully.");
        }
        catch (const std::exception &e)
        {
            QMessageBox::critical(this, "Error", QString("Failed to remove product: %1").arg(e.what()));
        }
    }
}

void MainWindow::clearForm()
{
    nameEdit->clear();
    categoryEdit->clear();
    priceSpinBox->setValue(0.0);
    quantitySpinBox->setValue(0);
    descriptionEdit->clear();

    productTable->clearSelection();
}

void MainWindow::selectionChanged()
{
    QList<QTableWidgetItem *> selectedItems = productTable->selectedItems();
    if (selectedItems.isEmpty())
    {
        return;
    }

    int row = selectedItems.first()->row();
    int id = productTable->item(row, 0)->text().toInt();

    try
    {
        Product product = inventoryManager.getProductById(id);
        nameEdit->setText(QString::fromStdString(product.getName()));
        categoryEdit->setText(QString::fromStdString(product.getCategory()));
        priceSpinBox->setValue(product.getPrice());
        quantitySpinBox->setValue(product.getQuantity());
        descriptionEdit->setText(QString::fromStdString(product.getDescription()));
    }
    catch (const std::exception &e)
    {
        QMessageBox::critical(this, "Error", QString("Failed to load product details: %1").arg(e.what()));
    }
}

void MainWindow::updateTable()
{
    productTable->setRowCount(0);

    const std::vector<Product> &products = inventoryManager.getAllProducts();
    double inventoryTotal = 0.0;

    for (const auto &product : products)
    {
        int row = productTable->rowCount();
        productTable->insertRow(row);

        // Calculate total value
        double totalValue = product.getPrice() * product.getQuantity();
        inventoryTotal += totalValue;

        QTableWidgetItem *idItem = new QTableWidgetItem(QString::number(product.getId()));
        idItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);

        QTableWidgetItem *nameItem = new QTableWidgetItem(QString::fromStdString(product.getName()));

        QTableWidgetItem *categoryItem = new QTableWidgetItem(QString::fromStdString(product.getCategory()));

        QTableWidgetItem *priceItem = new QTableWidgetItem(QString("$") + QString::number(product.getPrice(), 'f', 2));
        priceItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);

        QTableWidgetItem *quantityItem = new QTableWidgetItem(QString::number(product.getQuantity()));
        quantityItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);

        // Highlight low stock items
        if (product.getQuantity() < 10)
        {
            quantityItem->setBackground(QColor(255, 200, 200)); // Light red background
        }

        QTableWidgetItem *totalValueItem = new QTableWidgetItem(QString("$") + QString::number(totalValue, 'f', 2));
        totalValueItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);

        productTable->setItem(row, 0, idItem);
        productTable->setItem(row, 1, nameItem);
        productTable->setItem(row, 2, categoryItem);
        productTable->setItem(row, 3, priceItem);
        productTable->setItem(row, 4, quantityItem);
        productTable->setItem(row, 5, totalValueItem);
    }

    // Update status bar with total inventory value
    statusBar()->showMessage(QString("Total Inventory Value: $%1").arg(inventoryTotal, 0, 'f', 2));
}

void MainWindow::exportToCSV()
{
    QString filename = QFileDialog::getSaveFileName(this,
                                                    "Export Inventory", "", "CSV Files (*.csv)");

    if (filename.isEmpty())
    {
        return;
    }

    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::critical(this, "Error", "Could not open file for writing.");
        return;
    }

    QTextStream stream(&file);

    // Write header
    stream << "ID,Name,Category,Price,Quantity,Description,Total Value\n";

    // Write data
    const std::vector<Product> &products = inventoryManager.getAllProducts();
    for (const auto &product : products)
    {
        double totalValue = product.getPrice() * product.getQuantity();

        stream << product.getId() << ","
               << "\"" << QString::fromStdString(product.getName()).replace("\"", "\"\"") << "\","
               << "\"" << QString::fromStdString(product.getCategory()).replace("\"", "\"\"") << "\","
               << product.getPrice() << ","
               << product.getQuantity() << ","
               << "\"" << QString::fromStdString(product.getDescription()).replace("\"", "\"\"") << "\","
               << totalValue << "\n";
    }

    file.close();
    QMessageBox::information(this, "Success", "Inventory exported successfully to " + filename);
}

void MainWindow::showInventoryValueChart()
{
    // Create a dialog to display the chart
    QDialog *chartDialog = new QDialog(this);
    chartDialog->setWindowTitle("Inventory Value by Category");
    chartDialog->resize(600, 400);

    // Create a vertical layout for the dialog
    QVBoxLayout *layout = new QVBoxLayout(chartDialog);

    // Group products by category and calculate total value
    std::map<std::string, double> categoryValues;
    const std::vector<Product> &products = inventoryManager.getAllProducts();

    for (const auto &product : products)
    {
        double value = product.getPrice() * product.getQuantity();
        categoryValues[product.getCategory()] += value;
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

void MainWindow::showCategoryDistributionChart()
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
    const std::vector<Product> &products = inventoryManager.getAllProducts();

    for (const auto &product : products)
    {
        categoryCount[product.getCategory()]++;
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