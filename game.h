/* NUM FIGHT ASCII - clone original inspirado em Yomi Hustle (nenhum codigo/asset do original) */
#ifndef GAME_H
#define GAME_H

#define SCR_W 640
#define SCR_H 448

/* botoes (bits) - a camada de plataforma mapeia para estes */
enum {
    BTN_UP = 1, BTN_DOWN = 2, BTN_LEFT = 4, BTN_RIGHT = 8,
    BTN_CROSS = 16, BTN_CIRCLE = 32, BTN_START = 64
};

/* implementado pela plataforma: retangulo preenchido, cor 0xRRGGBB, coords em 640x448 */
void gfx_rect(int x, int y, int w, int h, unsigned rgb);

void game_init(unsigned seed);
void game_update(unsigned pressed);   /* chamar 1x por frame (60Hz); pressed = botoes recem apertados */
void game_draw(void);
void game_init_seed_hint(unsigned s);   /* mistura entropia (ex.: tempo de input) no RNG */

/* utilidades de teste */
int  game_screen(void);
int  game_sim(int c0, int c1, int d0, int d1, unsigned seed);  /* CPU vs CPU: 0, 1 ou -1 (empate) */
int  game_class_count(void);
const char *game_class_name(int c);

#endif
