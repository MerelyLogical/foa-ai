#include <iostream>
#include <algorithm>
#include <iomanip>
#include <cassert>
#include <unordered_map>
#include <vector>
#include <chrono>

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

    Resource() {
      this->min    = 0;
      this->max    = 0;
      this->amount = 0;
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

    // all points doubled
    int score_goods (int a) {
      switch (a) {
        case  0 ...  6: return  0;
        case  7 ... 10: return  2;
        case 11 ... 14: return  4;
        case 15 ... 21: return  6;
        case 22 ... 25: return  8;
        case 26 ... 29: return 10;
        case 30:        return 12;
        default:
          std::cout << "score error" << std::endl;
          return 0;
      }
    }

    int score_animals () {
      int animals[3] = {r_sheep.get(), r_cattle.get(), r_horse.get()};
      std::sort(animals, animals + 3);
      return 4 * animals[0] + 2 * animals[1];
    }

    int score_tools () {
      int sum = 0;
      if (r_fishtrap.get() == 6) { sum += 6; };
      if (r_fleshingbeam.get() == 5) { sum += 2; };
      if (r_fleshingbeam.get() == 6) { sum += 6; };
      if (r_weavingloom.get() == 3) { sum += 2; };
      if (r_weavingloom.get() == 4) { sum += 6; };
      if (r_weavingloom.get() == 5) { sum += 8; };
      if (r_slaughteringtable.get() == 3) { sum += 2; };
      if (r_slaughteringtable.get() == 4) { sum += 4; };
      if (r_spade.get() == 7) { sum += 4; };
      if (r_potterywheel.get() == 4) { sum += 4; };
      if (r_oven.get() == 2) { sum += 2; };
      if (r_oven.get() == 3) { sum += 10; };
      if (r_oven.get() == 4) { sum += 14; };
      if (r_workbench.get() == 3) { sum += 2; };
      if (r_workbench.get() == 4) { sum += 8; };
      return sum;
    }

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

    float score () {
      int sum = 0;
      sum += 1 * r_timber.get();
      sum += 2 * r_brick.get();
      sum += score_goods(r_food.get());
      sum += score_goods(r_grain.get());
      sum += score_goods(r_flax.get());
      sum += score_goods(r_wool.get());
      sum += score_goods(r_hide.get());
      sum += 2 * r_linen.get();
      sum += 2 * r_leather.get();
      sum += 2 * r_woolen.get();
      sum += 4 * r_summerwear.get();
      sum += 5 * r_leatherwear.get();
      sum += 5 * r_winterwear.get();
      sum += score_animals();
      sum += score_tools();
      return (float)sum / 2.0;
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

    Player(const Player &p) {
      r_wood = p.r_wood;
      r_timber = p.r_timber;
      r_clay = p.r_clay;
      r_brick = p.r_brick;
      r_peat = p.r_peat;
      r_uncutpeat = p.r_uncutpeat;
      r_food = p.r_food;
      r_grain = p.r_grain;
      r_flax = p.r_flax;
      r_wool = p.r_wool;
      r_hide = p.r_hide;
      r_linen = p.r_linen;
      r_leather = p.r_leather;
      r_woolen = p.r_woolen;
      r_summerwear = p.r_summerwear;
      r_leatherwear = p.r_leatherwear;
      r_winterwear = p.r_winterwear;
      r_sheep = p.r_sheep;
      r_cattle = p.r_cattle;
      r_horse = p.r_horse;
      r_fishtrap = p.r_fishtrap;
      r_fleshingbeam = p.r_fleshingbeam;
      r_weavingloom = p.r_weavingloom;
      r_slaughteringtable = p.r_slaughteringtable;
      r_spade = p.r_spade;
      r_shovel = p.r_shovel;
      r_potterywheel = p.r_potterywheel;
      r_oven = p.r_oven;
      r_axe = p.r_axe;
      r_workbench = p.r_workbench;
    }

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
      std::cout << "score: " << std::fixed << std::setprecision(1) << score() << std::endl;
      std::cout << std::endl;
    }
};

typedef std::unordered_map<resource_t, int> rlist_t;
typedef std::unordered_map<std::string, rlist_t> mlist_t;

bool check_move_useful (Player &p, rlist_t rlist) {
  for (auto& it: rlist) {
    if (it.second > 0) {
      if (p.get_space(it.first) < it.second) {
        return false;
      }
    } else if (it.second < 0) {
      if (p.get(it.first) < -it.second) {
        return false;
      }
    }
  }
  return true;
}

void perform_action (Player &p, rlist_t rlist) {
  for (auto& it: rlist) {
    if (it.second > 0) {
      p.add(it.first, it.second);
    } else if (it.second < 0) {
      p.use(it.first, -it.second);
    }
  }
}

mlist_t get_good_moves (Player &p) {
  mlist_t mlist;
  std::string move_command;
  rlist_t rlist;

  // TODO: try make these actions into functions/loop
  move_command = "fm";
  rlist = {{sheep, 1}, {fishtrap, 1}, {food, p.get(fishtrap)}};
  // TODO: maybe just check for validity, can still be useful to take food with max fishtraps
  if (check_move_useful(p, rlist)) { mlist[move_command] = rlist; }

  // repeating for grocer
  resource_t choices[5] = {timber, brick, sheep, cattle, horse};
  for (int i=0; i<5; i++) {
    move_command = "gr" + std::to_string(i);
    resource_t choice = choices[i];
    rlist = {{choice, 1}, {grain, 1}, {leather, 1}};

    if (check_move_useful(p, rlist)) { mlist[move_command] = rlist; }
  }

  // repeating for woolenweaver
  int wool_cost = std::min(p.get(weavingloom), p.get(wool));
  if (wool_cost != 0) {
    move_command = "ww" + std::to_string(wool_cost);
    rlist = {{wool, -wool_cost}, {woolen, wool_cost}};
    mlist[move_command] = rlist;
  }

  // repeating for peatcutting
  int peat_cut = std::min(p.get(spade), p.get(uncutpeat));
  if (peat_cut != 0) {
    move_command = "pc" + std::to_string(peat_cut);
    rlist = {{uncutpeat, -peat_cut}, {peat, peat_cut}};
    mlist[move_command] = rlist;
  }

  // repeating for woodcutter
  move_command = "wc";
  rlist = {{wood, p.get(axe)}};
  mlist[move_command] = rlist;

  // repeating for clayworker
  move_command = "cw";
  rlist = {{clay, p.get(shovel)}};
  mlist[move_command] = rlist;

  // case colonist:
  //   p.add(horse, 1);
  //   // TODO: add moor tile flipping
  //   break;
  // case dikebuilder1:
  //   p.add(sheep, 1);
  //   // TODO: add dike building
  //   break;
  // case dikebuilder2:
  //   p.add(cattle, 1);
  //   // TODO: add dike building
  //   break;
  // case farmer:
  //   // TODO: add buy plow
  //   // TODO: add building farms
  //   // TODO: figure out how to deal with grain vs flax choices per field
  //   break;
  // case forester:
  //   p.use(food, 1); // TODO: this is a requirement, add valid checks
  //   // TODO: add build forest
  //   // TODO: add build building
  //   break;
  // case master:
  //   // TODO: figure out how to deal with upgrading variable number of tools
  //   break;

  return mlist;
}

// TODO: keep more than just the one best move?
void evaluate_moves (Player &p, mlist_t moves, int depth, std::string history, std::string &best_line, float &best_score) {
  for (auto& it: moves) {
    Player p_temp = p;
    std::string temp_hist;
    if (history.empty()) {
      temp_hist = it.first;
    } else {
      temp_hist = history + ">" + it.first;
    }
    perform_action(p_temp, it.second);
    float temp_score = p_temp.score();
    // TODO: counting the number of spaces to get move lengths seems dumb
    int move_length = std::count(best_line.begin(), best_line.end(), '>');
    int temp_length = std::count(temp_hist.begin(), temp_hist.end(), '>');

    // record line that is better or achieve the same score in less moves
    if ((temp_score > best_score) ||
        (temp_score == best_score && move_length > temp_length)) {
      best_line = temp_hist;
      best_score = temp_score;
    }

    // search recursively from p_temp
    mlist_t temp_moves = get_good_moves(p_temp);
    if (depth > 0 && !temp_moves.empty()) {
      evaluate_moves(p_temp, temp_moves, depth-1, temp_hist, best_line, best_score);
    }
  }
}

// TODO: consider pulling some logic out from main function
// TODO: set timer for evaluate_move, keep searching higher depths until timer run out
int main() {
  Player p1;
  std::string input;
  mlist_t good_moves;

  p1.print();
  good_moves = get_good_moves(p1);
  std::cout << "Possible good moves: ";
  for (auto &it: good_moves) {
    std::cout << it.first << " ";
  }
  std::cout << std::endl;
  std::cout << "---------------------" << std::endl;
  std::cout << "Pick a move: " << std::flush;
  std::cin >> input;

  while(input != "q") {
    if (input == "ai") {
      // use evaluate_moves with timer
      std::string best_line = "";
      float best_score = 0.0;
      int depth;
      std::cout << "Enter search depth:" << std::endl;
      std::cin >> depth;
      auto start = std::chrono::steady_clock::now();
      evaluate_moves(p1, good_moves, depth, "", best_line, best_score);
      auto stop = std::chrono::steady_clock::now();
      auto time = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
      std::cout << "In " << time.count() << "ms," << std::endl;
      std::cout << "Found best line: " << best_line << " with score: " << std::fixed << std::setprecision(1) << best_score << std::endl;
    } else {
      // normal playing
      auto it = good_moves.find(input);
      if (it != good_moves.end()) {
        perform_action(p1, it->second);
        // TODO: repeated code
        p1.print();
        good_moves = get_good_moves(p1);
        std::cout << "Possible good moves: ";
        for (auto &it: good_moves) {
          std::cout << it.first << " ";
        }
        std::cout << std::endl;
        std::cout << "---------------------" << std::endl;
      } else {
        std::cout << "bad instruction" << std::endl;
      }
    }
    if (good_moves.empty()) {
      std::cout << "No more moves, game over." << std::endl;
      break;
    } else {
      std::cout << "Pick a move: " << std::flush;
      std::cin >> input;
    }
  }

  return 0;
}
