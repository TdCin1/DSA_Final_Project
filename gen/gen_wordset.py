#!/usr/bin/env python3

from string import ascii_lowercase
import json

count = 0

with open("wordset.dict", "w") as out:
    for c in ascii_lowercase:
        with open("wordset-dictionary/data/{}.json".format(c), "r") as input:
            d = json.load(input)
            for word, data in d.items():
                if " " in word or "." in word:
                    continue

                count += 1
                out.write(word)
                out.write("\0")

                if "meanings" in data:
                    for meaning in data["meanings"]:
                        out.write(meaning["def"])
                        out.write("\0")

                out.write("\n")

print("{} definitions added".format(count))
