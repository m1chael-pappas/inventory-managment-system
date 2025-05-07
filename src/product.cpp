#include "includes/product.h"
#include <sstream>
#include <iomanip>

Product::Product() : id(0), price(0.0), quantity(0) {}

Product::Product(int id, const std::string &name, const std::string &category,
                 double price, int quantity, const std::string &description)
    : id(id), name(name), category(category), price(price),
      quantity(quantity), description(description) {}

// Getters
int Product::get_id() const { return id; }
std::string Product::get_name() const { return name; }
std::string Product::get_category() const { return category; }
double Product::get_price() const { return price; }
int Product::get_quantity() const { return quantity; }
std::string Product::get_description() const { return description; }

// Setters
void Product::set_id(int new_id) { id = new_id; }
void Product::set_name(const std::string &new_name) { name = new_name; }
void Product::set_category(const std::string &new_category) { category = new_category; }
void Product::set_price(double new_price) { price = new_price; }
void Product::set_quantity(int new_quantity) { quantity = new_quantity; }
void Product::set_description(const std::string &new_description) { description = new_description; }

// Helper methods
double Product::get_total_value() const
{
    return price * quantity;
}

bool Product::is_low_stock(int threshold) const
{
    return quantity < threshold;
}

std::string Product::to_string() const
{
    std::stringstream ss;
    ss << "ID: " << id << ", "
       << "Name: " << name << ", "
       << "Category: " << category << ", "
       << "Price: $" << std::fixed << std::setprecision(2) << price << ", "
       << "Quantity: " << quantity;
    return ss.str();
}