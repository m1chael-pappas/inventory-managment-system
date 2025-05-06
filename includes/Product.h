#pragma once
#include <string>

/**
 * @brief Represents a product in the inventory system
 */
class Product
{
private:
    int id;                  // Unique identifier
    std::string name;        // Product name
    std::string category;    // Product category
    double price;            // Unit price
    int quantity;            // Available quantity
    std::string description; // Product description

public:
    /**
     * @brief Construct a new empty Product
     */
    Product();

    /**
     * @brief Construct a new Product with all attributes
     * @param id Unique identifier for the product
     * @param name The product name
     * @param category The product category
     * @param price The unit price
     * @param quantity The available quantity
     * @param description The product description (optional)
     */
    Product(int id, const std::string &name, const std::string &category,
            double price, int quantity, const std::string &description = "");

    // Getters
    /**
     * @brief Get the product ID
     * @return The product ID
     */
    int getId() const;

    /**
     * @brief Get the product name
     * @return The product name
     */
    std::string getName() const;

    /**
     * @brief Get the product category
     * @return The product category
     */
    std::string getCategory() const;

    /**
     * @brief Get the unit price
     * @return The unit price
     */
    double getPrice() const;

    /**
     * @brief Get the available quantity
     * @return The available quantity
     */
    int getQuantity() const;

    /**
     * @brief Get the product description
     * @return The product description
     */
    std::string getDescription() const;

    // Setters
    /**
     * @brief Set the product ID
     * @param newId The new ID value
     */
    void setId(int newId);

    /**
     * @brief Set the product name
     * @param newName The new name value
     */
    void setName(const std::string &newName);

    /**
     * @brief Set the product category
     * @param newCategory The new category value
     */
    void setCategory(const std::string &newCategory);

    /**
     * @brief Set the unit price
     * @param newPrice The new price value
     */
    void setPrice(double newPrice);

    /**
     * @brief Set the available quantity
     * @param newQuantity The new quantity value
     */
    void setQuantity(int newQuantity);

    /**
     * @brief Set the product description
     * @param newDescription The new description value
     */
    void setDescription(const std::string &newDescription);

    // Helper methods
    /**
     * @brief Calculate the total value of this product (price * quantity)
     * @return The total value
     */
    double getTotalValue() const;

    /**
     * @brief Check if the product is below the given stock threshold
     * @param threshold The minimum quantity threshold
     * @return true if quantity is below threshold, false otherwise
     */
    bool isLowStock(int threshold) const;

    /**
     * @brief Get a string representation of the product
     * @return A formatted string with product details
     */
    std::string toString() const;

    // For inventory manager internal use
    friend class InventoryManager;
};