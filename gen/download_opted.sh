#!/bin/sh

mkdir OPTED

for x in {a..z}
do
    curl "https://www.mso.anu.edu.au/~ralph/OPTED/v003/wb1913_$x.html" -o OPTED/$x.html
done
