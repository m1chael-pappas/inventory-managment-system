#include "includes/inventory_manager.h"
#include <fstream>
#include <sstream>
#include <algorithm>

InventoryManager::InventoryManager() : next_product_id(1) {}

int InventoryManager::add_product(const Product &product)
{
    // Create a new product with the next available ID
    Product new_product = product;
    new_product.set_id(next_product_id++);

    // Add to vector
    products.push_back(new_product);
    return new_product.get_id();
}

void InventoryManager::update_product(int id, const Product &updated_product)
{
    auto it = std::find_if(products.begin(), products.end(),
                           [id](const Product &p)
                           { return p.get_id() == id; });

    if (it != products.end())
    {
        it->set_name(updated_product.get_name());
        it->set_category(updated_product.get_category());
        it->set_price(updated_product.get_price());
        it->set_quantity(updated_product.get_quantity());
        it->set_description(updated_product.get_description());
    }
    else
    {
        throw ProductNotFoundException(id);
    }
}

void InventoryManager::remove_product(int id)
{
    auto it = std::find_if(products.begin(), products.end(),
                           [id](const Product &p)
                           { return p.get_id() == id; });

    if (it != products.end())
    {
        products.erase(it);
    }
    else
    {
        throw ProductNotFoundException(id);
    }
}

Product InventoryManager::get_product_by_id(int id) const
{
    auto it = std::find_if(products.begin(), products.end(),
                           [id](const Product &p)
                           { return p.get_id() == id; });

    if (it != products.end())
    {
        return *it;
    }
    else
    {
        throw ProductNotFoundException(id);
    }
}

std::vector<Product> InventoryManager::find_products_by_name(const std::string &name) const
{
    std::vector<Product> result;
    for (const auto &product : products)
    {
        if (product.get_name().find(name) != std::string::npos)
        {
            result.push_back(product);
        }
    }
    return result;
}

std::vector<Product> InventoryManager::find_products_by_category(const std::string &category) const
{
    std::vector<Product> result;
    for (const auto &product : products)
    {
        if (product.get_category() == category)
        {
            result.push_back(product);
        }
    }
    return result;
}

const std::vector<Product> &InventoryManager::get_all_products() const
{
    return products;
}

int InventoryManager::get_total_product_count() const
{
    return products.size();
}

double InventoryManager::get_total_inventory_value() const
{
    double total = 0.0;
    for (const auto &product : products)
    {
        total += product.get_total_value();
    }
    return total;
}

std::vector<Product> InventoryManager::get_low_stock_products(int threshold) const
{
    std::vector<Product> result;
    for (const auto &product : products)
    {
        if (product.is_low_stock(threshold))
        {
            result.push_back(product);
        }
    }
    return result;
}

void InventoryManager::save_to_file(const std::string &filename)
{
    std::ofstream file(filename);
    if (!file.is_open())
    {
        throw FileOperationException("open", filename);
    }

    // Write header
    file << "ID,Name,Category,Price,Quantity,Description,Total Value\n";

    for (const auto &product : products)
    {
        double total_value = product.get_total_value();

        file << product.get_id() << ",";

        // Escape name with quotes
        file << "\"" << replace_all(product.get_name(), "\"", "\"\"") << "\",";

        // Escape category with quotes
        file << "\"" << replace_all(product.get_category(), "\"", "\"\"") << "\",";

        file << product.get_price() << ","
             << product.get_quantity() << ",";

        // Escape description with quotes
        file << "\"" << replace_all(product.get_description(), "\"", "\"\"") << "\",";

        // Add total value
        file << total_value << "\n";
    }

    file.close();
}

void InventoryManager::load_from_file(const std::string &filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        throw FileOperationException("open", filename);
    }

    products.clear();
    next_product_id = 1;

    std::string line;
    // Skip header line
    std::getline(file, line);

    while (std::getline(file, line))
    {
        if (line.empty())
            continue;

        int id = 0;
        std::string name, category, description;
        double price = 0.0;
        int quantity = 0;

        // CSV parsing state
        size_t pos = 0;
        bool in_quotes = false;
        std::vector<std::string> fields;
        std::string current_field;

        // Parse CSV row manually to handle quoted fields
        while (pos < line.length())
        {
            char c = line[pos];

            if (c == '"')
            {
                // Toggle quote state
                in_quotes = !in_quotes;

                // Check for escaped quotes (two quotes in a row)
                if (in_quotes && pos + 1 < line.length() && line[pos + 1] == '"')
                {
                    current_field += '"';
                    pos += 2; // Skip both quotes
                }
                else
                {
                    pos++; // Skip the quote character
                }
            }
            else if (c == ',' && !in_quotes)
            {
                // End of field
                fields.push_back(current_field);
                current_field.clear();
                pos++;
            }
            else
            {
                // Regular character
                current_field += c;
                pos++;
            }
        }

        // Don't forget the last field
        fields.push_back(current_field);

        // Now process the fields
        if (fields.size() < 6) // Need at least ID, name, category, price, quantity, description
            continue;

        try
        {
            id = std::stoi(fields[0]);
            next_product_id = std::max(next_product_id, id + 1);

            name = fields[1];
            category = fields[2];
            price = std::stod(fields[3]);
            quantity = std::stoi(fields[4]);
            description = fields[5];

            products.emplace_back(id, name, category, price, quantity, description);
        }
        catch (const std::exception &e)
        {
            // Skip invalid line
            continue;
        }
    }

    file.close();
}
std::string InventoryManager::replace_all(std::string str, const std::string &from, const std::string &to)
{
    size_t start_pos = 0;
    while ((start_pos = str.find(from, start_pos)) != std::string::npos)
    {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
    return str;
}