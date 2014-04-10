#include <iostream>
#include "Position.h"
#include <typeinfo>
#include "GameState.h"
//change back config type and extension
void drop_mine(std::shared_ptr<Unit> m, int x, int y)
{
    std::cout << dynamic_cast<Boat&>(*m).get_num_mines_left() << std::endl;
    if ((dynamic_cast<Boat&>(*m).drop_mine(Point(x,y))).result_type == CommandResult::ERROR)
    {
        std::cout << "error";
    }
    else
    {
        std::cout << "success";
    }
    std::cout << std::endl << dynamic_cast<Boat&>(*m).get_num_mines_left() << std::endl << std::endl;
}

void pick(std::shared_ptr<Unit> m)
{
    std::cout << dynamic_cast<Boat&>(*m).get_num_mines_left() << std::endl;
    if ((dynamic_cast<Boat&>(*m).pick_up_mine(Point(6, 6))).result_type == CommandResult::ERROR)
    {
        std::cout << "error";
    }
    else
    {
        std::cout << "success";
    }
    std::cout << std::endl << dynamic_cast<Boat&>(*m).get_num_mines_left() << std::endl << std::endl;
}

int main()
{
    GameState state;
    auto m = Boat::s_construct_cruiser(Position(Point(5, 5), Direction::EAST), RgbColor(255, 0, 0), &state, &state);
    auto mlayer = Boat::s_construct_mine_layer(Position(Point(7, 2), Direction::EAST), RgbColor(255, 0, 0), &state, &state);
    //drop_mine(mlayer, 7, 3);
    CommandResult result = dynamic_cast<Boat&>(*m).rotate(LEFT, 1);
    std::cout << (result.result_type == CommandResult::SUCCESS);
    /*drop_mine(mlayer, 6, 6);
    std::cout << "Bow:" << dynamic_cast<Boat&>(*m).get_bow().get_x() << std::endl;
    std::cout << std::endl;
    CommandResult result = dynamic_cast<Boat&>(*m).shift(RIGHT);
    auto test = result.result_points;
    for (auto it = test.begin(); it != test.end(); it++)
    {
        std::cout << "Collided at: " << "(" << (*it).get_x() << ", " << (*it).get_y() << ")" << std::endl;
    }
    std::cout << (result.result_type == CommandResult::SUCCESS);*/
    //std::cout<< state.get_mines().size();
    //std::cout << GameState::unserialize(state.serialize()).serialize().to_string().size();
    while (true);
}
