/* Simple Axe Game written while practicing udemy course */

#include "raylib.h"

int main()
{
    // game window details
    int width = 800;
    int height = 450;
    const char* title = "Cherno";

    // Creating game window with above details
    InitWindow(width, height, title);


    // circle details
    int circle_x = 200;
    int circle_y = 200;
    int circle_radius = 25;
    int l_circle_x, r_circle_x, u_circle_y, b_circle_y;


    // axe details
    int axe_x = 400;
    int axe_y = 0;
    int axe_length = 50;
    int direction = 10;
    int l_axe_x, r_axe_x, u_axe_y, b_axe_y;

    // collison with axe
    bool collison_with_axe;


    SetTargetFPS(60);
    while(!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(WHITE);

        // updating the edges
         
        l_circle_x = circle_x - circle_radius;
        r_circle_x = circle_x + circle_radius;
        u_circle_y = circle_y = circle_radius;
        b_circle_y = circle_y - circle_radius;

        l_axe_x = axe_x;
        r_axe_x = axe_x + axe_length;
        u_axe_y = axe_y;
        b_axe_y = axe_y + axe_length;

        // updating collison with axe
        collison_with_axe = (r_axe_x >= l_circle_x) && 
                            (l_axe_x <= r_circle_x) && 
                            (u_axe_y <= b_circle_y) && 
                            (b_axe_y>= u_circle_y);

        if(collison_with_axe)
        {
            DrawText("Game Over!", 400, 200, 20, RED);
        }
        else
        {
            // Game logic starts
            
            DrawCircle(circle_x, circle_y, circle_radius, BLUE);
            DrawRectangle(axe_x, axe_y, axe_length, axe_length, RED);

            axe_y += direction;

            if(axe_y > height || axe_y < 0)
            {
                direction = -direction;
            }
            
            if(IsKeyDown(KEY_D) && circle_x < width)
            {
                circle_x += 5;
            }
            if(IsKeyDown(KEY_A) && circle_x > 0)
            {
                circle_x -= 5;
            }
    
            // Game logic ends
        }

        EndDrawing();
    }
}