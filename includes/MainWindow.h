
#pragma once

#include <QMainWindow>
#include <QTableWidget>
#include <QLineEdit>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include "InventoryManager.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private slots:
    void addProduct();
    void updateProduct();
    void removeProduct();
    void clearForm();
    void selectionChanged();
    void updateTable();

private:
    QTableWidget *productTable;
    QLineEdit *nameEdit;
    QLineEdit *categoryEdit;
    QDoubleSpinBox *priceSpinBox;
    QSpinBox *quantitySpinBox;
    QLineEdit *descriptionEdit;

    InventoryManager inventoryManager;
};