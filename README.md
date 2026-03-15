# Pixel Frog

Um jogo de plataformas 2D em pixel art desenvolvido em **C** com a biblioteca **Raylib**, criado pelo estúdio **Anchor Studios**.

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

### Compilação

```bash
gcc main.c -o pixel_frog -lraylib -lm
```

### Execução

```bash
./pixel_frog
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
Desenvolvido por **Gabrie João**