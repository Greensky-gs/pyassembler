% PyAssembler(1) PyAssembler Use Manuals
% Greensky-gs
% April 2026

# NAME

pyassembler - A C-written python files assembler

# SYNOPSIS

pyassembler <*directory name*> [*options*]

See **OPTIONS** below

# DESCRIPTION

PyAssembler is a small C program that aims to assemble all python files from a folder into one unique python file. It is written in C and uses POSIX system calls for the most part

PyAssembler will recursively read all the files from a given directory, and keep only the python files, remembering imports and filtering only standard/non-custom files imports, to put them in the output. The output can be controlled using the options, such as `--start-of-file` and `--end-of-file`. It will, by default, create a comment to signal the start of the imports, the end of the imports, the start of a file and the end of a file

    pyassembler src

If no *directory name* is specified, it will throw an error. If it does not exist, it will not be created and will fail.

The program follows all symlinks, directories and files.

If no output file is specified, it defaults to `./output/main.py`. The output can be changed using `-o` flag

    pyassembler src -o dist/out.py

# OPTIONS

-h,   \--help : Displays the help page if it is found. The same behavior is executed if no arguments is given

    pyassembler -h
    pyassembler
    pyassembler src --no-comments -o dist/out.py --help

-o *OUTPUT PATH* : Specify the output path. If the file does not exist, it will be created. If it exists, it will be truncated.

    pyassembler src -o dist/out.py
    pyassembler src -o build/output.py

-v,   \--verbose : Enables verbosity if found. It might do nothing, if the binary has been compiled with the `DISABLE_VERBOSE=1` flag, but the verbosity is available by default. If you want, you can compile your own **pyassembler** binary, with the `DISABLE_VERBOSE=1` flag, so the checks are removed and gives a slight performance boost (not much)

    pyassembler src -v
    pyassembler src --verbose

\--end-of-file *PRELUDE* : The comment message to be displayed before the file name in the assembled file. By default, if comments are enabled, this will be written : # PyAssembler::end: /path/of/the/source/file.py. The message can be modified, but the file name will be appened at the end. Enabling `--no-comments` removes this features, by removing all comments

    pyassembler src --end-of-file "End of file: "
    pyassembler src --end-of-file "========[END]======[FILE]:"

\--fullpaths : Shows fullpaths in comments of start and end (the ones that can be modified by `--start-of-file` and `--end-of-file`). Enabling `--no-comments` removes this features, by removing all comments, and thus the file names, shortened or longened by `--fullpaths`

    pyassembler src --fullpaths
    pyassembler src --start-of-file "Start of (full): " --end-of-file "End of (full): "

\--last-file *FILE PATH* : Specify the last file to be assembled. This will tell **PyAssembler** to remember the file at *directory name/FILE PATH* if it encounters it during the recursion, and treat it as last file. It is useful to have one file at the end of the assembled file. The given *directory name* will be prepended before *FILE PATH* to, so it is a relative path name to the input directory

    pyassembler src --last-file main.py

\--max-newlines *NEW LINES COUNT* : Specify the maximum numbers of consecutives empty lines. It might be useful if `--no-comments` is specified, as it can create unwanted blank lines between files. Default value is 1

    pyassembler src --max-newlines 1
    pyassembler src --max-newlines 0 --no-comments

\--no-comments : Disable all assembler comments, being : **start of files**, **end of files**, **start of imports list** and **end of imports list**. If is is enabled, the `--start-of-file`, `--end-of-file` and `--fullpaths` flags have no effect

    pyassembler src --no-comments

\--start-of-flie *PRELUDE* : The comment message to be displayed before the file name in the assembled file. By default, if comments are enabled, this will be written : # PyAssembler::Start: /path/of/the/source/file.py. The message can be modified, but the file name will be appened at the end anyway. Enabling `--no-comments` removes this features, by removing all comments

    pyassembler src --end-of-file "Start of file: "
    pyassembler src --end-of-file "========[START]======[FILE]:"

# EXAMPLES

The following command will scan all files from `src`, treat the file `src/runtime/main.py` in last, allowing 2 consecutive lines, and showing fullpaths in comments, and write the output in `./dist/out.py`

    pyassembler src --last-file runtime/main.py --max-newlines 2 -o dist/out.py

The following commands will scan all files from `src`, will verbosely tell what it is doing, writting no comments, treat `src/main.py` in last, and write the output in `./output/main.py`
    
    pyassembler src --last-file main.py --no-comments
    pyassembler src --last-file main.py --no-comments -o output/main.py

The following command will use (almost) all parameters

    pyassembler src --last-file main.py --start-of-file "START OF: " --end-of-file "END OF: " --fullpaths --max-newlines 2 -v

# SEE ALSO

`python`(1)
