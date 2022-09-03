#include <iostream>
#include <iomanip>
#include <cassert>
#include <unordered_map>

enum action_t: int {fisherman, grocer1, grocer2, grocer3, grocer4, grocer5, woolenweaver};

enum resource_t: int {wood, timber, clay, brick,
                      peat, uncutpeat,
                      food, grain, flax, wool, hide,
                      linen, leather, woolen,
                      summerwear, leatherwear, winterwear,
                      sheep, cow, horse,
                      fishtrap, fleshingbeam, weavingloom, slaughteringtable,
                      spade, shovel, potterywheel, oven, axe, workbench};

class Resource {
  private:
    std::string name;
    int max = 10;
    int min = 0;
    int amount = 0;

  public:
    bool use(int cost) {
      if (amount - cost < min) {
        return false; // insufficient resources
      } else {
        amount -= cost;
        return true;
      }
    }

    void add(int gain) {
      amount += gain;
      if (amount > max) {
        amount = max; // overflow
      }
    }

    int get() {
      return amount;
    }

    void setmax(int newmax) {
      max = newmax;
    }

    void print() {
      std::cout << std::setw(20) << name << std::setw(3) << amount << std::endl;
    }

    Resource(std::string name) {
      // have a look-up table here for max/min/init values
      static std::unordered_map<std::string, std::array<int,3>> r_lut = {
        {"wood",              {0, 99, 4}},
        {"timber",            {0, 99, 0}},
        {"clay",              {0, 99, 4}},
        {"brick",             {0, 99, 0}},
        {"peat",              {0, 99, 3}},
        {"uncutpeat",         {0, 99, 4}},
        {"food",              {0, 99, 5}},
        {"grain",             {0, 99, 2}},
        {"flax",              {0, 99, 3}},
        {"wool",              {0, 99, 4}},
        {"hide",              {0, 99, 1}},
        {"linen",             {0, 99, 0}},
        {"leather",           {0, 99, 0}},
        {"woolen",            {0, 99, 0}},
        {"summerwear",        {0, 99, 0}},
        {"leatherwear",       {0, 99, 0}},
        {"winterwear",        {0, 99, 0}},
        {"sheep",             {0, 99, 0}},
        {"cow",               {0, 99, 0}},
        {"horse",             {0, 99, 1}},
        {"fishtrap",          {2,  6, 2}},
        {"fleshingbeam",      {3,  6, 3}}, // 3->5->6
        {"weavingloom",       {2,  5, 2}},
        {"slaughteringtable", {2,  4, 2}},
        {"spade",             {3,  7, 3}}, // 3->5->7
        {"shovel",            {3,  6, 3}},
        {"potterywheel",      {2,  4, 2}},
        {"oven",              {1,  4, 1}},
        {"axe",               {3,  6, 3}},
        {"workbench",         {2,  4, 2}}
      }; 
      auto it = r_lut.find(name);
      // resource must be in the table
      assert(it != r_lut.end());
      this->name = name;
      this->min = it->second[0];
      this->max = it->second[1];
      this->amount = it->second[2];
    }
};

class Player {
  public:
    // put all these into an array for easier looping etc
    Resource wood, timber, clay, brick;
    // always use uncutpeat first when cutting since no checks implemented
    Resource peat, uncutpeat;
    Resource food, grain, flax, wool, hide;
    Resource linen, leather, woolen;
    Resource summerwear, leatherwear, winterwear;
    // extend resource class for animals?
    Resource sheep, cow, horse;
    // probably need different class for upgrades
    Resource fishtrap, fleshingbeam, weavingloom, slaughteringtable, spade;
    Resource shovel, potterywheel, oven, axe, workbench;

    Player():
      wood("wood"), timber("timber"), clay("clay"), brick("brick"),
      peat("peat"), uncutpeat("uncutpeat"),
      food("food"), grain("grain"), flax("flax"), wool("wool"), hide("hide"),
      linen("linen"), leather("leather"), woolen("woolen"),
      summerwear("summerwear"), leatherwear("leatherwear"), winterwear("winterwear"),
      sheep("sheep"), cow("cow"), horse("horse"),
      fishtrap("fishtrap"), fleshingbeam("fleshingbeam"),
      weavingloom("weavingloom"), slaughteringtable("slaughteringtable"),
      spade("spade"), shovel("shovel"), potterywheel("potterywheel"),
      oven("oven"), axe("axe"), workbench("workbench") {}

    void print() {
      wood.print();
      timber.print();
      clay.print();
      brick.print();
      peat.print();
      uncutpeat.print();
      food.print();
      grain.print();
      flax.print();
      wool.print();
      hide.print();
      linen.print();
      leather.print();
      woolen.print();
      summerwear.print();
      leatherwear.print();
      winterwear.print();
      sheep.print();
      cow.print();
      horse.print();
      fishtrap.print();
      fleshingbeam.print();
      weavingloom.print();
      slaughteringtable.print();
      spade.print();
      shovel.print();
      potterywheel.print();
      oven.print();
      axe.print();
      workbench.print();
    }
};

bool action(Player &p, action_t action) {
  bool status = true;
  switch(action) {
    case fisherman:
      p.sheep.add(1);
      p.fishtrap.add(1);
      p.food.add(p.fishtrap.get());
      break;
    case grocer1:
      p.timber.add(1);
      p.grain.add(1);
      p.leather.add(1);
      break;
    case grocer2:
      p.brick.add(1);
      p.grain.add(1);
      p.leather.add(1);
      break;
    case grocer3:
      p.sheep.add(1);
      p.grain.add(1);
      p.leather.add(1);
      break;
    case grocer4:
      p.cow.add(1);
      p.grain.add(1);
      p.leather.add(1);
      break;
    case grocer5:
      p.horse.add(1);
      p.grain.add(1);
      p.leather.add(1);
      break;
    case woolenweaver:
      status = p.wool.use(p.weavingloom.get());
      p.woolen.add(p.weavingloom.get());
  }
  return status;
}

int main() {
  Player p1;
  bool status = true;
  std::string input;
  static std::unordered_map<std::string, action_t> input_lut = {
    {"fisherman", fisherman},
    {"grocer1", grocer1},
    {"grocer2", grocer2},
    {"grocer3", grocer3},
    {"grocer4", grocer4},
    {"grocer5", grocer5},
    {"woolenweaver", woolenweaver}
  };

  while(status) {
    std::cin >> input;
    auto it = input_lut.find(input);
    if (it != input_lut.end()) {
      status = action(p1, it->second);
      p1.print();
    } else {
      std::cout << "bad instruction" << std::endl;
    }
  }

  return 0;
}
