#include "player.hpp"
#include "board.hpp"
#include <bitset>
#include<limits.h>

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
    delete my_board;
}

/* Compute the board score using a basic heuristic which prioritises the
 * corner pieces.
 */

int Player::board_score(Board *board, Move *move)
{
    int weights[8][8] = {{3,-2,2,2,2,2,-2,3},{-2,-3,1,1,1,1,-3,-2},{2,1,1,1,1,1,1,2}, {2,1,1,1,1,1,1,2},{2,1,1,1,1,1,1,2},{2,1,1,1,1,1,1,2},{-2,-3,1,1,1,1,-3,-2}, {3,-2,2,2,2,2,-2,3}};
    int score = board->count(my_side) - board->count(opp_side);
    return score * weights[move->getX()][move->getY()];
}

/* Compute the board score for the minimax algorithm using a simple heuristic */

int Player::minimax_score(Board *board)
{
    return board->count(my_side) - board->count(opp_side);
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

Move *Player::doMove(Move *opponentsMove, int msLeft) {
    int depth = 5;
    int alpha = INT_MIN;
    int beta = INT_MAX;
    my_board->doMove(opponentsMove, opp_side);
    int max_score = INT_MIN;
    Move *best_move = nullptr;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            Move *move = new Move(i, j);
            if (my_board->checkMove(move, my_side)) {
                Board *temp = my_board->copy();
                temp->doMove(move, my_side);
                int score = opp_move(temp, move, opp_side, depth, alpha, beta);
                if (score >= max_score)
                {
                    delete best_move;
                    max_score = score;
                    best_move = move;
                }
                else {
                    delete move;
                }
                delete temp;
            }
            else {
                delete move;
            }
        }
    }
    my_board->doMove(best_move, my_side);
    return best_move;
}

int Player::opp_move(Board *board, Move *move, Side side, int depth, int alpha, int beta)
{
    if (depth == 0)
    {
        return board_score(board, move);
    }
    if (side == my_side)
    {
        int max_score = INT_MIN;
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                Move *move = new Move(i, j);
                if (board->checkMove(move, side))
                {
                    Board *temp = board->copy();
                    temp->doMove(move, side);
                    int score = opp_move(temp, move, opp_side, depth - 1, alpha, beta);
                    max_score = max(max_score, score);
                    alpha = max(max_score, alpha);
                    if (beta <= alpha)
                    {
                        delete move;
                        delete temp;
                        break;
                    }
                    delete temp;
                }
                delete move;
            }
        }
        return max_score;
    }
    else
    {
        int min_score = INT_MAX;
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                Move *move = new Move(i, j);
                if (board->checkMove(move, side)) {
                    Board *temp = board->copy();
                    temp->doMove(move, side);
                    int score = opp_move(temp, move, my_side, depth - 1, alpha, beta);
                    min_score = min(min_score, score);
                    beta = min(min_score, beta);
                    if (beta <= alpha)
                    {
                        delete move;
                        delete temp;
                        break;
                    }
                    delete temp;
                }
                delete move;
            }
        }
        return min_score;
    }
}
