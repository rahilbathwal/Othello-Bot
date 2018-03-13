#include "player.hpp"
#include "board.hpp"
#include <bitset>
#include <limits.h>
#include <vector>
#include <algorithm>

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
 * Compute the board score using a heuristic which accounts for mobility,
 * frontier squares, position and piece count.
 */

/*int Player::CompleteHeuristic(Board *board)
 {
 double value = 0.0;
 int position = Position(board);
 int piece = Piece(board);
 double mobility = Mobility(board);
 double frontier = Frontier(board);
 value += position + mobility - frontier - piece;
 return value;
 }*/


int Player::CompleteHeuristic(Board *board)
{
    int position = 0;
    int my = 0;
    int opp = 0;
    double pieces = 0;
    double frontier = 0.0;
    int myCount = 0;
    int opCount = 0;
    int my_move = 0;
    int opp_move = 0;
    double mobility = 0.0;
    int weights[8][8] = {{1000,-100,5,5,5,5,-100,1000},{-100,-200,2,2,2,2,-200,-100},{5,2,6,6,6,6,2,5}, {5,2,6,7,7,6,2,5},{5,2,6,7,7,6,2,5},{5,2,6,6,6,6,2,5},
        {-100,-200,2,2,2,2,-200,-100}, {1000,-100,5,5,5,5,-100,1000}};
    int xChanges[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int yChanges[8] = {-1, 0, 1, -1, 1, -1, 0, 1};
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (board->real_get(my_side, i, j) == 1) {

                position += weights[i][j];
                my++;
            }
            else if (board->real_get(opp_side, i, j) == 1) {
                position -= weights[i][j];
                opp++;
            }
            for (int k = 0; k < 8; k++) {
                int xCoord = i + xChanges[k];
                int yCoord = j + yChanges[k];
                if (xCoord >= 0 && xCoord <= 7 && yCoord >= 0 && yCoord <= 7) {
                    if (board->real_get(my_side, xCoord, yCoord)) {
                        myCount++;
                        break;
                    }
                    else if (board->real_get(opp_side, xCoord, yCoord)) {
                        opCount++;
                        break;
                    }
                }
            }
            Move *move = new Move(i, j);
            if (board->checkMove(move, my_side))
            {
                my_move++;
            }
            if (board->checkMove(move, opp_side))
            {
                opp_move++;
            }
            delete move;
        }
    }
    pieces = 100 * (my - opp) / (my + opp);
    if (myCount + opCount != 0)
    {
        frontier = 100 * (opCount - myCount) / (myCount + opCount);
    }
    else
    {
        frontier = 0;
    }
    if (my_move + opp_move != 0)
    {
        mobility = (100.0 * (my_move - opp_move) / (my_move + opp_move));
    }
    else
    {
        mobility = 0;
    }
    if (my + opp - 4 < 30) {
        return 0.5 * position + 1 * mobility - 0.3 * frontier - 0.3 * pieces;
    }
    return 2 * position + 0.05 * mobility + 0.3 * frontier + pieces;
}

int Player::Piece(Board * board)
{
    double pieceDiff;
    int myCount, opCount;

    if (my_side == WHITE)
    {
        myCount = board->countWhite();
        opCount = board->countBlack();
    }
    else
    {
        myCount = board->countBlack();
        opCount = board->countWhite();
    }

    if (myCount + opCount != 0)
    {
        pieceDiff = 100 * (myCount - opCount) / (myCount + opCount);
    }
    else
    {
        pieceDiff = 0;
    }
    return pieceDiff;
}

double Player::Mobility(Board *board)
{
    int myCount = 0;
    int opCount = 0;
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            Move *move = new Move(i, j);
            if (board->checkMove(move, my_side))
            {
                myCount++;
            }
            if (board->checkMove(move, opp_side))
            {
                opCount++;
            }
            delete move;
        }
    }
    if (myCount + opCount != 0)
    {
        return (100.0 * (myCount - opCount) / (myCount + opCount));
    }
    else
    {
        return 0;
    }
}

double Player::Frontier(Board *board)
{
    int xChanges[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int yChanges[8] = {-1, 0, 1, -1, 1, -1, 0, 1};
    int myCount = 0;
    int opCount = 0;
    double frontier;
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            for (int k = 0; k < 8; k++)
            {
                int xCoord = i + xChanges[k];
                int yCoord = j + yChanges[k];
                if (xCoord >= 0 && xCoord <= 7 && yCoord >= 0 && yCoord <= 7)
                {
                    if (board->real_get(my_side, xCoord, yCoord))
                    {
                        myCount++;
                        break;
                    }
                    else if (board->real_get(opp_side, xCoord, yCoord))
                    {
                        opCount++;
                        break;
                    }
                }
            }
        }
    }
    if (myCount + opCount != 0)
    {
        frontier = 100 * (opCount - myCount) / (myCount + opCount);
    }
    else
    {
        frontier = 0;
    }
    return frontier;
}

/* Compute the board score for the algorithm using board positions */

int Player::Position(Board *board)
{
    int score = 0;
    int weights[8][8] = {{100,1,5,5,5,5,1,100},{1,0,2,2,2,2,0,1},{5,2,6,6,6,6,2,5}, {5,2,6,7,7,6,2,5},{5,2,6,7,7,6,2,5},{5,2,6,6,6,6,2,5},
        {1,0,2,2,2,2,0,1}, {100,1,5,5,5,5,1,100}};
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (board->real_get(my_side, i, j) == 1) {
                score += weights[i][j];
            }
            else if (board->real_get(opp_side, i, j) == 1) {
                score -= weights[i][j];
            }
        }
    }
    return score;
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
    int depth = 7;
    my_board->doMove(opponentsMove, opp_side);
    if (my_board->hasMoves(my_side)) {
        int alpha = INT_MIN;
        int beta = INT_MAX;
        int max_score = INT_MIN;
        Move *best_move = nullptr;
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                Move *move = new Move(i, j);
                if (my_board->checkMove(move, my_side)) {
                    Board *temp = my_board->copy();
                    temp->doMove(move, my_side);
                    int score = alphabeta(temp, opp_side, depth, alpha, beta);
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
    return nullptr;
}

int Player::alphabeta(Board *board, Side side, int depth, int alpha, int beta)
{
    if (depth == 0)
    {
        return CompleteHeuristic(board);
    }
    if (side == my_side)
    {
        if (board->hasMoves(my_side)) {
            std::vector<pair<int, Move*>> moves_list;
            int max_score = INT_MIN;
            for (int i = 0; i < 8; i++) {
                for (int j = 0; j < 8; j++) {
                    Move *move = new Move(i, j);
                    if (board->checkMove(move, side)) {
                        Board *temp = board->copy();
                        temp->doMove(move, side);
                        int score = CompleteHeuristic(temp);
                        moves_list.push_back(std::pair<int,Move*>(score,move));
                        delete temp;
                    }
                    else {
                        delete move;
                    }
                }
            }
            sort(moves_list.begin(), moves_list.end());
            while (moves_list.size() > 0) {
                Move *move = moves_list.back().second;
                moves_list.pop_back();
                Board *temp = board->copy();
                temp->doMove(move, side);
                int score = alphabeta(temp, opp_side, depth - 1, alpha, beta);
                max_score = max(max_score, score);
                alpha = max(max_score, alpha);
                if (alpha >= beta) {
                    delete move;
                    delete temp;
                    break;
                }
                delete temp;
                delete move;
            }
            while (moves_list.size() > 0) {
                Move *move = moves_list.back().second;
                delete move;
                moves_list.pop_back();
            }
            return max_score;
        }
        int score = alphabeta(board, opp_side, depth - 1, alpha, beta);
        return score;
    }
    if (board->hasMoves(opp_side)) {
        std::vector<pair<int, Move*>> moves_list;
        int min_score = INT_MAX;
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                Move *move = new Move(i, j);
                if (board->checkMove(move, side)) {
                    Board *temp = board->copy();
                    temp->doMove(move, side);
                    int score = CompleteHeuristic(temp);
                    moves_list.push_back(std::pair<int,Move*>(score,move));
                    delete temp;
                }
                else {
                    delete move;
                }
            }
        }
        sort(moves_list.begin(), moves_list.end());
        while (moves_list.size() > 0) {
            Move *move = moves_list.front().second;
            moves_list.erase(moves_list.begin());
            Board *temp = board->copy();
            temp->doMove(move, side);
            int score = alphabeta(temp, my_side, depth - 1, alpha, beta);
            min_score = min(min_score, score);
            beta = min(min_score, beta);
            if (beta <= alpha)
            {
                delete move;
                delete temp;
                break;
            }
            delete temp;
            delete move;
        }
        while (moves_list.size() > 0) {
            Move *move = moves_list.back().second;
            delete move;
            moves_list.pop_back();
        }
        return min_score;
    }
    int score = alphabeta(board, my_side, depth - 1, alpha, beta);
    return score;
}
