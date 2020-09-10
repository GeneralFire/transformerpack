#***********************************************************************
#*                                                                     *
#*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
#*                                                                     *
#*      All rights reserved. Subject to AMI licensing agreement.       *
#*                                                                     *
#***********************************************************************
#<AMI_FHDR_START>
#----------------------------------------------------------------------
#
# Name: Ast2600.mak
#
# Description: SIO module MAK file.
#
#----------------------------------------------------------------------
#<AMI_FHDR_END>
#**********************************************************************
#Include this only for Static Setup 
#ifeq ("$(SIO_SETUP_USE_APTIO_4_STYLE)", "1")) 
#----------------------------------------------------------------------
Prepare : BuildSioElink 

#----------------------------------------------------------------------
#       Generate SIO elink table
#		1. SIO init string table for SioSetup.c
#----------------------------------------------------------------------
BuildSioElink : $(BUILD_DIR)/Token.mak
	$(ECHO) \
"\
#ifndef _SIO_ELINK_H_$(EOL)\
#define _SIO_ELINK_H_$(EOL)\
$(EOL)\
#define SIO_Init_Str_LIST $(SetupStrTableList)$(EOL)\
#endif$(EOL)\
"> $(BUILD_DIR)/SIOElink.h

#----------------------------------------------------------------------
#endif
