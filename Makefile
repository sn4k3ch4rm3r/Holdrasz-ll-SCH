CC := gcc
SRCDIR := src
BUILDDIR := build
OBJDIR := $(BUILDDIR)/obj
TARGET := $(BUILDDIR)/bin
CFLAGS := -Wall
INCLUDE := -I include/ -I assets/
LIB := -lSDL2_gfx -lSDL2_image -lSDL2_mixer -lSDL2_ttf
OBJECTS := main.o game.o vector.o lander.o terrain.o camera.o button.o menu.o particle.o text_io.o file_handler.o leaderboard.o

# Windows
ifdef OS
	RM := rmdir /S /Q
	INCLUDE += -I"$(SDL_INCLUDE)"
	LIB := -L"$(MINGW_LIB)" -lmingw32 -lSDL2main -lSDL2 $(LIB) -luser32 -lgdi32 -lwinmm -ldxguid
	MKDIR := if not exist "$(BUILDDIR)" mkdir $(BUILDDIR) & if not exist "$(OBJDIR)" mkdir "$(OBJDIR)" & if not exist "$(TARGET)" mkdir "$(TARGET)"
	COPYASSETS := xcopy assets\ build\bin\assets\ /Y
# Linux
else
	RM := rm -rf
	INCLUDE += `sdl2-config --cflags`
	LIB := -lm `sdl2-config --libs` $(LIB)
	MKDIR := mkdir -p $(BUILDDIR); mkdir -p $(OBJDIR); mkdir -p $(TARGET)
	COPYASSETS := cp assets/ build/bin/ -r -f
endif

default: CFLAGS += -O3
default: setupdirs Holdraszallasch

debug: CFLAGS += -g
debug: setupdirs Holdraszallasch

setupdirs:
	$(MKDIR)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) $(INCLUDE) -c -o $@ $<

Holdraszallasch: $(addprefix $(OBJDIR)/, $(OBJECTS))
	$(CC) $(CFLAGS) -o $(TARGET)/$@ $^ $(LIB)
	$(COPYASSETS)

clean:
	$(RM) $(BUILDDIR)