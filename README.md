# kernel module

This is a simple example of a kernel module.

## Building the Module

```sh
make
```

The module is compiled to `goodbye.ko`.

## Installing / Uninstalling

```sh
# Install:
sudo insmod goodbye.ko

# Install with parameters:
sudo insmod goodbye.ko name=Frodo

# Uninstall
sudo rmmod goodbye
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
modinfo goodbye.ko
```

Note: if you get the following error:

```
insmod: ERROR: could not insert module goodbye.ko: Invalid parameters
```

Then make sure you are not trying to install the module from a shared location (such as a shared folder on a virtual machine). Copy it to the home directory and install from there instead.

## Reading

The bulk of this code came from:

- http://derekmolloy.ie/writing-a-linux-kernel-module-part-1-introduction/#prettyPhoto

With the inspiration from the "Introduction to Linux Kernels" webinar from the Linux Foundation.
