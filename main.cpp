#include <iostream>
#include <iomanip>
#include <cassert>
#include <unordered_map>
#include <vector>

enum action_t: int {fisherman, grocer1, grocer2, grocer3, grocer4, grocer5,
                    woolenweaver, colonist, peatcutter, dikebuilder1,
                    dikebuilder2, clayworker, farmer, forester, woodcutter,
                    master};

// enum resource_t: int {wood, timber, clay, brick,
//                       peat, uncutpeat,
//                       food, grain, flax, wool, hide,
//                       linen, leather, woolen,
//                       summerwear, leatherwear, winterwear,
//                       sheep, cattle, horse,
//                       fishtrap, fleshingbeam, weavingloom, slaughteringtable,
//                       spade, shovel, potterywheel, oven, axe, workbench};

class Resource {
  private:
    int max;
    int min;
    int amount;

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

    int get_space() {
      return max - amount;
    }

    // TODO: only used by animals, can bring to animal class once made
    void setmax(int newmax) {
      max = newmax;
    }

    Resource(int min, int max, int amount) {
      this->min    = min;
      this->max    = max;
      this->amount = amount;
    }
};

class Player {
  public:
    // TODO: put all these into an array for easier looping etc
    // TODO: put all these into private
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

    std::unordered_map<std::string, Resource&> r_lut = {
      {"wood", wood},
      {"timber", timber},
      {"clay", clay},
      {"brick", brick},
      {"peat", peat},
      {"uncutpeat", uncutpeat},
      {"food", food},
      {"grain", grain},
      {"flax", flax},
      {"wool", wool},
      {"hide", hide},
      {"linen", linen},
      {"leather", leather},
      {"woolen", woolen},
      {"summerwear", summerwear},
      {"leatherwear", leatherwear},
      {"winterwear", winterwear},
      {"sheep", sheep},
      {"cattle", cattle},
      {"horse", horse},
      {"fishtrap", fishtrap},
      {"fleshingbeam", fleshingbeam},
      {"weavingloom", weavingloom},
      {"slaughteringtable", slaughteringtable},
      {"spade", spade},
      {"shovel", shovel},
      {"potterywheel", potterywheel},
      {"oven", oven},
      {"axe", axe},
      {"workbench", workbench}
    };

    Resource find(std::string name) {
      auto it = r_lut.find(name);
      // TODO: add some error handling here
      // if (it != r_lut.end()) {
        return it->second;
      // }
    }

    int use(std::string name, int cost) {
      return find(name).use(cost);
    }

    int add(std::string name, int gain) {
      return find(name).add(gain);
    }

    int get(std::string name) {
      return find(name).get();
    }

    int get_space(std::string name) {
      return find(name).get_space();
    }

    Player():
      wood              (0, 99, 4),
      timber            (0, 99, 0),
      clay              (0, 99, 4),
      brick             (0, 99, 0),
      peat              (0, 99, 3),
      uncutpeat         (0, 16, 4),
      food              (0, 30, 5),
      grain             (0, 15, 2),
      flax              (0, 15, 3),
      wool              (0, 15, 4),
      hide              (0, 15, 1),
      linen             (0, 15, 0),
      leather           (0, 99, 0),
      woolen            (0, 99, 0),
      summerwear        (0, 99, 0),
      leatherwear       (0, 99, 0),
      winterwear        (0, 99, 0),
      sheep             (0, 99, 0),
      cattle            (0, 99, 0),
      horse             (0, 99, 1),
      fishtrap          (2,  6, 2),
      fleshingbeam      (3,  6, 3), // 3->5->6
      weavingloom       (2,  5, 2),
      slaughteringtable (2,  4, 2),
      spade             (3,  7, 3), // 3->5->7
      shovel            (3,  6, 3),
      potterywheel      (2,  4, 2),
      oven              (1,  4, 1),
      axe               (3,  6, 3),
      workbench         (2,  4, 2) {}

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

bool check_move_cost (Player &p, std::unordered_map<std::string, int> reqs) {
  for (auto& it: reqs) {
    if (p.get(it.first) < it.second) {
      return false;
    }
  }
  return true;
}

bool check_move_useful (Player &p, std::unordered_map<std::string, int> gains) {
  for (auto& it: gains) {
    if (p.get_space(it.first) < it.second) {
      return false;
    }
  }
  return true;
}

std::vector<std::string> get_good_moves (Player &p) {
  std::vector<std::string> move_list;
  std::unordered_map<std::string, int> gains;
  gains = {{"sheep", 1}, {"fishtrap", 1}, {"food", p.get("fishtrap")}};
  bool useful = false;
  useful = check_move_useful(p, gains);
  if(useful) {
    move_list.push_back("fm");
  }
  
  return move_list;
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

  std::vector<std::string> good_moves;
  good_moves = get_good_moves(p1);
  for (auto& it: good_moves) {
    std::cout << it << " ";
  }
  std::cout << std::endl;

  std::cin >> input;
  while(input != "q") {
    auto it = input_lut.find(input);
    if (it != input_lut.end()) {
      action(p1, it->second);
    } else {
      std::cout << "bad instruction" << std::endl;
    }

    p1.print();

    // TODO: repeated code
    good_moves = get_good_moves(p1);
    for (auto &it: good_moves) {
      std::cout << it << " ";
    }
    std::cout << std::endl;
    std::cin >> input;
  }

  return 0;
}
