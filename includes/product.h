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
    int get_id() const;

    /**
     * @brief Get the product name
     * @return The product name
     */
    std::string get_name() const;

    /**
     * @brief Get the product category
     * @return The product category
     */
    std::string get_category() const;

    /**
     * @brief Get the unit price
     * @return The unit price
     */
    double get_price() const;

    /**
     * @brief Get the available quantity
     * @return The available quantity
     */
    int get_quantity() const;

    /**
     * @brief Get the product description
     * @return The product description
     */
    std::string get_description() const;

    // Setters
    /**
     * @brief Set the product ID
     * @param new_id The new ID value
     */
    void set_id(int new_id);

    /**
     * @brief Set the product name
     * @param new_name The new name value
     */
    void set_name(const std::string &new_name);

    /**
     * @brief Set the product category
     * @param new_category The new category value
     */
    void set_category(const std::string &new_category);

    /**
     * @brief Set the unit price
     * @param new_price The new price value
     */
    void set_price(double new_price);

    /**
     * @brief Set the available quantity
     * @param new_quantity The new quantity value
     */
    void set_quantity(int new_quantity);

    /**
     * @brief Set the product description
     * @param new_description The new description value
     */
    void set_description(const std::string &new_description);

    // Helper methods
    /**
     * @brief Calculate the total value of this product (price * quantity)
     * @return The total value
     */
    double get_total_value() const;

    /**
     * @brief Check if the product is below the given stock threshold
     * @param threshold The minimum quantity threshold
     * @return true if quantity is below threshold, false otherwise
     */
    bool is_low_stock(int threshold) const;

    /**
     * @brief Get a string representation of the product
     * @return A formatted string with product details
     */
    std::string to_string() const;

    // For inventory manager internal use
    friend class InventoryManager;
};