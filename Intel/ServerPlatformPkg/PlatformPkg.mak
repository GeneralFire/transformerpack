#***********************************************************************
#*                                                                     *
#*   Copyright (c) 1985-2020, American Megatrends International LLC.   *
#*                                                                     *
#*      All rights reserved. Subject to AMI licensing agreement.       *
#*                                                                     *
#***********************************************************************

## @file PlatformPkg.mak
#   Make file for Platform Package
##


Prepare : $(BUILD_DIR)$(PATH_SLASH)ProgramOemSvidSid.h

$(BUILD_DIR)$(PATH_SLASH)ProgramOemSvidSid.h : $(BUILD_DIR)$(PATH_SLASH)Token.mak $(ServerPlatformPkg_DIR)$(PATH_SLASH)PlatformPkg.mak
	@$(ECHO) \
" #define PROGRAM_IIO_OEM_SVID_SID_LIST $(PROGRAM_IIO_OEM_SVID_SID)$(EOL)\
$(EOL)\
#define PROGRAM_PCH_OEM_SVID_SID_LIST $(PROGRAM_PCH_OEM_SVID_SID)$(EOL)"\
>$(BUILD_DIR)/ProgramOemSvidSid.h
