######################
# ARINC653-simulator #
######################
# UseCases rules.mk

sp 		:= $(sp).x
dirstack_$(sp)	:= $(d)
d		:= $(dir)

#keep only the needed use case

#dir    := $(d)/Chaire_SE_Student
#include        $(dir)/rules.mk

dir    := $(d)/BEI_AGILE
include        $(dir)/rules.mk

dir	:= $(d)/JNI_Template
include	$(dir)/rules.mk

#dir	:= $(d)/multimachine
#include	$(dir)/rules.mk

dir	:= $(d)/Multithreading
include	$(dir)/rules.mk

dir	:= $(d)/petshop
include	$(dir)/rules.mk

dir	:= $(d)/Queueing
include	$(dir)/rules.mk

dir	:= $(d)/Sampling
include	$(dir)/rules.mk



#To add a new use case, duplicate the Template directory
#dir	:= $(d)/Template
#include	$(dir)/rules.mk

d		:= $(dirstack_$(sp))
sp		:= $(basename $(sp))

