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

int Player::board_score(Board *board, Move *move)
{
    int weights[8][8] = {{1,-1,0,0,0,0,-1,1},{-1,-1,0,0,0,0,-1,-1},{0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{-1,-1,0,0,0,0,-1,-1},
    {1,-1,0,0,0,0,-1,1}};
    int score = board->count(my_side) - board->count(opp_side);
    return score + (abs(score) / 2) * weights[move->getX()][move->getY()];
}

int Player::minimax_score(Board *board)
{
    return board->count(my_side) - board->count(opp_side);
}

int Player::opp_move(Board *board, Side side)
{
    int min_score = INT_MAX;
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            Move *move = new Move(i, j);
            if (board->checkMove(move, side))
            {
                Board *temp = board->copy();
                temp->doMove(move, side);
                int score = minimax_score(temp);
                if(score < min_score)
                {
                    min_score = score;
                }
                delete temp;
            }
        }
    }
    return min_score;
}

Move *Player::doMove(Move *opponentsMove, int msLeft) {
    if (testingMinimax) {
        my_board->doMove(opponentsMove, opp_side);
        Move *best_move = nullptr;
        int max_score = INT_MIN;
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                Move *move = new Move(i, j);
                if (my_board->checkMove(move, my_side)) {
                    Board *temp = my_board->copy();
                    temp->doMove(move, my_side);
                    int score = opp_move(temp, opp_side);
                    if (score > max_score)
                    {
                        max_score = score;
                        best_move = move;
                    }
                    delete temp;
                }
            }
        }
        my_board->doMove(best_move, my_side);
        return best_move;
    }
    else {
        my_board->doMove(opponentsMove, opp_side);
        Move *best_move = nullptr;
        int max_score = INT_MIN;
        if (my_board->hasMoves(my_side))
        {
            for (int i = 0; i < 8; i++) {
                for (int j = 0; j < 8; j++) {
                    Move *move = new Move(i, j);
                    if (my_board->checkMove(move, my_side)) {
                        Board *temp = my_board->copy();
                        temp->doMove(move, my_side);
                        int score = board_score(temp, move);
                        if (score > max_score)
                        {
                            max_score = score;
                            best_move = move;
                        }
                        delete temp;
                    }
                }
            }
        }
        my_board->doMove(best_move, my_side);
        return best_move;
    }
}
