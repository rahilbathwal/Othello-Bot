#include "player.hpp"
#include "board.hpp"
#include <bitset>

/*
 * Constructor for the player; initialize everything here. The side your AI is
 * on (BLACK or WHITE) is passed in as "side". The constructor must finish
 * within 30 seconds.
 */
Player::Player(Side side) {
    // Will be set to true in test_minimax.cpp.
    testingMinimax = false;

    if (side == WHITE)
    {
        my_side = side;
        opp_side = BLACK;
    }
    else
    {
        my_side = side;
        opp_side = WHITE;
    }
    my_board = new Board();
}

/*
 * Destructor for the player.
 */
Player::~Player() {
}

/*
 * Compute the next move given the opponent's last move. Your AI is
 * expected to keep track of the board on its own. If this is the first move,
 * or if the opponent passed on the last move, then opponentsMove will be
 * nullptr.
 *
 * msLeft represents the time your AI has left for the total game, in
 * milliseconds. doMove() must take no longer than msLeft, or your AI will
 * be disqualified! An msLeft value of -1 indicates no time limit.
 *
 * The move returned must be legal; if there are no valid moves for your side,
 * return nullptr.
 */

int Player::board_score(Board *board)
{
    int weights[8][8] = {{7,2,5,4,4,5,2,7},{2,1,3,3,3,3,1,2},{5,3,6,5,5,6,3,5},
    {4,3,5,6,6,5,3,4},{4,3,5,6,6,5,3,4},{5,3,6,5,5,6,3,5},{2,1,3,3,3,3,1,2},
    {7,2,5,4,4,5,2,7}};
    int score = 0;
    for (int i = 0; i < 8; i++)
    {
        for (int j; j < 8; j++)
        {
            if (board->real_get(my_side, i, j))
            {
                score += weights[i][j];
            }
            if (board->real_get(opp_side, i, j))
            {
                score -= weights[i][j];
            }
        }
    }
    return score;
}

Move *Player::doMove(Move *opponentsMove, int msLeft) {
    my_board->doMove(opponentsMove, opp_side);
    Move *move = nullptr;
    int max_score = 0;
    if (my_board->hasMoves(my_side))
    {
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                move = new Move(i, j);
                if (my_board->checkMove(move, my_side)) {
                }
            }
        }
    }
    my_board->doMove(move, my_side); 
    return move;
}
