#***********************************************************************
#*                                                                     *
#*   Copyright (c) 1985-2020, American Megatrends International LLC.   *
#*                                                                     *
#*      All rights reserved. Subject to AMI licensing agreement.       *
#*                                                                     *
#***********************************************************************

Prepare : $(BUILD_DIR)/NbInc.h

AMI_CSP_LIB_INCLUDE_FILES := $(BUILD_DIR)/NbInc.h \
$(AMI_CSP_LIB_INCLUDE_FILES)

$(BUILD_DIR)/NbInc.h : $(NB_BOARD_DIR)/Nb.h $(NB_CHIPSET_DIR)/NBCspLib.h $(NB_BOARD_DIR)/Nb.mak
	$(ECHO) \
"// Don't delete this line$(EOL)\
#include <$(NB_BOARD_DIR)/Nb.h>$(EOL)\
#include <$(NB_CHIPSET_DIR)/NBCspLib.h>$(EOL)\
// Don't delete this line$(EOL)"\
> $@
