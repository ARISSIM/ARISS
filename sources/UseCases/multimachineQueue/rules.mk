sp 		:= $(sp).x
dirstack_$(sp)	:= $(d)
d		:= $(dir)

#the directory were the make function have to search must be incuded here following this template
#dir			:= $(d)/template
#include		$(dir)/rules.mk

dir	:= $(d)/f1
include	$(dir)/rules.mk

dir	:= $(d)/f2
include	$(dir)/rules.mk

dir	:= $(d)/f3
include	$(dir)/rules.mk

dir	:= $(d)/f4
include	$(dir)/rules.mk

#for the automatic creation of symbolic links
$(d).sym :  $(d)/Configuration $(d)/../../GUI/gui.py $(d)/../../GUI/gui_for_emulator.py $(d)/../../../binary/simulator/simulator.out
SYMLINKS	+= $(d).sym
#

d		:= $(dirstack_$(sp))
sp		:= $(basename $(sp))

