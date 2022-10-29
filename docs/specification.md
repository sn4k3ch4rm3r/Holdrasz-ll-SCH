# HoldraszálláSCH - Specifikáció  <!-- omit in toc -->

## Tartalom  <!-- omit in toc -->
- [A játék lényege](#a-játék-lényege)
- [A program felépítése](#a-program-felépítése)
	- [Menü](#menü)
	- [Játék](#játék)
	- [Dicsőséglista](#dicsőséglista)
- [Pontos játékmenet](#pontos-játékmenet)
	- [Kezdő állapot](#kezdő-állapot)
	- [Leszállás](#leszállás)
	- [A játék vége](#a-játék-vége)
	- [Irányítás](#irányítás)
	- [Pontozás](#pontozás)
- [Fájlkezelés](#fájlkezelés)

## A játék lényege

Le kell szállnunk a hold felszínére. Ez azonban nem olyan egyszerű, hiszen erre megfelelően egyenletes területet kell találnunk, vigyáznunk kell, hogy megfelelően kis sebességgel érjünk földet és az üzemanyag se fogyjon el közben.

## A program felépítése

### Menü
A program indulásakor ez a képernyő jelenik meg és gombokra kattintva az alábbi menüpontok közül választhatunk:
- Játék
- Dicsőséglista

Ezek átvisznek minket a megfelelő nézetbe. 

### Játék
Elindul a játék, majd a játék végén, ha sikeres volt a leszállás megadhatjuk a nevünket, ami a dicsőséglistán meg fog jelenni. Ezután az alábbi menüpontok közül választhatunk:
- Újra próbálkozás - Újra indítja a játékot.
- Új pálya - Új felszínt generál, majd újra indítja a játékot.
- Kilépés - Vissza lép a menübe.


### Dicsőséglista
Itt jelennek meg a korábbi sikeres leszállások pontszám szerint csökkenő sorrendben. A megjelenítés táblázatként történik.

|Helyezés|Név|Pontszám|
|---|---|---|
|1.|Példa 1|999|
|2.|Példa 2|100|

Egy oldalon egyszerre 10 elem jeleníthető meg, ha ennél több elem van a listán, a táblázat alatti gombok segítségével léptethetjük a megjelenített adatokat. Ezen kívül lehetőségünk van vissza lépni a menübe.

Egyes lista elemekre kattintva bővebb információ jelenik meg az adott leszállásról:
- Név
- Helyezés
- Pontszám
- Eltelt idő
- Felhasznált üzemanyag
- Leszállás minősége

## Pontos játékmenet

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

## Fájlkezelés
A dicsőséglista elemeit csv formátumú fájlban tároljuk.
```csv
pontszám;név;idő;üzemanyag;minőség
```

Minden sikeres leszállást automatikusan fájlba mentünk, illetve a dicsőségtábla megnyitásakor a fájlból töltjük be az adatokat.
