
//  The product object.
class Product{
  private:
    std::string name;
    std::string type;
    int price;
    int stock;

  public:

    Product(std::string name, std::string type, int price, int stock) {
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

    void setName(std::string name) {
      this->name = name;
    }

    std::string getName() {
      return name;
    }

    void setType(std::string type) {
      this->type = type;
    }

    std::string getType() {
      return type;
    }
};