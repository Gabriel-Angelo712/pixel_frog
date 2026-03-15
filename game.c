#include "raylib.h"

#define MAX_OBTACLES 10

int main(void)
{

    // Medidas da tela
    const int screenWidth = 1072;
    const int screenHeight = 613;

    // Variáveis para controle de fade (transição entre telas)

    float alfa = 0.0f;
    float default_music_volume = 1.0f;
    bool fading = false;
    bool init_screen = true;

    int revealed_tiles = 0;
    float tile_timer = 0.0f;
    int max_tiles;
    float tile_delay = 1.0f;
    int tiles_per_step = 2;
    float tile_width = 16;
    float tile_height = 16;

    float blade_rotation = 0.0f;
    float blade_time = 0.0f;
    float blade_amplitude = 30.0f;
    float blade_speed = 2.0f;
    float base_y = screenHeight - tile_height * 3.6f;

    float platform_time = 0.0f;
    float platform_amplitude = 45.0f;
    float platform_speed = 2.0f;
    float base_x_platform = screenWidth - tile_width * 41.0f;

    int player_frames_width = 32;
    int player_frames_height = 32;
    int num_frames = 4;
    int num_frames_idle = 4;
    int current_frame = 0;
    float frame_time_jump = 0.2f;
    float frame_time_run = 0.08f;
    float timer = 0.0f;
    bool is_jumping = false;
    bool is_runing = false;
    float gravity = 0.3f;
    float velocityY = 0.0f;
    float velocityX = 0.0f;
    float jump_speed = -4.8f;
    float run_speed = 3.8f;
    int num_frames_run = 12;

    // Inicialização da tela e Audio Device
    InitWindow(screenWidth, screenHeight, "Anchor_Studios");
    InitAudioDevice();

    // Definição de áudios
    Music default_music = LoadMusicStream("./audios/default.mp3");
    Music init_game_scene_music = LoadMusicStream("./audios/default2.mp3");
    Sound btn_sound1 = LoadSound("./audios/play_btn_click.wav");

    // Definição de fontes
    Font default_font = LoadFont("./fonts/Pixelify_Sans/PixelifySans-VariableFont_wght.ttf");

    // Definição de imagens
    Texture2D bg = LoadTexture("./assets/background1.png");
    Texture2D play_btn_img = LoadTexture("./assets/Menu/Buttons/Play.png");
    Texture2D saw_blade = LoadTexture("./assets/Traps/Saw/Off.png");
    Texture2D bg_tile = LoadTexture("./assets/Background/Brown.png");
    Texture2D terrain = LoadTexture("./assets/Terrain/Terrain.png");
    Texture2D spikes = LoadTexture("./assets/Traps/Spikes/Idle.png");
    Texture2D moving_platform = LoadTexture("./assets/Traps/Platforms/Grey_Off.png");
    Texture2D player = LoadTexture("./assets/Main_Characters/Ninja_Frog/Idle.png");
    Texture2D player_jumping = LoadTexture("./assets/Main_Characters/Ninja_Frog/Jump.png");
    Texture2D player_runing = LoadTexture("./assets/Main_Characters/Ninja_Frog/Run.png");

    // Definindo shapes
    Rectangle play_btn_shape = {500, 460, 65, 65};

    Rectangle cobre[8] = {
        {192, 0, tile_width, tile_height},
        {192, 16, tile_width, tile_height},
        {208, 0, tile_width, tile_height},
        {224, 0, tile_width, tile_height},
        {224, 16, tile_width, tile_height},
        {224, 32, tile_width, tile_height},
        {240, 0, tile_width, tile_height},
    };

    Rectangle spike_frames = {0, 0, 24, 16};

    Rectangle bloco_cobre = {208, 16, 32, 32};

    Rectangle madeira[8] = {
        {0, 64, tile_width, tile_height},  // extremo superior esquerdo
        {0, 80, tile_width, tile_height},  // lateral esqerda
        {0, 96, tile_width, tile_height},  // extremo inferior esquerdo
        {16, 96, tile_width, tile_height}, // centro base
        {32, 96, tile_width, tile_height}, // extremo inferior direito
        {32, 80, tile_width, tile_height}, // lateral direita
        {32, 64, tile_width, tile_height}, // extremo superior direito
        {16, 64, tile_width, tile_height}, // centro superior
    };

    Rectangle saw_blade_shape = {0, 0, 38, 38};

    Rectangle pedra_2[12] = {
        {192, 64, tile_width, tile_height}, // barra horizontal (extremo esquerdo)
        {208, 64, tile_width, tile_height}, // barra horizontal (centro)
        {224, 64, tile_width, tile_height}, // barra horizontal (extremo direito)
        {240, 64, tile_width, tile_height}, // barra vertical (extremo superior)
        {240, 80, tile_width, tile_height}, // barra vertical (centro)
        {240, 96, tile_width, tile_height}, // barra vertical (extremo inferior)
        {192, 80, tile_width, tile_height}, // bloco isolado
        {208, 80, tile_width, tile_height}, // bloco (extremo superior esquerdo)
        {224, 80, tile_width, tile_height}, // bloco (extremo superior direito)
        {208, 96, tile_width, tile_height}, // bloco (extremo inferior esquerdo)
        {224, 96, tile_width, tile_height}, // bloco (extremo inferior direito)
    };

    Rectangle terra_seca[14] = {
        {96, 64, tile_width, tile_height},
        {112, 64, tile_width, tile_height},
        {128, 64, tile_width, tile_height},
        {96, 96, tile_width, tile_height},
        {112, 96, tile_width, tile_height},
        {128, 96, tile_width, tile_height},
        {96, 80, tile_width, tile_height},
        {112, 80, tile_width, tile_height},
        {128, 80, tile_width, tile_height},
    };

    Rectangle relvado[10] = {
        {96, 0, tile_width, tile_height},
        {96, 16, tile_width, tile_height},
        {96, 32, tile_width, tile_height},
        {112, 0, tile_width, tile_height},
        {112, 16, tile_width, tile_height},
        {112, 32, tile_width, tile_height},
        {128, 0, tile_width, tile_height},
        {128, 16, tile_width, tile_height},
        {128, 32, tile_width, tile_height},
    };

    Rectangle blade_dest = {
        tile_width * 21,
        screenHeight - tile_height * 5,
        38,
        38,
    };

    Rectangle platform_dest = {
        tile_width,
        screenHeight - tile_height * 5,
        32,
        8,
    };

    Rectangle left_rock = {0, 16, tile_width, tile_height};
    Rectangle right_rock = {32, 16, tile_width, tile_height};
    Rectangle barra_horizontal = {192, 0, tile_width * 3, tile_height};

    // Rectangle floor_blocks[MAX_OBTACLES] = {
    //     barra_horizontal,
    //     relvado[0],
    //     relvado[3],
    //     relvado[6],
    //     terra_seca[0],
    //     terra_seca[1],
    //     terra_seca[2],
    // };

    // Definição de vetores
    Vector2 bg_pos = {0, 0};
    Vector2 title_pos = {380, 150};
    Vector2 origin = {19, 19};
    Vector2 play_btn_pos = {play_btn_shape.x, play_btn_shape.y};
    Vector2 player_pos = {tile_width * 3, screenHeight - tile_height * 6};

    // Tocando a música de fundo (default)
    PlayMusicStream(default_music);

    // Configurando de frames por segundo
    SetTargetFPS(60);

    // game loop
    while (!WindowShouldClose())
    {

        Rectangle player_jumping_frame_rec = {
            (float)(current_frame * player_frames_width),
            0.0f,
            (float)player_frames_width,
            (float)player_frames_height,
        };

        Rectangle player_frame_rec = {
            (float)(current_frame * player_frames_width),
            0.0f,
            (float)player_frames_width,
            (float)player_frames_height,
        };

        Rectangle player_runing_frame_rec = {
            (float)(current_frame * player_frames_width),
            0.0f,
            (float)player_frames_width,
            (float)player_frames_height,
        };

        timer += GetFrameTime();

        if (timer >= 0.16f)
        {
            timer = 0;
            current_frame = (current_frame + 1) % num_frames_idle;
        }

        if (IsKeyPressed(KEY_SPACE) && !is_jumping)
        {
            is_jumping = true;
            velocityY = jump_speed;
        }

        if (is_jumping)
        {
            timer += GetFrameTime();
            player_pos.y += velocityY;
            velocityY += gravity;

            if (player_pos.y >= screenHeight - tile_height * 6)
            {
                player_pos.y = screenHeight - tile_height * 6;
                is_jumping = false;
                current_frame = 0;
            }

            if (timer >= frame_time_jump)
            {
                timer = 0.0f;
                current_frame = (current_frame + 1) % num_frames;
            }
        }

        if (IsKeyPressed(KEY_RIGHT) && !is_runing)
        {
            is_runing = true;
            velocityX = 1.8f;
        }

        if (is_runing)
        {
            timer += GetFrameTime();

            if (timer >= frame_time_run)
            {
                timer = 0.0f;
                current_frame = (current_frame + 1) % num_frames_run;
            }
            else
            {
                player_pos.x += velocityX;

                if (CheckCollisionPointRec(player_pos, (Rectangle){192, 0, tile_width * 3, tile_height}) || CheckCollisionPointRec(player_pos, (Rectangle){96, 0, tile_width, tile_height}) || CheckCollisionPointRec(player_pos, (Rectangle){112, 0, tile_width, tile_height}) || CheckCollisionPointRec(player_pos, (Rectangle){128, 0, tile_width, tile_height}) || CheckCollisionPointRec(player_pos, (Rectangle){96, 64, tile_width, tile_height}) || CheckCollisionPointRec(player_pos, (Rectangle){112, 64, tile_width, tile_height}) || CheckCollisionPointRec(player_pos, (Rectangle){128, 80, tile_width, tile_height}))
                {
                    is_runing = false;
                    current_frame = 0;
                }
            }
        }

        // Desenhando na tela
        BeginDrawing();

        ClearBackground(RAYWHITE);
        // Tela de início (elememntos básicos)
        if (init_screen)
        {
            UpdateMusicStream(default_music);
            DrawTextureEx(bg, bg_pos, 0.0f, 0.85f, WHITE);
            DrawTextEx(default_font, "PIXEL FROG", title_pos, 70, 2, BLACK);
            DrawTextureEx(play_btn_img, play_btn_pos, 0.0f, 3.0f, WHITE);
        }

        Vector2 mouse = GetMousePosition(); // Pegando a posição atual do mouse

        // Verificando se o mouse não está sobre o botão de play
        if (!(CheckCollisionPointRec(mouse, play_btn_shape)))
        {
            SetMouseCursor(MOUSE_CURSOR_DEFAULT); // Voltando o cursor para o padrão
        }

        // Verificando se o mouse está sobre o botão de play para (hover)
        if (CheckCollisionPointRec(mouse, play_btn_shape))
        {
            SetMouseCursor(MOUSE_CURSOR_POINTING_HAND); // Alterando o cursor para pointing hand
        }

        // Verificando se o botão de play game foi pressionado
        if (CheckCollisionPointRec(mouse, play_btn_shape) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            PlaySound(btn_sound1); // Tocando o som de clique do botão
            fading = true;
        }

        if (fading)
        {
            // Iniciando o fading (transição entre telas)
            DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, alfa));
            alfa += 0.8f * GetFrameTime();

            default_music_volume -= 0.8f * GetFrameTime();

            if (default_music_volume < 0)
                default_music_volume = 0;

            SetMusicVolume(default_music, default_music_volume);

            if (alfa >= 1.0f)
            {
                // fading = false;
                init_screen = false;

                // Retirando os elementos da tela de início (término do fading)
                play_btn_shape.x = 0;
                play_btn_shape.y = 0;
                play_btn_shape.width = 0;
                play_btn_shape.height = 0;
                StopMusicStream(default_music);
            }
        }

        if (!init_screen)
        {
            tile_timer += GetFrameTime();
            max_tiles = 24 * 14;

            if (tile_timer >= tile_delay)
            {
                revealed_tiles += tiles_per_step;
                tile_timer = 0.0f;
            }

            if (revealed_tiles > max_tiles)
            {
                revealed_tiles = max_tiles;
            }

            for (int i = 0; i <= max_tiles; i++)
            {
                int col = i % 24;
                int row = i / 24;
                DrawTextureEx(bg_tile, (Vector2){col * 64, row * 64}, 0.0f, 1.0f, WHITE);
            }

            for (int i = 16; i < screenWidth - 16; i += tile_width)
            {

                DrawTextureRec(terrain, madeira[7], (Vector2){i, 0}, WHITE);
                DrawTextureRec(terrain, madeira[3], (Vector2){i, screenHeight - tile_height}, WHITE);

                if (i < 16 * 6 || (i > 16 * 10 && i < screenHeight - tile_height))
                {
                    DrawTextureRec(terrain, left_rock, (Vector2){0, i}, WHITE);
                }

                if (i < 16 * 26 || (i > 16 * 30 && i < screenHeight - tile_height))
                {
                    DrawTextureRec(terrain, right_rock, (Vector2){screenWidth - tile_width, i}, WHITE);
                }

                // Achar o termo geral e substituir por um loop
                if (i == 16 * 6)
                {

                    // LADO ESQUERDO

                    DrawTextureRec(terrain, pedra_2[3], (Vector2){0, i}, WHITE);
                    DrawTextureRec(terrain, pedra_2[4], (Vector2){0, i + tile_height}, WHITE);
                    DrawTextureRec(terrain, pedra_2[5], (Vector2){0, i + (tile_height * 2)}, WHITE);
                    DrawTextureRec(terrain, pedra_2[0], (Vector2){0, i + (tile_height * 3)}, WHITE);
                    DrawTextureRec(terrain, pedra_2[6], (Vector2){0, i + (tile_height * 4)}, WHITE);
                    DrawTextureRec(terrain, pedra_2[6], (Vector2){tile_width, i + (tile_height * 2)}, WHITE);
                    DrawTextureRec(terrain, pedra_2[1], (Vector2){tile_width, i + (tile_height * 3)}, WHITE);
                    DrawTextureRec(terrain, pedra_2[1], (Vector2){tile_width * 2, i + (tile_height * 3)}, WHITE);
                    DrawTextureRec(terrain, pedra_2[2], (Vector2){tile_width * 3, i + (tile_height * 3)}, WHITE);
                }

                // Achar o termo geral e substituir por um loop
                if (i == 16 * 26)
                {
                    // LADO DIREITO

                    DrawTextureRec(terrain, pedra_2[3], (Vector2){screenWidth - tile_width, i}, WHITE);
                    DrawTextureRec(terrain, pedra_2[4], (Vector2){screenWidth - tile_width, i + tile_height}, WHITE);
                    DrawTextureRec(terrain, pedra_2[5], (Vector2){screenWidth - tile_width, i + (tile_height * 2)}, WHITE);
                    DrawTextureRec(terrain, pedra_2[6], (Vector2){screenWidth - (tile_width * 2), i + (tile_height * 2)}, WHITE);
                    DrawTextureRec(terrain, pedra_2[0], (Vector2){screenWidth - (tile_width * 4), i + (tile_height * 3)}, WHITE);
                    DrawTextureRec(terrain, pedra_2[1], (Vector2){screenWidth - (tile_width * 2), i + (tile_height * 3)}, WHITE);
                    DrawTextureRec(terrain, pedra_2[1], (Vector2){screenWidth - (tile_width * 3), i + (tile_height * 3)}, WHITE);
                    DrawTextureRec(terrain, pedra_2[2], (Vector2){screenWidth - tile_width, i + (tile_height * 3)}, WHITE);
                    DrawTextureRec(terrain, pedra_2[6], (Vector2){screenWidth - tile_width, i + (tile_height * 4)}, WHITE);
                }
            }

            DrawTextureRec(terrain, cobre[6], (Vector2){0, 0}, WHITE);
            DrawTextureRec(terrain, cobre[6], (Vector2){screenWidth - tile_width, 0}, WHITE);
            DrawTextureRec(terrain, cobre[6], (Vector2){screenWidth - tile_width, screenHeight - tile_height}, WHITE);
            DrawTextureRec(terrain, cobre[6], (Vector2){0, screenHeight - tile_height}, WHITE);

            DrawTextureRec(terrain, terra_seca[0], (Vector2){tile_width, screenHeight - tile_height * 4}, WHITE);
            DrawTextureRec(terrain, terra_seca[6], (Vector2){tile_width, screenHeight - tile_height * 3}, WHITE);
            DrawTextureRec(terrain, terra_seca[3], (Vector2){tile_width, screenHeight - tile_height * 2}, WHITE);

            DrawTextureRec(terrain, terra_seca[1], (Vector2){tile_width * 2, screenHeight - tile_height * 4}, WHITE);
            DrawTextureRec(terrain, terra_seca[7], (Vector2){tile_width * 2, screenHeight - tile_height * 3}, WHITE);
            DrawTextureRec(terrain, terra_seca[4], (Vector2){tile_width * 2, screenHeight - tile_height * 2}, WHITE);

            DrawTextureRec(terrain, terra_seca[1], (Vector2){tile_width * 3, screenHeight - tile_height * 4}, WHITE);
            DrawTextureRec(terrain, terra_seca[7], (Vector2){tile_width * 3, screenHeight - tile_height * 3}, WHITE);
            DrawTextureRec(terrain, terra_seca[4], (Vector2){tile_width * 3, screenHeight - tile_height * 2}, WHITE);

            DrawTextureRec(terrain, terra_seca[2], (Vector2){tile_width * 4, screenHeight - tile_height * 4}, WHITE);
            DrawTextureRec(terrain, terra_seca[8], (Vector2){tile_width * 4, screenHeight - tile_height * 3}, WHITE);
            DrawTextureRec(terrain, terra_seca[5], (Vector2){tile_width * 4, screenHeight - tile_height * 2}, WHITE);

            DrawTextureRec(terrain, relvado[0], (Vector2){tile_width * 5, screenHeight - tile_height * 5}, WHITE);
            DrawTextureRec(terrain, relvado[3], (Vector2){tile_width * 6, screenHeight - tile_height * 5}, WHITE);
            DrawTextureRec(terrain, relvado[3], (Vector2){tile_width * 7, screenHeight - tile_height * 5}, WHITE);
            DrawTextureRec(terrain, relvado[6], (Vector2){tile_width * 8, screenHeight - tile_height * 5}, WHITE);

            DrawTextureRec(terrain, relvado[1], (Vector2){tile_width * 5, screenHeight - tile_height * 4}, WHITE);
            DrawTextureRec(terrain, relvado[4], (Vector2){tile_width * 6, screenHeight - tile_height * 4}, WHITE);
            DrawTextureRec(terrain, relvado[4], (Vector2){tile_width * 7, screenHeight - tile_height * 4}, WHITE);
            DrawTextureRec(terrain, relvado[7], (Vector2){tile_width * 8, screenHeight - tile_height * 4}, WHITE);

            DrawTextureRec(terrain, relvado[1], (Vector2){tile_width * 5, screenHeight - tile_height * 3}, WHITE);
            DrawTextureRec(terrain, relvado[4], (Vector2){tile_width * 6, screenHeight - tile_height * 3}, WHITE);
            DrawTextureRec(terrain, relvado[4], (Vector2){tile_width * 7, screenHeight - tile_height * 3}, WHITE);
            DrawTextureRec(terrain, relvado[4], (Vector2){tile_width * 9, screenHeight - tile_height * 5}, WHITE);
            DrawTextureRec(terrain, relvado[4], (Vector2){tile_width * 10, screenHeight - tile_height * 5}, WHITE);
            DrawTextureRec(terrain, relvado[4], (Vector2){tile_width * 11, screenHeight - tile_height * 5}, WHITE);
            DrawTextureRec(terrain, relvado[4], (Vector2){tile_width * 13, screenHeight - tile_height * 5}, WHITE);
            DrawTextureRec(terrain, relvado[4], (Vector2){tile_width * 12, screenHeight - tile_height * 5}, WHITE);
            DrawTextureRec(terrain, relvado[4], (Vector2){tile_width * 9, screenHeight - tile_height * 4}, WHITE);
            DrawTextureRec(terrain, relvado[4], (Vector2){tile_width * 10, screenHeight - tile_height * 4}, WHITE);
            DrawTextureRec(terrain, relvado[4], (Vector2){tile_width * 11, screenHeight - tile_height * 4}, WHITE);
            DrawTextureRec(terrain, relvado[4], (Vector2){tile_width * 13, screenHeight - tile_height * 4}, WHITE);
            DrawTextureRec(terrain, relvado[4], (Vector2){tile_width * 12, screenHeight - tile_height * 4}, WHITE);
            DrawTextureRec(terrain, bloco_cobre, (Vector2){tile_width * 14, screenHeight - tile_height * 5}, WHITE);
            DrawTextureRec(terrain, relvado[4], (Vector2){tile_width * 16, screenHeight - tile_height * 5}, WHITE);
            DrawTextureRec(terrain, relvado[0], (Vector2){tile_width * 17, screenHeight - tile_height * 5}, WHITE);
            DrawTextureRec(terrain, relvado[3], (Vector2){tile_width * 18, screenHeight - tile_height * 5}, WHITE);
            DrawTextureRec(terrain, relvado[3], (Vector2){tile_width * 19, screenHeight - tile_height * 5}, WHITE);
            DrawTextureRec(terrain, relvado[3], (Vector2){tile_width * 20, screenHeight - tile_height * 5}, WHITE);

            DrawTextureRec(terrain, relvado[2], (Vector2){tile_width * 5, screenHeight - tile_height * 2}, WHITE);
            DrawTextureRec(terrain, relvado[5], (Vector2){tile_width * 6, screenHeight - tile_height * 2}, WHITE);
            DrawTextureRec(terrain, relvado[5], (Vector2){tile_width * 7, screenHeight - tile_height * 2}, WHITE);

            DrawTextureRec(terrain, relvado[0], (Vector2){tile_width * 7, screenHeight - tile_height * 7}, WHITE);
            DrawTextureRec(terrain, relvado[3], (Vector2){tile_width * 8, screenHeight - tile_height * 7}, WHITE);
            DrawTextureRec(terrain, relvado[3], (Vector2){tile_width * 9, screenHeight - tile_height * 7}, WHITE);
            DrawTextureRec(terrain, relvado[3], (Vector2){tile_width * 10, screenHeight - tile_height * 7}, WHITE);
            DrawTextureRec(terrain, relvado[3], (Vector2){tile_width * 11, screenHeight - tile_height * 7}, WHITE);
            DrawTextureRec(terrain, relvado[3], (Vector2){tile_width * 12, screenHeight - tile_height * 7}, WHITE);
            DrawTextureRec(terrain, relvado[3], (Vector2){tile_width * 13, screenHeight - tile_height * 7}, WHITE);
            DrawTextureRec(terrain, relvado[6], (Vector2){tile_width * 14, screenHeight - tile_height * 7}, WHITE);

            DrawTextureRec(terrain, relvado[1], (Vector2){tile_width * 7, screenHeight - tile_height * 6}, WHITE);
            DrawTextureRec(terrain, relvado[4], (Vector2){tile_width * 8, screenHeight - tile_height * 6}, WHITE);
            DrawTextureRec(terrain, relvado[4], (Vector2){tile_width * 9, screenHeight - tile_height * 6}, WHITE);
            DrawTextureRec(terrain, relvado[4], (Vector2){tile_width * 10, screenHeight - tile_height * 6}, WHITE);
            DrawTextureRec(terrain, relvado[4], (Vector2){tile_width * 11, screenHeight - tile_height * 6}, WHITE);
            DrawTextureRec(terrain, relvado[4], (Vector2){tile_width * 12, screenHeight - tile_height * 6}, WHITE);
            DrawTextureRec(terrain, relvado[0], (Vector2){tile_width * 13, screenHeight - tile_height * 6}, WHITE);
            DrawTextureRec(terrain, relvado[3], (Vector2){tile_width * 14, screenHeight - tile_height * 6}, WHITE);
            DrawTextureRec(terrain, relvado[3], (Vector2){tile_width * 15, screenHeight - tile_height * 6}, WHITE);
            DrawTextureRec(terrain, relvado[3], (Vector2){tile_width * 16, screenHeight - tile_height * 6}, WHITE);
            DrawTextureRec(terrain, relvado[3], (Vector2){tile_width * 17, screenHeight - tile_height * 6}, WHITE);

            DrawTextureRec(terrain, bloco_cobre, (Vector2){tile_width * 8, screenHeight - tile_height * 3}, WHITE);
            DrawTextureRec(terrain, relvado[4], (Vector2){tile_width * 10, screenHeight - tile_height * 3}, WHITE);
            DrawTextureRec(terrain, relvado[4], (Vector2){tile_width * 11, screenHeight - tile_height * 3}, WHITE);
            DrawTextureRec(terrain, relvado[4], (Vector2){tile_width * 12, screenHeight - tile_height * 3}, WHITE);
            DrawTextureRec(terrain, relvado[4], (Vector2){tile_width * 13, screenHeight - tile_height * 3}, WHITE);
            DrawTextureRec(terrain, relvado[4], (Vector2){tile_width * 14, screenHeight - tile_height * 3}, WHITE);
            DrawTextureRec(terrain, relvado[4], (Vector2){tile_width * 14, screenHeight - tile_height * 2}, WHITE);
            DrawTextureRec(terrain, relvado[4], (Vector2){tile_width * 15, screenHeight - tile_height * 2}, WHITE);
            DrawTextureRec(terrain, relvado[4], (Vector2){tile_width * 15, screenHeight - tile_height * 3}, WHITE);

            DrawTextureRec(terrain, relvado[4], (Vector2){tile_width * 16, screenHeight - tile_height * 3}, WHITE);
            DrawTextureRec(terrain, relvado[4], (Vector2){tile_width * 17, screenHeight - tile_height * 3}, WHITE);
            DrawTextureRec(terrain, relvado[4], (Vector2){tile_width * 18, screenHeight - tile_height * 3}, WHITE);
            DrawTextureRec(terrain, relvado[4], (Vector2){tile_width * 19, screenHeight - tile_height * 3}, WHITE);
            DrawTextureRec(terrain, relvado[4], (Vector2){tile_width * 20, screenHeight - tile_height * 3}, WHITE);

            DrawTextureRec(terrain, relvado[4], (Vector2){tile_width * 16, screenHeight - tile_height * 2}, WHITE);
            DrawTextureRec(terrain, relvado[4], (Vector2){tile_width * 17, screenHeight - tile_height * 2}, WHITE);
            DrawTextureRec(terrain, relvado[4], (Vector2){tile_width * 18, screenHeight - tile_height * 2}, WHITE);
            DrawTextureRec(terrain, relvado[4], (Vector2){tile_width * 19, screenHeight - tile_height * 2}, WHITE);
            DrawTextureRec(terrain, relvado[4], (Vector2){tile_width * 20, screenHeight - tile_height * 2}, WHITE);

            DrawTextureRec(terrain, relvado[4], (Vector2){tile_width * 20, screenHeight - tile_height * 4}, WHITE);
            DrawTextureRec(terrain, relvado[4], (Vector2){tile_width * 16, screenHeight - tile_height * 4}, WHITE);
            DrawTextureRec(terrain, relvado[4], (Vector2){tile_width * 17, screenHeight - tile_height * 4}, WHITE);
            DrawTextureRec(terrain, relvado[4], (Vector2){tile_width * 18, screenHeight - tile_height * 4}, WHITE);
            DrawTextureRec(terrain, relvado[4], (Vector2){tile_width * 19, screenHeight - tile_height * 4}, WHITE);

            DrawTextureRec(terrain, relvado[4], (Vector2){tile_width * 10, screenHeight - tile_height * 2}, WHITE);
            DrawTextureRec(terrain, relvado[4], (Vector2){tile_width * 11, screenHeight - tile_height * 2}, WHITE);

            DrawTextureRec(terrain, cobre[0], (Vector2){tile_width * 12, screenHeight - tile_height * 2}, WHITE);
            DrawTextureRec(terrain, cobre[3], (Vector2){tile_width * 13, screenHeight - tile_height * 2}, WHITE);
            DrawTextureRec(terrain, cobre[1], (Vector2){tile_width * 6, screenHeight - tile_height * 6}, WHITE);

            blade_time += GetFrameTime();
            blade_dest.y = base_y + sinf(blade_time * blade_speed) * blade_amplitude;
            DrawTexturePro(
                saw_blade,
                saw_blade_shape,
                blade_dest,
                origin,
                blade_rotation,
                WHITE);
            blade_rotation += 180 * GetFrameTime();

            DrawTextureRec(spikes, spike_frames, (Vector2){tile_width * 22, screenHeight - tile_height * 2}, WHITE);
            DrawTextureRec(spikes, spike_frames, (Vector2){tile_width * 23, screenHeight - tile_height * 2}, WHITE);
            DrawTextureRec(spikes, spike_frames, (Vector2){tile_width * 24, screenHeight - tile_height * 2}, WHITE);
            DrawTextureRec(spikes, spike_frames, (Vector2){tile_width * 25, screenHeight - tile_height * 2}, WHITE);
            DrawTextureRec(spikes, spike_frames, (Vector2){tile_width * 26, screenHeight - tile_height * 2}, WHITE);
            DrawTextureRec(spikes, spike_frames, (Vector2){tile_width * 27, screenHeight - tile_height * 2}, WHITE);
            DrawTextureRec(spikes, spike_frames, (Vector2){tile_width * 28, screenHeight - tile_height * 2}, WHITE);
            DrawTextureRec(spikes, spike_frames, (Vector2){tile_width * 29, screenHeight - tile_height * 2}, WHITE);
            DrawTextureRec(spikes, spike_frames, (Vector2){tile_width * 30, screenHeight - tile_height * 2}, WHITE);
            DrawTextureRec(spikes, spike_frames, (Vector2){tile_width * 31, screenHeight - tile_height * 2}, WHITE);

            platform_dest.x = base_x_platform + cosf(platform_time * platform_speed) * platform_amplitude;
            platform_time += GetFrameTime();

            DrawTexturePro(moving_platform, (Rectangle){0, 0, 32, 8}, platform_dest, (Vector2){0, 0}, 0.0f, WHITE);
            DrawTextureRec(terrain, relvado[0], (Vector2){tile_width * 33, screenHeight - tile_height * 2}, WHITE);
            DrawTextureRec(terrain, relvado[3], (Vector2){tile_width * 34, screenHeight - tile_height * 2}, WHITE);
            DrawTextureRec(terrain, relvado[3], (Vector2){tile_width * 35, screenHeight - tile_height * 2}, WHITE);
            DrawTextureRec(terrain, relvado[4], (Vector2){tile_width * 36, screenHeight - tile_height * 2}, WHITE);
            DrawTextureRec(terrain, relvado[6], (Vector2){tile_width * 37, screenHeight - tile_height * 2}, WHITE);
            DrawTextureRec(terrain, relvado[0], (Vector2){tile_width * 35, screenHeight - tile_height * 3}, WHITE);
            DrawTextureRec(terrain, relvado[3], (Vector2){tile_width * 36, screenHeight - tile_height * 3}, WHITE);

            DrawTextureRec(terrain, relvado[4], (Vector2){tile_width * 37, screenHeight - tile_height * 2}, WHITE);
            DrawTextureRec(terrain, relvado[4], (Vector2){tile_width * 38, screenHeight - tile_height * 2}, WHITE);
            DrawTextureRec(terrain, relvado[4], (Vector2){tile_width * 39, screenHeight - tile_height * 2}, WHITE);
            DrawTextureRec(terrain, relvado[4], (Vector2){tile_width * 40, screenHeight - tile_height * 2}, WHITE);
            DrawTextureRec(terrain, relvado[4], (Vector2){tile_width * 41, screenHeight - tile_height * 2}, WHITE);
            DrawTextureRec(terrain, relvado[4], (Vector2){tile_width * 42, screenHeight - tile_height * 2}, WHITE);
            DrawTextureRec(terrain, relvado[4], (Vector2){tile_width * 43, screenHeight - tile_height * 2}, WHITE);
            DrawTextureRec(terrain, relvado[4], (Vector2){tile_width * 44, screenHeight - tile_height * 2}, WHITE);
            DrawTextureRec(terrain, relvado[4], (Vector2){tile_width * 45, screenHeight - tile_height * 2}, WHITE);
            DrawTextureRec(terrain, relvado[4], (Vector2){tile_width * 46, screenHeight - tile_height * 2}, WHITE);

            DrawTextureRec(terrain, relvado[1], (Vector2){tile_width * 37, screenHeight - tile_height * 3}, WHITE);
            DrawTextureRec(terrain, relvado[4], (Vector2){tile_width * 38, screenHeight - tile_height * 3}, WHITE);
            DrawTextureRec(terrain, relvado[4], (Vector2){tile_width * 39, screenHeight - tile_height * 3}, WHITE);
            DrawTextureRec(terrain, relvado[4], (Vector2){tile_width * 40, screenHeight - tile_height * 3}, WHITE);
            DrawTextureRec(terrain, relvado[4], (Vector2){tile_width * 41, screenHeight - tile_height * 3}, WHITE);
            DrawTextureRec(terrain, relvado[4], (Vector2){tile_width * 42, screenHeight - tile_height * 3}, WHITE);
            DrawTextureRec(terrain, relvado[4], (Vector2){tile_width * 43, screenHeight - tile_height * 3}, WHITE);
            DrawTextureRec(terrain, relvado[4], (Vector2){tile_width * 44, screenHeight - tile_height * 3}, WHITE);
            DrawTextureRec(terrain, relvado[4], (Vector2){tile_width * 45, screenHeight - tile_height * 3}, WHITE);
            DrawTextureRec(terrain, relvado[4], (Vector2){tile_width * 46, screenHeight - tile_height * 3}, WHITE);
            DrawTextureRec(terrain, bloco_cobre, (Vector2){tile_width * 47, screenHeight - tile_height * 3}, WHITE);

            DrawTextureRec(terrain, relvado[2], (Vector2){tile_width * 36, screenHeight - tile_height * 4}, WHITE);
            DrawTextureRec(terrain, relvado[4], (Vector2){tile_width * 37, screenHeight - tile_height * 4}, WHITE);
            DrawTextureRec(terrain, relvado[4], (Vector2){tile_width * 38, screenHeight - tile_height * 4}, WHITE);
            DrawTextureRec(terrain, relvado[4], (Vector2){tile_width * 39, screenHeight - tile_height * 4}, WHITE);
            DrawTextureRec(terrain, relvado[4], (Vector2){tile_width * 40, screenHeight - tile_height * 4}, WHITE);
            DrawTextureRec(terrain, relvado[4], (Vector2){tile_width * 41, screenHeight - tile_height * 4}, WHITE);
            DrawTextureRec(terrain, relvado[4], (Vector2){tile_width * 42, screenHeight - tile_height * 4}, WHITE);
            DrawTextureRec(terrain, relvado[4], (Vector2){tile_width * 43, screenHeight - tile_height * 4}, WHITE);
            DrawTextureRec(terrain, relvado[4], (Vector2){tile_width * 44, screenHeight - tile_height * 4}, WHITE);
            DrawTextureRec(terrain, relvado[4], (Vector2){tile_width * 45, screenHeight - tile_height * 4}, WHITE);
            DrawTextureRec(terrain, relvado[0], (Vector2){tile_width * 46, screenHeight - tile_height * 4}, WHITE);
            DrawTextureRec(terrain, relvado[3], (Vector2){tile_width * 47, screenHeight - tile_height * 4}, WHITE);
            DrawTextureRec(terrain, relvado[3], (Vector2){tile_width * 48, screenHeight - tile_height * 4}, WHITE);

            DrawTextureRec(terrain, relvado[1], (Vector2){tile_width * 36, screenHeight - tile_height * 5}, WHITE);
            DrawTextureRec(terrain, relvado[4], (Vector2){tile_width * 37, screenHeight - tile_height * 5}, WHITE);
            DrawTextureRec(terrain, relvado[4], (Vector2){tile_width * 38, screenHeight - tile_height * 5}, WHITE);
            DrawTextureRec(terrain, relvado[4], (Vector2){tile_width * 39, screenHeight - tile_height * 5}, WHITE);
            DrawTextureRec(terrain, relvado[4], (Vector2){tile_width * 40, screenHeight - tile_height * 5}, WHITE);
            DrawTextureRec(terrain, relvado[4], (Vector2){tile_width * 41, screenHeight - tile_height * 5}, WHITE);
            DrawTextureRec(terrain, relvado[4], (Vector2){tile_width * 42, screenHeight - tile_height * 5}, WHITE);
            DrawTextureRec(terrain, relvado[4], (Vector2){tile_width * 43, screenHeight - tile_height * 5}, WHITE);
            DrawTextureRec(terrain, relvado[4], (Vector2){tile_width * 44, screenHeight - tile_height * 5}, WHITE);
            DrawTextureRec(terrain, relvado[4], (Vector2){tile_width * 45, screenHeight - tile_height * 5}, WHITE);
            DrawTextureRec(terrain, relvado[4], (Vector2){tile_width * 46, screenHeight - tile_height * 5}, WHITE);

            DrawTextureRec(terrain, relvado[0], (Vector2){tile_width * 36, screenHeight - tile_height * 6}, WHITE);
            DrawTextureRec(terrain, relvado[3], (Vector2){tile_width * 37, screenHeight - tile_height * 6}, WHITE);
            DrawTextureRec(terrain, relvado[3], (Vector2){tile_width * 38, screenHeight - tile_height * 6}, WHITE);
            DrawTextureRec(terrain, relvado[3], (Vector2){tile_width * 39, screenHeight - tile_height * 6}, WHITE);
            DrawTextureRec(terrain, relvado[3], (Vector2){tile_width * 40, screenHeight - tile_height * 6}, WHITE);
            DrawTextureRec(terrain, relvado[3], (Vector2){tile_width * 41, screenHeight - tile_height * 6}, WHITE);
            DrawTextureRec(terrain, relvado[3], (Vector2){tile_width * 42, screenHeight - tile_height * 6}, WHITE);
            DrawTextureRec(terrain, relvado[3], (Vector2){tile_width * 43, screenHeight - tile_height * 6}, WHITE);
            DrawTextureRec(terrain, relvado[3], (Vector2){tile_width * 44, screenHeight - tile_height * 6}, WHITE);
            DrawTextureRec(terrain, relvado[3], (Vector2){tile_width * 45, screenHeight - tile_height * 6}, WHITE);
            DrawTextureRec(terrain, relvado[6], (Vector2){tile_width * 46, screenHeight - tile_height * 6}, WHITE);

            DrawTextureRec(terrain, barra_horizontal, (Vector2){tile_width * 47, screenHeight - tile_height * 7}, WHITE);
            DrawTextureRec(terrain, barra_horizontal, (Vector2){tile_width * 50, screenHeight - tile_height * 8}, WHITE);
            DrawTextureRec(terrain, barra_horizontal, (Vector2){tile_width * 53, screenHeight - tile_height * 9}, WHITE);

            DrawTextureRec(terrain, relvado[0], (Vector2){tile_width * 49, screenHeight - tile_height * 2}, WHITE);
            DrawTextureRec(terrain, relvado[3], (Vector2){tile_width * 50, screenHeight - tile_height * 2}, WHITE);
            DrawTextureRec(terrain, relvado[3], (Vector2){tile_width * 51, screenHeight - tile_height * 2}, WHITE);
            DrawTextureRec(terrain, relvado[3], (Vector2){tile_width * 52, screenHeight - tile_height * 2}, WHITE);
            DrawTextureRec(terrain, relvado[3], (Vector2){tile_width * 53, screenHeight - tile_height * 2}, WHITE);
            DrawTextureRec(terrain, relvado[3], (Vector2){tile_width * 54, screenHeight - tile_height * 2}, WHITE);
            DrawTextureRec(terrain, relvado[3], (Vector2){tile_width * 55, screenHeight - tile_height * 2}, WHITE);
            DrawTextureRec(terrain, relvado[3], (Vector2){tile_width * 56, screenHeight - tile_height * 2}, WHITE);
            DrawTextureRec(terrain, relvado[3], (Vector2){tile_width * 57, screenHeight - tile_height * 2}, WHITE);
            DrawTextureRec(terrain, relvado[3], (Vector2){tile_width * 58, screenHeight - tile_height * 2}, WHITE);
            DrawTextureRec(terrain, relvado[3], (Vector2){tile_width * 59, screenHeight - tile_height * 2}, WHITE);
            DrawTextureRec(terrain, bloco_cobre, (Vector2){tile_width * 60, screenHeight - tile_height * 3}, WHITE);
            DrawTextureRec(terrain, relvado[4], (Vector2){tile_width * 62, screenHeight - tile_height * 2}, WHITE);
            DrawTextureRec(terrain, relvado[4], (Vector2){tile_width * 63, screenHeight - tile_height * 2}, WHITE);
            DrawTextureRec(terrain, relvado[4], (Vector2){tile_width * 62, screenHeight - tile_height * 3}, WHITE);
            DrawTextureRec(terrain, relvado[4], (Vector2){tile_width * 63, screenHeight - tile_height * 3}, WHITE);
            DrawTextureRec(terrain, bloco_cobre, (Vector2){tile_width * 64, screenHeight - tile_height * 3}, WHITE);

            DrawTextureRec(terrain, relvado[0], (Vector2){tile_width * 58, screenHeight - tile_height * 5}, WHITE);
            DrawTextureRec(terrain, relvado[3], (Vector2){tile_width * 59, screenHeight - tile_height * 5}, WHITE);
            DrawTextureRec(terrain, relvado[6], (Vector2){tile_width * 60, screenHeight - tile_height * 5}, WHITE);
            DrawTextureRec(terrain, relvado[4], (Vector2){tile_width * 58, screenHeight - tile_height * 4}, WHITE);
            DrawTextureRec(terrain, relvado[4], (Vector2){tile_width * 59, screenHeight - tile_height * 4}, WHITE);
            DrawTextureRec(terrain, relvado[8], (Vector2){tile_width * 60, screenHeight - tile_height * 4}, WHITE);
            DrawTextureRec(terrain, cobre[1], (Vector2){tile_width * 57, screenHeight - tile_height * 3}, WHITE);
            DrawTextureRec(terrain, cobre[1], (Vector2){tile_width * 57, screenHeight - tile_height * 4}, WHITE);
            DrawTextureRec(terrain, cobre[1], (Vector2){tile_width * 56, screenHeight - tile_height * 3}, WHITE);
            DrawTextureRec(terrain, relvado[5], (Vector2){tile_width * 58, screenHeight - tile_height * 3}, WHITE);
            DrawTextureRec(terrain, relvado[8], (Vector2){tile_width * 59, screenHeight - tile_height * 3}, WHITE);

            DrawTextureRec(terrain, relvado[0], (Vector2){tile_width * 63, screenHeight - tile_height * 4}, WHITE);
            DrawTextureRec(terrain, relvado[3], (Vector2){tile_width * 64, screenHeight - tile_height * 4}, WHITE);
            DrawTextureRec(terrain, relvado[3], (Vector2){tile_width * 64, screenHeight - tile_height * 4}, WHITE);
            DrawTextureRec(terrain, relvado[6], (Vector2){tile_width * 65, screenHeight - tile_height * 4}, WHITE);

            DrawTextureRec(terrain, relvado[4], (Vector2){tile_width * 62, screenHeight - tile_height * 4}, WHITE);
            DrawTextureRec(terrain, relvado[4], (Vector2){tile_width * 61, screenHeight - tile_height * 4}, WHITE);
            DrawTextureRec(terrain, relvado[4], (Vector2){tile_width * 62, screenHeight - tile_height * 5}, WHITE);
            DrawTextureRec(terrain, relvado[4], (Vector2){tile_width * 61, screenHeight - tile_height * 5}, WHITE);
            DrawTextureRec(terrain, relvado[4], (Vector2){tile_width * 63, screenHeight - tile_height * 5}, WHITE);
            DrawTextureRec(terrain, relvado[4], (Vector2){tile_width * 64, screenHeight - tile_height * 5}, WHITE);
            DrawTextureRec(terrain, relvado[4], (Vector2){tile_width * 65, screenHeight - tile_height * 5}, WHITE);
            DrawTextureRec(terrain, relvado[4], (Vector2){tile_width * 62, screenHeight - tile_height * 6}, WHITE);
            DrawTextureRec(terrain, relvado[4], (Vector2){tile_width * 61, screenHeight - tile_height * 6}, WHITE);
            DrawTextureRec(terrain, relvado[4], (Vector2){tile_width * 63, screenHeight - tile_height * 6}, WHITE);
            DrawTextureRec(terrain, relvado[4], (Vector2){tile_width * 64, screenHeight - tile_height * 6}, WHITE);
            DrawTextureRec(terrain, relvado[4], (Vector2){tile_width * 65, screenHeight - tile_height * 6}, WHITE);
            DrawTextureRec(terrain, relvado[0], (Vector2){tile_width * 61, screenHeight - tile_height * 7}, WHITE);
            DrawTextureRec(terrain, relvado[3], (Vector2){tile_width * 62, screenHeight - tile_height * 7}, WHITE);
            DrawTextureRec(terrain, relvado[3], (Vector2){tile_width * 63, screenHeight - tile_height * 7}, WHITE);
            DrawTextureRec(terrain, relvado[3], (Vector2){tile_width * 64, screenHeight - tile_height * 7}, WHITE);
            DrawTextureRec(terrain, relvado[6], (Vector2){tile_width * 65, screenHeight - tile_height * 7}, WHITE);

            DrawTextureRec(terrain, cobre[1], (Vector2){tile_width * 60, screenHeight - tile_height * 6}, WHITE);

            // Desenhando o personagem
            if (is_jumping)
            {
                DrawTextureRec(player_jumping, player_jumping_frame_rec, player_pos, WHITE);
            }
            else if (is_runing)
            {
                DrawTextureRec(player_runing, player_runing_frame_rec, player_pos, WHITE);
            }
            else
            {
                DrawTextureRec(player, player_frame_rec, player_pos, WHITE);
            }
        }

        EndDrawing();
    }

    CloseAudioDevice();
    CloseWindow();

    return 0;
}
