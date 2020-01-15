#include <iostream>

#include "../include/algorithm.h"
#include "../include/board.h"
#include "../include/player.h"
#include "../include/rules.h"

using namespace std;

#ifdef RATE
#ifdef MANY
#define PLAY 100000
#else
#define PLAY 1000
#endif
#endif

int main()
{
#ifndef RATE

    Board board;
    Player red_player(RED);
    Player blue_player(BLUE);

    bool first_two_step = true;
    int round = 1;
    int index[2];

    while (1)
    {
        //////////// Red Player operations ////////////
        algorithm_A(board, red_player, index);
        board.place_orb(index[0], index[1], &red_player);

        if (rules_violation(red_player))
            return 0;

        board.print_current_board(index[0], index[1], round);
        round++;

        if (board.win_the_game(red_player) && !first_two_step)
        {
            cout << "Red Player won the game !!!" << endl;
            return 0;
        }

        //////////// Blue Player operations ////////////
        algorithm_E(board, blue_player, index);
        board.place_orb(index[0], index[1], &blue_player);

        if (rules_violation(blue_player))
            return 0;

        board.print_current_board(index[0], index[1], round);
        round++;

        if (board.win_the_game(blue_player) && !first_two_step)
        {
            cout << "Blue Player won the game !!!" << endl;
            return 0;
        }

        first_two_step = false;
    }

#else
    int red_win = 0, blue_win = 0;

    for (int i = 0; i < PLAY; ++i)
    {
        Board board;
        Player red_player(RED);
        Player blue_player(BLUE);

        bool first_two_step = true;
        int round = 1;
        int index[2];

        while (true)
        {
            algorithm_A(board, red_player, index);
            board.place_orb(index[0], index[1], &red_player);

            if (rules_violation(red_player))
                return 0;

            ++round;

            if (board.win_the_game(red_player) && !first_two_step)
            {
                ++red_win;
                break;
            }

            algorithm_B(board, blue_player, index);
            board.place_orb(index[0], index[1], &blue_player);

            if (rules_violation(blue_player))
                return 0;

            ++round;

            if (board.win_the_game(blue_player) && !first_two_step)
            {
                ++blue_win;
                break;
            }

            first_two_step = false;
        }
    }
    cout << red_win << ' ' << blue_win;
#endif
    return 0;
}