# CHIPPER

A CHIP-8 interpreter written in C++ and SFML

![pong](https://github.com/2bit-hack/CHIPPER/blob/master/img/pong.png "pong")

![blinky](https://github.com/2bit-hack/CHIPPER/blob/master/img/blinky.png "blinky")

## Work In Progress

### TODO

* Implement audio
* Implement 60hz frame perfect emulation
* Test more ROMS

### Build


Make sure SFML is included in the compiler standard include path

```
make
./chip [ROM name]
```

For a list of available ROMs, check the ```roms``` folder

Run
```
make clean
```
to clean up

### Resources

* [Cowgod's CHIP-8 Technical Reference](http://devernay.free.fr/hacks/chip8/C8TECH10.HTM)
* [Mastering CHIP-8 by Matthew Mikolay](http://mattmik.com/files/chip8/mastering/chip8.html)
* [Wikipedia article on CHIP-8](https://en.wikipedia.org/wiki/CHIP-8)
* [A collection of ROMs from Zophar's Domain](https://www.zophar.net/pdroms/chip8/chip-8-games-pack.html)
* [A test ROM by corax89](https://github.com/corax89/chip8-test-rom)
* [Another test ROM by BestCoder](https://slack-files.com/T3CH37TNX-F3RF5KT43-0fb93dbd1f)