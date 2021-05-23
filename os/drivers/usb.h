#ifndef __USBH__
#define __USBH__
#define USB_CONTROLLER_CLASS 0xC
#include "pci.h"

struct usb_controller{
    struct pci_device *device;

    //Not sure what else I need to include here
};

struct usb_device{

};

struct usb_controller usb_pci_controller;

//This may need to change whenever I figure out if a computer can have more than one usb controller
void usb_get_controller();

#endif