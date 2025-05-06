
#include "includes/MainWindow.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QFormLayout>
#include <QGroupBox>
#include <QMessageBox>
#include <QHeaderView>
#include <QStatusBar>

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

    // Connect signals and slots
    connect(addButton, &QPushButton::clicked, this, &MainWindow::addProduct);
    connect(updateButton, &QPushButton::clicked, this, &MainWindow::updateProduct);
    connect(removeButton, &QPushButton::clicked, this, &MainWindow::removeProduct);
    connect(clearButton, &QPushButton::clicked, this, &MainWindow::clearForm);
    connect(productTable, &QTableWidget::itemSelectionChanged, this, &MainWindow::selectionChanged);

    // Initialize inventory manager and update table
    updateTable();
}

void MainWindow::addProduct()
{
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

        // Highlight low stock items (less than 10 units)
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