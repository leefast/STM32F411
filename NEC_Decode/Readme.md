
Thanh Phat
# To use this library, u must init Timer Input Capture DMA
In this Project, i use Timer 2 - Channel 1 - PA0 pin to use input capture FALLING EDGE

Timer tick 1us

By the way, i use UART2 DMA to transmit data from MCU to PC

# First, I Start Input capture DMA to get header - time(duration between 2 falling edge)
# -----------Header Type---------
# if Time > Type_Boundary (frequently 12.5 ms) -> Data Frame
# else                                            Repeat Frame
# ----------Data Frame-----------
#    addr--naddr--com--ncom
# 		bit 1: duration between 2 input capture > 2000us
# 		bit 0:                                    2000us
