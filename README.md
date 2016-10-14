FDiff
=====

Welcome to fdiff. This is a simple program designed to do forward differencing on files. This means that the patches are only one way, old to new. This program functions fundamentally differently than the normal diff tool and is designed to work on binary files in addition to text files.

Fdiff was primarily designed to work as a game patcher but can be used in nearly any setting.

Usage
-----
Default (create patch):
```
fdiff [old file] [new file] [patch file] (options)
```

Use patch to update file
```
fdiff -p [patch file] [old file] [target file]
```

Options:

`-b` - Allows use of binary files.

`-h` or `--help` - Displays a help text.
