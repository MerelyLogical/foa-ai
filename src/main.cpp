#include <algorithm>
#include <iostream>
#include <iomanip>
#include <cassert>
#include <unordered_map>
#include <vector>
#include <chrono>

#include "player.h"

typedef std::unordered_map<resource_t, int> rlist_t;
typedef std::unordered_map<std::string, std::pair<rlist_t, action_t>> mlist_t;

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

void perform_action (Player &p, rlist_t rlist, action_t action_type) {
  for (auto& it: rlist) {
    if (it.second > 0) {
      p.add(it.first, it.second);
    } else if (it.second < 0) {
      p.use(it.first, -it.second);
    }
  }
  // TODO: this should be under board class in the future
  p.mark_action(action_type); // mark board action space as taken
  p.next_turn();
}

mlist_t get_good_moves (Player &p) {
  mlist_t mlist;
  std::string move_command;
  rlist_t rlist;

  season_t season = p.get_season();
  // TODO: try make these actions into functions/loop
  if (season == summer) {
    // fisherman
    if (!p.board_status.fisherman) {
      move_command = "fm";
      rlist = {{sheep, 1}, {fishtrap, 1}, {food, p.get(fishtrap)}};
      // TODO: maybe just check for validity, can still be useful to take food with max fishtraps
      if (check_move_useful(p, rlist)) { mlist[move_command] = {rlist, fisherman}; }
    }

    // grocer
    if (!p.board_status.summergrocer) {
      resource_t gsc[5] = {timber, brick, sheep, cattle, horse};
      char gsc_names[5] = {'t', 'b', 's', 'c', 'h'};
      for (int i=0; i<5; i++) {
        move_command = "gs";
        move_command.push_back(gsc_names[i]);
        resource_t choice = gsc[i];
        rlist = {{choice, 1}, {grain, 1}, {leather, 1}};
        if (check_move_useful(p, rlist)) { mlist[move_command] = {rlist, summergrocer}; }
      }
    }

    // wool weaver
    if (!p.board_status.woolweaver) {
      int wool_cost = std::min(p.get(weavingloom), p.get(wool));
      if (wool_cost != 0) {
        move_command = "ww" + std::to_string(wool_cost);
        rlist = {{wool, -wool_cost}, {woolen, wool_cost}};
        mlist[move_command] = {rlist, woolweaver};
      }
    }

      // case colonist:
      //   p.add(horse, 1);
      //   // TODO: add moor tile flipping
      //   break;

    // peat cutter
    if (!p.board_status.peatcutter) {
      int peat_cut = std::min(p.get(spade), p.get(uncutpeat));
      if (peat_cut != 0) {
        move_command = "pc" + std::to_string(peat_cut);
        rlist = {{uncutpeat, -peat_cut}, {peat, peat_cut}};
        mlist[move_command] = {rlist, peatcutter};
      }
    }

      // case dikebuilder1:
      //   p.add(sheep, 1);
      //   // TODO: add dike building
      //   break;
      // case dikebuilder2:
      //   p.add(cattle, 1);
      //   // TODO: add dike building
      //   break

    // clayworker
    if (!p.board_status.clayworker) {
      move_command = "cw";
      rlist = {{clay, p.get(shovel)}};
      mlist[move_command] = {rlist, clayworker};
    }

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

    // woodcutter
    if (!p.board_status.woodcutter) {
      move_command = "wc";
      rlist = {{wood, p.get(axe)}};
      mlist[move_command] = {rlist, woodcutter};
    }
      // case master:
      //   // TODO: figure out how to deal with upgrading variable number of tools
      //   break;

  } else if (season == winter) {
    // TODO: peatboats

    // tanner
    if (!p.board_status.tanner) {
      int hide_cost = std::min(p.get(fleshingbeam), p.get(hide));
      if (hide_cost != 0) {
        move_command = "ta" + std::to_string(hide_cost);
        rlist = {{hide, -hide_cost}, {leather, hide_cost}};
        mlist[move_command] = {rlist, tanner};
      }
    }

    // linen weaver
    if (!p.board_status.linenweaver) {
      int flax_cost = std::min(p.get(weavingloom), p.get(flax));
      if (flax_cost != 0) {
        move_command = "lw" + std::to_string(flax_cost);
        rlist = {{flax, -flax_cost}, {linen, flax_cost}};
        mlist[move_command] = {rlist, linenweaver};
      }
    }

      // TODO: butcher

    // cattle trader
    if (!p.board_status.cattletrader) {
      resource_t ctc[2] = {cattle, horse};
      char ctc_names[2] = {'c', 'h'};
      for (int i=0; i<2; i++) {
        move_command = "ct";
        move_command.push_back(ctc_names[i]);
        resource_t choice = ctc[i];
        rlist = {{grain, 2}, {sheep, 1}, {choice, 1}};
        if (check_move_useful(p, rlist)) { mlist[move_command] = {rlist, cattletrader}; }
      }
    }

    // grocer
    if (!p.board_status.wintergrocer) {
      resource_t gwc[3] = {sheep, cattle, horse};
      char gwc_names[3] = {'s', 'c', 'h'};
      for (int i=0; i<3; i++) {
        move_command = "gw";
        move_command.push_back(gwc_names[i]);
        resource_t choice = gwc[i];
        rlist = {{uncutpeat, -1}, {peat, 1}, {choice, 1}, {wood, 1}, {brick, 1}};
        // TODO: maybe allow it even without cut peat?
        if (check_move_useful(p, rlist)) { mlist[move_command] = {rlist, wintergrocer}; }
      }
    }

    // builders' merchant
    if (!p.board_status.buildersmerchant) {
      resource_t bmc1[2] = {wood, clay};
      char bmc1_names[2] = {'w', 'c'};
      resource_t bmc2[2] = {timber, brick};
      char bmc2_names[2] = {'t', 'b'};
      for (int i=0; i<2; i++) {
        for (int j=0; j<2; j++) {
          move_command = "bm";
          move_command.push_back(bmc1_names[i]);
          resource_t c1 = bmc1[i];
          move_command.push_back(bmc2_names[j]);
          resource_t c2 = bmc2[j];
          rlist = {{hide, 2}, {c1, 1}, {c2, 1}};
          if (check_move_useful(p, rlist)) { mlist[move_command] = {rlist, buildersmerchant}; }
        }
      }
    }

    // potter
    if (!p.board_status.potter) {
      int clay_cost = std::min(p.get(potterywheel), p.get(clay));
      if (clay_cost != 0) {
        move_command = "po" + std::to_string(clay_cost);
        rlist = {{clay, -clay_cost}, {food, 3*clay_cost}, {peat, clay_cost}};
        mlist[move_command] = {rlist, potter};
      }
    }

    // baker
    if (!p.board_status.baker) {
      int gs = p.get(grain);
      int ps = p.get(peat);
      int bake_cost = std::min({
        p.get(oven),
        gs + p.get(flax),
        ps + p.get(wood),
        p.get_space(food) / 6 + (p.get_space(food) % 6 != 0)
      });
      if (bake_cost != 0) {
        move_command = "ba" + std::to_string(bake_cost);
        if (bake_cost > gs && bake_cost > ps) {
          // not enough grain and not enough peat
          rlist = {{grain, -gs}, {flax, gs-bake_cost}, {peat, -ps}, {wood, ps-bake_cost},
                   {food, 6*bake_cost}};
        } else if (bake_cost > gs) {
          // not enough grain but enough peat
          rlist = {{grain, -gs}, {flax, gs-bake_cost}, {peat, -bake_cost}, {food, 6*bake_cost}};
        } else if (bake_cost > ps) {
          // enough grain but not enough peat
          rlist = {{grain, -bake_cost}, {peat, -ps}, {wood, ps-bake_cost}, {food, 6*bake_cost}};
        } else {
          // enough grain and enough peat
          rlist = {{grain, -bake_cost}, {peat, -bake_cost}, {food, 6*bake_cost}};
        }
        mlist[move_command] = {rlist, baker};
      }
    }

    // wood trader
    // TODO: add building
    if (!p.board_status.woodtrader) {
      move_command = "wt";
      if (p.get(food) == 0 && p.get(grain) > 0) {
        rlist = {{grain, -1}, {wood, 4}};
      } else {
        rlist = {{food, -1}, {wood, 4}};
      }
      mlist[move_command] = {rlist, wintermaster};
    }

    // TODO:master

  }
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
    perform_action(p_temp, it.second.first, it.second.second);
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
// TODO: consider multithreading the first layer of evaluate_move
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
        perform_action(p1, it->second.first, it->second.second);
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
