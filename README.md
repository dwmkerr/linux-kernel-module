# linux-kernel-module

[![GuardRails badge](https://badges.production.guardrails.io/dwmkerr/linux-kernel-module.svg)](https://www.guardrails.io)

Simple examples of Linux Kernel Modules, written as a learning exercise.

<!-- vim-markdown-toc GFM -->

* [Sample 1: Greeter](#sample-1-greeter)
    * [Building the Module](#building-the-module)
    * [Installing / Uninstalling](#installing--uninstalling)
* [Sample 2: Babel](#sample-2-babel)
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

## Sample 2: Babel

`babel` is a character driver which adds a device called `/dev/babel`. This is a device you can 'talk' to. It'll babble back in gibberish.

Install with:

```sh
cd babel
make
sudo insmod babel.ko
```

Then run the test client, which'll let you chat with the `/dev/babel` device:

```sh
sudo ./babel-client
```

Uninstall with:

```sh
sudo rmmod babel
```

Make the `/dev/babel` device read/write accessible without super user priviledges by adding a rule. First get the Kernal and Subsystem name:

```sh
udevadm info -a -p /sys/class/babel/babel

# Will show something like:
#   KERNEL=="babel"
#   SUBSYSTEM=="babel"
```

Now create a low-priority rule to enable user access:

```sh
echo 'KERNEL=="babel", SUBSYSTEM=="babel", MODE="0666"' >> /etc/udev/rules.d/99-babel.rules
```

Reload the driver and it will be accessible without superuser rights.

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

Trace module calls with:

```sh
sudo apt-get install strace
sudo strace ./babel/babel_client
```

`strace` will show low-level system calls in realtime as the program makes them.

## Reading

The bulk of this code came from:

- http://derekmolloy.ie/writing-a-linux-kernel-module-part-1-introduction/#prettyPhoto

With the inspiration from the "Introduction to Linux Kernels" webinar from the Linux Foundation.
