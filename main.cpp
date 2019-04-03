#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <algorithm>
#include <vector>
#include <string.h>
#include <set>

std::vector<std::string> availablePartTypes({"Graphics Card", "Motherboard", "RAM", "Hard Drive", "Power Supply"});
#include "Part.cpp"
#include "ConWin.cpp"
#include "PartDatabase.cpp"

//The part types available

Part* addPartWindow() {
  char *p_name = (char*)malloc(sizeof(char) * 99);
  int p_type;
  int p_cost;
  int p_stock;

  //Create a window that asks the user for the part name
  p_name = ConWin::getStringWindow("ADD NEW PART", "Enter part name", 1);
  if(p_name == NULL) {
    return NULL;
  }

  //Creates a window that asks you how much the part costs.
  p_cost = ConWin::getIntegerWindow("ADD NEW PART", "Enter Part Cost", 1);
  if(p_cost == -69) { //magic "cancel" character
    return NULL;
  }

  //Creates a window that displays all the part types and asks the user to select one.
  p_type = ConWin::getOptionWindow("ADD NEW PART", "Enter Part Type", availablePartTypes, 1);
  if(p_type == -69) {
    return NULL;
  }

  //Creates a window that asks you how many of the part are currently in stock.
  p_stock = ConWin::getIntegerWindow("ADD NEW PART", "Enter Current Stock", 1);
  if(p_stock == -69) { //magic "cancel" character
    return NULL;
  }

  Part* new_part = new Part(p_name, (int)p_type, p_cost, p_stock);
  ConWin::drawPartWindow(new_part, "PART SUCCESFULLY CREATED", "Please confirm the part information...");
  return new_part;
}

void partOptionPrompt(Part *part) {
  ConWin::drawPartWindow(part, "PART INFO", "Select an option in the next page");
  int result = ConWin::getOptionWindow("SELECT PART OPTION", "Choose what to do with this part", {"Change name", "Change price", "Change stock", "Change product type", "Delete"}, 1);
  if(result == 1) {
    //rename
  }else if(result == 2) {
    //change price
  }else if(result == 3) {
    //change stock
  }else if(result == 4) {
    //change type
  }else if(result == 5) {
    //delete ):
  }
}

void listPartWindow(PartDatabase * partTree) {
  std::vector<Part *> localPartVector = partTree->getByPriceAscending();
  bool finished = false;
  int page = 0;
  int filter = 0;
  int sortBy = 1; //1 == price ASC, 2 == price DESC, 3 == stock ASC, 4 == stock DESC.

  while(!finished) {

    /* SORT VECTOR */
    if(sortBy == 1) {
      localPartVector = partTree->getByPriceAscending(filter);
    }else if(sortBy == 2){
      localPartVector = partTree->getByPriceDescending(filter);
    }else if(sortBy == 3) {
      localPartVector = partTree->getByStockAscending(filter);
    }else if(sortBy == 4) {
      localPartVector = partTree->getByStockDescending(filter);
    }

    std::vector<std::string> options;
    std::vector<Part *>::iterator localPartVectorIterator = localPartVector.begin();
    //localMapIterator++; //set page. (++ page*10) //maybe use vector::advance

    bool foundEnd = false;
    for(int i = 0; i < 10; i++) {
      if(foundEnd || localPartVectorIterator == localPartVector.end()) {
        options.insert(options.end(), "");
        foundEnd =true;
      }else{
        options.insert(options.end(), (*localPartVectorIterator)->getName() + " #" + std::to_string((*localPartVectorIterator)->getType()) + " $" + std::to_string((*localPartVectorIterator)->getPrice()) + " x" + std::to_string((*localPartVectorIterator)->getStock()));
      }
      localPartVectorIterator++;
    }
    options.insert(options.end(), "Filter Results");
    options.insert(options.end(), "Next Page");
    options.insert(options.end(), "Previous page");
      
    int result = ConWin::getOptionWindow("Select a part", "Select an option (pg 1 of 1)", options, 1);
    if(result >= 1 && result <= 10) {

      partOptionPrompt(localPartVector[result-1]);
    }else if(result == 11) {
      //show the filter window to filter the results.
      int result = ConWin::getOptionWindow("SORT AND FILTER", "Select a sorting/filtering method", {"Sort Price (asc)", "Sort Price (desc)", "Sort Stock (asc)", "Sort Stock (desc)", "Filter product type", "Reset all filters"}, 1);
      if(result >= 1 && result <= 4) {
        sortBy = result;
      }else if(result == 5) {
        std::vector<std::string> filterTypes;
        filterTypes.assign(availablePartTypes.begin(), availablePartTypes.end());
        filterTypes.push_back("None");
        int t_fil = ConWin::getOptionWindow("FILTER BY TYPE", "Select the type to filter by", filterTypes, 1);
        if(t_fil == 6) {
          filter = 0;
        }else if(t_fil != -69) {
          filter = t_fil;
        }
      }
    }else if(result == 12) {
      //start showing the previous ten items
    }else if(result == 13) {
    // start showing the next ten items
    }else if(result == -69) {
      finished = true;
    }
  }
}

//The series of methods for 
int findPartWindow() {
    int p_type_id = 0;
    p_type_id = ConWin::getOptionWindow("FIND A PART", "What type of parts?", availablePartTypes, 1);
        return p_type_id;
}

int main() {
  bool running = true;
  std::vector<std::string> initial_options = {"Add a part", "Find parts", "Exit"};

  PartDatabase * partTree = new PartDatabase();
  partTree->loadFromFile("database.db");
 // printf("NUMER OF ELEMENTS: %d", partTree->size());

  while(running) {
    switch(ConWin::getOptionWindow("PC PART EXPLORER", "Please select an option", initial_options, 0)) {
      case 1: 
      {
        partTree->insert(addPartWindow());
        partTree->saveToFile("database.db");
        break;
      }
      case 2: 
      {
          listPartWindow(partTree);
          break;
      }
      case 3: 
      {
        running = false;
        break;
      }
    }
  }
  printf("Thanks for using PC Part Explorer!\n");
  return 0;
}