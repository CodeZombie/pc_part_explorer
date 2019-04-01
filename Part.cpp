
//  The part object.
class Part{
  private:
    std::string name;
    int type;
    int price;
    int stock;

  public:
    Part(std::string name, int type, int price, int stock) {
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

    std::string getName() {
      return name;
    }

    int getType() {
      return type;
    }
};
