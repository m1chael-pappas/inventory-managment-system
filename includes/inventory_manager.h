#pragma once
#include <vector>
#include <string>
#include <stdexcept>
#include "product.h"

// Custom exceptions
/**
 * @brief Base exception class for inventory-related errors
 */
class InventoryException : public std::runtime_error
{
public:
    /**
     * @brief Construct a new Inventory Exception
     * @param message The error message
     */
    InventoryException(const std::string &message) : std::runtime_error(message) {}
};

/**
 * @brief Exception thrown when a product cannot be found
 */
class ProductNotFoundException : public InventoryException
{
public:
    /**
     * @brief Construct a new Product Not Found Exception
     * @param id The ID of the product that was not found
     */
    ProductNotFoundException(int id)
        : InventoryException("Product with ID " + std::to_string(id) + " not found") {}
};

/**
 * @brief Exception thrown when a file operation fails
 */
class FileOperationException : public InventoryException
{
public:
    /**
     * @brief Construct a new File Operation Exception
     * @param operation The operation that failed (e.g., "open", "read", "write")
     * @param filename The name of the file involved
     */
    FileOperationException(const std::string &operation, const std::string &filename)
        : InventoryException("Failed to " + operation + " file: " + filename) {}
};

/**
 * @brief Manages a collection of products and provides CRUD operations
 */
class InventoryManager
{
private:
    std::vector<Product> products;
    int next_product_id;

public:
    /**
     * @brief Construct a new Inventory Manager with default values
     */
    InventoryManager();

    // CRUD operations

    /**
     * @brief Add a new product to the inventory
     * @param product The product to add (ID will be assigned automatically)
     * @return The ID assigned to the new product
     */
    int add_product(const Product &product);

    /**
     * @brief Update an existing product's details
     * @param id The ID of the product to update
     * @param updated_product The product with updated values
     * @throws ProductNotFoundException If the product with the given ID doesn't exist
     */
    void update_product(int id, const Product &updated_product);

    /**
     * @brief Remove a product from the inventory
     * @param id The ID of the product to remove
     * @throws ProductNotFoundException If the product with the given ID doesn't exist
     */
    void remove_product(int id);

    /**
     * @brief Get a product by its ID
     * @param id The ID of the product to retrieve
     * @return The product with the specified ID
     * @throws ProductNotFoundException If the product with the given ID doesn't exist
     */
    Product get_product_by_id(int id) const;

    /**
     * @brief Find products by matching their name
     * @param name The name or partial name to search for
     * @return A vector of products whose names contain the search term
     */
    std::vector<Product> find_products_by_name(const std::string &name) const;

    /**
     * @brief Find products by exact category match
     * @param category The category to search for
     * @return A vector of products in the specified category
     */
    std::vector<Product> find_products_by_category(const std::string &category) const;

    /**
     * @brief Get all products in the inventory
     * @return A const reference to the vector of all products
     */
    const std::vector<Product> &get_all_products() const;

    // Inventory statistics
    /**
     * @brief Get the total number of unique products in the inventory
     * @return The count of products
     */
    int get_total_product_count() const;

    /**
     * @brief Calculate the total monetary value of all inventory
     * @return The sum of (price * quantity) for all products
     */
    double get_total_inventory_value() const;

    /**
     * @brief Find products with stock below a specified threshold
     * @param threshold The quantity threshold below which products are considered low stock
     * @return A vector of products with quantity less than the threshold
     */
    std::vector<Product> get_low_stock_products(int threshold) const;

    // File operations
    /**
     * @brief Save the current inventory to a CSV file
     * @param filename The name of the file to save to
     * @throws FileOperationException If the file cannot be opened or written to
     */
    void save_to_file(const std::string &filename);

    /**
     * @brief Load inventory from a CSV file
     * @param filename The name of the file to load from
     * @throws FileOperationException If the file cannot be opened or read from
     */
    void load_from_file(const std::string &filename);
};