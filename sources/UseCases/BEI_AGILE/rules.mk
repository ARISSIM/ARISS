sp 		:= $(sp).x
dirstack_$(sp)	:= $(d)
d		:= $(dir)

dir	:= $(d)/Capteur
include	$(dir)/rules.mk

dir	:= $(d)/Primary
include	$(dir)/rules.mk

dir	:= $(d)/Backup
include	$(dir)/rules.mk

#for the automatic creation of symbolic links
$(d).sym :  $(d)/Configuration $(d)/../../GUI/gui.py $(d)/../../GUI/gui_for_emulator.py $(d)/../../../binary/simulator/simulator.out
SYMLINKS	+= $(d).sym
#

d		:= $(dirstack_$(sp))
sp		:= $(basename $(sp))

