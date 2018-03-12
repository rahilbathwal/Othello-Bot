#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <iostream>
#include "common.hpp"
#include "board.hpp"
using namespace std;

class Player {

public:
    Player(Side side);
    ~Player();
    Side my_side;
    Side opp_side;
    Board *my_board;
    Move *doMove(Move *opponentsMove, int msLeft);
    int board_score(Board *board, Move *move);
    int minimax_score(Board *board);
    int opp_move(Board *board, Move *move, Side side, int depth, int alpha, int beta);
    int CompleteHeuristic(Board *board, Move *move);
    int Piece(Board * board, Move *move);
    double Mobility(Board *board, Move *move);
    double Frontier(Board *board, Move *move);

    // Flag to tell if the player is running within the test_minimax context

    bool testingMinimax;
    int counter;
};

#endif
