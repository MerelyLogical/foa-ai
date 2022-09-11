#ifndef PLAYER_H
#define PLAYER_H

#include <iomanip>
#include <unordered_map>

enum resource_t: int {wood, timber, clay, brick,
                      peat, uncutpeat,
                      food, grain, flax, wool, hide,
                      linen, leather, woolen,
                      summerwear, leatherwear, winterwear,
                      sheep, cattle, horse,
                      fishtrap, fleshingbeam, weavingloom, slaughteringtable,
                      spade, shovel, potterywheel, oven, axe, workbench};

enum season_t: int { spring, summer, autumn, winter };

class Resource {
  private:
    int max;
    int min;
    int amount;

  public:
    // returns how many used
    int use(int cost);
    // returns how many gained
    int add(int gain);
    int get();
    int get_space();
    void setmax(int newmax);
    Resource();
    Resource(int min, int max, int amount);
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
    int score_goods (int a);
    int score_animals ();
    int score_tools ();

  public:
    int turn_number;
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
    int next_turn();
    season_t get_season();
    std::string get_season_str();
    int use(resource_t name, int cost);
    int add(resource_t name, int gain);
    int get(resource_t name);
    int get_space(resource_t name);
    float score ();
    Player();
    Player(const Player &p);
    void print();
};

#endif
