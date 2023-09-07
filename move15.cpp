#include <algorithm> // std::shuffle
#include <array>
#include <cassert>
#include <ctime> // std::time
#include <iostream>
#include <random> // std::mt19937
#include <assert.h>
#include "Random.h"
// Increase amount of new lines if your board isn't
// at the very bottom of the console
constexpr int g_consoleLines{25};
constexpr int g_lines{4};

class Tile
{
private:
    int num;

public:
    Tile(int n = 0) : num{n}
    {
    }
    bool isEmpty()
    {
        return (num == 0);
    }
    int getNum()
    {
        return num;
    }
    friend std::ostream &operator<<(std::ostream &out, Tile tile)
    {
        if (tile.num > 9)
        {
            out << " " << tile.num << " ";
        }
        else if (tile.num > 0)
        {
            out << "  " << tile.num << " ";
        }
        else
        {
            out << "    ";
        }

        return out;
    }
};
class Direction
{
public:
    enum dir
    {
        up,
        down,
        left,
        right,
        max_dirs
    };

private:
    dir m_d = dir::up;

public:
    Direction(dir type) : m_d{type} {}
    Direction operator-() const
    {
        switch (m_d)
        {
        case (dir::up):
            return Direction(down);
        case (dir::down):
            return Direction(up);
        case (dir::right):
            return Direction(left);
        case (dir::left):
            return Direction(right);
        default:
            return Direction(up);
        }
    }
    friend std::ostream &operator<<(std::ostream &os, Direction direction)
    {
        switch (direction.m_d)
        {
        case (dir::up):
            os << "up\n";
            break;
        case (dir::down):
            os << "down\n";
            break;
        case (dir::right):
            os << "right\n";
            break;
        case (dir::left):
            os << "left\n";
            break;
        default:
            os << "not supposed to happen\n";
        }
        return os;
    }
    static Direction random_move()
    {
        return Direction(static_cast<dir>(Random::get(0, dir::max_dirs)));
    }
    Direction::dir getType()
    {
        return m_d;
    }
};
namespace UserInput
{

    Direction translateCommand(char in)
    {
        switch (in)
        {
        case 'w':
            return Direction(Direction::dir::up);
        case 'a':
            return Direction(Direction::dir::left);
        case 's':
            return Direction(Direction::dir::down);
        case 'd':
            return Direction(Direction::dir::right);
        default:;
        }
        return Direction(Direction::dir::up);
    }
    char getCommandFromUser()
    {
        char in;
        std::cin >> in;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        while (in != 'w' && in != 'a' && in != 's' && in != 'd' && in != 'q')
        {
            std::cin.clear();
            std::cin >> in;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        if (in != 'q')
        {
            std::cout << "Valid command: " << translateCommand(in) << "\n";
        }
        return in;
    }

}
class Point
{
private:
    int m_x;
    int m_y;

public:
    Point(int x = 0, int y = 0) : m_x{x}, m_y{y} {}
    Point getAdjacentPoint(Direction direction)
    {
        Point rtn = Point(m_x, m_y);
        switch (direction.getType())
        {
        case (Direction::dir::up):
            if (m_x != 0)
            {
                rtn.m_x -= 1;
            }
            break;
        case (Direction::dir::down):
            if (m_x != g_lines - 1)
            {
                rtn.m_x += 1;
            }
            break;
        case (Direction::dir::right):
            if (m_y != g_lines - 1)
            {
                rtn.m_y += 1;
            }
            break;
        case (Direction::dir::left):
            if (m_y != 0)
            {
                rtn.m_y -= 1;
            }
            break;
        default:
            std::cout << "not suppposed to happen\n";
        }
        return rtn;
    }
    bool operator==(const Point a)
    {
        return (a.m_x == m_x && a.m_y == m_y);
    }
    bool operator!=(const Point a)
    {
        return !(a.m_x == m_x && a.m_y == m_y);
    }
    int getX() const
    {
        return m_x;
    }
    int getY() const
    {
        return m_y;
    }
};
class Board
{
private:
    std::array<std::array<Tile, g_lines>, g_lines> m_board;
    Point empty;
    Tile &at(const Point point)
    {
        return m_board[point.getX()][point.getY()];
    }
    void swapTiles(const Point a, const Point b)
    {
        int temp = at(a).getNum();
        at(a) = Tile(at(b).getNum());
        at(b) = Tile(temp);
    }
    void initializeBoard()
    {
        static int rand = Random::get(1, 15);
        for (int i = 0; i < rand; ++i)
        {
            moveTile(Direction::random_move());
        }
    }

public:
    Board()
    {
        for (int r = 0; r < g_lines; ++r)
        {
            for (int c = 0; c < g_lines; ++c)
            {
                if (r * g_lines + c + 1 == 16)
                {
                    m_board[r][c] = Tile(0);
                }
                else
                {
                    m_board[r][c] = Tile(r * g_lines + c + 1);
                }
            }
        }
        empty = Point(3, 3);
        initializeBoard();
    }
    friend std::ostream &operator<<(std::ostream &os, Board board)
    {
        for (int i = 0; i < g_consoleLines; ++i)
        {
            os << '\n';
        }
        for (int r = 0; r < g_lines; ++r)
        {
            for (int c = 0; c < g_lines; ++c)
            {
                os << board.m_board[r][c];
            }
            os << '\n';
        }
        return os;
    }

    void moveTile(const Direction direction)
    {
        Point adj = empty.getAdjacentPoint(-direction);
        swapTiles(empty, adj);
        empty = adj;
    }
    bool solved()
    {
        for (int r = 0; r < g_lines; ++r)
        {
            for (int c = 0; c < g_lines; ++c)
            {
                if (m_board[r][c].getNum() != (r * g_lines + c + 1))
                {
                    if (m_board[r][c].getNum() == 0 && (r * g_lines + c + 1) == 16)
                    {
                        continue;
                    }
                    return false;
                }
            }
        }
        return true;
    }
};

int main()
{
    Board board = Board();
    std::cout << board << "\n";
    while (true)
    {
        char input = UserInput::getCommandFromUser();
        if (input == 'q')
        {
            std::cout << "\n\nBye!\n\n";
            break;
        }
        board.moveTile(UserInput::translateCommand(input));
        std::cout << board << "\n";
        if (board.solved())
        {
            std::cout << "\n\nYou won!\n\n";
            break;
        }
    }
    return 0;
}