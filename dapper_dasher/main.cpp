# include "raylib.h"

struct anim_data
{
    Rectangle rec;
    Vector2 pos;
    int frame;
    float update_time;
    float running_time;
};


bool is_on_ground(anim_data scarfy, int window_height)
{
    return (scarfy.pos.y + scarfy.rec.height >= window_height);
}


anim_data update_anim_data(anim_data data, float delta_time, int max_frame)
{
    data.running_time += delta_time;

    if(data.running_time >= data.update_time)
    {
        data.running_time = 0.0;
        // updating animation frame
        data.rec.x = data.frame * data.rec.width;
        data.frame++;
        if(data.frame > max_frame)
            data.frame = 0;
    }
    return data;
}


int main()
{
    int window_dimensions[2] = {512, 380};
    const char* title = "dapper_dasher";

    // creating window with above details
    InitWindow(window_dimensions[0], window_dimensions[1], title);

    // acceleration due to gravity
    const int g = 1000;         //   (pixels/sec)/sec

    


    // nebula details
    Texture2D nebula = LoadTexture("textures/12_nebula_spritesheet.png");
    const int size_of_nebulae = 10;
    anim_data nebulae[size_of_nebulae];

    for(int i=0; i<size_of_nebulae; i++)
    {
        nebulae[i].rec.x = 0.0; 
        nebulae[i].rec.y = 0.0;
        nebulae[i].rec.width = nebula.width/8;
        nebulae[i].rec.height = nebula.height/8;
        nebulae[i].pos.y = window_dimensions[1] - nebulae[i].rec.height;
        nebulae[i].pos.x = window_dimensions[0] + i * 300;
        nebulae[i].frame = 0;
        nebulae[i].running_time = 0.0;
        nebulae[i].update_time = 1.0/12.0;
    }

    float finish_line = nebulae[size_of_nebulae-1].pos.x;

    int neb_vel = -200;      //  pixels/sec


    // scarfy details
    Texture2D scarfy = LoadTexture("textures/scarfy.png");
    anim_data scarfy_data;
    scarfy_data.rec.x = 0;
    scarfy_data.rec.y = 0;
    scarfy_data.rec.width = scarfy.width/6;
    scarfy_data.rec.height = scarfy.height;

    scarfy_data.pos  = {window_dimensions[0]/2 - scarfy_data.rec.width/2,  window_dimensions[1] - scarfy_data.rec.height};
    scarfy_data.frame = 0;
    scarfy_data.update_time = 1.0/12.0;
    scarfy_data.running_time = 0.0;

    int velocity = 0;     // pixels/sec
    bool is_in_air = false;
    const int jump_velocity = -600;   //  pixels/sec
    

    Texture2D foreground = LoadTexture("textures/foreground.png");
    float fgx = 0.0;
    Texture2D midground = LoadTexture("textures/back-buildings.png");
    float mgx = 0.0;
    Texture2D background = LoadTexture("textures/far-buildings.png");
    float bgx = 0.0;
    
    bool collision = false;
 
    SetTargetFPS(60);       // frames/second
    while(!WindowShouldClose())
    {
        // delta time
        const float dT = GetFrameTime();

        BeginDrawing();
        ClearBackground(WHITE);


        // Scroll background
        bgx -= 20 * dT;
        if (bgx <= -background.width*2)
        {
            bgx = 0.0;
        }
        // Scroll the midground
        mgx -= 40 * dT;
        if (mgx <= -midground.width*2)
        {
            mgx = 0.0;
        }
        // Scroll the foreground
        fgx -= 80 * dT;
        if (fgx <= -foreground.width*2)
        {
            fgx = 0.0;
        }

        // draw the background
        Vector2 bg1Pos{bgx, 0.0};
        DrawTextureEx(background, bg1Pos, 0.0, 2.0, WHITE);
        Vector2 bg2Pos{bgx + background.width*2, 0.0};
        DrawTextureEx(background, bg2Pos, 0.0, 2.0, WHITE);

        // draw the midground
        Vector2 mg1Pos{mgx, 0.0};
        DrawTextureEx(midground, mg1Pos, 0.0, 2.0, WHITE);
        Vector2 mg2Pos{mgx + midground.width*2, 0.0};
        DrawTextureEx(midground, mg2Pos, 0.0, 2.0, WHITE);

        // draw the foreground
        Vector2 fg1Pos{fgx, 0.0};
        DrawTextureEx(foreground, fg1Pos, 0.0, 2.0, WHITE);
        Vector2 fg2Pos{fgx + foreground.width*2, 0.0};
        DrawTextureEx(foreground, fg2Pos, 0.0, 2.0, WHITE);



        if(is_on_ground(scarfy_data, window_dimensions[1]))
        {
            // on ground
            velocity = 0;
            is_in_air = false;
        }
        else
        {
            // in air
            // applying gravity
            velocity += g * dT;
            is_in_air = true;
        }

        // checking jump
        if(IsKeyPressed(KEY_SPACE) && !is_in_air)
            velocity += jump_velocity;
        

        // updating nebulae position
        for(int i=0; i<size_of_nebulae; i++)
            nebulae[i].pos.x += neb_vel * dT;
        
        // updating finish line
        finish_line += neb_vel * dT;
         
        
        //updating scarfy position
        scarfy_data.pos.y += velocity * dT;
 


        if(!is_in_air)
            scarfy_data = update_anim_data(scarfy_data, dT, 5);      // updating animation frame for scarfy


         //updating animation frame for nebulae
        for(int i=0; i<size_of_nebulae; i++)
            nebulae[i] = update_anim_data(nebulae[i], dT, 7);        


        for(anim_data nebula: nebulae)
        {
            float pad = 50;
            Rectangle neb_rec {
                nebula.pos.x + pad,
                nebula.pos.y + pad,
                nebula.rec.width - 2*pad,
                nebula.rec.height - 2*pad
            };

            Rectangle scarfy_rec {
                scarfy_data.pos.x,
                scarfy_data.pos.y,
                scarfy_data.rec.width,
                scarfy_data.rec.height
            };

            if(CheckCollisionRecs(neb_rec, scarfy_rec))
                collision = true;
        }

        
        if(collision)
        {
            // Game over
            DrawText("Game Over!", window_dimensions[0]/4, window_dimensions[1]/2, 45, MAROON);
        }
        else if(scarfy_data.pos.x >= finish_line)
        {
            // Game Won
            DrawText("Game Won!", window_dimensions[0]/4, window_dimensions[1]/2, 45, GREEN);
        }
        else
        {
            // drawing nebula
            for(int i=0; i<size_of_nebulae; i++)
                DrawTextureRec(nebula, nebulae[i].rec, nebulae[i].pos, WHITE);
            

            //drawing scarfy
            DrawTextureRec(scarfy, scarfy_data.rec, scarfy_data.pos, WHITE);
        }

        EndDrawing();
    }
    UnloadTexture(scarfy);
    UnloadTexture(nebula);
    UnloadTexture(background);
    UnloadTexture(midground);
    UnloadTexture(foreground);
    
    CloseWindow();
}