# Iomico Zephyr Course 2026

This project is forked from Iomico's Zephyr Course 2026 template project.  The
course covers topics of embedded development with Zephyr RTOS, starting from
environment set up and running an initial "blinky" application, to kernel
config or Kconfig language use, device tree and board files development, Zephyr
driver development (introductory) and more.

At the commit ``l5-task1`` where board file development takes place, the command
to compile the application necessarily changes a little to reflect a new board
name.  This is mentioned in the readme section "How To Build".

Work in this repo was carried out by the author, following the lectures, posted
online instruction and guidance of Iomico's course presenters.

## Hardware Involved

Applications across the commit history of this project were built and tested
using an STMicro nucleo_wl55jc development board.

## How To Build

For early commits of this project, commits up to the commit tagged l5-task1,
compile the blinky app by invoking west meta-tool:

```shell
west build -b nucleo_wl55jc -p always
```

As part of development workflows, Zephyr's meta-tool for building and flashing
can be invoked to produce an interactive Kconfig menu.  For a given supported
board one may append ``-t menuconfig`` to the ``west`` build command.

```shell
west build -b nucleo_wl55jc -p always -t menuconfig
```

Note the menuconfig option and argument may also appear in the middle of
arguments to ``west``.

### Build invocation for l5-task1

Lecture 5 tasks involve the creation of custom board files.  While no new
hardware is involved, these files represent a new board to Zephyr's build
system.  They require a change to invocation of ``west``.  For the newly
created board named ``nuestra-placa`` the build command is:

```shell
west build -b nuestra-placa -p always -- -DBOARD_ROOT=..
```

### Build invocation for l5-task2

```shell
west build -b mi-placa -p always -- -DBOARD_ROOT=..
```

Note that for the Zephyr driver tasks (lecture 6) and attempted shell work
(lecture 7) the custom board ``nuestra-placa`` was used, so the west build command
for l5-task1 should be used for l6 tasks and going forward.

## How To Flash

For some STMicro dev board reasons ``west flash ...`` did not work following
the Zephyr environment set up instructions for building on a local host.  The
author wrote a shell script to encapsulate the specific config files and
arguments to ``openocd``, to work with the nucleo_wl55jc.  This script is
invoked:

```shell
./flash-manually.sh -p ../scripts/ -c oocd-wl55jc.cfg -f
```

This line with multiple options is wrapped in a yet shorter helper script:

```shell
./flash.sh
```

The original issue with flashing and ``west`` goes to an apparently missing
utility named STM32Cubeprogrammer.  This runner did not appear to be available
following installation of Zephyr 4.3.0.  The program ``openocd`` has worked
reliably with STMicro's STLink-V2 and STLink-V3 built-in programmers.  (These
come on most development boards from STMicro.)

TODO:  figure out how to patch the arguments and files passed to Zephyr's
west meta-tool to support flashing the nucleo_wl55jc.

## Expected Outputs

Start up messages from app tagged 'l6-task1'.

Early commits up through tags l5-task2 are all the same or similar, and begin
with Zephyr's kernel boot time banner message.  At the point of Zephyr driver
creation exercises, there is an interesting pre-banner message.

```
[00:00:00.000,000] <inf> nn_driver: nn_driver init() function called
*** Booting Zephyr OS build v4.2.0 ***
[00:00:00.000,000] <inf> nn_driver: Hello from nn_driver_channel_get()
[00:00:00.000,000] <inf> main: back from call to nn_driver API, status = 0
[00:00:00.000,000] <inf> main: LED state: OFF
[00:00:01.000,000] <inf> main: LED state: ON
[00:00:02.000,000] <inf> main: LED state: OFF
[00:00:03.000,000] <inf> main: LED state: ON
[00:00:04.000,000] <inf> main: LED state: OFF
[00:00:05.000,000] <inf> main: LED state: ON
```

- Outputs from l6-task2: custom driver replacing DEVICE_API() use

```
[00:00:00.000,000] <inf> nn_driver: nn_driver init() function called
*** Booting Zephyr OS build v4.2.0 ***
[00:00:00.000,000] <inf> nn_driver: nn_driver extended API settings
[00:00:00.000,000] <inf> nn_driver: active_led: 0
[00:00:00.000,000] <inf> nn_driver: id: . . . . 0
[00:00:00.000,000] <inf> main: - DEV 0530 - setting driver id to non-zero value . . .
[00:00:00.000,000] <inf> main: - DEV 0530 - showing settings again:
[00:00:00.000,000] <inf> nn_driver: nn_driver extended API settings
[00:00:00.000,000] <inf> nn_driver: active_led: 0
[00:00:00.000,000] <inf> nn_driver: id: . . . . 555
[00:00:00.000,000] <inf> main: LED state: OFF
[00:00:01.000,000] <inf> main: LED state: ON
[00:00:02.000,000] <inf> main: LED state: OFF
[00:00:03.000,000] <inf> main: LED state: ON
[00:00:04.000,000] <inf> main: LED state: OFF
```

The key addition in the custom driver is the changing of a driver side data,
here the ``id`` field.  In the app it is changed from a starting value of 0 to
the value 555.  A small integer value to change was chosen because it does not
depend on any special hardware, not even an LED or GPIO.

## TODO / Not Completed

For lack of time the second task for lecture 7 on Zephyr shell, and the tasks
for Zephyr ztest framework were not completed by day's end 2026 May 31.

## Reference

Class topics, resources and ssignments are outlined at:

- https://iomico.atlassian.net/wiki/external/OTFlYTBiYmVjYjU5NGY2M2IyOWJhNGY4ZTQxZWM5ODg
