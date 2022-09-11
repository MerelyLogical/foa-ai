#include <iostream>
#include <unordered_map>

#include "player.h"

// returns how many used
int Resource::use(int cost) {
  int before = amount;
  amount -= cost;
  if (amount < min) {
    amount = min;
    return before;
  }
  return cost;
}

// returns how many gained
int Resource::add(int gain) {
  int before = amount;
  amount += gain;
  if (amount > max) {
    amount = max;
    return max - before;
  }
  return gain;
}

int Resource::get() {
  return amount;
}

int Resource::get_space() {
  return max - amount;
}

// TODO: only used by animals, can bring to animal class once made
void Resource::setmax(int newmax) {
  max = newmax;
}

Resource::Resource() {
  this->min    = 0;
  this->max    = 0;
  this->amount = 0;
}

Resource::Resource(int min, int max, int amount) {
  this->min    = min;
  this->max    = max;
  this->amount = amount;
}


// all points doubled
int Player::score_goods (int a) {
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

int Player::score_animals () {
  int animals[3] = {r_sheep.get(), r_cattle.get(), r_horse.get()};
  std::sort(animals, animals + 3);
  return 4 * animals[0] + 2 * animals[1];
}

int Player::score_tools () {
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

int Player::next_turn() {
  turn_number++;
  return turn_number;
}

void Player::mark_action(action_t action_type) {
  switch (action_type) {
    case fisherman:         board_status.fisherman = true;        break;
    case summergrocer:      board_status.summergrocer = true;     break;
    case woolweaver:        board_status.woolweaver = true;       break;
    case colonist:          board_status.colonist = true;         break;
    case peatcutter:        board_status.peatcutter = true;       break;
    case dikebuilder:       board_status.dikebuilder = true;      break;
    case clayworker:        board_status.clayworker = true;       break;
    case farmer:            board_status.farmer = true;           break;
    case forester:          board_status.forester = true;         break;
    case woodcutter:        board_status.woodcutter = true;       break;
    case summermaster:      board_status.summermaster = true;     break;
    case peatboats:         board_status.peatboats = true;        break;
    case tanner:            board_status.tanner = true;           break;
    case linenweaver:       board_status.linenweaver = true;      break;
    case butcher:           board_status.butcher = true;          break;
    case cattletrader:      board_status.cattletrader = true;     break;
    case wintergrocer:      board_status.wintergrocer = true;     break;
    case buildersmerchant:  board_status.buildersmerchant = true; break;
    case potter:            board_status.potter = true;           break;
    case baker:             board_status.baker = true;            break;
    case woodtrader:        board_status.woodtrader = true;       break;
    case wintermaster:      board_status.wintermaster = true;     break;
  }
}

// season_t Player::get_season() {
//   switch (turn_number % 10) {
//     case 0:       return spring;
//     case 1 ... 4: return summer;
//     case 5      : return autumn;
//     case 6 ... 9: return winter;
//     default:
//       std::cout << "season error" << std::endl;
//       return spring;
//   }
// }

// std::string Player::get_season_str() {
//   switch (turn_number % 10) {
//     case 0:       return "Spring";
//     case 1 ... 4: return "Summer";
//     case 5      : return "Autumn";
//     case 6 ... 9: return "Winter";
//     default:
//       std::cout << "season error" << std::endl;
//       return "error";
//   }
// }

season_t Player::get_season() {
  switch (turn_number % 8) {
    case 0:       return winter;
    case 1 ... 4: return summer;
    case 5 ... 7: return winter;
    default:
      std::cout << "season error" << std::endl;
      return spring;
  }
}

std::string Player::get_season_str() {
  switch (turn_number % 8) {
    case 0:       return "Winter";
    case 1 ... 4: return "Summer";
    case 5 ... 7: return "Winter";
    default:
      std::cout << "season error" << std::endl;
      return "error";
  }
}

int Player::use(resource_t name, int cost) {
  return r_lut.at(name).use(cost);
}

int Player::add(resource_t name, int gain) {
  return r_lut.at(name).add(gain);
}

int Player::get(resource_t name) {
  return r_lut.at(name).get();
}

int Player::get_space(resource_t name) {
  return r_lut.at(name).get_space();
}

float Player::score () {
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

Player::Player():
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
  r_workbench         (2,  4, 2) {
    turn_number = 1;
  }

Player::Player(const Player &p) {
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
  turn_number = p.turn_number;
  board_status = p.board_status;
}

void Player::print() {
  std::cout << std::endl;
  std::cout << "Turn " << turn_number << ", " << get_season_str() << std::endl;
  std::cout << std::endl;
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
