/*
  Changes
  1. implemented each of the 5 product modification options; changing the 4 attributes and delete.
  2. added a loop for the modifications window so that when one change is made the window is 
     reopened to allow for more changes to the same product.
  3. added remove() to ProductDatabase.cpp for deleting from the sets
  4. small change in listProductWindow when calling productOptionPrompt to add page*10 to the index being
     selected. Previously was opening items from the first page even when selecting from the second.
  5. again when calling productOptionPrompt, now passes productTree for deletion purposes

  Following changes are unique to this copy of the project file, I have a backup copy without these:
  1. Changed product type to be a string which the user can input when adding a new product
  2. removed case 3 from main, no longer allowing the addition of new product types
  3. Updated various outputs to accomodate the string format of product type
  4. Changed methods for sorting in ProductDatabase.cpp to use a string filter instead of int
  
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
#include "Product.cpp"
#include "ConWin.cpp"
#include "ProductDatabase.cpp"

std::string getProductTypeWindow(ProductDatabase * productDB, bool canEnterNewType) {
  std::string p_type;
  int p_type_int;

  std::vector<std::string> ptypes;
  bool asking_for_product_type = true;

  //Creates a window that asks for the product type, based on a list of all existing product types.
  //also lets the user define their own product type.
  while(asking_for_product_type) {
    ptypes = productDB->getProductTypes();
    if(canEnterNewType) {
      ptypes.insert(ptypes.end(), "NEW PRODUCT TYPE");
    }

    p_type_int = ConWin::getOptionWindow("ADD NEW PRODUCT", "Choose product type", ptypes, 1);
    if(p_type_int == -69) {
      return ""; 
    }else if(canEnterNewType && p_type_int == ptypes.size()) { //if we selected NEW PRODUCT TYPE
      //ask the user to create a new product;
      bool asking_for_new_product_type = true;
      while(asking_for_new_product_type){
        p_type = ConWin::getStringWindow("CREATE NEW PRODUCT TYPE", "Enter new product type", 1);
        if(p_type.empty()){
          asking_for_new_product_type = false;
        }else {
          //succesfully entered new product type.
          productDB->insertProductType(p_type);
          asking_for_new_product_type = false;
          asking_for_product_type = false;
        }
      }
    }else {
      asking_for_product_type = false;
      p_type = ptypes[p_type_int-1];
    }
  }
  return p_type;
}

Product* addProductWindow(ProductDatabase * productDB) {
  std::string p_name;
  std::string p_type;
  int p_cost;
  int p_stock;

  //Create a window that asks the user for the product name
  p_name = ConWin::getStringWindow("ADD NEW PRODUCT", "Enter product name", 1);
  if(p_name.empty()) {
    return NULL;
  }

  //Creates a window that asks you how much the product costs.
  p_cost = ConWin::getIntegerWindow("ADD NEW PRODUCT", "Enter Product Cost", 1);
  if(p_cost == -69) { //magic "cancel" character
    return NULL;
  }

  //Creates a window that asks you how many of the product are currently in stock.
  p_stock = ConWin::getIntegerWindow("ADD NEW PRODUCT", "Enter Current Stock", 1);
  if(p_stock == -69) { //magic "cancel" character
    return NULL;
  }


  p_type = getProductTypeWindow(productDB, true);
  if(p_type.empty()) { return NULL;}

  Product* new_product = new Product(p_name, p_type, p_cost, p_stock);
  ConWin::drawProductWindow(new_product, "PRODUCT SUCCESFULLY ADDED", "Please confirm the product information...");
  return new_product;
}

void productOptionPrompt(Product *product, ProductDatabase * productTree) {
  if(product == NULL) {
    return;
  }
  ConWin::drawProductWindow(product, "PRODUCT INFO", "Select an option in the next page");
  int result;
  std::string p_name;

  while (result != 6) {
    result = ConWin::getOptionWindow("SELECT PRODUCT OPTION", "Choose what to do with this product", {"Change name", "Change price", "Change stock", "Change product type", "Delete", "Back"}, 0);
    if(result == 1) {

      //Changes the Product name
      p_name = ConWin::getStringWindow("MODIFY PRODUCT", "Enter new product name", 1);
      if (p_name.empty()) {return;}
      product->setName(p_name);

    }else if(result == 2) {
      
      //Changes the Product price
      int p_cost = ConWin::getIntegerWindow("MODIFY PRODUCT", "Enter new product price", 1);
      if (p_cost == -69) {return;}
      product->setPrice(p_cost);
      productTree->update(product);

    }else if(result == 3) {
      
      //Changes the Product stock
      int p_stock = ConWin::getIntegerWindow("MODIFY PRODUCT", "Enter new stock", 1);
      if (p_stock == -69) {return;}
      product->setStock(p_stock);
      productTree->update(product);

    }else if(result == 4) {
      
      //Changes the Product type
      std::string p_type;
      p_type = getProductTypeWindow(productTree, true);
      if (p_type.empty()) {return;}
      product->setType(p_type);

    }else if(result == 5) {
      
      //Deletes the Product
      productTree->remove(product);
      ConWin::drawDialogWindow("MODIFY PRODUCT", "This product has been DELETED", {});
      result++;
    }
  }
}

void listProductWindow(ProductDatabase * productTree) {
  std::vector<Product *> localProductVector = productTree->getByPriceAscending();
  bool finished = false;
  int page = 0;
  std::string filter = "";
  int sortBy = 1; //1 == price ASC, 2 == price DESC, 3 == stock ASC, 4 == stock DESC.

  while(!finished) {

    /* SORT VECTOR */
    if(sortBy == 1) {
      localProductVector = productTree->getByPriceAscending(filter);
    }else if(sortBy == 2){
      localProductVector = productTree->getByPriceDescending(filter);
    }else if(sortBy == 3) {
      localProductVector = productTree->getByStockAscending(filter);
    }else if(sortBy == 4) {
      localProductVector = productTree->getByStockDescending(filter);
    }

    std::vector<std::string> options;
    std::vector<Product *>::iterator localProductVectorIterator = localProductVector.begin();
    std::advance(localProductVectorIterator, page*10);
    //localMapIterator++; //set page. (++ page*10) //maybe use vector::advance

    bool foundEnd = false;
    for(int i = 0; i < 10; i++) {
      if(foundEnd || localProductVectorIterator == localProductVector.end()) {
        options.insert(options.end(), "");
        foundEnd =true;
      }else{
        options.insert(options.end(), (*localProductVectorIterator)->getName() + " | " + (*localProductVectorIterator)->getType() + " $" + std::to_string((*localProductVectorIterator)->getPrice()) + " x" + std::to_string((*localProductVectorIterator)->getStock()));
      }
      localProductVectorIterator++;
    }
    options.insert(options.end(), "Filter results");
    options.insert(options.end(), "Next page");
    options.insert(options.end(), "Previous page");
      
    std::string pageInstr= "Select an option (pg " + std::to_string(page+1) + " of " + std::to_string((int)(localProductVector.size() / 10)+1) + ")";
    int result = ConWin::getOptionWindow("Select a product", pageInstr, options, 1);
    if(result >= 1 && result <= 10) {
      if(page*10 + result-1 < localProductVector.size()) {
        if(localProductVector[page*10 + result-1] != NULL) {
          productOptionPrompt(localProductVector[page*10 + result-1], productTree); //potentially make changes
          productTree->saveToFile("database.db");//save all changes
        }
      }
    }else if(result == 11) {
      //show the filter window to filter the results.
      int result = ConWin::getOptionWindow("SORT AND FILTER", "Select a sorting/filtering method", {"Sort Price (asc)", "Sort Price (desc)", "Sort Stock (asc)", "Sort Stock (desc)", "Filter product type", "Reset all filters"}, 1);
      if(result >= 1 && result <= 4) {
        sortBy = result;
      }else if(result == 5) {
        std::string t_fil = getProductTypeWindow(productTree, false);//ConWin::getStringWindow("FILTER BY TYPE", "Input the type to filter by", 1);
        if(!t_fil.empty()) {
          filter = t_fil;
        }
      }else if(result == 6) {
        filter = "";
      }
    }else if(result == 12) {
      //start showing the next ten items

      if(page < ceil(localProductVector.size() / 10)) {
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
  std::vector<std::string> initial_options = {"Add a product", "Find products", "Exit"};

  ProductDatabase * productTree = new ProductDatabase();
  productTree->loadFromFile("database.db");
 // printf("NUMER OF ELEMENTS: %d", productTree->size());

  while(running) {
    switch(ConWin::getOptionWindow("BUSINESS PRODUCT CATALOG v4.2.0", "Please make a selection~", initial_options, 0)) {
      case 1: 
      {
        productTree->insert(addProductWindow(productTree));
        productTree->saveToFile("database.db");
        break;
      }
      case 2: 
      {
        listProductWindow(productTree);
        break;
      }
      case 3: 
      {
        running = false;
        break;
      }
    }
  }
  printf("Thanks for using Business Product Catalog!\n");
  return 0;
}