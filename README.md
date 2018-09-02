# linux-kernel-module

Simple examples of Linux Kernel Modules, written as a learning exercise.

<!-- vim-markdown-toc GFM -->

* [Sample 1: Greeter](#sample-1-greeter)
    * [Building the Module](#building-the-module)
    * [Installing / Uninstalling](#installing--uninstalling)
* [Tips](#tips)
* [Reading](#reading)

<!-- vim-markdown-toc -->

## Sample 1: Greeter

This sample simply takes a name as a parameter, and writes a greeting to the kernel log (`/var/log/kern.log`). `cd` into `greeter/` before running any of the commands before.

### Building the Module

```sh
make
```

The module is compiled to `greeter.ko`.

### Installing / Uninstalling

```sh
# Install:
sudo insmod greeter.ko

# Install with parameters:
sudo insmod greeter.ko name=Frodo

# Uninstall
sudo rmmod greeter
```

## Tips

Show loaded modules with:

```sh
lsmod
```

Show `menuconfig` with:

```sh
sudo make menuconfig
```

Show the kernel log with:

```sh
dmesg
```

Show info about the module:

```sh
modinfo greeter.ko
```

Note: if you get the following error:

```
insmod: ERROR: could not insert module greeter.ko: Invalid parameters
```

Then make sure you are not trying to install the module from a shared location (such as a shared folder on a virtual machine). Copy it to the home directory and install from there instead.

## Reading

The bulk of this code came from:

- http://derekmolloy.ie/writing-a-linux-kernel-module-part-1-introduction/#prettyPhoto

With the inspiration from the "Introduction to Linux Kernels" webinar from the Linux Foundation.
