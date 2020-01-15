#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "../include/algorithm.h"

using namespace std;
//////declarations of classes and functions///////
struct Node
{
    int row;
    int col;
    int affect_increment;
};
class nodecontainer
{
public:
    int num_element;
    Node node[ROW * COL];
    nodecontainer() { num_element = 0; };
    Node get(int i);
    void push(int r, int c, int aff);
    bool isempty();
    int getsize();
};

class DuplicateBoard
{
public:
    DuplicateBoard(Board a); //duplicate a DuplicateBoard from a board
    Cell cells[ROW][COL];
    void cell_reaction_marker();            // After the explosion, mark all the cell that  will explode in next iteration
    bool cell_is_full(Cell *cell);          // Check wether the cell is full of orbs and set the explosion variable to be true
    void add_orb(int i, int j, char color); // Add orb into the cell (i, j)
    void cell_reset(int i, int j);          // Reset the cell to the initial state (color = 'w', orb_num = 0)
    void cell_explode(int i, int j);        // The explosion of cell (i, j). It will add the orb into neighbor cells
    bool cell_chain_reaction(int color);    //** If there is a explosion, check wether the chain reaction is win or not

    int get_orbs_num(int i, int j);
    int get_capacity(int i, int j);
    char get_cell_color(int i, int j);
    bool place_orb(int i, int j, Player *player);      // Use this function to place a orb into a cell
    void print_current_board(int i, int j, int round); // Print out the current state of the hole board
    bool win_the_game(int color);                      //check wether the player wins the game after his/her placemnet operation
};

bool LegalStep(int row, int col, int color, DuplicateBoard d_board);                  //check if it is a legal step
bool virtualCR_placeOrb_WIN(int row, int col, int color, DuplicateBoard d_board);     //check whether Win after placement on (r,c) in d_board
bool virtualCR_placeOrb_NOTLose(int row, int col, int color, DuplicateBoard d_board); //check whether NotLose after placement on (r,c) in d_board
int SwitchColor(int color);
int count_nodes(int r, int c, int color, DuplicateBoard d_board); //count the difference num of specific color orbs after placement on (r,c) in d_board

//////definitions of nodecontainer///////
Node nodecontainer::get(int i)
{
    return node[i];
}
void nodecontainer::push(int r, int c, int aff)
{
    node[num_element].row = r;
    node[num_element].col = c;
    node[num_element].affect_increment = aff;
    num_element++;
}
bool nodecontainer::isempty()
{
    if (num_element == 0)
        return true;
    else
        return false;
}
int nodecontainer::getsize()
{
    return num_element;
}
//////definitions of DuplicateBoard///////
DuplicateBoard::DuplicateBoard(Board a)
{
    for (int r = 0; r < ROW; ++r)
    {
        for (int c = 0; c < COL; ++c)
        {
            cells[r][c].set_orbs_num(a.get_orbs_num(r, c));
            cells[r][c].set_capacity(a.get_capacity(r, c));
            cells[r][c].set_color(a.get_cell_color(r, c));
            cells[r][c].set_explode(0);
        }
    }
}
bool DuplicateBoard::cell_is_full(Cell *cell)
{
    if (cell->get_orbs_num() >= cell->get_capacity())
    {
        cell->set_explode(true);
        return true;
    }
    else
        return false;
}
void DuplicateBoard::add_orb(int i, int j, char color)
{
    int orb_num = cells[i][j].get_orbs_num();
    orb_num++;
    cells[i][j].set_orbs_num(orb_num);
    cells[i][j].set_color(color);
}
void DuplicateBoard::cell_reset(int i, int j)
{
    cells[i][j].set_orbs_num(0); //maybe.....cells[i][j].set_orbs_num(cells[i][j].get_orbs_num()-cells[i][j]get_capacity());
    cells[i][j].set_explode(false);
    cells[i][j].set_color('w'); //if(cells[i][j].get_orbs_num()==0)cells[i][j].set_color('w');
}
void DuplicateBoard::cell_explode(int i, int j)
{
    int orb_num;
    char color = cells[i][j].get_color();
    cell_reset(i, j);

    if (i + 1 < ROW)
    {
        add_orb(i + 1, j, color);
    }
    if (j + 1 < COL)
    {
        add_orb(i, j + 1, color);
    }
    if (i - 1 >= 0)
    {
        add_orb(i - 1, j, color);
    }
    if (j - 1 >= 0)
    {
        add_orb(i, j - 1, color);
    }
}
void DuplicateBoard::cell_reaction_marker()
{
    // Mark the next cell whose number of orbs is equal to the capacity
    for (int i = 0; i < ROW; i++)
    {
        for (int j = 0; j < COL; j++)
        {
            cell_is_full(&cells[i][j]);
        }
    }
}
bool DuplicateBoard::cell_chain_reaction(int color)
{
    bool chain_reac = true;
    while (chain_reac)
    {
        chain_reac = false;
        for (int i = 0; i < ROW; i++)
        {
            for (int j = 0; j < COL; j++)
            {
                if (cells[i][j].get_explode())
                { //****is there a bug??!****//
                    cell_explode(i, j);
                    chain_reac = true;
                }
            }
        }
        if (win_the_game(color))
        {
            return true;
        }
        cell_reaction_marker();
    }
    return false;
}
bool DuplicateBoard::win_the_game(int color)
{
    char player_color = color; //**********
    bool win = true;
    for (int i = 0; i < ROW; i++)
    {
        for (int j = 0; j < COL; j++)
        {
            if (cells[i][j].get_color() == player_color || cells[i][j].get_color() == 'w')
                continue;
            else
            {
                win = false;
                break;
            }
        }
        if (!win)
            break;
    }
    return win;
}
int DuplicateBoard::get_orbs_num(int i, int j)
{
    return cells[i][j].get_orbs_num();
}
int DuplicateBoard::get_capacity(int i, int j)
{
    return cells[i][j].get_capacity();
}
char DuplicateBoard::get_cell_color(int i, int j)
{
    return cells[i][j].get_color();
}

//////////// My Algorithm ////////////

void algorithm_F(Board board, Player player, int index[])
{
    srand(time(NULL));
    int color = player.get_color();
    DuplicateBoard d_board(board);

    ////// the condition that would win the game in one step //////
    for (int r = 0; r < ROW; ++r)
    {
        for (int c = 0; c < COL; ++c)
        {
            if (LegalStep(r, c, color, d_board))
            {
                if (virtualCR_placeOrb_WIN(r, c, color, d_board))
                {
                    index[0] = r;
                    index[1] = c;
                    return;
                }
            }
        }
    }

    ////// the condition that would not lose the game in opponent's coming one step //////
    nodecontainer container;
    for (int r = 0; r < ROW; ++r)
    {
        for (int c = 0; c < COL; ++c)
        {
            if (LegalStep(r, c, color, d_board) && virtualCR_placeOrb_NOTLose(r, c, color, d_board))
            {
                container.push(r, c, count_nodes(r, c, color, d_board));
            }
        }
    }

    if (container.isempty()) ////must lose condition////
    {
        for (int r = 0; r < ROW; ++r)
        {
            for (int c = 0; c < COL; ++c)
            {
                if (LegalStep(r, c, color, d_board))
                {
                    index[0] = r;
                    index[1] = c;
                }
            }
        }
    }
    else ////would not lose the game////
    {
        int size = container.getsize();
        int idx, tmp = 0;
        for (int i = 0; i < size; i++)
        {
            if (container.get(i).affect_increment > tmp)
            {
                tmp = container.get(i).affect_increment;
                idx = i;
            }
        }
        Node orb = container.get(idx);
        index[0] = orb.row;
        index[1] = orb.col;
    }
}
//////////// End of My Algorithm ////////////

//////definitions of my functions///////
bool LegalStep(int row, int col, int color, DuplicateBoard d_board)
{
    bool legal = true;
    //Check color
    if (d_board.get_cell_color(row, col) == SwitchColor(color) || (row > 4 || col > 5))
        legal = false;
    return legal;
}
bool virtualCR_placeOrb_WIN(int row, int col, int color, DuplicateBoard d_board)
{
    int temp = d_board.get_orbs_num(row, col);
    temp += 1;
    d_board.cells[row][col].set_orbs_num(temp);
    d_board.cells[row][col].set_color(color);
    if (d_board.cell_is_full(&d_board.cells[row][col]))
    {
        d_board.cell_explode(row, col);
        d_board.cell_reaction_marker();
        return (d_board.cell_chain_reaction(color));
    }
    else
        return false;
}
bool virtualCR_placeOrb_NOTLose(int row, int col, int color, DuplicateBoard d_board)
{
    //my step
    int temp = d_board.get_orbs_num(row, col);
    temp += 1;
    d_board.cells[row][col].set_orbs_num(temp);
    d_board.cells[row][col].set_color(color);
    if (d_board.cell_is_full(&d_board.cells[row][col]))
    {
        d_board.cell_explode(row, col);
        d_board.cell_reaction_marker();
        d_board.cell_chain_reaction(color);
    }
    //opponent's step
    int op_color = SwitchColor(color);
    for (int r = 0; r < ROW; ++r)
    {
        for (int c = 0; c < COL; ++c)
        {
            if (LegalStep(r, c, op_color, d_board) && virtualCR_placeOrb_WIN(r, c, op_color, d_board))
                return false; //Lose
        }
    }
    return true; //NotLose
}
int SwitchColor(int color)
{
    if (color == 'r')
        return 'b';
    else if (color == 'b')
        return 'r';
    else
    {
        std::cout << "color switch error";
        system("pause");
        return 'w';
    }
}

int count_nodes(int r, int c, int color, DuplicateBoard d_board)
{
    //count before placement
    int before_num = 0, after_num = 0;
    for (int r = 0; r < ROW; ++r)
    {
        for (int c = 0; c < COL; ++c)
        {
            if (d_board.get_cell_color(r, c) == color)
            {
                before_num += d_board.get_orbs_num(r, c);
            }
        }
    }
    //chain reaction
    int temp = d_board.get_orbs_num(r, c);
    temp += 1;
    d_board.cells[r][c].set_orbs_num(temp);
    d_board.cells[r][c].set_color(color);
    if (d_board.cell_is_full(&d_board.cells[r][c]))
    {
        d_board.cell_explode(r, c);
        d_board.cell_reaction_marker();
        d_board.cell_chain_reaction(color);
    }
    //count after placement
    for (int r = 0; r < ROW; ++r)
    {
        for (int c = 0; c < COL; ++c)
        {
            if (d_board.get_cell_color(r, c) == color)
            {
                after_num += d_board.get_orbs_num(r, c);
            }
        }
    }
    return after_num - before_num;
}