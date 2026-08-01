#include "raylib.h"

int main(void)
{
    const float screenWidth = 1072.0f;
    const float screenHeight = 613.0f;

    // Dimensões do sprite do jogador
    const int player_frames_width = 32;
    const int player_frames_height = 32;

    // Estados e animação
    int current_frame = 0;
    float timer_anim = 0.0f;
    float frame_time_idle = 0.16f;
    float frame_time_jump = 0.2f;
    float frame_time_run = 0.08f;

    bool is_jumping = false;
    bool is_runing = false;
    bool facing_left = false;
    bool is_left_last_pos = false;

    // Parâmetros de física
    float gravity = 0.3f;
    float velocityY = 0.0f;
    float velocityX = 0.0f;
    float jump_speed = -7.8f;
    float run_speed = 1.8f;

    const int num_frames_idle = 4;
    const int num_frames_jump = 4;
    const int num_frames_run = 12;

    float tile_width = 48.0f;
    float tile_height = 48.0f;
    Vector2 player_pos = {tile_width + 16, screenHeight - tile_height * 3.28f};
    float ground_y;

    // Fatores de escala
    const float player_scale = 2.0f;  // personagem
    const float terrain_scale = 1.6f; // relvado
    const float button_scale = 2.5f;  // botão de configurações

    InitWindow((int)screenWidth, (int)screenHeight, "Anchor_Studios");
    SetTargetFPS(60);

    // Carregar texturas
    Texture2D terrain = LoadTexture("../assets/Terrain/Terrain.png");
    Texture2D player_idle = LoadTexture("../assets/Main_Characters/Ninja_Frog/Idle.png");
    Texture2D player_idle_left = LoadTexture("../assets/Main_Characters/Ninja_Frog/Idle_left.png");
    Texture2D player_jumping = LoadTexture("../assets/Main_Characters/Ninja_Frog/Jump.png");
    Texture2D player_jumping_left = LoadTexture("../assets/Main_Characters/Ninja_Frog/Jump_left.png");
    Texture2D player_run_right = LoadTexture("../assets/Main_Characters/Ninja_Frog/Run.png");
    Texture2D player_run_left = LoadTexture("../assets/Main_Characters/Ninja_Frog/Run_left.png");
    Texture2D settings_btn = LoadTexture("../assets/Menu/Buttons/Settings.png");

    Rectangle frame_rec = {0, 0, (float)player_frames_width, (float)player_frames_height};

    // Retângulos de recorte dos tiles do relvado (tamanhos originais)
    Rectangle grass_tiles[2] = {
        {98, 0, tile_width - 5, tile_height - 1}, // largura 43, altura 47
        {98, 16, tile_width - 5, tile_height}     // largura 43, altura 48
    };

    while (!WindowShouldClose())
    {
        float dt = GetFrameTime();

        // ----- ENTRADA E MOVIMENTO HORIZONTAL -----
        bool move_right = IsKeyDown(KEY_RIGHT);
        bool move_left = IsKeyDown(KEY_LEFT);

        if (move_right && !move_left)
        {
            is_runing = true;
            velocityX = run_speed;
            facing_left = false;
            is_left_last_pos = facing_left;
        }
        else if (move_left && !move_right)
        {
            is_runing = true;
            velocityX = -run_speed;
            facing_left = true;
            is_left_last_pos = facing_left;
        }
        else
        {
            is_runing = false;
            velocityX = 0.0f;
        }

        // ----- PULO -----
        if (IsKeyPressed(KEY_SPACE) && !is_jumping)
        {
            is_jumping = true;
            velocityY = jump_speed;
            current_frame = 0;
            timer_anim = 0.0f;
        }

        // ----- ATUALIZAÇÃO FÍSICA -----
        player_pos.x += velocityX * dt * 60.0f;

        if (is_jumping)
        {
            player_pos.y += velocityY;
            velocityY += gravity;

            ground_y = screenHeight - tile_height * 3.28f;
            if (CheckCollisionPointRec(player_pos, (Rectangle){0, ground_y, screenWidth, tile_height}))
            {
                player_pos.y = ground_y;
                is_jumping = false;
                velocityY = 0.0f;
                current_frame = 0;
                timer_anim = 0.0f;
            }
        }

        // ----- ATUALIZAÇÃO DA ANIMAÇÃO -----
        timer_anim += dt;
        if (is_jumping)
        {
            if (timer_anim >= frame_time_jump)
            {
                timer_anim = 0.0f;
                current_frame = (current_frame + 1) % num_frames_jump;
            }
        }
        else if (is_runing)
        {
            if (timer_anim >= frame_time_run)
            {
                timer_anim = 0.0f;
                current_frame = (current_frame + 1) % num_frames_run;
            }
        }
        else
        {
            if (timer_anim >= frame_time_idle)
            {
                timer_anim = 0.0f;
                current_frame = (current_frame + 1) % num_frames_idle;
            }
        }

        frame_rec.x = (float)(current_frame * player_frames_width);

        // ----- RENDERIZAÇÃO -----
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Botão de configurações (escalado)
        DrawTextureEx(settings_btn, (Vector2){32.0f, 32.0f}, 0.0f, button_scale, WHITE);

        // Cenário (relvado escalado)
        float tile_real_width = grass_tiles[0].width; // 43
        float step_x = tile_real_width * terrain_scale;

        for (float x = 0.0f; x <= screenWidth + step_x; x += step_x)
        {
            // Primeira camada (grama superior) – manter base na mesma linha
            float h0 = grass_tiles[0].height;                    // 47
            float y0_orig = screenHeight - tile_height * 2;      // y original
            float y0_dest = y0_orig - (h0 * terrain_scale - h0); // sobe para manter base

            DrawTexturePro(terrain,
                           grass_tiles[0],
                           (Rectangle){x, y0_dest,
                                       grass_tiles[0].width * terrain_scale,
                                       h0 * terrain_scale},
                           (Vector2){0, 0}, 0.0f, WHITE);

            // Segunda camada
            float h1 = grass_tiles[1].height; // 48
            float y1_orig = screenHeight - tile_height - 8.0f;
            float y1_dest = y1_orig - (h1 * terrain_scale - h1);

            DrawTexturePro(terrain,
                           grass_tiles[1],
                           (Rectangle){x, y1_dest,
                                       grass_tiles[1].width * terrain_scale,
                                       h1 * terrain_scale},
                           (Vector2){0, 0}, 0.0f, WHITE);

            // Terceira camada (mais inferior)
            float y2_orig = screenHeight - 16.0f;
            float y2_dest = y2_orig - (h1 * terrain_scale - h1);

            DrawTexturePro(terrain,
                           grass_tiles[1],
                           (Rectangle){x, y2_dest,
                                       grass_tiles[1].width * terrain_scale,
                                       h1 * terrain_scale},
                           (Vector2){0, 0}, 0.0f, WHITE);
        }

        // Personagem escalado (mantém pés na posição física)
        Rectangle destRec = {
            player_pos.x,
            player_pos.y - (player_frames_height * (player_scale - 1.1f)),
            player_frames_width * player_scale,
            player_frames_height * player_scale};

        if (is_jumping && facing_left)
            DrawTexturePro(player_jumping_left, frame_rec, destRec, (Vector2){0, 0}, 0.0f, WHITE);
        else if (is_jumping)
            DrawTexturePro(player_jumping, frame_rec, destRec, (Vector2){0, 0}, 0.0f, WHITE);
        else if (is_runing && facing_left)
            DrawTexturePro(player_run_left, frame_rec, destRec, (Vector2){0, 0}, 0.0f, WHITE);
        else if (is_runing)
            DrawTexturePro(player_run_right, frame_rec, destRec, (Vector2){0, 0}, 0.0f, WHITE);
        else if (!is_left_last_pos)
            DrawTexturePro(player_idle, frame_rec, destRec, (Vector2){0, 0}, 0.0f, WHITE);
        else
            DrawTexturePro(player_idle_left, frame_rec, destRec, (Vector2){0, 0}, 0.0f, WHITE);

        EndDrawing();
    }

    UnloadTexture(terrain);
    UnloadTexture(player_idle);
    UnloadTexture(player_idle_left);
    UnloadTexture(player_jumping);
    UnloadTexture(player_jumping_left);
    UnloadTexture(player_run_right);
    UnloadTexture(player_run_left);
    UnloadTexture(settings_btn);

    CloseWindow();
    return 0;
}