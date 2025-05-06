#pragma once
#include <vector>
#include <string>
#include <stdexcept>
#include "Product.h"

// Custom exceptions
class InventoryException : public std::runtime_error
{
public:
    InventoryException(const std::string &message) : std::runtime_error(message) {}
};

class ProductNotFoundException : public InventoryException
{
public:
    ProductNotFoundException(int id)
        : InventoryException("Product with ID " + std::to_string(id) + " not found") {}
};

class FileOperationException : public InventoryException
{
public:
    FileOperationException(const std::string &operation, const std::string &filename)
        : InventoryException("Failed to " + operation + " file: " + filename) {}
};

class InventoryManager
{
private:
    std::vector<Product> products;
    int nextProductId;

public:
    // Constructor
    InventoryManager();

    // CRUD operations
    int addProduct(const Product &product);
    void updateProduct(int id, const Product &updatedProduct);
    void removeProduct(int id);
    Product getProductById(int id) const;
    std::vector<Product> findProductsByName(const std::string &name) const;
    std::vector<Product> findProductsByCategory(const std::string &category) const;
    const std::vector<Product> &getAllProducts() const;

    // Inventory statistics
    int getTotalProductCount() const;
    double getTotalInventoryValue() const;
    std::vector<Product> getLowStockProducts(int threshold) const;

    // File operations
    void saveToFile(const std::string &filename);
    void loadFromFile(const std::string &filename);
};