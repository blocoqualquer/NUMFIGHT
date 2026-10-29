# NUM FIGHT ASCII - build PS2 (requer ps2sdk: export PS2SDK=/usr/local/ps2dev/ps2sdk)
EE_BIN  = NUMFIGHT.ELF
EE_OBJS = game.o main_ps2.o
EE_LIBS = -lgskit -ldmakit -lpad -lc -lkernel
EE_CFLAGS += -O2 -std=gnu99 -Wall

all: $(EE_BIN)

clean:
	rm -f $(EE_BIN) $(EE_OBJS)

include $(PS2SDK)/samples/Makefile.pref
include $(PS2SDK)/samples/Makefile.eeglobal
