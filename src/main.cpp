#include <algorithm>
#include <iostream>
#include <iomanip>
#include <cassert>
#include <unordered_map>
#include <vector>
#include <chrono>

#include "player.h"

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
