#include <iostream>
#include <iomanip>
#include <cassert>
#include <unordered_map>

enum action_t: int {fisherman, grocer1, grocer2, grocer3, grocer4, grocer5,
                    woolenweaver, colonist, peatcutter, dikebuilder1,
                    dikebuilder2, clayworker, farmer, forester, woodcutter,
                    master};

enum resource_t: int {wood, timber, clay, brick,
                      peat, uncutpeat,
                      food, grain, flax, wool, hide,
                      linen, leather, woolen,
                      summerwear, leatherwear, winterwear,
                      sheep, cattle, horse,
                      fishtrap, fleshingbeam, weavingloom, slaughteringtable,
                      spade, shovel, potterywheel, oven, axe, workbench};

class Resource {
  private:
    std::string name;
    int max = 10;
    int min = 0;
    int amount = 0;

  public:
    // returns how many used
    int use(int cost) {
      int before = amount;
      amount -= cost;
      if (amount < min) {
        amount = min;
        return before;
      }
      return cost;
    }

    // returns how many gained
    int add(int gain) {
      int before = amount;
      amount += gain;
      if (amount > max) {
        amount = max;
        return max - before;
      }
      return gain;
    }

    int get() {
      return amount;
    }

    // TODO: only used by animals, can bring to animal class once made
    void setmax(int newmax) {
      max = newmax;
    }

    Resource(std::string name) {
      static std::unordered_map<std::string, std::array<int,3>> r_lut = {
        {"wood",              {0, 99, 4}},
        {"timber",            {0, 99, 0}},
        {"clay",              {0, 99, 4}},
        {"brick",             {0, 99, 0}},
        {"peat",              {0, 99, 3}},
        {"uncutpeat",         {0, 16, 4}},
        {"food",              {0, 30, 5}},
        {"grain",             {0, 15, 2}},
        {"flax",              {0, 15, 3}},
        {"wool",              {0, 15, 4}},
        {"hide",              {0, 15, 1}},
        {"linen",             {0, 15, 0}},
        {"leather",           {0, 99, 0}},
        {"woolen",            {0, 99, 0}},
        {"summerwear",        {0, 99, 0}},
        {"leatherwear",       {0, 99, 0}},
        {"winterwear",        {0, 99, 0}},
        {"sheep",             {0, 99, 0}},
        {"cattle",            {0, 99, 0}},
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
    // TODO: put all these into an array for easier looping etc
    Resource wood, timber, clay, brick;
    // always use uncutpeat first when cutting since no checks implemented
    Resource peat, uncutpeat;
    Resource food, grain, flax, wool, hide;
    Resource linen, leather, woolen;
    Resource summerwear, leatherwear, winterwear;
    // TODO: extend resource class for animals?
    Resource sheep, cattle, horse;
    // TODO: probably need different class for upgrades
    Resource fishtrap, fleshingbeam, weavingloom, slaughteringtable, spade;
    Resource shovel, potterywheel, oven, axe, workbench;

    Player():
      wood("wood"), timber("timber"), clay("clay"), brick("brick"),
      peat("peat"), uncutpeat("uncutpeat"),
      food("food"), grain("grain"), flax("flax"), wool("wool"), hide("hide"),
      linen("linen"), leather("leather"), woolen("woolen"),
      summerwear("summerwear"), leatherwear("leatherwear"), winterwear("winterwear"),
      sheep("sheep"), cattle("cattle"), horse("horse"),
      fishtrap("fishtrap"), fleshingbeam("fleshingbeam"),
      weavingloom("weavingloom"), slaughteringtable("slaughteringtable"),
      spade("spade"), shovel("shovel"), potterywheel("potterywheel"),
      oven("oven"), axe("axe"), workbench("workbench") {}

    void print() {
      std::cout << "wood/timber clay/brick peat/uncut" << std::endl;
      std::cout << std::setw(4) << wood.get() << std::setw(7) << timber.get()
                << std::setw(5) << clay.get() << std::setw(6) << brick.get()
                << std::setw(5) << peat.get() << std::setw(6) << uncutpeat.get()
                << std::endl;
      std::cout << std::endl;
      std::cout << "food grain flax wool hide" << std::endl;
      std::cout << std::setw(4) << food.get() << std::setw(6) << grain.get()
                << std::setw(5) << flax.get() << std::setw(5) << wool.get()
                << std::setw(5) << hide.get() << std::endl;
      std::cout << std::endl;
      std::cout << "linen/wear leather/wear woolen/wear" << std::endl;
      std::cout << std::setw(5) << linen.get() << std::setw(5) << summerwear.get()
                << std::setw(8) << leather.get() << std::setw(5) << leatherwear.get()
                << std::setw(7) << woolen.get() << std::setw(5) << winterwear.get()
                << std::endl;
      std::cout << std::endl;
      std::cout << "sheep cattle horse" << std::endl;
      std::cout << std::setw(5) << sheep.get() << std::setw(7) << cattle.get()
                << std::setw(6) << horse.get() << std::endl;
      std::cout << std::endl;

      std::cout << "fish traps: " << fishtrap.get()
                << " fleshing beams: " << fleshingbeam.get()
                << " weaving looms: " << weavingloom.get()
                << std::endl;
      std::cout << "slaughtering table: " << slaughteringtable.get()
                << " spades: " << spade.get()
                << " shovels: " << shovel.get()
                << std::endl;
      std::cout << "pottery wheels: " << potterywheel.get()
                << " ovens: " << oven.get()
                << " axes: "<< axe.get()
                << " workbenches: " << workbench.get()
                << std::endl;
      std::cout << std::endl;
    }
};

// bool action_is_valid(Player &p, action_t action) {
//   return true;
// }

void action(Player &p, action_t action) {
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
      p.cattle.add(1);
      p.grain.add(1);
      p.leather.add(1);
      break;
    case grocer5:
      p.horse.add(1);
      p.grain.add(1);
      p.leather.add(1);
      break;
    case woolenweaver:
      // always use as much as you can
      int wool_used;
      wool_used = p.wool.use(p.weavingloom.get());
      p.woolen.add(wool_used);
      break;
    case colonist:
      p.horse.add(1);
      // TODO: add moor tile flipping
      break;
    case peatcutter:
      // always use as much as you can
      int peat_cut;
      peat_cut = p.uncutpeat.use(p.spade.get());
      p.peat.add(peat_cut);
      break;
    case dikebuilder1:
      p.sheep.add(1);
      // TODO: add dike building
      break;
    case dikebuilder2:
      p.cattle.add(1);
      // TODO: add dike building
      break;
    case clayworker:
      p.clay.add(p.shovel.get());
      break;
    case farmer:
      // TODO: add buy plow
      // TODO: add building farms
      // TODO: figure out how to deal with grain vs flax choices per field
      break;
    case forester:
      p.food.use(1); // TODO: this is a requirement, add valid checks
      // TODO: add build forest
      // TODO: add build building
      break;
    case woodcutter:
      p.wood.add(p.axe.get());
      break;
    case master:
      // TODO: figure out how to deal with upgrading variable number of tools
      break;
  }
}

int main() {
  Player p1;
  std::string input;
  static std::unordered_map<std::string, action_t> input_lut = {
    {"fm", fisherman},
    {"g1", grocer1},
    {"g2", grocer2},
    {"g3", grocer3},
    {"g4", grocer4},
    {"g5", grocer5},
    {"ww", woolenweaver},
    {"co", colonist},
    {"pc", peatcutter},
    {"d1", dikebuilder1},
    {"d2", dikebuilder2},
    {"cw", clayworker},
    {"fa", farmer},
    {"fo", forester},
    {"wc", woodcutter},
    {"ma", master}
  };

  std::cin >> input;
  while(input != "q") {
    auto it = input_lut.find(input);
    if (it != input_lut.end()) {
      action(p1, it->second);
      p1.print();
    } else {
      std::cout << "bad instruction" << std::endl;
    }
    std::cin >> input;
  }

  return 0;
}
