/*
  Changes
  1. implemented each of the 5 part modification options; changing the 4 attributes and delete.
  2. added a loop for the modifications window so that when one change is made the window is 
     reopened to allow for more changes to the same part.
  3. added remove() to PartDatabase.cpp for deleting from the sets
  4. small change in listPartWindow when calling partOptionPrompt to add page*10 to the index being
     selected. Previously was opening items from the first page even when selecting from the second.
  5. again when calling partOptionPrompt, now passes partTree for deletion purposes

  Following changes are unique to this copy of the project file, I have a backup copy without these:
  1. Changed part type to be a string which the user can input when adding a new part
  2. removed case 3 from main, no longer allowing the addition of new part types
  3. Updated various outputs to accomodate the string format of part type
  4. Changed methods for sorting in PartDatabase.cpp to use a string filter instead of int
  
  I think thats pretty much everything
*/

#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <algorithm>
#include <vector>
#include <string.h>
#include <set>
#include "Part.cpp"
#include "ConWin.cpp"
#include "PartDatabase.cpp"

std::string getPartTypeWindow(PartDatabase * partDB, bool canEnterNewType) {
  std::string p_type;
  int p_type_int;

  std::vector<std::string> ptypes;
  bool asking_for_part_type = true;

  //Creates a window that asks for the product type, based on a list of all existing product types.
  //also lets the user define their own product type.
  while(asking_for_part_type) {
    ptypes = partDB->getPartTypes();
    if(canEnterNewType) {
      ptypes.insert(ptypes.end(), "NEW PART TYPE");
    }

    p_type_int = ConWin::getOptionWindow("ADD NEW PART", "Choose part type", ptypes, 1);
    if(p_type_int == -69) {
      return ""; 
    }else if(canEnterNewType && p_type_int == ptypes.size()) { //if we selected NEW PART TYPE
      //ask the user to create a new part;
      bool asking_for_new_part_type = true;
      while(asking_for_new_part_type){
        p_type = ConWin::getStringWindow("CREATE NEW PART TYPE", "Enter new part type", 1);
        if(p_type.empty()){
          asking_for_new_part_type = false;
        }else {
          //succesfully entered new part type.
          partDB->insertPartType(p_type);
          asking_for_new_part_type = false;
          asking_for_part_type = false;
        }
      }
    }else {
      asking_for_part_type = false;
      p_type = ptypes[p_type_int-1];
    }
  }
  return p_type;
}

Part* addPartWindow(PartDatabase * partDB) {
  std::string p_name;
  std::string p_type;
  int p_cost;
  int p_stock;

  //Create a window that asks the user for the part name
  p_name = ConWin::getStringWindow("ADD NEW PART", "Enter part name", 1);
  if(p_name.empty()) {
    return NULL;
  }

  //Creates a window that asks you how much the part costs.
  p_cost = ConWin::getIntegerWindow("ADD NEW PART", "Enter Part Cost", 1);
  if(p_cost == -69) { //magic "cancel" character
    return NULL;
  }

  //Creates a window that asks you how many of the part are currently in stock.
  p_stock = ConWin::getIntegerWindow("ADD NEW PART", "Enter Current Stock", 1);
  if(p_stock == -69) { //magic "cancel" character
    return NULL;
  }


  p_type = getPartTypeWindow(partDB, true);
  if(p_type.empty()) { return NULL;}

  Part* new_part = new Part(p_name, p_type, p_cost, p_stock);
  ConWin::drawPartWindow(new_part, "PART SUCCESFULLY CREATED", "Please confirm the part information...");
  return new_part;
}

void partOptionPrompt(Part *part, PartDatabase * partTree) {
  ConWin::drawPartWindow(part, "PART INFO", "Select an option in the next page");
  int result;
  std::string p_name;

  while (result != 6) {
    result = ConWin::getOptionWindow("SELECT PART OPTION", "Choose what to do with this part", {"Change name", "Change price", "Change stock", "Change product type", "Delete", "Back"}, 0);
    if(result == 1) {

      //Changes the Part name
      p_name = ConWin::getStringWindow("MODIFY PART", "Enter New Part Name", 1);
      if (p_name.empty()) {return;}
      part->setName(p_name);

    }else if(result == 2) {
      
      //Changes the Part price
      int p_cost = ConWin::getIntegerWindow("MODIFY PART", "Enter New Part Cost", 1);
      if (p_cost == -69) {return;}
      part->setPrice(p_cost);
      partTree->update(part);

    }else if(result == 3) {
      
      //Changes the Part stock
      int p_stock = ConWin::getIntegerWindow("MODIFY PART", "Enter New Stock", 1);
      if (p_stock == -69) {return;}
      part->setStock(p_stock);
      partTree->update(part);

    }else if(result == 4) {
      
      //Changes the Part type
      std::string p_type;
      p_type = getPartTypeWindow(partTree, true);
      if (p_type.empty()) {return;}
      part->setType(p_type);

    }else if(result == 5) {
      
      //Deletes the Part
      partTree->remove(part);
      ConWin::drawDialogWindow("MODIFY PART", "This Part Has Been DELETED", {});
      result++;
    }
  }
}

void listPartWindow(PartDatabase * partTree) {
  std::vector<Part *> localPartVector = partTree->getByPriceAscending();
  bool finished = false;
  int page = 0;
  std::string filter = "";
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
    std::advance(localPartVectorIterator, page*10);
    //localMapIterator++; //set page. (++ page*10) //maybe use vector::advance

    bool foundEnd = false;
    for(int i = 0; i < 10; i++) {
      if(foundEnd || localPartVectorIterator == localPartVector.end()) {
        options.insert(options.end(), "");
        foundEnd =true;
      }else{
        options.insert(options.end(), (*localPartVectorIterator)->getName() + " | " + (*localPartVectorIterator)->getType() + " $" + std::to_string((*localPartVectorIterator)->getPrice()) + " x" + std::to_string((*localPartVectorIterator)->getStock()));
      }
      localPartVectorIterator++;
    }
    options.insert(options.end(), "Filter Results");
    options.insert(options.end(), "Next Page");
    options.insert(options.end(), "Previous page");
      
    std::string pageInstr= "Select an option (pg " + std::to_string(page+1) + " of " + std::to_string((int)(localPartVector.size() / 10)+1) + ")";
    int result = ConWin::getOptionWindow("Select a part", pageInstr, options, 1);
    if(result >= 1 && result <= 10) {
      if(localPartVector[page*10 + result-1] != NULL) {
        partOptionPrompt(localPartVector[page*10 + result-1], partTree); //potentially make changes
        partTree->saveToFile("database.db");//save all changes
      }
    }else if(result == 11) {
      //show the filter window to filter the results.
      int result = ConWin::getOptionWindow("SORT AND FILTER", "Select a sorting/filtering method", {"Sort Price (asc)", "Sort Price (desc)", "Sort Stock (asc)", "Sort Stock (desc)", "Filter product type", "Reset all filters"}, 1);
      if(result >= 1 && result <= 4) {
        sortBy = result;
      }else if(result == 5) {
        std::string t_fil = getPartTypeWindow(partTree, false);//ConWin::getStringWindow("FILTER BY TYPE", "Input the type to filter by", 1);
        if(!t_fil.empty()) {
          filter = t_fil;
        }
      }else if(result == 6) {
        filter = "";
      }
    }else if(result == 12) {
      //start showing the next ten items

      if(page < ceil(localPartVector.size() / 10)) {
        page++;
      }
    }else if(result == 13) {
      if(page > 0) {
        page--;
      }
    // start showing the previous ten items
    }else if(result == -69) {
      finished = true;
    }
  }
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
        partTree->insert(addPartWindow(partTree));
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