# Introduction: 
    Hi there! Thanks for checking out my hobbyist operating system, AbstractOS! 

    This project is solely for learning purposes and fun, building everything from the ground up. This uses a custom bootloader I'm writing from scratch and will use a custom kernel in the near future!

# What the OS is capable of:
    Currently, not much. I am still in the early phases of writing the aforementioned bootloader. It's capable of booting the system, mapping memory, printing output, and an easy to use interrupt system. The next big milestone I am working towards is enumerating the PCI bus and interfacing with AHCI disk drives.

# How to build and run: 
- Firstly, you will need to make sure you have GCC and NASM to compile and assemble code. You will also need Make, and  GNU ld for linking and building the image
- After making sure you have those installed, navigate to the "boot" directory and use the "make" command
- You should find a new directory called "build" under the main project directory, in it you will find a file named "os.hdd", that is the raw binary image for the operating system
- Load that into any emulator you wish, currently I am only testing using VirtualBox.

And that's it! 