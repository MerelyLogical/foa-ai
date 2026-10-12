# TODO

Goal: turn this into something usable — a solo Fields of Arle advisor that can
suggest a good half year (4 worker placements) in a few seconds, and play a
whole simplified game end to end.

A full solo game is 9 half years x 4 workers = 36 placements. With ~8-10 legal
moves per placement, exhaustive search will never reach the end of the game, so
the plan is: fix bugs, make each node cheap, stop searching exhaustively, and
cut the rules that cost the most to model.

Rules reference: https://www.yucata.de/en/Rules/FieldsOfArle

## 0. Bugs (do first)

- [x] `multithread_eval` passes `std::ref(p_temp)` to threads, but `p_temp` is
      a loop local that dies before the threads are joined. Pass by value.
- [x] wood trader is registered as `wintermaster`, so it never gets blocked
- [x] wood trader is free when there is no food and no grain (`use` clamps
      instead of rejecting) — the move should not be offered
- [x] `player.cpp` uses `std::sort` without `#include <algorithm>`
- [ ] check against the physical game: winter grocer gives wood + brick in the
      code, the online rules say wood + clay
- [ ] check against the physical game: leather/winter wear score 2.5 VP in the
      code, the online rules say 2 VP for every wear

## 1. Finish the core loop (small, makes results meaningful)

- [ ] end the game after turn 36 (half year 9), 5 summers and 4 winters
- [ ] November inventorying: pay 3 food + 2 peat (grain, then animals, stand in
      for food; wood/timber for peat; -2 VP per item still missing)
- [ ] May inventorying: pay 3 food, same substitutions
- [ ] shearing in May: 1/2/3 wool for 1/4/6 sheep
- [ ] milking in November: 1/2/3 food for 2/5/8 sheep and for 1/3/5 cattle
- [ ] allow passing (placing a worker is not mandatory)
- [ ] remaining resource-only actions: peat boatman (3 peat), butcher,
      colonist (horse only for now), dike builder (animal only for now)
- [ ] master (tool upgrades) — costs are printed on the board and are not in
      the online rules, copy them from the physical game

Without sustenance the AI is solving a much easier game than the real one, so
this section matters more than adding more actions.

## 2. Make each node cheap

- [ ] replace `Player` with a flat POD: `std::array<int8_t, 30>` for amounts,
      a `uint32_t` bitmask for taken action spaces, a turn counter. Min/max
      become a static const table. This removes `r_lut` (a 30-entry
      `unordered_map` rebuilt on every copy) and the hand-written copy
      constructor, and makes the state trivially copyable and hashable.
- [ ] replace `Board`'s 22 bools and the `mark_action` switch with the bitmask
- [ ] moves: a small struct with a fixed array of (resource, delta) pairs and
      an action id; generate into a stack-allocated array instead of
      `unordered_map<string, ...>`. Keep command strings in a static table for
      the REPL only.
- [ ] store the line as an array of move ids instead of concatenating strings
      and counting `>` characters
- [ ] make/unmake moves instead of copying the state per node (optional once
      the state is a few dozen bytes)
- [ ] `-O3 -march=native`, add a `perf` target, and measure nodes per second
      before and after each change

## 3. Search smarter

- [ ] transposition table keyed on the state hash. Within a half year, most
      orderings of the same 4 actions reach the same state, so this alone
      removes up to a 24x redundancy per half year.
- [ ] search one half year at a time as the unit (choose 4 actions), not a
      fixed ply depth that stops mid-season
- [ ] heuristic evaluation for non-terminal states. Current score undervalues
      tools, animals that will breed, and food needed for upkeep. Start with
      score + weighted tool levels - upcoming sustenance shortfall.
- [ ] beam search across half years: keep the best N states after each half
      year, expand each exhaustively for the next one. This is what makes a
      full 36-move game reachable.
- [ ] iterative deepening with a time budget instead of asking for a depth
- [ ] split work across threads at ply 2 with a pool, not one thread per first
      move (uneven subtrees leave cores idle)
- [ ] later, if beam search plateaus: single-player MCTS, or branch and bound
      with an optimistic upper bound on remaining score

## 4. Descope

Keep:
- solo only. No opponent, no minimax, and solo has no off-season special
  action, which removes a whole class of moves.
- goods, tools, animals, sustenance, simple scoring

Abstract instead of modelling:
- [ ] home board as counters, not a grid: free land spaces, moors (hydrated /
      dehydrated / removed), dikes built, fields, forests, stalls, stables.
      Animal capacity and breeding become functions of those counters.
- [ ] buildings: a fixed set (the 4 beginner green houses plus the 3 large
      buildings and 6 major craft buildings), no random setup

Cut, at least for now:
- travel and vehicle loading (the largest rules subsystem; costs a flat -3 VP
  for having no large vehicle, which the scoring can just apply)
- laborer imitation, warden / tile upgrades, parks
- inns and minor craft buildings (set-dependent, mostly one-off effects)
- 2 player mode

## 5. Usability

- [ ] `ai` prints the best line for the current half year and its expected
      end-of-game score, not every tied line
- [ ] command to set the state by hand, so it can advise on a real game in
      progress
- [ ] save/load state, undo
- [ ] a few regression tests: scoring examples from the rulebook (e.g. 6 sheep,
      5 horses, 4 cattle = 13 VP) and a fixed-seed search result
