#***********************************************************************
#*                                                                     *
#*   Copyright (c) 1985-2020, American Megatrends International LLC.   *
#*                                                                     *
#*      All rights reserved. Subject to AMI licensing agreement.       *
#*                                                                     *
#***********************************************************************
## @file DsmAsl.mak
#   Build rules for creating PciSlot.asl file
#   that creates _DSM methods for the given Slot instance and name. 
##

Prepare : DsmAsl

DsmAsl: $(BUILD_DIR)$(PATH_SLASH)PciSlot.asl	

$(BUILD_DIR)$(PATH_SLASH)PciSlot.asl : $(BUILD_DIR)$(PATH_SLASH)DsmAsl.lst
$(BUILD_DIR)$(PATH_SLASH)DsmAsl.lst : $(BUILD_DIR)$(PATH_SLASH)Token.mak $(DsmAsl_DIR)$(PATH_SLASH)DsmAsl.bat
ifneq ("$(wildcard $(BUILD_DIR)$(PATH_SLASH)PciSlot.asl)","")
	$(RM) $(BUILD_DIR)$(PATH_SLASH)PciSlot.asl
endif
	$(ECHO)"$(subst PCI_SLOT_DSM,$(EOL),$(DsmAsl:PCI_SLOT_DSM=^))" > $@
ifeq ($(BUILD_OS), $(BUILD_OS_WINDOWS))
	$(DsmAsl_DIR)$(PATH_SLASH)DsmAsl.bat $@
else
	chmod +x $(DsmAsl_DIR)$(PATH_SLASH)DsmAsl.sh
	$(DsmAsl_DIR)$(PATH_SLASH)DsmAsl.sh $@
endif