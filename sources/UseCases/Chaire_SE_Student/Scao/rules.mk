sp              := $(sp).x
dirstack_$(sp)  := $(d)
d               := $(dir)

TARGET					:= $(call SRC_2_BIN, $(d)/Scao.out)
TARGETS 				+= $(call SRC_2_BIN, $(TARGET))
OBJS_$(d)				:= $(call SRC_2_OBJ, \
																	$(d)/common/CArgument.o $(d)/common/CQueuing.o $(d)/common/CSampling.o $(d)/common/CBasefunction.o $(d)/../SCAO.o $(d)/../Analyseur.o $(d)/../Camera.o $(d)/../Plan.o $(d)/../Memoire_stable.o $(d)/../Checkpoint.o $(d)/../Table.o $(d)/../Horloge.o $(d)/../Manager.o $(d)/../CarteComm.o $(d)/../CarteRecep.o $(d)/Scao.o)
OBJECTS 				+= $(OBJS_$(d))

$(OBJS_$(d))		:  CC_FLAGS_SPEC	:= -lpthread

$(TARGET)				:  $(OBJS_$(d))

d               := $(dirstack_$(sp))
sp              := $(basename $(sp))
