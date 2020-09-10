#**********************************************************************
#**********************************************************************
#**                                                                  **
#**        (C)Copyright 1985-2019, American Megatrends, Inc.         **
#**                                                                  **
#**                       All Rights Reserved.                       **
#**                                                                  **
#**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093        **
#**                                                                  **
#**                       Phone: (770)-246-8600                      **
#**                                                                  **
#**********************************************************************
#**********************************************************************

#**********************************************************************
# $Header: /Alaska/Tools/template.mak 6     1/13/10 2:13p Felixp $
#
# $Revision: 6 $
#
# $Date: 1/13/10 2:13p $
#**********************************************************************
#<AMI_FHDR_START>
#
# Name:	<ComponentName>.mak
#
# Description:	
#
#<AMI_FHDR_END>
#**********************************************************************

RomPatch : RunFitUtility

OUTPUT_DIR = $(OUTPUT_DIRECTORY)$(PATH_SLASH)$(TARGET)_$(TOOL_CHAIN_TAG)
export OUTPUT_DIR


.PHONY : RunFitUtility
RunFitUtility:
ifeq ("$(target)","MultiPlatformRom")
else
######################################## Linux Flow START ######################################## 
ifeq ($(BUILD_OS), $(BUILD_OS_LINUX))
	chmod +x $(FIT_UTILITY_LINUX:-@=)
	@$(ECHO) Run FIT Utility
	$(CP) -f $(OUTPUT_DIR)$(PATH_SLASH)FV$(PATH_SLASH)AMIROM.fd $(BIOS_TAG).ROM

ifeq ("$(wildcard $(BUILD_DIR)$(PATH_SLASH)FitUtility)","") 
	$(CP) -f $(FIT_UTILITY_LINUX) $(BUILD_DIR)$(PATH_SLASH)FitUtility
endif	

	@$(ECHO) $(FitTokens)
ifeq ($(INTEL_TXT_SUPPORT),1)
	@$(ECHO) Linux : Updating the TXT patch on $(BIOS_TAG).ROM
	$(BUILD_DIR)$(PATH_SLASH)FitUtility $(FitTokens)
else
	@$(ECHO) Linux : Updating the MICROCODE patch on $(BIOS_TAG).ROM
	$(BUILD_DIR)$(PATH_SLASH)FitUtility $(FitTokens)
endif

	$(CP) -f $(BIOS_TAG).ROM $(OUTPUT_DIR)$(PATH_SLASH)FV$(PATH_SLASH)AMIROM.fd

ifeq ($(SecureMod_SUPPORT),1)	
ifneq ("$(wildcard $(BIOS_TAG).ROM)","") 
	$(RM) $(BIOS_TAG).ROM
endif
endif
######################################## Linux Flow END ######################################## 

######################################## MSFT Flow START ######################################## 
else  
	@$(ECHO) Run FIT Utility
	$(CP) /Y $(OUTPUT_DIR)$(PATH_SLASH)FV$(PATH_SLASH)AMIROM.fd $(BIOS_TAG).ROM
	@if not exist $(BUILD_DIR)$(PATH_SLASH)FitUtility.exe $(CP) /Y $(FIT_UTILITY_MSFT) $(BUILD_DIR)$(PATH_SLASH)FitUtility.exe

	@$(ECHO) $(FitTokens)
ifeq ($(INTEL_TXT_SUPPORT),1)
	@$(ECHO) MSFT : Updating the TXT patch on $(BIOS_TAG).ROM
	$(BUILD_DIR)$(PATH_SLASH)FitUtility.exe $(FitTokens)
else
	@$(ECHO) MSFT : Updating the MICROCODE patch on $(BIOS_TAG).ROM
	$(BUILD_DIR)$(PATH_SLASH)FitUtility.exe $(FitTokens)
endif

	$(CP) /Y $(BIOS_TAG).ROM $(OUTPUT_DIR)$(PATH_SLASH)FV$(PATH_SLASH)AMIROM.fd

ifeq ($(SecureMod_SUPPORT),1)	
	if exist $(BIOS_TAG).ROM $(RM) $(BIOS_TAG).ROM
endif
endif  	
######################################## MSFT Flow End ######################################## 
endif

#**********************************************************************
#**********************************************************************
#**                                                                  **
#**        (C)Copyright 1985-2019, American Megatrends, Inc.         **
#**                                                                  **
#**                       All Rights Reserved.                       **
#**                                                                  **
#**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093        **
#**                                                                  **
#**                       Phone: (770)-246-8600                      **
#**                                                                  **
#**********************************************************************
#**********************************************************************