# finny
 A Linux front-end for the RadioSHARK2 AM/FM usb device.
 
#Summary

Finny is a Linux front-end for the RadioSHARK2 AM/FM usb receiver, intended for in-auto "carputer" or automobile-PC applications. It can also be used on home PCs and workstations, however. It is Qt4 based, with Gstreamer based audio capture, and some ALSA calls. It uses a PulseAudio server as an audio device.

#Status

Imported from Google Code when that was shutdown. Should still build and run if you have the hardware.

#Screenshots

Finny with visualization and presets enabled.

http://lh4.ggpht.com/_Gop86JBIkTc/SwCMWvjN_1I/AAAAAAAAAEc/BtwdYn9VHKg/s400/Screenshot-Finny%20-%20the%20RadioShark2%20Manager-4.png

Finny without options bar

http://lh5.ggpht.com/_Gop86JBIkTc/SwCMW9cZkwI/AAAAAAAAAEg/f2WC1l7Xv_0/s400/Screenshot-Finny%20-%20the%20RadioShark2%20Manager-5.png

Visualization Only

http://lh5.ggpht.com/_Gop86JBIkTc/SwCMWw6JXcI/AAAAAAAAAEk/a8cTFQXew1I/s400/Screenshot-Finny%20-%20the%20RadioShark2%20Manager-6.png

Credit Where It's Due

Its HID interface to the radioSHARK2 is largely based upon the execellent work of: * Michael Rolig * Hisaaki Shibata * Kawabe

##Design Objectives

Finny was put together with the following objectives:

* Simple. The Code should be as small as it possibly can be.
* As much as possible, it should be based upon standard Qt widgets.
* A person driving should be able to use it.
* The buttons ought to be large and easily found.
* Some small safety features ought to prevent the user from pressing "dangerous" buttons, like "erase preset" and "record."
* The text ought to be large, friendly and easily readable.
* The application ought to be usable when presented fullscreen on a small auto-pc screen of say 600x400.
* Finny will not be a general media player. It will only play back live audio from the RadioSHARK2 device.
* Finny will be able to record live audio to mp3 format for playback on another application or device.
* Finny will not include a way to schedule radio events to record. This has no application to the "carputer" environment, and on a workstation is more easily done via the command line.

##Implemented Functionality

Currently, finny can do the following:

* Control RadioSHARK2 fully, setting AM and FM frequencies.
* Step (quickly or slowly) over frequencies.
* Save, remove presets, and load presets at startup.
* Play live captured AM/FM audio.
* Record live captured AM/FM audio to mp3 format.
* General application settings in settings dialog.
* Correctly initializes the RS to a set (or last) frequency at startup.
* Visualization while muted.
* Basic control of MP3 recording quality.
* Libvisual, monoscope, and goom visualization support.
* Disable screensaver while playing option.
* Change of visualization at runtime.
* Ximagesink and Xvimagesink support.
