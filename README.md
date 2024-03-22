![C++](https://img.shields.io/badge/c++-%2300599C.svg?style=for-the-badge&logo=c%2B%2B&logoColor=white)
![GitHub last commit](https://img.shields.io/github/last-commit/darokin/term-temple)

# term-temple :computer:

**Term Temple** is a multi-tool console application / fantasy text-mode operating system.

Terminal printing is made with the curses library (**ncursesw** for wide character support).

It is graphically inspired by TempleOS. I'm learning **modern C++** while making this project.

## Usage

You will need a color enabled terminal.

Currently suggested font is square font. 

**~145x70** is a good terminal resolution for now (current background ANSI is quite large).

**Font** used for the screenshot is **Px AmstradPC1512 Regular** from the **"Old School PC Font Ressource"** website:

[Old School PC Font Ressource](https://int10h.org/oldschool-pc-fonts/fontlist/)

[Amstrad font preview](https://int10h.org/oldschool-pc-fonts/fontlist/font?amstrad_pc)

[Old School PC Font Download](https://int10h.org/oldschool-pc-fonts/download/)

Other useful fonts:

[Nerd Fonts](https://www.nerdfonts.com/font-downloads)

Install libncursesw5-dev ou juste libncurses-dev.
Depuis 'src/' :

```
>make  
>./term-temple && stty sane && clear
```

### Screenshots :camera:

![Screenshot of Term Temple console application by darokin](https://darokin.info/github/imgs/term-temple_01.png)
![Screenshot of Term Temple console application by darokin](https://darokin.info/github/imgs/term-temple_02.png)
![Screenshot of Term Temple console application by darokin](https://darokin.info/github/imgs/term-temple_03.png)

## Project

All the main curses calls (wmove() + wmaddnwstr()) are made in renderer.cpp. Renderer.cpp is made as a pass through to facilitate the migration to some other text mode (or graphical) library.

cursesANSI.cpp is the exception and along the basic ANSI art parser, there are few drawing calls that are made straight from curses library.

The goal of the project is to make a simple window manager, with a few tools inside (clock, text file reader, ANSI art reader, calculator etc.) so it feels like a text-mode pseudo operating system.

NCurses library is not made to intensive graphics / working in realtime, the way the WINDOW object are redrawn etc. :broken_heart:

So the trick is to use 0 capabilities of Curses (WINDOW or other lib (menu etc.)) to handle manually the rendering and using only the 'stdscr' WINDOW object Curses expose to draw everything.