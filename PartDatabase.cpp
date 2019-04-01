

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

  std::map<std::string, Part*> orderMapByPrice(int lower, int upper, bool asc) {

  }

  std::map<std::string, Part*> getFilteredMapByType(int type) {}

  std::map<std::string, Part*> getFilteredMapByStock(int lower, int upper, bool asc) {}
  
}