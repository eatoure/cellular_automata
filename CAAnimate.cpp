#include "emp/web/Animate.hpp"
#include "emp/web/web.hpp"

emp::web::Document doc{"target"};

class CAAnimator : public emp::web::Animate
{

    // grid width and height
    const int num_h_boxes = 8;
    const int num_w_boxes = 26;
    const double RECT_SIDE = 25;
    const double width{num_w_boxes * RECT_SIDE};
    const double height{num_h_boxes * RECT_SIDE};

    /*
     This method checks the number of neighborhoods of a specific cell, determines whether or not they are alive, and returns the number 
    of said neighbors that are alive
     */
    int number_of_neighbors(int x, int y, std::vector<std::vector<int>> cells)
    {
        int count = 0; // hold number of alive neighbors
        for (int i = -1; i <= 1; i++)
        {
            for (int j = -1; j <= 1; j++)
            {
                if (!(i == 0 && j == 0))
                {
                    if (cells[(x + i + num_w_boxes) % num_w_boxes][(y + j + num_h_boxes) % num_h_boxes] == 1) // modula used for edge cases
                    {
                        count++;
                    }
                }
            }
        }
        return count;
    }
    
    /*
     Method determines whether a cell will be alive or dead in the next generation following the glider rule
    */
    bool is_alive(int x, int y, std::vector<std::vector<int>> cells)
    {
        int count = number_of_neighbors(x, y, cells); // uses method above to determine how many neighbors are alive, and stores in count
        if (cells[x][y] == 1) // if statements from here are simply following the glider rule as mentioned in the book
        {
            if (count == 2 || count == 3)
            {
                return true;
            }
        }
        else
        {
            if (count == 3)
            {
                return true;
            }
        }
        return false;
    }
    
    /*
     An update method that advances the generations given the information from the method "is_alive"
     */
    void update_cells(std::vector<std::vector<int>> &cells)
    {
        std::vector<std::vector<int>> new_cells(num_w_boxes, std::vector<int>(num_h_boxes, 0));
        for (int i = 0; i < num_w_boxes; i++)
        {
            for (int j = 0; j < num_h_boxes; j++)
            {
                if (is_alive(i, j, cells))
                {
                    new_cells[i][j] = 1;
                }
                else
                {
                    new_cells[i][j] = 0;
                }
            }
        }
        cells = new_cells; // making the 'cells' the now updated version
    }

    // some vectors to hold information about the CA
    std::vector<std::vector<int>> cells;

    // where we'll draw
    emp::web::Canvas canvas{width, height, "canvas"};

public:
    CAAnimator()
    {
        // shove canvas into the div
        // along with some control buttons
        doc << canvas;
        doc << GetToggleButton("Toggle");
        doc << GetStepButton("Step");
        cells.resize(num_w_boxes, std::vector<int>(num_h_boxes, 0));

        // Heavyweight Spaceship Starting position
        cells[3][3] = 1;
        cells[4][2] = 1;
        cells[5][2] = 1;
        cells[6][2] = 1;
        cells[7][2] = 1;
        cells[8][2] = 1;
        cells[9][2] = 1;
        cells[9][3] = 1;
        cells[9][4] = 1;
        cells[8][5] = 1;
        

        // this is for a line (can be used to test if the glider actually works; should draw vertical then horizontal lines if it works 
        // (simply looking at glider rules))
        // Make sure to comment out above turned on cells when using this tester
            //cells[1][1] = 1;
            //cells[2][1] = 1;
            //cells[3][1] = 1;
    }

    void DoFrame() override
    {
        canvas.Clear();

        for (int x = 0; x < num_w_boxes; x++)
        {
            for (int y = 0; y < num_h_boxes; y++)
            {

                if (cells[x][y] == 0)
                {
                    // Draw a rectangle on the canvas filled with white and outlined in black
                    canvas.Rect(x * RECT_SIDE, y * RECT_SIDE, RECT_SIDE, RECT_SIDE, "white", "black");
                }
                else
                {
                    // Draw a rectangle filled with black
                    canvas.Rect(x * RECT_SIDE, y * RECT_SIDE, RECT_SIDE, RECT_SIDE, "black", "black");
                }
            }
        }
        update_cells(cells); // update the cells with the current cells
    }
};

CAAnimator animator;

int main()
{
    // Have animator call DoFrame once to start
    animator.Step();
}