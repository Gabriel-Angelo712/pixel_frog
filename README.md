# Pixel Frog

Um jogo de plataformas 2D em pixel art desenvolvido em **C** com a biblioteca **Raylib**.

---

## Sobre o Jogo

Pixel Frog é um jogo de plataformas clássico onde controlas um sapo ninja num nível cheio de obstáculos. O objetivo é navegar pelos vários blocos e plataformas, evitando armadilhas mortais pelo caminho.

---

## Funcionalidades

- **Menu inicial** com animação de fade e música de fundo
- **Nível construído com tiles** a partir de spritesheets (terreno, pedra, relvado, cobre, madeira)
- **Personagem animado** com 3 estados: idle, salto e corrida
- **Armadilhas:**
  - Serra rotativa com movimento sinusoidal vertical
  - Plataforma móvel com movimento horizontal
  - Espinhos fixos
- **Transições suaves** entre ecrãs com fade a preto e fade de áudio
- **Efeitos sonoros** e músicas de fundo

---

## Controlos

|   Tecla   |   Ação   |
|-----------|----------|
|  `ESPAÇO` |  Saltar  |
|    `→`    |  Correr  |

---

## Tecnologias

- **Linguagem:** C
- **Biblioteca:** [Raylib](https://www.raylib.com/)

---

## Estrutura do Projeto

```
/
├── assets/
│   ├── background1.png
│   ├── Background/
│   ├── Main_Characters/Ninja_Frog/
│   ├── Menu/Buttons/
│   ├── Terrain/
│   └── Traps/
├── audios/
│   ├── default.mp3
│   ├── default2.mp3
│   └── play_btn_click.wav
├── fonts/
│   └── Pixelify_Sans/
└── main.c
```

---

## Como Compilar e Executar

### Pré-requisitos

- [Raylib](https://www.raylib.com/) instalado
- Compilador C (GCC ou Clang)

### Compilação e execução (usando notepad++ configurado para rodar raylib 4.2)

```bash
echo > Setup required Environment
echo -------------------------------------
SET RAYLIB_PATH=C:\raylib\raylib
SET COMPILER_PATH=C:\raylib\w64devkit\bin
ENV_SET PATH=$(COMPILER_PATH)
SET CC=gcc
SET CFLAGS=$(RAYLIB_PATH)\src\raylib.rc.data -s -static -O2 -std=c99 -Wall -I$(RAYLIB_PATH)\src -Iexternal -DPLATFORM_DESKTOP
SET LDFLAGS=-lraylib -lopengl32 -lgdi32 -lwinmm
cd $(CURRENT_DIRECTORY)
echo
echo > Clean latest build
echo ------------------------
cmd /c IF EXIST $(NAME_PART).exe del /F $(NAME_PART).exe
echo
echo > Saving Current File
echo -------------------------
npp_save
echo
echo > Compile program
echo -----------------------
$(CC) -o $(NAME_PART).exe $(FILE_NAME) $(CFLAGS) $(LDFLAGS)
echo
echo > Reset Environment
echo --------------------------
ENV_UNSET PATH
echo
echo > Execute program
echo -----------------------
cmd /c IF EXIST $(NAME_PART).exe $(NAME_PART).exe

```
---

## Estado do Desenvolvimento

O projeto encontra-se em **fase inicial**. Funcionalidades planeadas:

- [ ] Sistema de colisão completo
- [ ] Câmera com scroll horizontal
- [ ] Condições de vitória e derrota
- [ ] Sistema de vidas / pontuação
- [ ] Ecrã de game over

---

## Créditos
Desenvolvido por **Gabrie Ângelo**
