#CC = /cygdrive/c/Program\ Files\ \(x86\)/CodeBlocks/MinGW/bin/g++
CC = g++
Debug: CC += -g

OUTPUT_DIR  = bin
INCLUDE_DIR = include
OBJECT_DIR  = $(OUTPUT_DIR)/obj
LIB_DIR     = lib

CFLAGS = -Wall -g -I$(INCLUDE_DIR) -L$(LIB_DIR)
Release: CFLAGS += -O3

LINKER_FLAGS = -static -lm -lbgi -lgdi32 -lcomdlg32 -luuid -loleaut32 -lole32

EXECUTABLE = Checkers

SOURCES = CheckersDrawing.c CheckersGame.c main.c

_DEPS = CheckersDrawing.h CheckersGame.h graphics.h winbgim.h
DEPENDENCIES = $(patsubst %, $(INCLUDE_DIR)/%, $(_DEPS))

_OBJ = $(patsubst %.c, %.o, $(wildcard *.c))
OBJECTS = $(patsubst %, $(OBJECT_DIR)/%, $(_OBJ))

Debug:   all
Release: all

cleanDebug:   clean
cleanRelease: clean

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(OUTPUT_DIR)/$@ $(LINKER_FLAGS)

$(OBJECT_DIR)/%.o: %.c $(DEPENDENCIES)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECT_DIR)/*.o *~ $(INCLUDE_DIR)/*~

