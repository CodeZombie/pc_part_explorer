/* Changelog

    1.added stock input when adding new part in addPartWindow()
    2.modified coolPartList to be an array of maps<string, part*> of size 5 in main()
    3.added code to main() in Case 1 to add new parts to coolPartList in the appropriate map
      and modified addPartWindow to return a pointer to a new part (or NULL on failing a check)
    4.created the for loop to iterate through a chosen part type's map in main() Case 2, and modified
      findPartWindow to return the entered part type number (as an int)
    5.changed p_type in addPartWindow to be an int instead of a partType since it seems easier and I think
      you said earlier that you didn't want/need the enum afterall. Change it back if you still want it,
      it doesn't interfere with any of the other code I added. Only reason I changed it is because
      it kept raising the always returns false warning when compiling 'if (p_type == -69)'

    if you have any critiques, if there's anything I forgot, or if I did
    everything wrong, just let me know. I won't take it personally :)
*/

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <vector>
#include <string.h>
#include <map>
#include "ConWin.cpp"
#include "Part.cpp"
#include "PartDatabase.cpp"

//The part types available.
std::vector<std::string> part_type = {"Graphics Card", "Motherboard", "RAM", "Hard Drive", "Power Supply"};


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
  p_type = ConWin::getOptionWindow("ADD NEW PART", "Enter Part Type", part_type, 1);
  if(p_type == -69) {
    return NULL;
  }

  //Creates a window that asks you how many of the part are currently in stock.
  p_stock = ConWin::getIntegerWindow("ADD NEW PART", "Enter Current Stock", 1);
  if(p_stock == -69) { //magic "cancel" character
    return NULL;
  }

  Part* new_part = new Part(p_name, (int)p_type, p_cost, p_stock);


  //sets up the lines of text that will be drawn in the upcoming drawDialogWindow...
  char *lineOne = (char*)malloc(sizeof(char) * 99);
  char *lineTwo = (char*)malloc(sizeof(char) * 99);
  char *lineThree = (char*)malloc(sizeof(char) * 99);
  char *lineFour = (char*)malloc(sizeof(char) * 99);
  sprintf(lineOne, "Part Name: %s", p_name);
  sprintf(lineTwo, "Part Cost: %d", p_cost);
  sprintf(lineThree, "Part Type: %d", (int)p_type);
  sprintf(lineFour, "Part Stock: %d", p_stock);
  //char *lines[] = {lineOne, lineTwo, lineThree, lineFour, NULL};

  std::vector<std::string> lines = {lineOne, lineTwo, lineThree, lineFour};

  //draws the lines[] in a window.
  ConWin::drawDialogWindow("NEW PART ADDED!", "New part has been created:", lines);

  //free all that memory allocated.
  free(p_name);
  free(lineOne);
  free(lineTwo);
  free(lineThree);
  free(lineFour);

  return new_part;
}

void listPartWindow() {
  bool finished = false;
  std::map<std::string, Part*> localMap = PartDatabase::getPartMap();
  int page = 0;

  while(!finished) {
    std::vector<std::string> options;
    
    std::map<std::string,Part*>::iterator localMapIterator = localMap.begin();
    //localMapIterator++; //set page. (++ page*10)

    bool foundEnd = false;
      
    for(int i = 0; i < 10; i++) {
      if(foundEnd || localMapIterator == localMap.end()) {
        options.insert(options.end(), "");
        foundEnd =true;
      }else{
        options.insert(options.end(), localMapIterator->second->getName());
      }
      localMapIterator++;
    }
    options.insert(options.end(), "Filter Results");
    options.insert(options.end(), "Next Page");
    options.insert(options.end(), "Previous page");
      
    int result = ConWin::getOptionWindow("Select a part", "Select an option (pg 1 of 1)", options, 1);

    if(result == 11) {
      //show the filter window to further filter the results.
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

    p_type_id = ConWin::getOptionWindow("FIND A PART", "What type of parts?", part_type, 1);
        return p_type_id;
}

int main()
{
  bool running = true;
  std::vector<std::string> initial_options = {"Add a part", "Find parts", "Exit"};

  PartDatabase::loadPartData();

  while(running) {
    switch(ConWin::getOptionWindow("PC PART EXPLORER", "Please select an option", initial_options, 0)) {
      case 1: 
      {
        PartDatabase::addPart(addPartWindow());
        PartDatabase::savePartData();
        break;
      }
      case 2: 
      {
          listPartWindow();
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