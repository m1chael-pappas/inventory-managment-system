
#pragma once
#include <string>

class Product
{
private:
    int id;
    std::string name;
    std::string category;
    double price;
    int quantity;
    std::string description;

public:
    // Constructors
    Product();
    Product(int id, const std::string &name, const std::string &category,
            double price, int quantity, const std::string &description = "");

    // Getters
    int getId() const;
    std::string getName() const;
    std::string getCategory() const;
    double getPrice() const;
    int getQuantity() const;
    std::string getDescription() const;

    // Setters
    void setId(int newId);
    void setName(const std::string &newName);
    void setCategory(const std::string &newCategory);
    void setPrice(double newPrice);
    void setQuantity(int newQuantity);
    void setDescription(const std::string &newDescription);

    // Helper methods
    double getTotalValue() const;
    bool isLowStock(int threshold) const;
    std::string toString() const;

    // For inventory manager internal use
    friend class InventoryManager;
};