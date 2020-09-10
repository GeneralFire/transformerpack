#***********************************************************************
#*                                                                     *
#*   Copyright (c) 1985-2020, American Megatrends International LLC.   *
#*                                                                     *
#*      All rights reserved. Subject to AMI licensing agreement.       *
#*                                                                     *
#***********************************************************************
#***********************************************************************
#<AMI_FHDR_START>
#
# Name:	AmiBuildOpiton.mak
#
# Description:	Set PACKAGES_PATH for supporting multiple workspaces ###
#
#<AMI_FHDR_END>
#**********************************************************************

.PHONY : SetPackagesPath

Prepare : SetPackagesPath

SetPackagesPath:
ifneq ($(PACKAGES_PATH),"")
### Set PACKAGES_PATH for supporting multiple workspaces ###
export PACKAGES_PATH
endif
