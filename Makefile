CC := gcc
SRCDIR := src
BUILDDIR := build
OBJDIR := $(BUILDDIR)/obj
TARGET := $(BUILDDIR)/bin
CFLAGS := -Wall
INCLUDE := -I include/
LIB := -lSDL2_gfx -lSDL2_image -lSDL2_mixer -lSDL2_ttf
OBJECTS := main.o game.o vector.o lander.o perlin_noise.o

# Windows
ifdef OS
	RM := rmdir /S /Q
	INCLUDE += -I"$(SDL_INCLUDE)"
	LIB := -L"$(MINGW_LIB)" -lmingw32 -lSDL2main -lSDL2 $(LIB) -luser32 -lgdi32 -lwinmm -ldxguid
	MKDIR := if not exist "$(BUILDDIR)" mkdir $(BUILDDIR) & if not exist "$(OBJDIR)" mkdir "$(OBJDIR)" & if not exist "$(TARGET)" mkdir "$(TARGET)"
# Linux
else
	RM := rm -rf
	INCLUDE += `sdl2-config --cflags`
	LIB := `sdl2-config --libs` $(LIB)
	MKDIR := mkdir -p $(BUILDDIR); mkdir -p $(OBJDIR); mkdir -p $(TARGET)
endif

default: CFLAGS += -O3
default: setupdirs Holdraszallasch

debug: CFLAGS += -g
debug: setupdirs Holdraszallasch

setupdirs:
	$(MKDIR)

%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) $(INCLUDE) -c -o $(OBJDIR)/$@ $<

Holdraszallasch: $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET)/$@ $(addprefix $(OBJDIR)/, $^) $(LIB)

clean:
	$(RM) $(BUILDDIR)