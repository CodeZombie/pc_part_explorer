//compare method used by the elementsByPrice Set (Binary Search Tree)
struct price_compare {
    bool operator()(Product * lhs, Product * rhs) const {
        return lhs->getPrice() < rhs->getPrice();
    }
};

//compare method used by the elementsByStock Set (Binary Search Tree)
struct stock_compare {
    bool operator()(Product * lhs, Product * rhs) const {
        return lhs->getStock() < rhs->getStock();
    }
};

//This class will hold references to every Product object.
//it stores each pointer in 2 Multisets. One sorted by Price, the other by stock.
// Insertion into a bst is O(n), making them very fast.
// Sorting in ascending or descending order is just a matter of reversing the output of the sets, which is an O(n) operation.

class ProductDatabase {
    private:
        std::multiset<Product *, price_compare> elementsByPrice;
        std::multiset<Product *, stock_compare> elementsByStock;
        std::vector<std::string> productTypes;

    public:
        void loadFromFile(std::string filename) {
            if(this->size() == 0) {
                std::ifstream dbFile;
                dbFile.open(filename);
                std::string line;
                
                int iterator = 0;
                std::string product_name;
                std::string product_type;
                std::string product_price;
                std::string product_stock;

                while(std::getline(dbFile, line)) {
                if(iterator % 4 == 0) {
                    product_name = line;
                }else if(iterator % 4 == 1) {
                    product_type = line;
                    if(!this->productTypeExists(product_type)) {
                        this->productTypes.insert(this->productTypes.end(), product_type);
                    }

                }else if(iterator % 4 == 2) {
                    product_price = line;
                }else if(iterator % 4 == 3) {
                    product_stock = line;
                    this->insert(new Product(product_name, product_type, std::stoi(product_price), std::stoi(product_stock)));
                }
                iterator++;
                }
                dbFile.close();
            }
        }

        void saveToFile(std::string filename) {
            std::ofstream dbFile;
            dbFile.open(filename);
            for (std::multiset<Product *, price_compare>::iterator iter = elementsByPrice.begin(); iter != elementsByPrice.end(); ++iter){
                dbFile << (*iter)->getName() << "\n";
                dbFile << (*iter)->getType() << "\n";
                dbFile << std::to_string((*iter)->getPrice()) << "\n";
                dbFile << std::to_string((*iter)->getStock()) << "\n";
            }
            dbFile.close();

        }

        void insertProductType(std::string newType) {
            if(! this->productTypeExists(newType)) {
                this->productTypes.insert(this->productTypes.end(), newType);
            }
        }

        std::vector<std::string> getProductTypes() {
            return this->productTypes;
        }

        bool productTypeExists(std::string type) {
            if (std::find(this->productTypes.begin(), this->productTypes.end(), type ) != this->productTypes.end()) {
                return true;
            }
            return false;
        }
        
        //get the number of elements in the tree.
        int size() {
            return elementsByPrice.size();
        } 

        //inserts an element into the tree
        void insert(Product * product) {
            if(product != NULL) {
                elementsByPrice.insert(product);
                elementsByStock.insert(product);
            }
        }

        //removes an element from the tree
        void remove(Product * product) {
                elementsByPrice.erase(product);
                elementsByStock.erase(product);
        }

        //call this to re-sort the product that has changed by re-inserting it into the database.
        void update(Product *product) {  
            this->remove(product);
            this->insert(product);
        }

        std::vector<Product *> getByPriceAscending() {
            std::vector<Product *> output;
            output.assign(elementsByPrice.begin(), elementsByPrice.end());
            return output;
        }

        std::vector<Product *> filterByType(std::vector<Product *> products, std::string typeFilter) {
            std::vector<Product *> output;
            for(std::vector<Product *>::iterator iter = products.begin(); iter != products.end(); ++iter) {
                if(typeFilter == "" || (*iter)->getType() == typeFilter) {
                    output.insert(output.end(), (*iter));
                }    
            }
            return output;
        }

        //returns a vector with the sorted set of data.
        std::vector<Product *> getByPriceAscending(std::string typeFilter) {
            std::vector<Product *> output;
            output.assign(elementsByPrice.begin(), elementsByPrice.end());
            return filterByType(output, typeFilter);
        }

        std::vector<Product *> getByPriceDescending(std::string typeFilter) {
            std::vector<Product *> output;
            output.assign(elementsByPrice.rbegin(), elementsByPrice.rend());
            return filterByType(output, typeFilter);
        }

        std::vector<Product *> getByStockAscending(std::string typeFilter) {
            std::vector<Product *> output;
            output.assign(elementsByStock.begin(), elementsByStock.end()); //now convert the sorted data to a vector and return that (for portability)
            return filterByType(output, typeFilter);
        }

        std::vector<Product *> getByStockDescending(std::string typeFilter) {
            std::vector<Product *> output;
            output.assign(elementsByStock.rbegin(), elementsByStock.rend());
            return filterByType(output, typeFilter);
        }
};