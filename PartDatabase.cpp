

//the methods used for interacting with the list of parts.
namespace PartDatabase {
  std::map<std::string, Part*> partMap; //Always holds the updated and current versions of the data.
  std::string filename = "database.db";

    //should not save internally, as this method is called by loadPartData
  void addPart(Part * part) {
    if(part != NULL) {
      partMap.insert(std::pair<std::string, Part*>(part->getName(), part));
    }
  } 

  std::vector<Part *> getPartsAsVector() {
    std::vector<Part *> output;
    for( std::map<std::string, Part*>::iterator it = partMap.begin(); it != partMap.end(); ++it ) {
      output.push_back(it->second);
    }
    return output;
  }

  void loadPartData() {
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
      }else if(iterator % 4 == 2) {
        part_price = line;
      }else if(iterator % 4 == 3) {
        part_stock = line;
        addPart(new Part(part_name, std::stoi(part_type), std::stoi(part_price), std::stoi(part_stock)));
      }
      iterator++;
    }
    dbFile.close();
  }

  void savePartData() {
    std::ofstream dbFile;
    dbFile.open(filename);
    for (std::map<std::string, Part*>::iterator iter = partMap.begin(); iter != partMap.end(); ++iter){
      dbFile << iter->second->getName() << "\n";
      dbFile << std::to_string(iter->second->getType()) << "\n";
      dbFile << std::to_string(iter->second->getPrice()) << "\n";
      dbFile << std::to_string(iter->second->getStock()) << "\n";
    }
    
    dbFile.close();

  } //converts the data from partList into a string and stores it to a file.



  void removePart(char * name) {} //calls Save

  void updatePartPrice(Part * part, int price) {} //calls Save

  void updatePartStock(Part * part, int stock) {} //calls Save

  std::map<std::string, Part*> getPartMap() {
    return partMap;
  }

  //sorts input by price
  std::vector<Part*> orderByPrice(std::vector<Part*> input, bool asc) {
    auto sortMethod = [](Part* A, Part* B) { return (A->getPrice() < B->getPrice()); };
    if(asc) {
      auto sortMethod = [](Part* A, Part* B) { return (A->getPrice() > B->getPrice()); };
    }
    std::sort(input.begin(), input.end(), sortMethod); 
    return input;
  }

  //sorts the input by stock
  std::vector<Part*> orderByStock(std::vector<Part*> input, bool asc) {
    auto sortMethod = [](Part* A, Part* B) { return (A->getStock() < B->getStock()); };
    if(asc) {
      auto sortMethod = [](Part* A, Part* B) { return (A->getStock() > B->getStock()); };
    }
    std::sort(input.begin(), input.end(), sortMethod); 
    return input;
  }

  //removes all items that are not of a certain type
  std::vector<Part*> filterByType(std::vector<Part*> input, int type) {
    for(std::vector<Part*>::iterator iter = input.begin(); iter != input.end();) {
      if((*iter)->getType() != type) {
        input.erase(iter);
      }else{
        ++iter;
      }
    }
    return input;
  }

  
}