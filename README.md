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

## Játékmenet

### Kezdő állapot
A Hold felszínét oldalnézetben rajzoljuk ki. A leszállóegység a képernyő bal felső sarkában van, nagy sebességgel halad jobbra és kezdetben nincs lefele irányú sebessége.

### Leszállás
A leszállóegységre állandó lefele irányuló gyorsulási erő hat. Feladatunk a kezdeti vízszintes irányú sebesség csökkentése, illetve az ereszkedés sebességének szabályozása, mindezt úgy, hogy kellően egyenletes terepen tudjunk landolni. Ha elég közel kerültünk a felszínhez a játék közelebbi nézetre vált, ezzel segítve a pontosabb manőverezést.

A leszállóegység sebességét a fő hajtómű segítségével szabályozhatjuk. Kisebb manővereket, illetve a leszállóegység forgatását az oldalán található kis hajtóművek segítségével végezhetjük. A hajtóművek által kifejtett erő állandó, azonban ahogy fogy az üzemanyag a leszállóegység egyre könnyebb lesz, így a rá ható erő nagyobb mértékben befolyásolja a gyorsulást.

### A játék vége
A játék mindenképpen véget ér amikor a leszállóegység eléri a felszínt. A leszállás sikeresnek minősül, ha a leszállóegység vízszintes talajra érkezik, és elég kicsi a vízszintes és függőleges sebessége is. A leszállóegység megsemmisül, vagyis a játékos veszít, ha túl nagy sebességgel csapódik be, vagy nem egyenletes talajra próbál leszállni, vagy túl nagy az elfordulása.

### Irányítás
A hajtóműveket billentyűk lenyomva tartásával irányíthatjuk.
- Fő hajtómű: `W`
- Jobb oldali hajtómű: `A`
- Bal oldali hajtómű: `D`
- Jobbra fordulás: `K`
- Balra fordulás: `J`

A hajtóművek egy, a hajtómű irányával ellentétes irányú erőt fejtenek ki a leszállóegységre.

### Pontozás
A játékos által szerzett pontokat a leszállás minőségéből (mekkora volt a sebessége, milyen volt az elfordulása) illetve a felhasznált üzemanyag mennyiségéből számítjuk ki.
