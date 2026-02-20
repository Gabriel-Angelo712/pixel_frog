#include "raylib.h"

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
    float tile_delay = 1.0f; // tempo entre grupos de tiles
    int tiles_per_step = 2;
    float tile_width = 16;
    float tile_heigh = 16;

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
    // Texture2D start_flag = LoadTexture("./assets/Items/Checkpoints/Start/Start_(Moving).png");
    Texture2D bg_tile = LoadTexture("./assets/Background/Brown.png");
    Texture2D terrain = LoadTexture("./assets/Terrain/Terrain.png");

    // Definindo shapes
    Rectangle play_btn_shape = {500, 460, 65, 65};

    Rectangle cobre[12] = {
        {192, 0, tile_width, tile_heigh},  // barra horizontal (extremo esquerdo)
        {192, 16, tile_width, tile_heigh}, // barra horizontal (centro)
        {208, 0, tile_width, tile_heigh},  // barra horizontal (extremo direito)
        {208, 16, tile_width, tile_heigh}, // barra vertical (extremo superior)
        {208, 32, tile_width, tile_heigh}, // barra vertical (centro)
        {224, 0, tile_width, tile_heigh},  // barra vertical (extremo inferior)
        {224, 16, tile_width, tile_heigh}, // bloco isolado
        {224, 32, tile_width, tile_heigh}, // bloco (extremo superior esquerdo)
        {240, 0, tile_width, tile_heigh},  // bloco (extremo superior direito)
        {240, 16, tile_width, tile_heigh}, // bloco (extremo inferior esquerdo)
        {240, 32, tile_width, tile_heigh}, // bloco (extremo inferior direito)
    };

    Rectangle madeira[8] = {
        {0, 64, tile_width, tile_heigh},  // extremo superior esquerdo
        {0, 80, tile_width, tile_heigh},  // lateral esqerda
        {0, 96, tile_width, tile_heigh},  // extremo inferior esquerdo
        {16, 96, tile_width, tile_heigh}, // centro base
        {32, 96, tile_width, tile_heigh}, // extremo inferior direito
        {32, 80, tile_width, tile_heigh}, // lateral direita
        {32, 64, tile_width, tile_heigh}, // extremo superior direito
        {16, 64, tile_width, tile_heigh}, // centro superior
    };

    Rectangle pedra_2[12] = {
        {192, 64, tile_width, tile_heigh}, // barra horizontal (extremo esquerdo)
        {208, 64, tile_width, tile_heigh}, // barra horizontal (centro)
        {224, 64, tile_width, tile_heigh}, // barra horizontal (extremo direito)
        {240, 64, tile_width, tile_heigh}, // barra vertical (extremo superior)
        {240, 80, tile_width, tile_heigh}, // barra vertical (centro)
        {240, 96, tile_width, tile_heigh}, // barra vertical (extremo inferior)
        {192, 80, tile_width, tile_heigh}, // bloco isolado
        {208, 80, tile_width, tile_heigh}, // bloco (extremo superior esquerdo)
        {224, 80, tile_width, tile_heigh}, // bloco (extremo superior direito)
        {208, 96, tile_width, tile_heigh}, // bloco (extremo inferior esquerdo)
        {224, 96, tile_width, tile_heigh}, // bloco (extremo inferior direito)
    };

    Rectangle terra_seca[14] = {
        {96, 64, tile_width, tile_heigh},
        {112, 64, tile_width, tile_heigh},
        {128, 64, tile_width, tile_heigh},
        {96, 96, tile_width, tile_heigh},
        {112, 96, tile_width, tile_heigh},
        {128, 96, tile_width, tile_heigh},
        {96, 80, tile_width, tile_heigh},
        {112, 80, tile_width, tile_heigh},
        {128, 80, tile_width, tile_heigh},
    };

    Rectangle relvado[10] = {
        {96, 0, tile_width, tile_heigh},
        {96, 16, tile_width, tile_heigh},
        {96, 32, tile_width, tile_heigh},
        {112, 0, tile_width, tile_heigh},
        {112, 16, tile_width, tile_heigh},
        {112, 32, tile_width, tile_heigh},
        {128, 0, tile_width, tile_heigh},
        {128, 16, tile_width, tile_heigh},
        {128, 32, tile_width, tile_heigh},
    };

    Rectangle white_space = {16, 16, tile_width, tile_heigh};
    Rectangle left_rock = {0, 16, tile_width, tile_heigh};
    Rectangle right_rock = {32, 16, tile_width, tile_heigh};

    // Definição de vetores
    Vector2 bg_pos = {0, 0};
    Vector2 title_pos = {300, 150};
    Vector2 play_btn_pos = {play_btn_shape.x, play_btn_shape.y};

    // Tocando a música de fundo (default)
    PlayMusicStream(default_music);

    // Configurando de frames por segundo
    SetTargetFPS(20);

    // game loop
    while (!WindowShouldClose())
    {

        // Desenhando na tela
        BeginDrawing();

        // Tela de início (elememntos básicos)
        if (init_screen)
        {
            UpdateMusicStream(default_music);
            DrawTextureEx(bg, bg_pos, 0.0f, 0.85f, WHITE);
            DrawTextEx(default_font, "PIXEL ADVENTURE", title_pos, 70, 2, BLACK);
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
                DrawTextureRec(terrain, madeira[3], (Vector2){i, screenHeight - tile_heigh}, WHITE);

                if (i < 16 * 6 || (i > 16 * 10 && i < screenHeight - tile_heigh))
                {
                    DrawTextureRec(terrain, left_rock, (Vector2){0, i}, WHITE);
                }

                if (i < 16 * 26 || (i > 16 * 30 && i < screenHeight - tile_heigh))
                {
                    DrawTextureRec(terrain, right_rock, (Vector2){screenWidth - tile_width, i}, WHITE);
                }

                // Achar o termo geral e substituir por um loop
                if (i == 16 * 6)
                {

                    // LADO ESQUERDO

                    DrawTextureRec(terrain, pedra_2[3], (Vector2){0, i}, WHITE);
                    DrawTextureRec(terrain, pedra_2[4], (Vector2){0, i + tile_heigh}, WHITE);
                    DrawTextureRec(terrain, pedra_2[5], (Vector2){0, i + (tile_heigh * 2)}, WHITE);
                    DrawTextureRec(terrain, pedra_2[0], (Vector2){0, i + (tile_heigh * 3)}, WHITE);
                    DrawTextureRec(terrain, pedra_2[6], (Vector2){0, i + (tile_heigh * 4)}, WHITE);
                    DrawTextureRec(terrain, pedra_2[6], (Vector2){tile_width, i + (tile_heigh * 2)}, WHITE);
                    DrawTextureRec(terrain, pedra_2[1], (Vector2){tile_width, i + (tile_heigh * 3)}, WHITE);
                    DrawTextureRec(terrain, pedra_2[1], (Vector2){tile_width * 2, i + (tile_heigh * 3)}, WHITE);
                    DrawTextureRec(terrain, pedra_2[2], (Vector2){tile_width * 3, i + (tile_heigh * 3)}, WHITE);
                }

                // Achar o termo geral e substituir por um loop
                if (i == 16 * 26)
                {
                    // LADO DIREITO

                    DrawTextureRec(terrain, pedra_2[3], (Vector2){screenWidth - tile_width, i}, WHITE);
                    DrawTextureRec(terrain, pedra_2[4], (Vector2){screenWidth - tile_width, i + tile_heigh}, WHITE);
                    DrawTextureRec(terrain, pedra_2[5], (Vector2){screenWidth - tile_width, i + (tile_heigh * 2)}, WHITE);
                    DrawTextureRec(terrain, pedra_2[6], (Vector2){screenWidth - (tile_width * 2), i + (tile_heigh * 2)}, WHITE);
                    DrawTextureRec(terrain, pedra_2[0], (Vector2){screenWidth - (tile_width * 4), i + (tile_heigh * 3)}, WHITE);
                    DrawTextureRec(terrain, pedra_2[1], (Vector2){screenWidth - (tile_width * 2), i + (tile_heigh * 3)}, WHITE);
                    DrawTextureRec(terrain, pedra_2[1], (Vector2){screenWidth - (tile_width * 3), i + (tile_heigh * 3)}, WHITE);
                    DrawTextureRec(terrain, pedra_2[2], (Vector2){screenWidth - tile_width, i + (tile_heigh * 3)}, WHITE);
                    DrawTextureRec(terrain, pedra_2[6], (Vector2){screenWidth - tile_width, i + (tile_heigh * 4)}, WHITE);
                }
            }

            DrawTextureRec(terrain, cobre[6], (Vector2){0, 0}, WHITE);
            DrawTextureRec(terrain, cobre[6], (Vector2){screenWidth - tile_width, 0}, WHITE);
            DrawTextureRec(terrain, cobre[6], (Vector2){screenWidth - tile_width, screenHeight - tile_heigh}, WHITE);
            DrawTextureRec(terrain, cobre[6], (Vector2){0, screenHeight - tile_heigh}, WHITE);

            DrawTextureRec(terrain, terra_seca[0], (Vector2){tile_width, screenHeight - tile_heigh * 4}, WHITE);
            DrawTextureRec(terrain, terra_seca[6], (Vector2){tile_width, screenHeight - tile_heigh * 3}, WHITE);
            DrawTextureRec(terrain, terra_seca[3], (Vector2){tile_width, screenHeight - tile_heigh * 2}, WHITE);

            DrawTextureRec(terrain, terra_seca[1], (Vector2){tile_width * 2, screenHeight - tile_heigh * 4}, WHITE);
            DrawTextureRec(terrain, terra_seca[7], (Vector2){tile_width * 2, screenHeight - tile_heigh * 3}, WHITE);
            DrawTextureRec(terrain, terra_seca[4], (Vector2){tile_width * 2, screenHeight - tile_heigh * 2}, WHITE);

            DrawTextureRec(terrain, terra_seca[1], (Vector2){tile_width * 3, screenHeight - tile_heigh * 4}, WHITE);
            DrawTextureRec(terrain, terra_seca[7], (Vector2){tile_width * 3, screenHeight - tile_heigh * 3}, WHITE);
            DrawTextureRec(terrain, terra_seca[4], (Vector2){tile_width * 3, screenHeight - tile_heigh * 2}, WHITE);

            DrawTextureRec(terrain, terra_seca[2], (Vector2){tile_width * 4, screenHeight - tile_heigh * 4}, WHITE);
            DrawTextureRec(terrain, terra_seca[8], (Vector2){tile_width * 4, screenHeight - tile_heigh * 3}, WHITE);
            DrawTextureRec(terrain, terra_seca[5], (Vector2){tile_width * 4, screenHeight - tile_heigh * 2}, WHITE);

            DrawTextureRec(terrain, relvado[0], (Vector2){tile_width * 5, screenHeight - tile_heigh * 5}, WHITE);
            DrawTextureRec(terrain, relvado[3], (Vector2){tile_width * 6, screenHeight - tile_heigh * 5}, WHITE);
            DrawTextureRec(terrain, relvado[3], (Vector2){tile_width * 7, screenHeight - tile_heigh * 5}, WHITE);
            DrawTextureRec(terrain, relvado[6], (Vector2){tile_width * 8, screenHeight - tile_heigh * 5}, WHITE);

            DrawTextureRec(terrain, relvado[1], (Vector2){tile_width * 5, screenHeight - tile_heigh * 4}, WHITE);
            DrawTextureRec(terrain, relvado[4], (Vector2){tile_width * 6, screenHeight - tile_heigh * 4}, WHITE);
            DrawTextureRec(terrain, relvado[4], (Vector2){tile_width * 7, screenHeight - tile_heigh * 4}, WHITE);
            DrawTextureRec(terrain, relvado[7], (Vector2){tile_width * 8, screenHeight - tile_heigh * 4}, WHITE);

            DrawTextureRec(terrain, relvado[1], (Vector2){tile_width * 5, screenHeight - tile_heigh * 3}, WHITE);
            DrawTextureRec(terrain, relvado[4], (Vector2){tile_width * 6, screenHeight - tile_heigh * 3}, WHITE);
            DrawTextureRec(terrain, relvado[7], (Vector2){tile_width * 7, screenHeight - tile_heigh * 3}, WHITE);

            DrawTextureRec(terrain, cobre[0], (Vector2){tile_width * 8, screenHeight - tile_heigh * 3}, WHITE);
            DrawTextureRec(terrain, cobre[5], (Vector2){tile_width * 9, screenHeight - tile_heigh * 3}, WHITE);
        }

        EndDrawing();
    }

    CloseAudioDevice();
    CloseWindow();

    return 0;
}