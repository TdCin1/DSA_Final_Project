#!/usr/bin/env python3

from string import ascii_lowercase, punctuation
import re

count = 0

with open("opted.dict", "w") as out:
    for c in ascii_lowercase:
        with open("OPTED/{}.html".format(c), "r", errors="replace") as input:
            for line in input:
                match = re.search(r"^<P><B>(.+)</B> \(<I>.+</I>\) (.+)</P>$", line)
                if match is None:
                    continue

                word = match.group(1).lower()
                definition = match.group(2)

                if " " in word:
                    continue

                count += 1
                out.write(word)
                out.write("\0")

                out.write(definition)
                out.write("\0")

                out.write("\n")

print("{} definitions added".format(count))
