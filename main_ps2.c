/* NUM FIGHT ASCII - camada PS2 (gsKit + libpad). Compilar com ps2sdk. */
#include <tamtypes.h>
#include <kernel.h>
#include <sifrpc.h>
#include <loadfile.h>
#include <string.h>
#include <stdio.h>
#include <gsKit.h>
#include <dmaKit.h>
#include <libpad.h>
#include "game.h"

static GSGLOBAL *gs;
static int yoff;                       /* centraliza 448 linhas em PAL (512) */
static char padbuf[256] __attribute__((aligned(64)));

void gfx_rect(int x, int y, int w, int h, unsigned rgb)
{
    if (w <= 0 || h <= 0) return;
    gsKit_prim_sprite(gs, (float)x, (float)(y + yoff), (float)(x + w), (float)(y + h + yoff), 1,
                      GS_SETREG_RGBAQ((rgb >> 16) & 0xff, (rgb >> 8) & 0xff, rgb & 0xff, 0x80, 0));
}

static void pad_setup(void)
{
    int tries = 0;
    SifLoadModule("rom0:SIO2MAN", 0, NULL);
    SifLoadModule("rom0:PADMAN", 0, NULL);
    padInit(0);
    padPortOpen(0, 0, padbuf);
    while (tries++ < 1000) {
        int st = padGetState(0, 0);
        if (st == PAD_STATE_STABLE || st == PAD_STATE_FINDCTP1) break;
        gsKit_vsync_wait();
    }
}

static unsigned pad_read(void)
{
    struct padButtonStatus b;
    unsigned btn, out = 0;
    int st = padGetState(0, 0);
    if (st != PAD_STATE_STABLE && st != PAD_STATE_FINDCTP1) return 0;
    if (!padRead(0, 0, &b)) return 0;
    btn = 0xffff ^ b.btns;                 /* libpad: bit=0 quando apertado */
    if (btn & PAD_UP)       out |= BTN_UP;
    if (btn & PAD_DOWN)     out |= BTN_DOWN;
    if (btn & PAD_LEFT)     out |= BTN_LEFT;
    if (btn & PAD_RIGHT)    out |= BTN_RIGHT;
    if (btn & PAD_CROSS)    out |= BTN_CROSS;
    if (btn & PAD_CIRCLE)   out |= BTN_CIRCLE;
    if (btn & PAD_START)    out |= BTN_START;
    return out;
}

int main(void)
{
    unsigned prev = 0, cur, pressed, seed = 1;

    SifInitRpc(0);

    dmaKit_init(D_CTRL_RELE_OFF, D_CTRL_MFD_OFF, D_CTRL_STS_UNSPEC, D_CTRL_STD_OFF, D_CTRL_RCYC_8, 1 << DMA_CHANNEL_GIF);
    dmaKit_chan_init(DMA_CHANNEL_GIF);

    gs = gsKit_init_global();             /* detecta NTSC/PAL automaticamente */
    gs->PrimAlphaEnable = GS_SETTING_OFF;
    gs->ZBuffering = GS_SETTING_OFF;
    gs->DoubleBuffering = GS_SETTING_ON;
    gs->Width = 640;
    gsKit_init_screen(gs);
    gsKit_mode_switch(gs, GS_ONESHOT);
    yoff = (gs->Height - SCR_H) / 2;
    if (yoff < 0) yoff = 0;

    pad_setup();
    game_init(seed);

    for (;;) {
        cur = pad_read();
        pressed = cur & ~prev;
        prev = cur;
        seed = seed * 1103515245u + 12345u + cur;   /* mistura entrada p/ aleatoriedade */
        if (pressed) game_init_seed_hint(seed);
        game_update(pressed);

        gsKit_clear(gs, GS_SETREG_RGBAQ(0x1c, 0x1c, 0x2e, 0x00, 0x00));
        game_draw();
        gsKit_queue_exec(gs);
        gsKit_sync_flip(gs);
        gsKit_queue_reset(gs->Os_Queue);
    }
    return 0;
}
