FDiff
=====

Welcome to fdiff. This is a simple program designed to do forward differencing on files. This means that the patches are only one way, old to new. This program functions fundamentally differently than the normal diff tool and is designed to work on binary files in addition to text files.

Fdiff was primarily designed to work as a game patcher but can be used in nearly any setting.

Notice
------
The algorithm used in this patching tool is terrible. :p I created it but never got around to cleaning it up and iterating on its design. This project will remain here as an archived repo for anyone that wishes to view it but I have no plans to ever develop on it again.

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
