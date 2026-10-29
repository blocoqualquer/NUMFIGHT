#include "game.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

/* ------------------------------------------------------------------ */
/* fonte bitmap 5x7                                                    */
/* ------------------------------------------------------------------ */
static const char FONT_CHARS[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789?.+-><!:/*";
static const unsigned char FONT[][7] = {
    {0b01110,0b10001,0b10001,0b11111,0b10001,0b10001,0b10001}, /* A */
    {0b11110,0b10001,0b10001,0b11110,0b10001,0b10001,0b11110}, /* B */
    {0b01110,0b10001,0b10000,0b10000,0b10000,0b10001,0b01110}, /* C */
    {0b11110,0b10001,0b10001,0b10001,0b10001,0b10001,0b11110}, /* D */
    {0b11111,0b10000,0b10000,0b11110,0b10000,0b10000,0b11111}, /* E */
    {0b11111,0b10000,0b10000,0b11110,0b10000,0b10000,0b10000}, /* F */
    {0b01110,0b10001,0b10000,0b10111,0b10001,0b10001,0b01111}, /* G */
    {0b10001,0b10001,0b10001,0b11111,0b10001,0b10001,0b10001}, /* H */
    {0b01110,0b00100,0b00100,0b00100,0b00100,0b00100,0b01110}, /* I */
    {0b00111,0b00010,0b00010,0b00010,0b00010,0b10010,0b01100}, /* J */
    {0b10001,0b10010,0b10100,0b11000,0b10100,0b10010,0b10001}, /* K */
    {0b10000,0b10000,0b10000,0b10000,0b10000,0b10000,0b11111}, /* L */
    {0b10001,0b11011,0b10101,0b10101,0b10001,0b10001,0b10001}, /* M */
    {0b10001,0b11001,0b10101,0b10011,0b10001,0b10001,0b10001}, /* N */
    {0b01110,0b10001,0b10001,0b10001,0b10001,0b10001,0b01110}, /* O */
    {0b11110,0b10001,0b10001,0b11110,0b10000,0b10000,0b10000}, /* P */
    {0b01110,0b10001,0b10001,0b10001,0b10101,0b10010,0b01101}, /* Q */
    {0b11110,0b10001,0b10001,0b11110,0b10100,0b10010,0b10001}, /* R */
    {0b01111,0b10000,0b10000,0b01110,0b00001,0b00001,0b11110}, /* S */
    {0b11111,0b00100,0b00100,0b00100,0b00100,0b00100,0b00100}, /* T */
    {0b10001,0b10001,0b10001,0b10001,0b10001,0b10001,0b01110}, /* U */
    {0b10001,0b10001,0b10001,0b10001,0b10001,0b01010,0b00100}, /* V */
    {0b10001,0b10001,0b10001,0b10101,0b10101,0b10101,0b01010}, /* W */
    {0b10001,0b10001,0b01010,0b00100,0b01010,0b10001,0b10001}, /* X */
    {0b10001,0b10001,0b01010,0b00100,0b00100,0b00100,0b00100}, /* Y */
    {0b11111,0b00001,0b00010,0b00100,0b01000,0b10000,0b11111}, /* Z */
    {0b01110,0b10011,0b10101,0b10101,0b10101,0b11001,0b01110}, /* 0 (cortado) */
    {0b00100,0b01100,0b00100,0b00100,0b00100,0b00100,0b01110}, /* 1 */
    {0b01110,0b10001,0b00001,0b00010,0b00100,0b01000,0b11111}, /* 2 */
    {0b11110,0b00001,0b00001,0b01110,0b00001,0b00001,0b11110}, /* 3 */
    {0b00010,0b00110,0b01010,0b10010,0b11111,0b00010,0b00010}, /* 4 */
    {0b11111,0b10000,0b11110,0b00001,0b00001,0b10001,0b01110}, /* 5 */
    {0b00110,0b01000,0b10000,0b11110,0b10001,0b10001,0b01110}, /* 6 */
    {0b11111,0b00001,0b00010,0b00100,0b01000,0b01000,0b01000}, /* 7 */
    {0b01110,0b10001,0b10001,0b01110,0b10001,0b10001,0b01110}, /* 8 */
    {0b01110,0b10001,0b10001,0b01111,0b00001,0b00010,0b01100}, /* 9 */
    {0b01110,0b10001,0b00001,0b00010,0b00100,0b00000,0b00100}, /* ? */
    {0b00000,0b00000,0b00000,0b00000,0b00000,0b01100,0b01100}, /* . */
    {0b00000,0b00100,0b00100,0b11111,0b00100,0b00100,0b00000}, /* + */
    {0b00000,0b00000,0b00000,0b11111,0b00000,0b00000,0b00000}, /* - */
    {0b10000,0b01000,0b00100,0b00010,0b00100,0b01000,0b10000}, /* > */
    {0b00001,0b00010,0b00100,0b01000,0b00100,0b00010,0b00001}, /* < */
    {0b00100,0b00100,0b00100,0b00100,0b00100,0b00000,0b00100}, /* ! */
    {0b00000,0b01100,0b01100,0b00000,0b01100,0b01100,0b00000}, /* : */
    {0b00001,0b00001,0b00010,0b00100,0b01000,0b10000,0b10000}, /* / */
    {0b00000,0b00100,0b10101,0b01110,0b10101,0b00100,0b00000}, /* * */
};

static void draw_glyph(int x, int y, char ch, int sc, unsigned col)
{
    const char *p;
    int idx, r, c, s;
    if (ch >= 'a' && ch <= 'z') ch = (char)(ch - 32);
    p = strchr(FONT_CHARS, ch);
    if (!p || ch == 0) return;
    idx = (int)(p - FONT_CHARS);
    for (r = 0; r < 7; r++) {
        unsigned char bits = FONT[idx][r];
        c = 0;
        while (c < 5) {
            if (bits & (0x10 >> c)) {
                s = c;
                while (c < 5 && (bits & (0x10 >> c))) c++;
                gfx_rect(x + s * sc, y + r * sc, (c - s) * sc, sc, col);
            } else c++;
        }
    }
}

static int text_w(const char *s, int sc) { int n = (int)strlen(s); return n ? n * 6 * sc - sc : 0; }

static void text(int x, int y, const char *s, int sc, unsigned col)
{
    for (; *s; s++, x += 6 * sc) draw_glyph(x, y, *s, sc, col);
}
static void text_c(int cx, int y, const char *s, int sc, unsigned col) { text(cx - text_w(s, sc) / 2, y, s, sc, col); }
static void text_r(int rx, int y, const char *s, int sc, unsigned col) { text(rx - text_w(s, sc), y, s, sc, col); }

static void rect_o(int x, int y, int w, int h, int t, unsigned col)
{
    gfx_rect(x, y, w, t, col);
    gfx_rect(x, y + h - t, w, t, col);
    gfx_rect(x, y, t, h, col);
    gfx_rect(x + w - t, y, t, h, col);
}

/* triangulo apontando para a esquerda (dir=-1) ou direita (dir=1) */
static void tri(int x, int y, int h, int dir, unsigned col)
{
    int i, half = h / 2;
    for (i = 0; i < h; i++) {
        int d = i < half ? i : h - 1 - i;      /* 0..half */
        int w = d * 2 + 2;
        if (dir < 0) gfx_rect(x + (half * 2 + 2) - w, y + i, w, 1, col);
        else gfx_rect(x - (half * 2 + 2), y + i, w, 1, col);
    }
}

#define C_BG     0x1c1c2eu
#define C_DARK   0x0f0f1eu
#define C_WHITE  0xffffffu
#define C_GREEN  0x14a80fu
#define C_RED    0xff0000u
#define C_CYAN   0x2de0ffu
#define C_YELLOW 0xffe030u
#define C_GREY   0x666680u

/* ------------------------------------------------------------------ */
/* dados das classes                                                   */
/* ------------------------------------------------------------------ */
enum { F_PIERCE = 1, F_ROOT = 2, F_RAND = 4, F_DASH = 8, F_STEAL = 16 };

typedef struct {
    const char *name; char sym; const char *desc;
    int hp, dmg, range;
    const char *sp_name; int sp_dmg, sp_range, sp_hits, sp_flags, sp_heal;
    const char *sp_desc;
} Class;

#define NCLASS 11
static const Class CL[NCLASS] = {
 {"MONK",     '0', "PUNCHES THINGS",      100,  9, 1, "FLURRY",   5, 1, 3, 0,       0,  "3 QUICK HITS"},
 {"WARRIOR",  '1', "HITS WITH A SWORD",   110,  9, 1, "BASH",    12, 1, 1, F_PIERCE,0, "BREAKS BLOCKS"},
 {"RANGER",   '2', "SHOOTS FROM FAR",      88,  7, 3, "VOLLEY",  10, 5, 1, 0,       0,  "LONG RANGE SHOT"},
 {"BARBARIAN",'3', "ANGRY. VERY ANGRY.",  100,  8, 1, "RAGE",    16, 1, 1, F_PIERCE,0, "HUGE HIT, NO BLOCK"},
 {"SORCERER", '4', "CHAOS IS A FRIEND",    92,  8, 2, "WILD",     0, 4, 1, F_RAND,  0,  "RANDOM 4-20 DMG"},
 {"DRUID",    '5', "NATURE HATES YOU",     112,  9, 1, "ROOTS",    8, 3, 1, F_ROOT,  0,  "FOE CANT MOVE"},
 {"MAGE",     '6', "BIG BOOM FROM AFAR",   75,  6, 3, "FIREBALL",16, 5, 1, 0,       0,  "BIG LONG RANGE HIT"},
 {"CLERIC",   '7', "HITS AND HEALS",       95,  7, 1, "HEAL",     0, 0, 0, 0,      12,  "HEALS 12 HP"},
 {"WARLOCK",  '8', "STEALS LIFE",          90,  7, 2, "DRAIN",    8, 3, 1, F_STEAL, 0,  "HIT AND HEAL SAME"},
 {"PALADIN",  '9', "HOLY SMITING",        105,  8, 1, "SMITE",   14, 1, 1, 0,       4,  "HITS AND HEALS 4"},
 {"ROGUE",    '+', "STABS FROM BEHIND",    90,  8, 1, "BACKSTAB",13, 6, 1, F_DASH,  0,  "DASHES THEN STABS"},
};

int game_class_count(void) { return NCLASS; }
const char *game_class_name(int c) { return CL[c].name; }

/* ------------------------------------------------------------------ */
/* estado                                                              */
/* ------------------------------------------------------------------ */
#define ARENA 16
#define CELL  (SCR_W / ARENA)
#define FLOOR 335
#define GRAB_DMG 12
#define SPEC_COST 3

enum { A_ADV, A_RET, A_STRIKE, A_GRAB, A_BLOCK, A_SPEC, A_NONE };
static const char *ACT_NAME[] = {"ADVANCE", "RETREAT", "STRIKE", "GRAB", "BLOCK", "SPECIAL", "WAIT"};
static const int MENU[6] = {A_ADV, A_STRIKE, A_BLOCK, A_RET, A_GRAB, A_SPEC};

enum { S_TITLE, S_CLASS, S_CONFIG, S_BATTLE, S_RESULT };

typedef struct {
    int cls, hp, maxhp, pos, meter, rooted;
    int flash, shake, dnum, dtimer, act, lastact;
} Fighter;

static Fighter P[2];
static unsigned rng_s = 1;
static int screen, sel_title, sel_class, cfg_diff = 1, cfg_foe = -1, cfg_sel;
static int menu_cur, phase, timer, result, nodice_msg, frame_no;
static char LOG[7][40];
static int nlog;

static int rnd(int n) { rng_s = rng_s * 1664525u + 1013904223u; return (int)((rng_s >> 16) % (unsigned)n); }
int game_screen(void) { return screen; }

static void logf_(const char *fmt, ...)
{
    va_list ap;
    if (nlog >= 7) return;
    va_start(ap, fmt);
    vsnprintf(LOG[nlog], sizeof LOG[0], fmt, ap);
    va_end(ap);
    nlog++;
}

static const char *aname(int who, int act, int spec)
{
    if (spec && act == A_SPEC) return CL[P[who].cls].sp_name;
    return ACT_NAME[act];
}

/* ------------------------------------------------------------------ */
/* IA                                                                  */
/* ------------------------------------------------------------------ */
static int cpu_pick(int me, int diff)
{
    Fighter *f = &P[me], *o = &P[1 - me];
    const Class *c = &CL[f->cls];
    int dist = abs(P[1].pos - P[0].pos), w[6] = {0, 0, 0, 0, 0, 0}, i, sum = 0, r;
    int canspec = f->meter >= SPEC_COST;

    if (diff == 0) {
        for (i = 0; i < 6; i++) w[i] = 1;
        if (!canspec) w[A_SPEC] = 0;
        if (dist > 1) w[A_GRAB] = 0;
    } else {
        if (dist <= c->range) {
            w[A_STRIKE] = 3; w[A_BLOCK] = 2; w[A_RET] = 1;
            if (dist <= 1) w[A_GRAB] = 2;
        } else {
            w[A_ADV] = 4; w[A_RET] = 1; w[A_BLOCK] = 1;
        }
        if (c->range > 1 && dist <= 1) w[A_RET] = 4;
        if (canspec) {
            if (c->sp_dmg == 0 && c->sp_heal) w[A_SPEC] = (f->hp * 100 / f->maxhp < 70) ? 7 : 0;
            else if (dist <= c->sp_range) w[A_SPEC] = 4;
        }
        if (diff >= 2 && rnd(10) < 6) {   /* tenta ler o jogador */
            switch (o->lastact) {
            case A_STRIKE: w[A_BLOCK] += 4; break;
            case A_GRAB:   w[A_STRIKE] += 4; break;
            case A_BLOCK:  if (dist <= 1) w[A_GRAB] += 4; break;
            case A_ADV:    w[A_STRIKE] += 3; break;
            case A_RET:    w[A_ADV] += 3; break;
            default: break;
            }
        }
    }
    for (i = 0; i < 6; i++) sum += w[i];
    if (sum == 0) return A_BLOCK;
    r = rnd(sum);
    for (i = 0; i < 6; i++) { if (r < w[i]) return i; r -= w[i]; }
    return A_BLOCK;
}

/* ------------------------------------------------------------------ */
/* resolucao do turno (simultanea)                                     */
/* ------------------------------------------------------------------ */
static int clampi(int v, int a, int b) { return v < a ? a : v > b ? b : v; }

static void resolve(int a0, int a1)
{
    int act[2], spec[2] = {0, 0}, np[2], rooted[2], i, j;
    int type[2], dmg[2], rng[2], hits[2], pierce[2];
    int hit[2] = {0, 0}, blocked[2] = {0, 0}, stuffed[2] = {0, 0}, teched = 0, dist;
    int total[2] = {0, 0};

    act[0] = a0; act[1] = a1;
    nlog = 0;

    for (i = 0; i < 2; i++) {
        Fighter *f = &P[i];
        if (act[i] == A_SPEC) {
            if (f->meter >= SPEC_COST) { f->meter -= SPEC_COST; spec[i] = 1; }
            else act[i] = A_NONE;
        }
        f->act = act[i]; f->lastact = act[i]; f->dtimer = 0;
    }
    logf_("YOU: %s", aname(0, act[0], spec[0]));
    logf_("FOE: %s", aname(1, act[1], spec[1]));

    /* --- movimento --- */
    np[0] = P[0].pos; np[1] = P[1].pos;
    rooted[0] = P[0].rooted; rooted[1] = P[1].rooted;
    P[0].rooted = P[1].rooted = 0;
    for (i = 0; i < 2; i++) {
        int dir = i == 0 ? 1 : -1;
        if (act[i] == A_ADV || act[i] == A_RET) {
            if (rooted[i]) { logf_("%s IS ROOTED", i ? "FOE" : "YOU"); continue; }
            np[i] += (act[i] == A_ADV ? 2 : -2) * dir;
        }
        np[i] = clampi(np[i], 0, ARENA - 1);
    }
    if (np[0] >= np[1]) {
        int m = (np[0] + np[1]) / 2;
        np[0] = m; np[1] = m + 1;
        if (np[1] >= ARENA) { np[1] = ARENA - 1; np[0] = ARENA - 2; }
    }
    for (i = 0; i < 2; i++) {
        const Class *c = &CL[P[i].cls];
        if (spec[i] && (c->sp_flags & F_DASH)) {
            int d = np[1] - np[0];
            if (d <= c->sp_range && d > 1) { if (i == 0) np[0] = np[1] - 1; else np[1] = np[0] + 1; }
        }
    }
    P[0].pos = np[0]; P[1].pos = np[1];
    dist = np[1] - np[0];

    /* --- tabela de ataques --- */
    for (i = 0; i < 2; i++) {
        const Class *c = &CL[P[i].cls];
        type[i] = 0; dmg[i] = 0; rng[i] = 0; hits[i] = 1; pierce[i] = 0;
        if (act[i] == A_STRIKE) { type[i] = 1; dmg[i] = c->dmg; rng[i] = c->range; }
        else if (act[i] == A_GRAB) { type[i] = 2; dmg[i] = GRAB_DMG; rng[i] = 1; }
        else if (spec[i] && (c->sp_dmg > 0 || (c->sp_flags & F_RAND))) {
            type[i] = 1;
            dmg[i] = (c->sp_flags & F_RAND) ? 4 + rnd(17) : c->sp_dmg;
            rng[i] = (c->sp_flags & F_DASH) ? 1 : c->sp_range;
            hits[i] = c->sp_hits;
            pierce[i] = (c->sp_flags & F_PIERCE) ? 1 : 0;
        }
    }

    /* --- quem acerta quem --- */
    {
        int whiff[2] = {0, 0};
        for (i = 0; i < 2; i++) {
            j = 1 - i;
            if (!type[i]) continue;
            if (dist > rng[i]) { whiff[i] = 1; continue; }
            if (type[i] == 1) {
                if (act[j] == A_BLOCK && !pierce[i]) blocked[i] = 1; else hit[i] = 1;
            } else {
                if (type[j] == 1 && dist <= rng[j]) stuffed[i] = 1;
                else if (type[j] == 2 && dist <= rng[j]) teched = 1;
                else hit[i] = 1;
            }
        }
        if (teched) logf_("GRABS CLASH");
        for (i = 0; i < 2; i++) {
            const char *who = i ? "FOE" : "YOU";
            if (whiff[i]) logf_("%s MISSED", who);
            if (stuffed[i]) logf_("%s GRAB STUFFED", who);
            if (blocked[i]) { logf_("%s WAS BLOCKED", who); P[1 - i].meter++; }
        }
    }

    /* --- aplicar dano/efeitos --- */
    for (i = 0; i < 2; i++) {
        const Class *c = &CL[P[i].cls];
        const char *who = i ? "FOE" : "YOU";
        j = 1 - i;
        if (hit[i]) {
            total[i] = dmg[i] * hits[i];
            logf_("%s HIT FOR %d", who, total[i]);
        }
        if (spec[i] && c->sp_heal && (c->sp_dmg == 0 || hit[i])) {
            int before = P[i].hp;
            P[i].hp = clampi(P[i].hp + c->sp_heal, 0, P[i].maxhp);
            logf_("%s HEALS %d", who, P[i].hp - before);
        }
    }
    for (i = 0; i < 2; i++) {
        const Class *c = &CL[P[i].cls];
        j = 1 - i;
        if (!hit[i]) continue;
        P[j].hp -= total[i];
        P[j].flash = 18; P[j].shake = 10; P[j].dnum = total[i]; P[j].dtimer = 50;
        if (spec[i] && (c->sp_flags & F_STEAL))
            P[i].hp = clampi(P[i].hp + total[i], 0, P[i].maxhp);
        if (spec[i] && (c->sp_flags & F_ROOT)) { P[j].rooted = 1; logf_("%s ROOTED", j ? "FOE" : "YOU"); }
    }
    for (i = 0; i < 2; i++) { P[i].meter = clampi(P[i].meter + 1, 0, 9); if (P[i].hp < 0) P[i].hp = 0; }
}

/* ------------------------------------------------------------------ */
/* controle                                                            */
/* ------------------------------------------------------------------ */
static void fighter_setup(Fighter *f, int cls, int pos)
{
    memset(f, 0, sizeof *f);
    f->cls = cls; f->maxhp = f->hp = CL[cls].hp; f->pos = pos; f->meter = 1;
    f->act = f->lastact = A_NONE;
}

static void battle_start(void)
{
    int foe = cfg_foe < 0 ? rnd(NCLASS) : cfg_foe;
    fighter_setup(&P[0], sel_class, 3);
    fighter_setup(&P[1], foe, 12);
    menu_cur = 1; phase = 0; timer = 0; nlog = 0; nodice_msg = 0;
    screen = S_BATTLE;
}

void game_init(unsigned seed)
{
    rng_s = seed ? seed : 1;
    screen = S_TITLE; sel_title = 0; sel_class = 0; cfg_diff = 1; cfg_foe = -1; frame_no = 0;
}

void game_init_seed_hint(unsigned s) { rng_s ^= s * 2654435761u; }

int game_sim(int c0, int c1, int d0, int d1, unsigned seed)
{
    int turn;
    rng_s = seed;
    fighter_setup(&P[0], c0, 3);
    fighter_setup(&P[1], c1, 12);
    for (turn = 0; turn < 80; turn++) {
        int a0 = cpu_pick(0, d0), a1 = cpu_pick(1, d1);
        resolve(a0, a1);
        if (P[0].hp <= 0 || P[1].hp <= 0) break;
    }
    if (P[0].hp == P[1].hp) return -1;
    if (P[0].hp <= 0 && P[1].hp <= 0) return -1;
    return P[0].hp > P[1].hp ? 0 : 1;
}

static void upd_title(unsigned p)
{
    if (p & BTN_UP) sel_title = (sel_title + 2) % 3;
    if (p & BTN_DOWN) sel_title = (sel_title + 1) % 3;
    if (p & (BTN_CROSS | BTN_START)) {
        if (sel_title == 0) battle_start();
        else if (sel_title == 1) screen = S_CLASS;
        else { screen = S_CONFIG; cfg_sel = 0; }
    }
}

static void upd_class(unsigned p)
{
    int col = sel_class % 3, row = sel_class / 3;
    if (p & BTN_LEFT) col = (col + 2) % 3;
    if (p & BTN_RIGHT) col = (col + 1) % 3;
    if (p & BTN_UP) row = (row + 3) % 4;
    if (p & BTN_DOWN) row = (row + 1) % 4;
    if (row * 3 + col < NCLASS) sel_class = row * 3 + col;
    else if (p & (BTN_LEFT | BTN_RIGHT)) sel_class = row * 3 + (col == 2 ? 0 : 0);
    if (p & BTN_CIRCLE) screen = S_TITLE;
    if (p & (BTN_CROSS | BTN_START)) battle_start();
}

static void upd_config(unsigned p)
{
    int d = 0;
    if (p & BTN_UP) cfg_sel = (cfg_sel + 2) % 3;
    if (p & BTN_DOWN) cfg_sel = (cfg_sel + 1) % 3;
    if (p & BTN_LEFT) d = -1;
    if (p & BTN_RIGHT) d = 1;
    if (cfg_sel == 0 && d) cfg_diff = clampi(cfg_diff + d, 0, 2);
    if (cfg_sel == 1 && d) { cfg_foe += d; if (cfg_foe < -1) cfg_foe = NCLASS - 1; if (cfg_foe >= NCLASS) cfg_foe = -1; }
    if ((p & BTN_CROSS) && cfg_sel == 2) screen = S_TITLE;
    if (p & BTN_CIRCLE) screen = S_TITLE;
}

static void upd_battle(unsigned p)
{
    int i;
    for (i = 0; i < 2; i++) {
        if (P[i].flash > 0) P[i].flash--;
        if (P[i].shake > 0) P[i].shake--;
        if (P[i].dtimer > 0) P[i].dtimer--;
    }
    if (p & BTN_START) { screen = S_TITLE; return; }
    if (phase == 0) {
        int row = menu_cur % 3, col = menu_cur / 3;
        if (p & BTN_UP) row = (row + 2) % 3;
        if (p & BTN_DOWN) row = (row + 1) % 3;
        if (p & (BTN_LEFT | BTN_RIGHT)) col = 1 - col;
        menu_cur = col * 3 + row;
        if (nodice_msg > 0) nodice_msg--;
        if (p & BTN_CROSS) {
            int a = MENU[menu_cur];
            if (a == A_SPEC && P[0].meter < SPEC_COST) { nodice_msg = 60; return; }
            resolve(a, cpu_pick(1, cfg_diff));
            phase = 1; timer = 90;
        }
    } else {
        if ((p & BTN_CROSS) && timer < 80) timer = 0;
        if (timer > 0) timer--;
        if (timer == 0) {
            if (P[0].hp <= 0 || P[1].hp <= 0) {
                result = (P[0].hp <= 0 && P[1].hp <= 0) ? 2 : (P[1].hp <= 0 ? 0 : 1);
                screen = S_RESULT; timer = 0;
            } else phase = 0;
        }
    }
}

void game_update(unsigned p)
{
    frame_no++;
    switch (screen) {
    case S_TITLE:  upd_title(p); break;
    case S_CLASS:  upd_class(p); break;
    case S_CONFIG: upd_config(p); break;
    case S_BATTLE: upd_battle(p); break;
    case S_RESULT: if (p & (BTN_CROSS | BTN_START | BTN_CIRCLE)) screen = S_TITLE; break;
    }
}

/* ------------------------------------------------------------------ */
/* desenho                                                             */
/* ------------------------------------------------------------------ */
static void draw_star(int x, int y, int sc, unsigned col)
{
    static const char *S[9] = {
        "....#....", "....#....", "...###...", "#########", ".#######.",
        "..#####..", "..##.##..", ".##...##.", ".#.....#."};
    int r, c;
    for (r = 0; r < 9; r++) for (c = 0; c < 9; c++)
        if (S[r][c] == '#') gfx_rect(x + c * sc, y + r * sc, sc, sc, col);
}

static void draw_title(void)
{
    static const char *ITEMS[3] = {"START", "CLASS", "CONFIG"};
    int i;
    rect_o(55, 28, 515, 225, 6, C_WHITE);
    text(78, 58, "NUM FIGHT", 9, C_WHITE);
    text(105, 150, "ASCII", 9, C_WHITE);
    draw_star(410, 140, 6, C_WHITE);
    for (i = 0; i < 3; i++) {
        unsigned col = i == sel_title ? C_YELLOW : C_WHITE;
        int y = 300 + i * 38;
        gfx_rect(212, y + 10, 8, 8, col);
        text(232, y, ITEMS[i], 4, col);
    }
    text(8, 436, "X: SELECT", 1, C_GREY);
}

static void draw_class(void)
{
    char buf[48];
    const Class *c = &CL[sel_class];
    int i;
    char s[2] = {0, 0};
    text_c(430, 14, "CLASSES", 6, C_CYAN);
    for (i = 0; i < NCLASS; i++) {
        int col = i % 3, row = i / 3, x = 215 + col * 146, y = 70 + row * 54;
        unsigned k = i == sel_class ? C_YELLOW : C_WHITE;
        s[0] = CL[i].sym;
        text(x, y, s, 3, k);
        text(x + 24, y + 6, CL[i].name, 2, k);
        if (i == sel_class) rect_o(x - 8, y - 8, 142, 40, 2, C_YELLOW);
    }
    s[0] = c->sym;
    text_c(105, 150, s, 14, C_WHITE);
    text_c(105, 270, c->name, 3, C_WHITE);
    text(215, 300, c->desc, 3, C_WHITE);
    snprintf(buf, sizeof buf, "HP %d  DMG %d  RANGE %d", c->hp, c->dmg, c->range);
    text(215, 335, buf, 2, C_CYAN);
    snprintf(buf, sizeof buf, "SPECIAL: %s", c->sp_name);
    text(215, 360, buf, 2, C_YELLOW);
    text(215, 380, c->sp_desc, 2, C_WHITE);
    gfx_rect(190, 408, 110, 32, C_GREEN);
    text_c(245, 416, "START ?", 2, C_WHITE);
    text(8, 436, "O: BACK", 1, C_GREY);
}

static void draw_config(void)
{
    static const char *DN[3] = {"EASY", "NORMAL", "HARD"};
    char buf[40];
    unsigned k;
    text_c(320, 30, "CONFIG", 6, C_CYAN);
    k = cfg_sel == 0 ? C_YELLOW : C_WHITE;
    snprintf(buf, sizeof buf, "CPU: < %s >", DN[cfg_diff]);
    text(130, 150, buf, 4, k);
    k = cfg_sel == 1 ? C_YELLOW : C_WHITE;
    snprintf(buf, sizeof buf, "FOE: < %s >", cfg_foe < 0 ? "RANDOM" : CL[cfg_foe].name);
    text(130, 210, buf, 4, k);
    k = cfg_sel == 2 ? C_YELLOW : C_WHITE;
    text(130, 270, "BACK", 4, k);
    text(8, 436, "LEFT/RIGHT: CHANGE", 1, C_GREY);
}

static void draw_bar(int x, int w, int hp, int maxhp, int meter)
{
    int g = w * hp / maxhp, i;
    char buf[16];
    gfx_rect(x, 34, w, 9, C_RED);
    if (g > 0) gfx_rect(x, 34, g, 9, C_GREEN);
    for (i = 0; i < 9; i++)
        gfx_rect(x + i * 12, 48, 9, 5, i < meter ? (meter >= SPEC_COST ? C_YELLOW : C_CYAN) : 0x33334au);
    snprintf(buf, sizeof buf, "%d/%d", hp, maxhp);
    (void)buf;
}

static void draw_fighter(int i)
{
    Fighter *f = &P[i];
    char s[2] = {CL[f->cls].sym, 0};
    int sc = 5, x = f->pos * CELL + (CELL - 5 * sc) / 2, y = FLOOR - 7 * sc - 4;
    unsigned col = f->flash > 0 && (f->flash & 2) ? C_RED : C_WHITE;
    if (f->shake > 0) x += (f->shake & 1) ? 4 : -4;
    text(x, y, s, sc, col);
    if (f->rooted) gfx_rect(x - 2, FLOOR - 3, 5 * sc + 4, 3, 0x30c030);
    if (phase == 1 && f->act != A_NONE && screen == S_BATTLE)
        text_c(x + 12, y - 18, f->act == A_SPEC ? CL[f->cls].sp_name : ACT_NAME[f->act], 2, C_GREY);
    if (f->dtimer > 0) {
        char b[8];
        snprintf(b, sizeof b, "-%d", f->dnum);
        text_c(x + 12, y - 40 - (50 - f->dtimer) / 3, b, 3, C_RED);
    }
}

static void draw_battle(void)
{
    char s[2] = {0, 0}, buf[40];
    int i;
    /* hud topo */
    s[0] = CL[P[0].cls].sym;
    text(10, 8, s, 4, C_WHITE);
    text(50, 8, CL[P[0].cls].name, 3, C_WHITE);
    draw_bar(50, 225, P[0].hp, P[0].maxhp, P[0].meter);
    s[0] = CL[P[1].cls].sym;
    text_r(630, 8, s, 4, C_WHITE);
    text_r(590, 8, CL[P[1].cls].name, 3, C_WHITE);
    draw_bar(365, 225, P[1].hp, P[1].maxhp, P[1].meter);
    /* cenario */
    gfx_rect(0, 195, 35, FLOOR - 195, C_DARK);
    gfx_rect(35, 215, 15, FLOOR - 215, C_DARK);
    gfx_rect(50, 265, 55, FLOOR - 265, C_DARK);
    gfx_rect(105, 295, 30, FLOOR - 295, C_DARK);
    gfx_rect(603, 230, 37, FLOOR - 230, C_DARK);
    gfx_rect(585, 255, 18, FLOOR - 255, C_DARK);
    gfx_rect(570, 300, 15, FLOOR - 300, C_DARK);
    tri(8, 198, 36, -1, C_WHITE);
    tri(632, 198, 36, 1, C_WHITE);
    gfx_rect(0, FLOOR, SCR_W, 2, C_WHITE);
    draw_fighter(0);
    draw_fighter(1);
    /* painel de acoes */
    text(5, 346, "PRESS X TO DO ACTIONS", 2, C_WHITE);
    rect_o(8, 370, 300, 70, 2, C_WHITE);
    for (i = 0; i < 6; i++) {
        int row = i % 3, col = i / 3, x = 30 + col * 140, y = 377 + row * 20;
        int a = MENU[i];
        unsigned k = C_WHITE;
        const char *nm = a == A_SPEC ? CL[P[0].cls].sp_name : ACT_NAME[a];
        if (a == A_SPEC && P[0].meter < SPEC_COST) k = C_GREY;
        if (i == menu_cur && phase == 0) { k = k == C_GREY ? 0x9999b0u : C_YELLOW; text(x - 16, y, ">", 2, k); }
        text(x, y, nm, 2, k);
    }
    if (nodice_msg > 0) text(8, 428 + 4, "", 1, C_GREY);
    /* caixa da direita */
    rect_o(335, 350, 295, 90, 2, C_WHITE);
    if (phase == 0) {
        if (nodice_msg > 0) text_c(482, 388, "NEED 3 METER", 3, C_YELLOW);
        else text_c(482, 372, "???", 8, C_WHITE);
    } else {
        for (i = 0; i < nlog && i < 6; i++) text(345, 356 + i * 14, LOG[i], 2, i < 2 ? C_CYAN : C_WHITE);
    }
    (void)buf;
}

static void draw_result(void)
{
    text_c(320, 140, result == 0 ? "YOU WIN" : result == 1 ? "YOU LOSE" : "DRAW", 10, result == 0 ? C_GREEN : result == 1 ? C_RED : C_WHITE);
    text_c(320, 300, "PRESS X", 4, C_YELLOW);
}

void game_draw(void)
{
    gfx_rect(0, 0, SCR_W, SCR_H, C_BG);
    switch (screen) {
    case S_TITLE:  draw_title(); break;
    case S_CLASS:  draw_class(); break;
    case S_CONFIG: draw_config(); break;
    case S_BATTLE: draw_battle(); break;
    case S_RESULT: draw_battle(); gfx_rect(0, 100, SCR_W, 250, C_BG); draw_result(); break;
    }
}
