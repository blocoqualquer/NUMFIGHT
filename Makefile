EE_BIN = NUMFIGHT.ELF
EE_OBJS = game.o main_ps2.o
EE_LIBS = -lgskit -ldmakit -lpad -lc -lkernel -lm
EE_CFLAGS = -O2 -std=gnu99 -Wall

all: $(EE_BIN)

include $(PS2SDK)/samples/Makefile.pref
include $(PS2SDK)/samples/Makefile.eeglobal

EE_INCS += -I$(PS2DEV)/gsKit/include
EE_LDFLAGS += -L$(PS2DEV)/gsKit/lib
