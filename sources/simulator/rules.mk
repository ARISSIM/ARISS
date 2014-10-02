sp              := $(sp).x
dirstack_$(sp)  := $(d)
d               := $(dir)

TARGET					:= $(call SRC_2_BIN, $(d)/simulator.out)
TARGETS 				+= $(call SRC_2_BIN, $(TARGET))
OBJS_$(d)				:= $(call SRC_2_OBJ, \
	$(d)/main.o $(d)/CConfig.o $(d)/CManager.o $(d)/CPartition.o $(d)/Cport_service.o)
OBJECTS 				+= $(OBJS_$(d))
#OBJS_$(d)				:= $(call SRC_2_OBJ, \
#	$(d)/main.o $(d)/CConfig.o $(d)/CManager.o $(d)/CPartition.o $(d)/Cport_service.o $(d)/common/CArgument.o $(d)/common/CQueuing.o $(d)/common/CSampling.o $(d)/common/CBasefunction.o)
#OBJECTS 				+= $(OBJS_$(d))

#$(OBJS_$(d))		:  CC_FLAGS_SPEC	:= 

$(TARGET)		:  $(OBJS_$(d))

d               := $(dirstack_$(sp))
sp              := $(basename $(sp))

