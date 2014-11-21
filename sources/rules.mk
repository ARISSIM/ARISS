######################
# ARINC653-simulator #
######################
# Sources head rules.mk

sp 		:= $(sp).x
dirstack_$(sp)	:= $(d)
d		:= $(dir)


dir	:= $(d)/simulator
include	$(dir)/rules.mk

dir    := $(d)/UseCases
include        $(dir)/rules.mk


d		:= $(dirstack_$(sp))
sp		:= $(basename $(sp))

