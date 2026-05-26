# Iomico Zephyr Course 2026

This project is forked from Iomico's Zephyr Course 2026 template project.  The
commits here represent work carried out by the author, following the
instruction and guidance of Iomico's course presenters.

## Hardware Involved

Every attendee has their own development board or target hardware for testing
their Zephyr applications.  Ted Havelka happens to be using a Nucleo wl55jc
board.

## How To Build

For early commits of this project, commits up to the commit tagged l5-task1,
compile the blinky app by invoking west meta-tool:

```shell
west build -b nucleo_wl55jc -p always
```

Build Kconfig menuconfig target for an interactive session to see and
potentially make changes to project Kconfig settings:

```shell
west build -b nucleo_wl55jc -p always -t menuconfig
```

## Build invocation for l5-task1

```shell
west build -b nuestra-placa -p always -- -DBOARD_ROOT=..
```

## Build invocation for l5-task2

west build -b mi-placa -p always -- -DBOARD_ROOT=..

## Expected Outputs

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
