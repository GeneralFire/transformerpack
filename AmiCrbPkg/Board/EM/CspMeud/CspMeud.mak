#***********************************************************************
#*                                                                     *
#*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
#*                                                                     *
#*      All rights reserved. Subject to AMI licensing agreement.       *
#*                                                                     *
#***********************************************************************

Prepare : PrepareWholeBin
PrepareWholeBin:
ifneq ($(BUILD_OS), $(BUILD_OS_LINUX))
	if exist $(BIOS_MEFW_SIGNED_FILE) @del $(BIOS_MEFW_SIGNED_FILE)
else
	if [ -f "$(BIOS_MEFW_SIGNED_FILE)" ]; then $(RM) $(BIOS_MEFW_SIGNED_FILE); fi
endif

GenWholeSignedImage:
ifneq ($(wildcard $(FWrootKey)),$(FWrootKey))
	$(ECHO) WARNING !!!'$(FWrootKey)' file with key can not be found.
else
ifeq ("$(wildcard $(MKDIR) $(BUILD_DIR)$(PATH_SLASH)AmiCrbMeRoms$(PATH_SLASH)NonBIOSValidate)","")
	$(MKDIR) $(BUILD_DIR)$(PATH_SLASH)AmiCrbMeRoms$(PATH_SLASH)NonBIOSValidate
endif
	$(ECHO) Generate whole signed binary
ifneq ($(BUILD_OS), $(BUILD_OS_LINUX))
ifeq ($(MEFWCAPSULE_CERT_FORMAT), 0)
# RSA2048_SHA256 signed certificate format. The FWroot key is provided in PKCS#1v1.2 DER encoded format. 
	if exist $(BIOS_MEFW_SIGNED) $(CRYPTCON) -g 87454886-F057-4032-A486-605A35435EDD -y -r1 -c $(FWrootKey) -q -l 4096 -f $(BIOS_MEFW_SIGNED) -o $(BIOS_MEFW_SIGNED_FILE)
	if not exist $(BIOS_MEFW_SIGNED) @echo WARNING !!! Signed Binary can not be found.
else
# PKCS#7 signed certificate. FWroot and FWpub keys are expected in PKCS#12 (.pfx) and DER encoded (.cer) file formats accordingly
	if exist $(BIOS_MEFW_SIGNED) $(CRYPTCON) -g 87454886-F057-4032-A486-605A35435EDD -r1 -c2 $(FWrootKey) -k $(FWpub) -l 4096 -f $(BIOS_MEFW_SIGNED) -o $(BIOS_MEFW_SIGNED_FILE)
	if not exist $(BIOS_MEFW_SIGNED) @echo WARNING !!! Signed Binary can not be found.
endif #($(MEFWCAPSULE_CERT_FORMAT), 0)
else
ifeq ($(MEFWCAPSULE_CERT_FORMAT), 0)
# RSA2048_SHA256 signed certificate format. The FWroot key is provided in PKCS#1v1.2 DER encoded format. 
	if [ -f "$(BIOS_MEFW_SIGNED)" ]; then $(CRYPTCON) -g 87454886-F057-4032-A486-605A35435EDD -y -r1 -c $(FWrootKey) -q -l 4096 -f $(BIOS_MEFW_SIGNED) -o $(BIOS_MEFW_SIGNED_FILE); fi
	if [ ! -f "$(BIOS_MEFW_SIGNED)" ]; then echo "WARNING !!! Signed Binary can not be found."; fi
else
# PKCS#7 signed certificate. FWroot and FWpub keys are expected in PKCS#12 (.pfx) and DER encoded (.cer) file formats accordingly
	if [ -f "$(BIOS_MEFW_SIGNED)" ]; then $(CRYPTCON) -g 87454886-F057-4032-A486-605A35435EDD -r1 -c2 $(FWrootKey) -k $(FWpub) -l 4096 -f $(BIOS_MEFW_SIGNED) -o $(BIOS_MEFW_SIGNED_FILE); fi
	if [ ! -f "$(BIOS_MEFW_SIGNED)" ]; then echo "WARNING !!! Signed Binary can not be found."; fi
endif #($(MEFWCAPSULE_CERT_FORMAT), 0)
endif #ifneq ($(BUILD_OS), $(BUILD_OS_LINUX))
endif #ifneq ($(wildcard $(FWrootKey)),$(FWrootKey))
#*************************************************************************
#*************************************************************************
#**                                                                     **
#**        (C)Copyright 1985-2018, American Megatrends, Inc.            **
#**                                                                     **
#**                       All Rights Reserved.                          **
#**                                                                     **
#**      5555 Oakbrook Parkway, Suite 200, Norcross, GA 30093           **
#**                                                                     **
#**                       Phone: (770)-246-8600                         **
#**                                                                     **
#*************************************************************************
#*************************************************************************