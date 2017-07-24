Simple SW for controlling KA radio only via rotary encoder
==========================================================

Wiring:

Rotary encoder |  Arduino | KA radio
------------ | -------------| -------------
GND|GND|GND
+|5V
DT|4
CLK|2
SW|8
.|TX|RX

Functions:
============================
Rotating knob : change volume / station (it depends on mode)

Short knob press: change mode: volume <-> station

Long press of knob (>300ms) : stop/play  

Added LED indication
