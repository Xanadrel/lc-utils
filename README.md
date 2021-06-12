# lc-utils

## Description

This is a compilation of small tools to work with files (usually containing hashes).

If you didn't notice it is **heavily** inspired by [hashcat-utils](https://github.com/hashcat/hashcat-utils) (which has other great tools, including the greatest of all: **rli2**).

## Important stuff

Never forget to set `LC_ALL=C` and `sort -u` input files.

## Files used

We are working with different files

- list: list of hashes, sorted and uniqued
- found: list of cracked hashes (in hash[:salt]:pass format), sorted and uniqued
- left: list of hashes not cracked yet (in hash[:salt] format), sorted and uniqued
- dict: list of only plaintext passwords, sorted but not always uniqued
- meta: based on a `list`, contains line number and count of a hash (if count > 1), "sorted"
- original list: list of hashes, sorted but **NOT** uniqued

> Note: the goal of the meta file is to reduce the size of a hashlist without losing the precious number of occurrence of each hash (it's obviously targetted at unsalted algorithms).

## Tools and their uses

### alarm

Stops a cmd ran through it if the time exceeds a given time in seconds.

Usage: `./alarm.bin <secs> <cmd>`

### cmp_rli2

Print the percentage of lines from `infile` which are in `removefile`

Usage: `./cmp_rli2.bin <infile> <removefile>`

### dgrep2

Print first line starting with the provided `search` string in a sorted and uniqued file

Usage: `./dgrep2.bin <file> <search>`

### dict

Generate a `dict` file using provided `list` and `found` files

Usage: `./dict.bin <listfile> <foundfile>`

### leftlist

Generate a `left` file using provided `list` and `found` files

Usage: `./leftlist.bin <listfile> <foundfile>`

### metabuild

Generate the `original list`, `dict` and `count` using the `list` and `meta` files (output will match the count of hashes in the **not** uniqued `original list`).

Usage: `./metabuild.bin <type> <listfile> <metafile> [foundfile]`

Where type can be:
- `list` generate the `original list`
- `dict` generate the dict, using `found` file (can use `uniq -c` to get stats and generate a dict sorted by occurrence as well)
- `count` count the total of cracked hashes

### metagen

Generate the `meta` file based on an `original list` file

Usage: `./metagen.bin <listfile> <metafile>`

(After this is done, you can unique the `original list` and keep only the resulting `list` file).

### rli2_len

Generate lines similar to both files but compare using length of `infile` lines

Usage: `./rli2_len.bin <infile> <removefile>`

### sed_cut_coffee

Print lines removing leading count & spaces (from the output of uniq -c)

Usage: `./sed_cut_coffee.bin <file>`

## Building

Install gcc and stuff and also `gcc-mingw-w64` to compile for windows

Just do `make all`

## Binaries

Binaries for Linux and Windows can be found here : https://github.com/Xanadrel/lc-utils/releases