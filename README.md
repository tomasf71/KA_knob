Simple SW for controlling KA radio only via rotary encoder
==========================================================

Wiring:
Rotary encoder |  Arduino | KA radio
------------ | -------------| -------------
GND|GND|GND
+|5V
DT|4
CLK|2
TX|RX

Functions:
============================
Rotating knob : change volume / station (it depends on mode)
Short knob press: change mode: volume <-> station
Long presentation of knob (>1s) : stop/play  
