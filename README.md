# RHRE Update Utility
A simple program to move and launch an auto-updated version of RHREfresh.exe

In the future, this tool will handle downloading, extracting, and installing any version of RHREfresh (both EXE and JAR). For now, it simply copies an extracted update into the install location, deletes the extracted update, and launches RHREfresh.exe.

Intended to be ran by RHRE, not by a user.

## Usage
``RHREUpdateUtility <extracted directory> <install directory>``

The steps that this program uses are:
1. For each file in the extracted directory, copy it to the install directory, replacing files/directories that share the same name
2. Delete the entire install directory
3. Search for an EXE in the install directory, and launch it

## Building
This project uses the Ninja build system. Simply run `ninja` in the project directory to build.

## Credits
- Tool created by [Zeo](https://github.com/ThatZeoGal), based on [Metronome](https://github.com/ThatZeoGal/Metronome) (which had significant help from, and was based on the work of, [Conhlee](https://github.com/conhlee))
