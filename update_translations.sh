#!/bin/sh

lupdate-qt5 sailfish/harbour-squarez.pro -no-obsolete -ts translations/fr.ts
lupdate-qt5 sailfish/harbour-squarez.pro -no-obsolete -ts translations/en.ts

linguist-qt5 translations/*.ts
