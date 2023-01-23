# Chip8-Emu

Chip8-Emu is a C++ program that allows a computer to interpret and run Chip-8
programs.
[Standard Chip-8 instructions](http://devernay.free.fr/hacks/chip8/C8TECH10.HTM#0.0)
are implemented.
<img src="https://github.com/mrowan137/Chip8-Emu/blob/main/docs/demo/space_invaders_demo.gif">
<img src="https://github.com/mrowan137/Chip8-Emu/blob/main/docs/demo/tetris_demo.gif">
<img src="https://github.com/mrowan137/Chip8-Emu/blob/main/docs/demo/flightrunner_demo.gif">


## Requirements

Chip8-Emu depends on the following libraries:
* [GLFW](https://www.glfw.org/)
* [GLEW](https://glew.sourceforge.net/)
* [Audio Unit](https://developer.apple.com/documentation/audiounit)
* [OpenSSL](https://www.openssl.org/)

  
## Building the code

From the `chip8-emu` directory,
```bash
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release -DOPENSSL_ROOT_DIR="/path/to/openssl" ..
```

## Usage

After building the code, a Chip-8 program ('ROM') can be run as:
```bash
./chip8 --play /PATH/TO/ROM
```

A library of known Chip-8 ROMs can be searched for info about the ROM, which
prints some information about the ROM (if present):
```bash
./chip8 --info /PATH/TO/ROM
```

Controls:
* ESC: quit Chip8-Emu.
* Mapping from user keyboard to Chip8-Emu keyboard:

<table>
  <tr>
    <th colspan="4 style="text-align: center"> User keyboard </td>
    <th colspan="1"> </td>
    <th colspan="4" style="text-align: center"> Chip-8 keyboard </td>
  </tr>
  <tr>
    <td> 1 </td>
    <td> 2 </td>
    <td> 3 </td>
    <td> 4 </td>
    <td> → </td>
    <td> 1 </td>
    <td> 2 </td>
    <td> 3 </td>
    <td> c </td>
  </tr>
  <tr>
    <td> q </td>
    <td> w </td>
    <td> e </td>
    <td> r </td>
    <td> → </td>
    <td> 4 </td>
    <td> 5 </td>
    <td> 6 </td>
    <td> d </td>
  </tr>
  <tr>
    <td> a </td>
    <td> s </td>
    <td> d </td>
    <td> f </td>
    <td> → </td>
    <td> 7 </td>
    <td> 8 </td>
    <td> 9 </td>
    <td> e </td>
  </tr>
  <tr>
    <td> z </td>
    <td> x </td>
    <td> c </td>
    <td> v </td>
    <td> → </td>
    <td> a </td>
    <td> 0 </td>
    <td> b </td>
    <td> f </td>
  </tr>
</table>

Chip8-Emu options:
* `-p` (`--play`) [ `/PATH/TO/ROM` ]: run a Chip-8 program (ROM); will print
    info if it's in the known ROMs.
* `-i` (`--info`) [ `/PATH/TO/ROM` ]: print info about a ROM, if it's in the
    known ROMs.
* `-wy` (`--wrap-around-y`) [ `on`; `off`; default=off; ]: whether to wrap
    around in y-direction; required `off` for some games to run
    correctly (e.g. BLITZ) and `on` for others (e.g. VERS).
* `-br` (`--background-red-pixel-value`) [ integer in the range [0, 255];
    default=0; ]: red pixel value for background.
* `-bg` (`--background-green-pixel-value`) [ integer in the range [0, 255];
    default=0; ]: green pixel value for background.
* `-bb` (`--background-blue-pixel-value`) [ integer in the range [0, 255];
    default=0; ]: blue pixel value for background.
* `-fr` (`--foreground-red-pixel-value`) [ integer in the range [0, 255];
    default=1; ]: red pixel value for foreground.
* `-fg` (`--foreground-green-pixel-value`) [ integer in the range [0, 255];
    default=1; ]: green pixel value for foreground.
* `-fb` (`--foreground-blue-pixel-value`) [ integer in the range [0, 255];
    default=1; ]: blue pixel value for foreground.
* `-cs` (`--color-scheme`) [ `black-white` (`bw`), `white-black` (`wb`),
    `grays` (`gr`), `gameboy` (`gb`), `blue-white` (`blw`); default=`bw` ]:
    color scheme for background and foreground.
* `-h` (`--help`): print help menu.


## Tips

* `DEBUG` build of Chip8-Emu enables stepping through execution of the program
while printing information that can be useful when debugging, including
description of the current state, next instruction, and the display buffer
('screen'). An example is shown below:
<img src="https://github.com/mrowan137/Chip8-Emu/blob/main/docs/demo/debug_demo.png">

* Finding ROMs: a selection of public domain Chip-8 ROMs is included in the
[roms](https://github.com/mrowan137/Chip8-Emu/blob/main/roms) directory; a
comprehensive library of public domain Chip-8 software can be found at the
[CHIP-8 Archive](https://johnearnest.github.io/chip8Archive/). A web search
can turn up additonal Chip-8 software.


## Disclaimers

* Chip8-Emu is tested only for macOS at this time.
* Some Chip-8 software is written assuming mutually incompatible convention for
  wrap-around in the y-direction, due to loose specification of Chip-8 standard
  (more discussion is [here](https://www.reddit.com/r/EmuDev/comments/oro6sn/ywraparound_issue_in_blitz_rom_in_chip8/)).
  Chip8-Emu provides an option `-wy` (`--wrap-around-y`) to toggle the
  y-wrap-around behavior (it can be set to `on` or `off`).


## Author

Michael E. Rowan — [mrowan137](https://github.com/mrowan137) — [michael@mrowan137.dev](mailto:michael@mrowan137.dev).


## License

[MIT License](https://github.com/mrowan137/chip8-emu/blob/main/LICENSE).


## References

There are many excellent resources on Chip-8, and wonderful interpreters
scattered about; these were especially helpful in my own quest to build a Chip-8
emulator:
* [Cowgod's Chip-8 Technical Reference v1.0](http://devernay.free.fr/hacks/chip8/C8TECH10.HTM)
* [David Winter's CHIP-8 emulation page](https://www.pong-story.com/chip8/)
* [Core Dump's Chip-8 tutorial](https://www.youtube.com/watch?v=jWpbHC6DtnU)
* [dmatlack's Chip-8 Emulator (written in C)](https://github.com/dmatlack/chip8)
* [linoscope's Chip-8 Emulator (written in OCaml)](https://github.com/linoscope/caml8/blob/main/bin/caml8.ml)
* [mir3z's Chip-8 Emulator (written in JavaScript)](https://github.com/mir3z/chip8-emu/blob/master/src/chip8-is.js)

These tutorials and discussions were very helpful with OpenGL, GLFW, and GLEW for graphics:
* [OpenGL begginer's tutorial](http://www.opengl-tutorial.org/beginners-tutorials/tutorial-2-the-first-triangle/)
* [Learn OpenGL example](https://learnopengl.com/code_viewer_gh.php?code=src/1.getting_started/4.1.textures/textures.cpp)
* [GLFW CMake tips for macOS](https://stackoverflow.com/questions/49038142/minimal-example-with-glfw-fails-because-of-undefined-symbols-for-architecture-x8)
* [Modern OpenGL tutorial](http://www.opengl-tutorial.org/beginners-tutorials/tutorial-1-opening-a-window/#opening-a-window)
* [GLFW getting started](https://www.glfw.org/docs/3.3/quick.html#quick_include)
  
These resources were very helpful in getting working audio:
* [Apple Developer Documentation](https://developer.apple.com/documentation/): [Core Audio](https://developer.apple.com/documentation/coreaudio),
  [Core Audio Types](https://developer.apple.com/documentation/coreaudiotypes/)
* [Device input using the HAL Output Audio Unit](https://developer.apple.com/library/archive/technotes/tn2091/_index.html)
* [Multi-platform audio header](https://github.com/zserge/beep/blob/master/beep.h)
* [Tone generator using Audio Units](https://christianfloisand.wordpress.com/2013/07/30/building-a-tone-generator-for-ios-using-audio-units/)
* [Using Core Audio to create real-time I/O stream on OS X](https://stackoverflow.com/questions/28513924/how-can-i-use-apples-core-audio-c-api-to-create-a-simple-real-time-i-o-stream)

These resources provided useful discussion on OpenSSL and hashing:
* [OpenSSL cmake](https://stackoverflow.com/questions/45548088/include-openssl-in-a-cmakelist-txt-file)
* [How to use SHA1 hashing in C programming](https://stackoverflow.com/questions/9284420/how-to-use-sha1-hashing-in-c-programming)
