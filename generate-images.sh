#!/bin/sh

for i in shape0 shape1 shape2
do
    m4 -Dm4_config_file=data/img/${i}.m4 data/img/shape.svg.m4 > data/img/${i}.svg
    m4 -Dm4_config_file=data/img/${i}.m4 -Dselected data/img/shape.svg.m4 > data/img/${i}-selected.svg
done

function svg2png()
{
    FILEIN=$1
    FILEOUT=$2
    ICONSIZE=$3
    DENSITY=$(convert $FILEIN -format "%w %h %[resolution.x] %[resolution.y]" info: | awk '{ printf "%fx%f", '$ICONSIZE' * $3 / $1, '$ICONSIZE' * $4 / $2 }')

    mkdir -p $(dirname $FILEOUT)
    convert -background none -density $DENSITY $FILEIN $FILEOUT
}

svg2png sailfish/harbour-squarez.svg android/res/drawable-ldpi/icon.png 36
svg2png sailfish/harbour-squarez.svg android/res/drawable-mdpi/icon.png 48
svg2png sailfish/harbour-squarez.svg android/res/drawable-hdpi/icon.png 72
svg2png sailfish/harbour-squarez.svg android/res/drawable-xhdpi/icon.png 96
svg2png sailfish/harbour-squarez.svg android/res/drawable-xxhdpi/icon.png 144
svg2png sailfish/harbour-squarez.svg android/res/drawable-xxxhdpi/icon.png 192
