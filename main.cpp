#include <iostream>

class Player {
  public:
    int wood = 4;
    int timber = 0;
    int clay = 4;
    int brick = 0;
    int peat = 3;

    int food = 5;
    int grain = 2;
    int flax = 3;
    int wool = 4;
    int hide = 1;

    int linen = 0;
    int leather = 0;
    int woolen = 0;

    int summerwear = 0;
    int leatherwear = 0;
    int winterwear = 0;

    int sheep = 0;
    int cow = 0;
    int horse = 0;

    int fishtrap = 2;
    int fleshingbeam = 3;
    int weavingloom = 2;
    int slaughteringtable = 2;
    int spade = 3;
    int shovel = 3;
    int potterywheel = 2;
    int oven = 1;
    int axe = 3;
    int workbench = 2;

    void print() {
      std::cout << "wood timber clay brick peat: "
                << this->wood << " " << this->timber << " "
                << this->clay << " " << this->brick << " "
                << this->peat << " "
                << std::endl;

      std::cout << "food grain flax wool hide:   "
                << this->food << " " << this->grain << " "
                << this->flax << " " << this->wool << " "
                << this->hide << " "
                << std::endl;

      std::cout << "linen leather woolen:        "
                << this->linen << " " << this->leather << " "
                << this->woolen << " "
                << std::endl;

      std::cout << "summer leather winter wear:  "
                << this->summerwear << " " << this->leatherwear << " "
                << this->winterwear << " "
                << std::endl;

      std::cout << "sheep cow horse:             "
                << this->sheep << " " << this->cow << " "
                << this->horse << " "
                << std::endl;

      std::cout << "upgrades:                    "
                << this->fishtrap << " " << this->fleshingbeam << " "
                << this->weavingloom << " " << this->slaughteringtable << " "
                << this->spade << " " << this->shovel << " "
                << this->potterywheel << " " << this->oven << " "
                << this->axe << " " << this->workbench << " "
                << std::endl;
    }
};

enum action_t: int {fisherman, grocer1, grocer2, grocer3, grocer4, grocer5,
                    woolenweaver};

void action(Player &p, action_t action) {
  switch(action) {
    case fisherman:
      p.sheep += 1;
      p.fishtrap += 1;
      p.food += p.fishtrap;
      break;
    case grocer1:
      p.timber += 1;
      p.grain += 1;
      p.leather += 1;
      break;
    case grocer2:
      p.brick += 1;
      p.grain += 1;
      p.leather += 1;
      break;
    case grocer3:
      p.sheep += 1;
      p.grain += 1;
      p.leather += 1;
      break;
    case grocer4:
      p.cow += 1;
      p.grain += 1;
      p.leather += 1;
      break;
    case grocer5:
      p.horse += 1;
      p.grain += 1;
      p.leather += 1;
      break;
    case woolenweaver:
      p.wool -= p.weavingloom;
      p.woolen += p.weavingloom;
  }
}

int main() {
  Player p1;
  action(p1, fisherman);
  action(p1, grocer1);
  action(p1, woolenweaver);
  p1.print();
  return 0;
}
