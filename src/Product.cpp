
#include "includes/Product.h"
#include <sstream>
#include <iomanip>

Product::Product() : id(0), price(0.0), quantity(0) {}

Product::Product(int id, const std::string &name, const std::string &category,
                 double price, int quantity, const std::string &description)
    : id(id), name(name), category(category), price(price),
      quantity(quantity), description(description) {}

// Getters
int Product::getId() const { return id; }
std::string Product::getName() const { return name; }
std::string Product::getCategory() const { return category; }
double Product::getPrice() const { return price; }
int Product::getQuantity() const { return quantity; }
std::string Product::getDescription() const { return description; }

// Setters
void Product::setId(int newId) { id = newId; }
void Product::setName(const std::string &newName) { name = newName; }
void Product::setCategory(const std::string &newCategory) { category = newCategory; }
void Product::setPrice(double newPrice) { price = newPrice; }
void Product::setQuantity(int newQuantity) { quantity = newQuantity; }
void Product::setDescription(const std::string &newDescription) { description = newDescription; }

// Helper methods
double Product::getTotalValue() const
{
    return price * quantity;
}

bool Product::isLowStock(int threshold) const
{
    return quantity < threshold;
}

std::string Product::toString() const
{
    std::stringstream ss;
    ss << "ID: " << id << ", "
       << "Name: " << name << ", "
       << "Category: " << category << ", "
       << "Price: $" << std::fixed << std::setprecision(2) << price << ", "
       << "Quantity: " << quantity;
    return ss.str();
}