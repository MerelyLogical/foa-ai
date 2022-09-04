#include <iostream>
#include <iomanip>
#include <cassert>
#include <unordered_map>
#include <vector>

// enum action_t: int {fisherman, grocer1, grocer2, grocer3, grocer4, grocer5,
//                     woolenweaver, colonist, peatcutter, dikebuilder1,
//                     dikebuilder2, clayworker, farmer, forester, woodcutter,
//                     master};

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
  private:
    Resource r_wood, r_timber, r_clay, r_brick;
    // always use uncutpeat first when cutting since no checks implemented
    Resource r_peat, r_uncutpeat;
    Resource r_food, r_grain, r_flax, r_wool, r_hide;
    Resource r_linen, r_leather, r_woolen;
    Resource r_summerwear, r_leatherwear, r_winterwear;
    // TODO: extend resource class for animals?
    Resource r_sheep, r_cattle, r_horse;
    // TODO: probably need different class for upgrades
    Resource r_fishtrap, r_fleshingbeam, r_weavingloom, r_slaughteringtable;
    Resource r_spade, r_shovel, r_potterywheel, r_oven, r_axe, r_workbench;

  public:
    std::unordered_map<resource_t, Resource&> r_lut = {
      {wood, r_wood},
      {timber, r_timber},
      {clay, r_clay},
      {brick, r_brick},
      {peat, r_peat},
      {uncutpeat, r_uncutpeat},
      {food, r_food},
      {grain, r_grain},
      {flax, r_flax},
      {wool, r_wool},
      {hide, r_hide},
      {linen, r_linen},
      {leather, r_leather},
      {woolen, r_woolen},
      {summerwear, r_summerwear},
      {leatherwear, r_leatherwear},
      {winterwear, r_winterwear},
      {sheep, r_sheep},
      {cattle, r_cattle},
      {horse, r_horse},
      {fishtrap, r_fishtrap},
      {fleshingbeam, r_fleshingbeam},
      {weavingloom, r_weavingloom},
      {slaughteringtable, r_slaughteringtable},
      {spade, r_spade},
      {shovel, r_shovel},
      {potterywheel, r_potterywheel},
      {oven, r_oven},
      {axe, r_axe},
      {workbench, r_workbench}
    };

    int use(resource_t name, int cost) {
      return r_lut.at(name).use(cost);
    }

    int add(resource_t name, int gain) {
      return r_lut.at(name).add(gain);
    }

    int get(resource_t name) {
      return r_lut.at(name).get();
    }

    int get_space(resource_t name) {
      return r_lut.at(name).get_space();
    }

    Player():
      r_wood              (0, 99, 4),
      r_timber            (0, 99, 0),
      r_clay              (0, 99, 4),
      r_brick             (0, 99, 0),
      r_peat              (0, 99, 3),
      r_uncutpeat         (0, 16, 4),
      r_food              (0, 30, 5),
      r_grain             (0, 15, 2),
      r_flax              (0, 15, 3),
      r_wool              (0, 15, 4),
      r_hide              (0, 15, 1),
      r_linen             (0, 15, 0),
      r_leather           (0, 99, 0),
      r_woolen            (0, 99, 0),
      r_summerwear        (0, 99, 0),
      r_leatherwear       (0, 99, 0),
      r_winterwear        (0, 99, 0),
      r_sheep             (0, 99, 0),
      r_cattle            (0, 99, 0),
      r_horse             (0, 99, 1),
      r_fishtrap          (2,  6, 2),
      r_fleshingbeam      (3,  6, 3), // 3->5->6
      r_weavingloom       (2,  5, 2),
      r_slaughteringtable (2,  4, 2),
      r_spade             (3,  7, 3), // 3->5->7
      r_shovel            (3,  6, 3),
      r_potterywheel      (2,  4, 2),
      r_oven              (1,  4, 1),
      r_axe               (3,  6, 3),
      r_workbench         (2,  4, 2) {}

    void print() {
      std::cout << "wood/timber clay/brick peat/uncut" << std::endl;
      std::cout << std::setw(4) << r_wood.get() << std::setw(7) << r_timber.get()
                << std::setw(5) << r_clay.get() << std::setw(6) << r_brick.get()
                << std::setw(5) << r_peat.get() << std::setw(6) << r_uncutpeat.get()
                << std::endl;
      std::cout << std::endl;
      std::cout << "food grain flax wool hide" << std::endl;
      std::cout << std::setw(4) << r_food.get() << std::setw(6) << r_grain.get()
                << std::setw(5) << r_flax.get() << std::setw(5) << r_wool.get()
                << std::setw(5) << r_hide.get() << std::endl;
      std::cout << std::endl;
      std::cout << "linen/wear leather/wear woolen/wear" << std::endl;
      std::cout << std::setw(5) << r_linen.get() << std::setw(5) << r_summerwear.get()
                << std::setw(8) << r_leather.get() << std::setw(5) << r_leatherwear.get()
                << std::setw(7) << r_woolen.get() << std::setw(5) << r_winterwear.get()
                << std::endl;
      std::cout << std::endl;
      std::cout << "sheep cattle horse" << std::endl;
      std::cout << std::setw(5) << r_sheep.get() << std::setw(7) << r_cattle.get()
                << std::setw(6) << r_horse.get() << std::endl;
      std::cout << std::endl;

      std::cout << "fish traps: " << r_fishtrap.get()
                << " fleshing beams: " << r_fleshingbeam.get()
                << " weaving looms: " << r_weavingloom.get()
                << std::endl;
      std::cout << "slaughtering table: " << r_slaughteringtable.get()
                << " spades: " << r_spade.get()
                << " shovels: " << r_shovel.get()
                << std::endl;
      std::cout << "pottery wheels: " << r_potterywheel.get()
                << " ovens: " << r_oven.get()
                << " axes: "<< r_axe.get()
                << " workbenches: " << r_workbench.get()
                << std::endl;
      std::cout << std::endl;
    }
};

// void action(Player &p, action_t action) {
//   switch(action) {
//     case fisherman:
//       p.add(sheep, 1);
//       p.add(fishtrap, 1);
//       p.add(food, p.get(fishtrap));
//       break;
//     case grocer1:
//       p.add(timber, 1);
//       p.add(grain, 1);
//       p.add(leather, 1);
//       break;
//     case grocer2:
//       p.add(brick, 1);
//       p.add(grain, 1);
//       p.add(leather, 1);
//       break;
//     case grocer3:
//       p.add(sheep, 1);
//       p.add(grain, 1);
//       p.add(leather, 1);
//       break;
//     case grocer4:
//       p.add(cattle, 1);
//       p.add(grain, 1);
//       p.add(leather, 1);
//       break;
//     case grocer5:
//       p.add(horse, 1);
//       p.add(grain, 1);
//       p.add(leather, 1);
//       break;
//     case woolenweaver:
//       // always use as much as you can
//       int wool_used;
//       wool_used = p.use(wool, p.get(weavingloom));
//       p.add(woolen, wool_used);
//       break;
//     case colonist:
//       p.add(horse, 1);
//       // TODO: add moor tile flipping
//       break;
//     case peatcutter:
//       // always use as much as you can
//       int peat_cut;
//       peat_cut = p.use(uncutpeat, p.get(spade));
//       p.add(peat, peat_cut);
//       break;
//     case dikebuilder1:
//       p.add(sheep, 1);
//       // TODO: add dike building
//       break;
//     case dikebuilder2:
//       p.add(cattle, 1);
//       // TODO: add dike building
//       break;
//     case clayworker:
//       p.add(clay, p.get(shovel));
//       break;
//     case farmer:
//       // TODO: add buy plow
//       // TODO: add building farms
//       // TODO: figure out how to deal with grain vs flax choices per field
//       break;
//     case forester:
//       p.use(food, 1); // TODO: this is a requirement, add valid checks
//       // TODO: add build forest
//       // TODO: add build building
//       break;
//     case woodcutter:
//       p.add(wood, p.get(axe));
//       break;
//     case master:
//       // TODO: figure out how to deal with upgrading variable number of tools
//       break;
//   }
// }

typedef std::unordered_map<resource_t, int> rlist_t;

// bool check_move_cost (Player &p, rlist_t reqs) {
//   for (auto& it: reqs) {
//     if (p.get(it.first) < it.second) {
//       return false;
//     }
//   }
//   return true;
// }

bool check_move_useful (Player &p, rlist_t rlist) {
  for (auto& it: rlist) {
    if (it.second > 0) {
      if (p.get_space(it.first) < it.second) {
        return false;
      }
    } else (it.second < 0) {
      if (p.get(it.first) < it.second) {
        return false;
      }
    }
  }
  return true;
}

void perform_action (Player &p, rlist_t rlist) {
  for (auto& it: costs) {
    if (it.second > 0) {
      p.add(it.first, it.second);
    } else (it.second < 0) {
      p.use(it.first, it.second);
    }
  }
}

std::vector<std::string> get_good_moves (Player &p) {
  std::vector<std::string> move_list;
  rlist_t rlist;
  rlist = {{sheep, 1}, {fishtrap, 1}, {food, p.get(fishtrap)}};
  bool useful = false;
  useful = check_move_useful(p, gains);
  if(useful) {
    move_list.push_back("fm");
  }
  return move_list;
}

// TODO: have a unordered_map<string action_name, rlist_t rlist>
//       get_good_moves will make a vector of it
//       main() will pick one in the repl loop
//       perform_action will then perform it

int main() {
  Player p1;
  std::string input;
  // static std::unordered_map<std::string, action_t> input_lut = {
  //   {"fm", fisherman},
  //   {"g1", grocer1},
  //   {"g2", grocer2},
  //   {"g3", grocer3},
  //   {"g4", grocer4},
  //   {"g5", grocer5},
  //   {"ww", woolenweaver},
  //   {"co", colonist},
  //   {"pc", peatcutter},
  //   {"d1", dikebuilder1},
  //   {"d2", dikebuilder2},
  //   {"cw", clayworker},
  //   {"fa", farmer},
  //   {"fo", forester},
  //   {"wc", woodcutter},
  //   {"ma", master}
  // };

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
