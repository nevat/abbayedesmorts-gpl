# Abbaye-SDL

Port of indie game "l'Abbaye des Morts". Originally released by Locomalito to Windows
platform in year 2010. This port uses C language with SDL libraries.

# Media / Screenshots

[![shot1](https://picload.org/image/rrwogddo/1473017079_th.png)](https://pl.vc/bcnge)
[![shot2](https://picload.org/image/rrwogdag/1473017146_th.png)](https://pl.vc/b5qoo)
[![shot3](https://picload.org/image/rrwogddc/1473017093_th.png)](https://pl.vc/155ocq)
[![shot4](https://picload.org/image/rrwogddp/1473017123_th.png)](https://pl.vc/7vdx8)

# Platforms

Currently, this port works on:

  * GNU/Linux (32 & 64 bits), including Raspberry Pi
  * FreeBSD
  * OpenPandora
  * CGW Zero
  * Nintendo Wii

Fell free if you want to port this program to another system, it's opensource, take the
code and modify!

# Installation from source

Under GNU/Linux and FreeBSD, in order to compile this program, you need to install SDL2
libraries. Check your distribution instructions.

Unpack the source code file:

`$ tar xf abbaye_linux_v2_beta.tar.gz`

Enter the directory created an run:

`$ make`

As root, install the game with:

`# make install' (Ubuntu users run: `$ sudo make install`)

An icon will appear in your application menu, in game section.
Alternatively you can run the game with `abbayev2`.

# Uninstallation from source

Enter the directory and run (as root):

`# make uninstall` (Ubuntu users run: `$ sudo make uninstall`).

# History

Abbaye des Morts is a freeware game made by LocoMalito (with the help of Gryzor87 in
music side) in 2010. Was developed with Gamemaker and only runs natively in Microsoft
Windows systems.

This is a port to GNU/Linux systems & any system that supports C and SDL libraries. It is
written in C code with the help of SDL2 libraries.

The version 2.0 of the game is a port from SDL1.2 to SDL2 libraries. This includes a nice
set of features: OpenGL rendering, auto-scaling, Android & IOS support, VSync, etc.

# License

The code is licensed under GNU GPL version 3, so anyone can download, see, change and
redistribute the code.
