#***********************************************************************
#*                                                                     *
#*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
#*                                                                     *
#*      All rights reserved. Subject to AMI licensing agreement.       *
#*                                                                     *
#***********************************************************************
#<AMI_FHDR_START>
#-----------------------------------------------------------------------
#
# Name:         RebuildFspBios.mak
#
# Description: Build FSP and BIOS mak
#
#-----------------------------------------------------------------------
#<AMI_FHDR_END>

Prepare: SaveRestoreFspConfig

all: FinishRebuildFsp

.PHONY : SaveRestoreFspConfig FinishRebuildFsp

SaveRestoreFspConfig:
ifeq ($(BUILD_FSP_BINARY), 1)
	attrib -r $(REBUILD_FSP_DIR)$(PATH_SLASH)RebuildFspBios.sdl
	$(CP) /b /y $(REBUILD_FSP_DIR)$(PATH_SLASH)RebuildFspBios.sdl $(REBUILD_FSP_DIR)$(PATH_SLASH)RebuildFspBiosO.sdl
	@$(ECHO)		
	@$(ECHO) !!!!!!!!!!!!! Building FSP Binary !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	@$(ECHO) !!!!!!!!!!!!! Building FSP Binary !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	@$(ECHO) !!!!!!!!!!!!! Building FSP Binary !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	@$(ECHO)	
else
	$(CP) /b /y $(REBUILD_FSP_DIR)$(PATH_SLASH)RebuildFspBiosO.sdl $(REBUILD_FSP_DIR)$(PATH_SLASH)RebuildFspBios.sdl
	$(RM) $(REBUILD_FSP_DIR)$(PATH_SLASH)RebuildFspBiosO.sdl
endif

FinishRebuildFsp:
ifeq ($(BUILD_FSP_BINARY), 1)
	$(CP) /b /y $(REBUILD_FSP_DIR)$(PATH_SLASH)RebuildFspBiosO.sdl+$(REBUILD_FSP_DIR)$(PATH_SLASH)RebuildFspBios.xdl $(REBUILD_FSP_DIR)$(PATH_SLASH)RebuildFspBios.sdl
	@$(ECHO)		
	@$(ECHO) !!!!!!!!!!!!! Building BIOS  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	@$(ECHO) !!!!!!!!!!!!! Building BIOS  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	@$(ECHO) !!!!!!!!!!!!! Building BIOS  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!		
	@$(ECHO)
	
	$(REBUILD_FSP_DIR)$(PATH_SLASH)RebuildFspBios.bat

endif	
