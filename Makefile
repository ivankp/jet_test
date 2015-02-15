CC := g++
CCFLAGS := -Wall -O2

FJ_DIR    := $(shell fastjet-config --prefix)
FJ_CFLAGS := -I$(FJ_DIR)/include
FJ_LIBS   := -L$(FJ_DIR)/lib -lfastjet

.PHONY: all clean

EXE := my fj

all: $(EXE)

my: %: %.cc
	$(CC) $(CCFLAGS) $^ -o $@

fj: %: %.cc
	$(CC) $(CCFLAGS) $(FJ_CFLAGS) $^ -o $@ $(FJ_LIBS)

clean:
	rm -f $(EXE)
