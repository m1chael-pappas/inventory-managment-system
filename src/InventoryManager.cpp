#include "includes/InventoryManager.h"
#include <fstream>
#include <sstream>
#include <algorithm>

InventoryManager::InventoryManager() : nextProductId(1) {}

int InventoryManager::addProduct(const Product &product)
{
    // Create a new product with the next available ID
    Product newProduct = product;
    newProduct.setId(nextProductId++);

    // Add to vector
    products.push_back(newProduct);
    return newProduct.getId();
}

void InventoryManager::updateProduct(int id, const Product &updatedProduct)
{
    auto it = std::find_if(products.begin(), products.end(),
                           [id](const Product &p)
                           { return p.getId() == id; });

    if (it != products.end())
    {

        it->setName(updatedProduct.getName());
        it->setCategory(updatedProduct.getCategory());
        it->setPrice(updatedProduct.getPrice());
        it->setQuantity(updatedProduct.getQuantity());
        it->setDescription(updatedProduct.getDescription());
    }
    else
    {
        throw ProductNotFoundException(id);
    }
}

void InventoryManager::removeProduct(int id)
{
    auto it = std::find_if(products.begin(), products.end(),
                           [id](const Product &p)
                           { return p.getId() == id; });

    if (it != products.end())
    {
        products.erase(it);
    }
    else
    {
        throw ProductNotFoundException(id);
    }
}

Product InventoryManager::getProductById(int id) const
{
    auto it = std::find_if(products.begin(), products.end(),
                           [id](const Product &p)
                           { return p.getId() == id; });

    if (it != products.end())
    {
        return *it;
    }
    else
    {
        throw ProductNotFoundException(id);
    }
}

std::vector<Product> InventoryManager::findProductsByName(const std::string &name) const
{
    std::vector<Product> result;
    for (const auto &product : products)
    {
        if (product.getName().find(name) != std::string::npos)
        {
            result.push_back(product);
        }
    }
    return result;
}

std::vector<Product> InventoryManager::findProductsByCategory(const std::string &category) const
{
    std::vector<Product> result;
    for (const auto &product : products)
    {
        if (product.getCategory() == category)
        {
            result.push_back(product);
        }
    }
    return result;
}

const std::vector<Product> &InventoryManager::getAllProducts() const
{
    return products;
}

int InventoryManager::getTotalProductCount() const
{
    return products.size();
}

double InventoryManager::getTotalInventoryValue() const
{
    double total = 0.0;
    for (const auto &product : products)
    {
        total += product.getTotalValue();
    }
    return total;
}

std::vector<Product> InventoryManager::getLowStockProducts(int threshold) const
{
    std::vector<Product> result;
    for (const auto &product : products)
    {
        if (product.isLowStock(threshold))
        {
            result.push_back(product);
        }
    }
    return result;
}

void InventoryManager::saveToFile(const std::string &filename)
{
    std::ofstream file(filename);
    if (!file.is_open())
    {
        throw FileOperationException("open", filename);
    }

    for (const auto &product : products)
    {
        file << product.getId() << ','
             << product.getName() << ','
             << product.getCategory() << ','
             << product.getPrice() << ','
             << product.getQuantity() << ','
             << product.getDescription() << '\n';
    }

    file.close();
}

void InventoryManager::loadFromFile(const std::string &filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        throw FileOperationException("open", filename);
    }

    products.clear();
    nextProductId = 1;

    std::string line;
    while (std::getline(file, line))
    {
        std::stringstream ss(line);
        std::string token;

        int id = 0;
        std::string name, category, description;
        double price = 0.0;
        int quantity = 0;

        // Parse ID
        if (std::getline(ss, token, ','))
        {
            try
            {
                id = std::stoi(token);
                nextProductId = std::max(nextProductId, id + 1);
            }
            catch (const std::exception &e)
            {
                continue; // Skip invalid line
            }
        }

        // Parse name
        if (!std::getline(ss, name, ','))
            continue;

        // Parse category
        if (!std::getline(ss, category, ','))
            continue;

        // Parse price
        if (std::getline(ss, token, ','))
        {
            try
            {
                price = std::stod(token);
            }
            catch (const std::exception &e)
            {
                continue; // Skip invalid line
            }
        }

        // Parse quantity
        if (std::getline(ss, token, ','))
        {
            try
            {
                quantity = std::stoi(token);
            }
            catch (const std::exception &e)
            {
                continue; // Skip invalid line
            }
        }

        // Parse description
        std::getline(ss, description);

        products.emplace_back(id, name, category, price, quantity, description);
    }

    file.close();
}