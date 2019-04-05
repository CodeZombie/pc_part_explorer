//compare method used by the elementsByPrice Set (Binary Search Tree)
struct price_compare {
    bool operator()(Part * lhs, Part * rhs) const {
        return lhs->getPrice() < rhs->getPrice();
    }
};

//compare method used by the elementsByStock Set (Binary Search Tree)
struct stock_compare {
    bool operator()(Part * lhs, Part * rhs) const {
        return lhs->getStock() < rhs->getStock();
    }
};

//This class will hold references to every Part object.
//it stores each pointer in 2 Multisets. One sorted by Price, the other by stock.
// Insertion into a bst is O(n), making them very fast.
// Sorting in ascending or descending order is just a matter of reversing the output of the sets, which is an O(n) operation.

class PartDatabase {
    private:
        std::multiset<Part *, price_compare> elementsByPrice;
        std::multiset<Part *, stock_compare> elementsByStock;
        std::vector<std::string> partTypes;

    public:
        void loadFromFile(std::string filename) {
            if(this->size() == 0) {
                std::ifstream dbFile;
                dbFile.open(filename);
                std::string line;
                
                int iterator = 0;
                std::string part_name;
                std::string part_type;
                std::string part_price;
                std::string part_stock;

                while(std::getline(dbFile, line)) {
                if(iterator % 4 == 0) {
                    part_name = line;
                }else if(iterator % 4 == 1) {
                    part_type = line;
                    if(!this->partTypeExists(part_type)) {
                        this->partTypes.insert(this->partTypes.end(), part_type);
                    }

                }else if(iterator % 4 == 2) {
                    part_price = line;
                }else if(iterator % 4 == 3) {
                    part_stock = line;
                    this->insert(new Part(part_name, part_type, std::stoi(part_price), std::stoi(part_stock)));
                }
                iterator++;
                }
                dbFile.close();
            }
        }

        void saveToFile(std::string filename) {
            std::ofstream dbFile;
            dbFile.open(filename);
            for (std::multiset<Part *, price_compare>::iterator iter = elementsByPrice.begin(); iter != elementsByPrice.end(); ++iter){
                dbFile << (*iter)->getName() << "\n";
                dbFile << (*iter)->getType() << "\n";
                dbFile << std::to_string((*iter)->getPrice()) << "\n";
                dbFile << std::to_string((*iter)->getStock()) << "\n";
            }
            dbFile.close();

        }

        void insertPartType(std::string newType) {
            if(! this->partTypeExists(newType)) {
                this->partTypes.insert(this->partTypes.end(), newType);
            }
        }

        std::vector<std::string> getPartTypes() {
            return this->partTypes;
        }

        bool partTypeExists(std::string type) {
            if (std::find(this->partTypes.begin(), this->partTypes.end(), type ) != this->partTypes.end()) {
                return true;
            }
            return false;
        }
        
        //get the number of elements in the tree.
        int size() {
            return elementsByPrice.size();
        } 

        //inserts an element into the tree
        void insert(Part * part) {
            if(part != NULL) {
                elementsByPrice.insert(part);
                elementsByStock.insert(part);
            }
        }

        //removes an element from the tree
        void remove(Part * part) {
                elementsByPrice.erase(part);
                elementsByStock.erase(part);
        }

        //call this to re-sort the part that has changed by re-inserting it into the database.
        void update(Part *part) {  
            this->remove(part);
            this->insert(part);
        }

        std::vector<Part *> getByPriceAscending() {
            std::vector<Part *> output;
            output.assign(elementsByPrice.begin(), elementsByPrice.end());
            return output;
        }

        std::vector<Part *> filterByType(std::vector<Part *> parts, std::string typeFilter) {
            std::vector<Part *> output;
            for(std::vector<Part *>::iterator iter = parts.begin(); iter != parts.end(); ++iter) {
                if(typeFilter == "" || (*iter)->getType() == typeFilter) {
                    output.insert(output.end(), (*iter));
                }    
            }
            return output;
        }

        //returns a vector with the sorted set of data.
        std::vector<Part *> getByPriceAscending(std::string typeFilter) {
            std::vector<Part *> output;
            output.assign(elementsByPrice.begin(), elementsByPrice.end());
            return filterByType(output, typeFilter);
        }

        std::vector<Part *> getByPriceDescending(std::string typeFilter) {
            std::vector<Part *> output;
            output.assign(elementsByPrice.rbegin(), elementsByPrice.rend());
            return filterByType(output, typeFilter);
        }

        std::vector<Part *> getByStockAscending(std::string typeFilter) {
            std::vector<Part *> output;
            output.assign(elementsByStock.begin(), elementsByStock.end()); //now convert the sorted data to a vector and return that (for portability)
            return filterByType(output, typeFilter);
        }

        std::vector<Part *> getByStockDescending(std::string typeFilter) {
            std::vector<Part *> output;
            output.assign(elementsByStock.rbegin(), elementsByStock.rend());
            return filterByType(output, typeFilter);
        }
};