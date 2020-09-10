#***********************************************************************
#*                                                                     *
#*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
#*                                                                     *
#*      All rights reserved. Subject to AMI licensing agreement.       *
#*                                                                     *
#***********************************************************************

#This should match with ASL_FLAGS in MAKEFILE of AcpiTables.inf in Build folder.
#Add default flags from AmiPkg\Configuration\CommonDef.txt file
#No defaults defined
ACPITABLE_ASL_FLAGS = 
#Flag -so: Generates offset.h file
#Flag -ve: Report only errors (ignore warnings and remarks)
ACPITABLE_ASL_FLAGS += -ve -so
#Add additional flags here. Ex: Overrides done in DSC file and etc.
ACPITABLE_ASL_FLAGS += -oi
#Add additional flags here. flags added to Match the AcpiTables.inf
ACPITABLE_ASL_FLAGS += -vw 3133 -vw 2008

#This should match with ASL_OUTFLAGS in MAKEFILE of AcpiTables.inf in Build folder
#Add default flags from AmiPkg\Configuration\CommonDef.txt file
ACPITABLE_ASL_OUTFLAGS = -p
#Add additional flags here. Ex: Overrides done in DSC file and etc.
#ACPITABLE_ASL_OUTFLAGS += 

#This should match with ASLPP in MAKEFILE of AcpiTables.inf in Build folder
ifeq ($(BUILD_OS),$(BUILD_OS_WINDOWS))
ACPITABLE_ASLPP =  $(CCX64DIR)\cl.exe

#This should match ASLPP_FLAGS in MAKEFILE of AcpiTables.inf in Build folder
#Add default flags from AmiPkg\Configuration\CommonDef.txt file
ACPITABLE_ASLPP_FLAGS = /nologo /E /TC
else
#This should match with ASLPP in MAKEFILE of AcpiTables.inf in Build folder
ACPITABLE_ASLPP =  gcc
#This should match ASLPP_FLAGS in MAKEFILE of AcpiTables.inf in Build folder
#Add default flags from AmiPkg\Configuration\CommonDef.txt file
ACPITABLE_ASLPP_FLAGS = -x c -E
endif
#Include INTEL RC CC Flags for pre-processing
ACPITABLE_ASLPP_FLAGS += $(EDKII_DSC_FEATURE_BUILD_OPTIONS)
#Add additional flags here. Ex: Overrides done in DSC file and etc.
#ACPITABLE_ASLPP_FLAGS += 

#Include files for Trim and Iasl tools.
TRIM_INC_LIST = $(WORKSPACE)$(PATH_SLASH)$(AcpiTables_DIR)$(PATH_SLASH)TrimIncludes.lst
include $(WORKSPACE)$(PATH_SLASH)$(AcpiTables_DIR)$(PATH_SLASH)IaslIncludes.lst

export BUILD_DIR AcpiTables_DIR  AmlOffsetHeaderFile_DEST_DIR

CreateAmlOffsetHeaderFile:
	@echo "***************** Generate AML offset header file Start *****************"
# CpuPm.offset.h is not consumed in the source.
ifeq (0, 1)
	@echo "----- Generating SSDT CpuPm.offset.h -----"
	Trim --asl-file -o $(WORKSPACE)$(PATH_SLASH)$(BUILD_DIR)$(PATH_SLASH)CpuPm.i -i $(TRIM_INC_LIST) $(WORKSPACE)$(PATH_SLASH)$(AcpiTables_DIR)$(PATH_SLASH)Ssdt$(PATH_SLASH)CpuPm.asl
	"$(ACPITABLE_ASLPP)" $(ACPITABLE_ASLPP_FLAGS) $(ACPITABLE_ASL_INC_LIST) $(BUILD_DIR)$(PATH_SLASH)CpuPm.i > $(BUILD_DIR)$(PATH_SLASH)CpuPm.iii
	Trim --source-code -l -o $(WORKSPACE)$(PATH_SLASH)$(BUILD_DIR)$(PATH_SLASH)CpuPm.iiii $(WORKSPACE)$(PATH_SLASH)$(BUILD_DIR)$(PATH_SLASH)CpuPm.iii
	"$(PLATFORM_ASL_COMPILER)" $(ACPITABLE_ASL_FLAGS) $(ACPITABLE_ASL_OUTFLAGS) $(WORKSPACE)$(PATH_SLASH)$(BUILD_DIR)$(PATH_SLASH)CpuPm.aml $(WORKSPACE)$(PATH_SLASH)$(BUILD_DIR)$(PATH_SLASH)CpuPm.iiii
ifeq ($(BUILD_OS),$(BUILD_OS_WINDOWS))	
	@call $(AcpiTables_DIR)$(PATH_SLASH)FilterOutAmlOffsetHeaderFile.bat SSDT CpuPm
else
     chmod +x $(AcpiTables_DIR)$(PATH_SLASH)FilterOutAmlOffsetHeaderFile.sh 
     $(AcpiTables_DIR)$(PATH_SLASH)FilterOutAmlOffsetHeaderFile.sh SSDT CpuPm
endif
    @echo "----- SSDT CpuPm.offset.h file is generated -----"
endif	
	
	@echo "----- Generating DSDT DsdtAsl.offset.h -----"
# Using DsdtAsl.asl from ACPI module.
	Trim --asl-file -o $(WORKSPACE)$(PATH_SLASH)$(BUILD_DIR)$(PATH_SLASH)DsdtAsl.i -i $(TRIM_INC_LIST) $(WORKSPACE)$(PATH_SLASH)$(ACPI_BOARD_DIR)$(PATH_SLASH)DsdtAsl.asl
	"$(ACPITABLE_ASLPP)" $(ACPITABLE_ASLPP_FLAGS) $(ACPITABLE_ASL_INC_LIST) $(BUILD_DIR)$(PATH_SLASH)DsdtAsl.i > $(BUILD_DIR)$(PATH_SLASH)DsdtAsl.iii
	Trim --source-code -l -o $(WORKSPACE)$(PATH_SLASH)$(BUILD_DIR)$(PATH_SLASH)DsdtAsl.iiii $(WORKSPACE)$(PATH_SLASH)$(BUILD_DIR)$(PATH_SLASH)DsdtAsl.iii
ifeq ($(BUILD_OS), $(BUILD_OS_WINDOWS))
	"$(PLATFORM_ASL_COMPILER)" $(ACPITABLE_ASL_FLAGS) $(ACPITABLE_ASL_OUTFLAGS) $(WORKSPACE)$(PATH_SLASH)$(BUILD_DIR)$(PATH_SLASH)DsdtAsl.aml $(WORKSPACE)$(PATH_SLASH)$(BUILD_DIR)$(PATH_SLASH)DsdtAsl.iiii
else
	chmod +x $(PLATFORM_ASL_GCC_COMPILER)
	"$(PLATFORM_ASL_GCC_COMPILER)" $(ACPITABLE_ASL_FLAGS) $(ACPITABLE_ASL_OUTFLAGS) $(WORKSPACE)$(PATH_SLASH)$(BUILD_DIR)$(PATH_SLASH)DsdtAsl.aml $(WORKSPACE)$(PATH_SLASH)$(BUILD_DIR)$(PATH_SLASH)DsdtAsl.iiii
endif
ifeq ($(BUILD_OS),$(BUILD_OS_WINDOWS))
	@call $(AcpiTables_DIR)$(PATH_SLASH)FilterOutAmlOffsetHeaderFile.bat DSDT DsdtAsl
else
	chmod +x $(AcpiTables_DIR)$(PATH_SLASH)FilterOutAmlOffsetHeaderFile.sh
	$(AcpiTables_DIR)$(PATH_SLASH)FilterOutAmlOffsetHeaderFile.sh DSDT DsdtAsl
endif	
	@echo "----- DSDT DsdtAsl.offset.h file is generated -----"


	@echo "***************** Generate AML offset header file End *****************"
	