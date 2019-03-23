
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ConWin.cpp"

enum partType {MOTHERBOARD, GRAPHICS_CARD, RAM, HARD_DRIVE, POWER_SUPPLY};

//The part types available.
const char *part_type[] = {"Graphics Card", "Motherboard", "Ram", "Hard Drive", "Power Supply", NULL};

//  The part object.
class Part{
  private:
    char * name;
    partType type;
    int price;
    int stock;

  public:
    Part(char * name, partType type, int price, int stock) {
      this->name = name;
      this->type = type;
      this->price = price;
      this->stock = stock;
    }

    void setPrice(int price) {
      this->price = price;
    }

    int getPrice() {
      return price;
    }

    void setStock(int stock) {
      this->stock = stock;
    }

    int getStock() {
      return stock;
    }

    char * getName() {
      return name;
    }
};

//the methods used for interacting with the list of parts.
namespace PartList {
  Part * partList[999]; //Always holds the updated and current versions of the data. 
  Part * filteredPartList[999]; //the part list that will be shown to the user. If no filter methods are called, 
                                //it will be exactly the same as partList.
                                //resetParList() must be called to re-initialize this array, which will undo all the filters.

  void loadPartsFromFile(char * filename) { }

  void savePartData(char * filename) {} //converts the data from partList into a string and stores it to a file.

  void addPart(Part * part) {} //calls a Save

  void removePart(char * name) {} //calls Save

  void updatePartPrice(Part * part, int price) {} //calls Save

  void updatePartStock(Part * part, int stock) {} //calls Save

  void resetPartList() {}

  void filterPartListByPrice(int lower, int upper, bool asc) {}

  void filterPartListByType(partType type) {}
  
}

//The series of methods for adding a part.
void addPartWindow() {
  char *p_name = (char*)malloc(sizeof(char) * 99);
  partType p_type;
  int p_cost;
  int p_stock;

  //Create a window that asks the user for the part name
  p_name = ConWin::getStringWindow("ADD NEW PART", "Enter part name", 1);
  if(p_name == NULL) {
    return;
  }

  //Creates a window that asks you how much the part costs.
  p_cost = ConWin::getIntegerWindow("ADD NEW PART", "Enter Part Cost", 1);
  if(p_cost == -69) { //magic "cancel" character
    return;
  }

  //Creates a window that displays all the part types and asks the user to select one.
  p_type = (partType)ConWin::getOptionWindow("ADD NEW PART", "Enter Part Type", part_type, 1);
  if(p_type == -69) {
    return;
  }

  //sets up the lines of text that will be drawn in the upcoming drawDialogWindow...
  char *lineOne = (char*)malloc(sizeof(char) * 99);
  char *lineTwo = (char*)malloc(sizeof(char) * 99);
  char *lineThree = (char*)malloc(sizeof(char) * 99);
  sprintf(lineOne, "Part Name: %s", p_name);
  sprintf(lineTwo, "Part Cost: %d", p_cost);
  sprintf(lineThree, "Part Type: %d", (int)p_type);
  const char *lines[] = {lineOne, lineTwo, lineThree, NULL};

  //draws the lines[] in a window.
  ConWin::drawDialogWindow("NEW PART ADDED!","New part has been created:", lines);

  //free all that memory allocated.
  free(p_name);
  free(lineOne);
  free(lineTwo);
  free(lineThree);
}

//The series of methods for 
void findPartWindow() {
}

int main()
{
  bool running = true;
  /*
  Part *w = new Part("ASROCK Mobo", MOTHERBOARD, 69, 5);
  Part *x = new Part("Nvidia GTX 550 Ti", GRAPHICS_CARD, 12, 122);
  Part *y = new Part("Cooler Master Whatever", RAM, 420, 0);
  Part *z = new Part("900w IED Bomb", POWER_SUPPLY, 200, 99);
  char *a[] = {w->getName(), x->getName(), y->getName(), z->getName(), NULL};
  */
  const char *initial_options[] = {"Add a part", "Find parts", "Exit", NULL};

  while(running) {
    switch(ConWin::getOptionWindow("PC PART EXPLORER", "Please select an option", initial_options, 0)) {
      case 1: 
        addPartWindow();
        break;
      case 2: 
        findPartWindow();
        break;
      case 3:  
        running = false;
        break;
    }
  }
  printf("Thanks for using PC Part Explorer!\n");
  return 0;
}