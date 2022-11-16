# HoldraszálláSCH

## Fordítás

### Make

A projekthez tartozik egy Makefile, ennek segítségével viszonylag egyszerű lefordítani a projektet. 

**Linux**

Ubuntu-n lett tesztelve a makefile, de elméletileg más disztibúción is működik. Elég belépnünk a projekt mappájába és kiadni a `make` parancsot. Az elkészült futtatható fájl és az egyéb futáshoz szükséges fájlok a `build/bin/` mappában lesznek megtalálhatóak.

**Windows**

Windows-on egy kicsit bonyolultabb, itt először a `MINGW_LIB` és az `SDL_INCLUDE` környezeti változókban meg kell adnunk a megfelelő elérési utat.

Powershell-ben ezt a következő parancsok segítségével tehetjük meg:
```ps
$env:MINGW_LIB = "C:\Program Files\CodeBlocks\MinGW\lib"
$env:SDL_INCLUDE = "C:\Program Files\CodeBlocks\MinGW\include\SDL2"
```
Ezután be kell lépnünk a projekt mappájába majd a `mingw32-make` parancs segítségével fordíthatjuk a projektet. Az elkészült `.exe` fájl és a futáshoz szükséges egyéb fájlok a `build\bin\` mappában lesznek megtalálhatóak.

### Manuálisan

A forrás fájlok az `src`, a header fájlok pedig az `include` mappában találhatóak. Ezen kívül a fordításhoz szükség van még az SDL2 grafikus könyvtárra. Futtatáshoz pedig fontos, hogy az elkészült futtatható fájl és az `assets` mappa egy mappában legyenek.

## Haladás

A program jelenleg csak a játék fizika szimuláció részét tartalmazza. Tehát ki lehet próbálni a leszállás folyamatát, de játék végállapotai illetve a leszállást segítő műszerek még hiányoznak. A program többi része sem készült még el, tehát a menü és a dicsőséglista még hátra van.

## Irányítás

A hajtóműveket billentyűk lenyomva tartásával irányíthatjuk.
- Fő hajtómű: `W`
- Jobb oldali hajtómű: `A`
- Bal oldali hajtómű: `D`
- Jobbra fordulás: `K`
- Balra fordulás: `J`