#  PyAssembler

A simple script that takes all python files in a given folder to assemble them in a single python file

## Why

Why ? Because I have a project to do for college, and they allow only one single python file, so let's write a python files assembler

## Installation

You will need :
* `gcc` to compile the code
* `make` to use the make file (you can do without, but you'll need to compile by hand)
* `git` if you want to clone if from source, otherwise you can just click **Download**

Then you can do :
1. `git clone https://github.com/Greensky-gs/pyassembler`
2. `cd pyassembler`
3. `make cleanbuild` to build without sanatizers and flags. Just run `make` to run with them if you want
4. Executable is produced in `bin/main.uwu`. Feel free to move it where you please.

### Additional compiling options

By default, `DISABLE_VERBOSE` is active. What it means, is that a macro is set for when you run with `-v` or `--verbose`. However, it stills does some checks, even if running without verbosing.

If you care about that maximum efficiency, disable this macro by running `make cleanbuild DISABLE_VERBOSE=1`

## Developpement

This is still in developpement, for now there are a few things to do :

- [X] Having a working assembler
- [ ] A man page entry ? Or proper documentation ?
- [ ] Actually handle errors

## Usage

Just run `pyassembler directory` to take all python files and assemble them.

Alternatively get the (limited) help by running `pyassembler`
