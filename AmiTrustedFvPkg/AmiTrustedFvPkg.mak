#***********************************************************************
#*                                                                     *
#*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
#*                                                                     *
#*      All rights reserved. Subject to AMI licensing agreement.       *
#*                                                                     *
#***********************************************************************


.PHONY : MAKE_FVDXE_SIG_IMAGE MAKE_FVDXE_SIG_MAP CLEAR_FVDXE_SIG_HELP_FILES	MAKE_FVDXE_SIG_BAT_FILES

#---------------------------------------------------------------------------
# Invoke cryptocon.exe to create FvDxe signature
#---------------------------------------------------------------------------
# Should be the last step after creating of the ROM image.
# All fixups to the .ROM must be made prior to this step.
#---------------------------------------------------------------------------
FVDXE_SIG_IMAGE := $(UNSIGNED_BIOS_ROM)
ifndef 	FWBUILD_OPTION_CHAR
ifeq ($(BUILD_OS), $(BUILD_OS_LINUX))
FWBUILD_OPTION_CHAR := -
else
FWBUILD_OPTION_CHAR := /
endif
endif
ifneq ($(SecureMod_SUPPORT),1)
$(ROM_LAYOUT_EX): $(FVDXE_SIG_IMAGE)
	@echo ----FvDxe: Build extended rom map '$(ROM_LAYOUT_EX)'
	$(FWBUILD)  $(FVDXE_SIG_IMAGE) $(FWBUILD_OPTION_CHAR)s $(FWBUILD_OPTION_CHAR)m $(ROM_LAYOUT_EX)

RomPatch: MAKE_FVDXE_SIG_MAP MAKE_FVDXE_SIG_IMAGE
else
RomPatch: MAKE_FVDXE_SIG_MAP MOD_FWCAPSULE_HDR_FFS MAKE_FVDXE_SIG_IMAGE
endif

#---------------------------------------------------------------------------
# Update Rom map descriptor table "$(ROM_LAYOUT_EX)" with un-signed region for FvDxeSig Hdr
#--------------------------------------------------------------------------
MAKE_FVDXE_SIG_MAP: $(FVDXE_SIG_IMAGE) $(ROM_LAYOUT_EX)
ifeq ($(BUILD_OS), $(BUILD_OS_LINUX))
	chmod +x $(CRYPTCON:-@=)
endif
	@echo ----FVDXE: Update extended rom map '$(ROM_LAYOUT_EX)' with un-signed region for FvDxeSig Hdr
	$(CRYPTCON) -m  $(FVDXE_SIG_CMDLINE) -f $(FVDXE_SIG_IMAGE) -o $(FVDXE_SIG_IMAGE)

MAKE_FVDXE_SIG_IMAGE: $(FVDXE_SIG_IMAGE)
ifeq ($(FVDXE_SIGNED_AT_BUILD),1)
	@echo ----FVDXE: Sign and embed the signature into $(FVDXE_SIG_IMAGE)
	$(CRYPTCON) $(FVDXE_SIG_CMDLINE) -f $(FVDXE_SIG_IMAGE) -o $(FVDXE_SIG_IMAGE)
endif
ifeq ($(DEBUG_MODE), 1)
	$(FWBUILD) $(FVDXE_SIG_IMAGE) $(FWBUILD_OPTION_CHAR)v
endif

#---------------------------------------------------------------------------
#   Build time warnings
#---------------------------------------------------------------------------
ifneq ($(wildcard $(FWpub)),$(FWpub))
$(warning *** BIOS will not boot unless the FV is signed and a valid key is placed inside IBB(FV_BB->FWKey ffs))
ifeq ($(FVDXE_SIGNED_AT_BUILD),1)
ifneq ($(wildcard $(FWpriv)),$(FWpriv))
$(error Provide valid sign keys or disable build time FV Signing via SDL:FVDXE_SIGNED_AT_BUILD)
endif
endif
endif

ifeq ($(FWCAPSULE_CERT_FORMAT),1)
ifeq ($(CONFIG_PEI_PKCS7),0)
$(warning *** AMI Signed Firmare Volume)
$(warning *** PKCS#7 certificate parser in PEI is disabled in AmiCryptoPkg via SDL:CONFIG_PEI_PKCS7)
$(error BIOS boot will abort with failure to verify PKCS#7 signed FV)
endif
endif

#---------------------------------------------------------------------------
#   Create a batch file with instructions to sign measured FVs inside Aptio binary
#---------------------------------------------------------------------------
FVDXE_SIG_MAKE_BAT := make_fv_trusted.bat

clean  : CLEAR_FVDXE_SIG_BAT_FILES
Prepare: MAKE_FVDXE_SIG_BAT_FILES

CLEAR_FVDXE_SIG_BAT_FILES:
ifneq ("$(wildcard  $(FVDXE_SIG_MAKE_BAT))", "")
	-$(RM) $(FVDXE_SIG_MAKE_BAT)
endif

MAKE_FVDXE_SIG_BAT_FILES: CLEAR_FVDXE_SIG_BAT_FILES
	@$(ECHO) "@echo ----Sign 'trusted' FVs and store the signature inside $(notdir $(FVDXE_SIG_IMAGE)) image...$(EOL)\
@echo ----REQUIRED:$(EOL)\
@echo ----Aptio images : '$(notdir $(FVDXE_SIG_IMAGE))' and '$(notdir $(ROM_LAYOUT_EX))'$(EOL)\
@echo ----Sign keys    : '$(notdir $(FWpriv))' and '$(notdir $(FWpub))'$(EOL)\
@echo ----Ami tools    : 'cryptocon' and 'fwbuild'$(EOL)\
@echo ----OPTIONAL steps:$(EOL)\
@echo ---- 1. Re-generate Rom map file if the BIOS binary was modified by Aptio tools$(EOL)\
@echo ---- $(FWBUILD) $(FVDXE_SIG_IMAGE) $(FWBUILD_OPTION_CHAR)s $(FWBUILD_OPTION_CHAR)m $(ROM_LAYOUT_EX)$(EOL)\
@echo ---- 2. Update '$(ROM_LAYOUT_EX)' with un-signed region for FvDxeSig Hdr (needed for Aptio FW Capsule)$(EOL)\
@echo ---- $(CRYPTCON) -m -r $(ROM_LAYOUT_EX) $(FVDXE_SIG_CMDLINE) -f $(FVDXE_SIG_IMAGE) -o $(FVDXE_SIG_IMAGE)$(EOL)\
$(EOL)\
@echo ---- Sign '$(notdir $(FVDXE_SIG_IMAGE))' using sign key from '$(FWpriv)' file$(EOL)\
$(CRYPTCON) $(FVDXE_SIG_CMDLINE) -f $(FVDXE_SIG_IMAGE) -o $(FVDXE_SIG_IMAGE)$(EOL)\
$(EOL)\
@echo ----===================================================$(EOL)\
@echo ----4 step FV signing using external(remote) sign process$(EOL)\
@echo ----===================================================$(EOL)\
@echo ----Note: When signing FVs within full SPI image, use cryptocon command '-$ offset' to locate the BIOS within SPI image$(EOL)\
@echo ----1. Insert signer verification key 'FWpub' into the BIOS image$(EOL)\
@echo ----   $(notdir $(CRYPTCON)) -c2 -y -n -k FWpub -f $(notdir $(FVDXE_SIG_IMAGE)) -o $(notdir $(FVDXE_SIG_IMAGE))$(EOL)\
@echo ----2. Serialize all FV regions to be signed$(EOL)\
@echo ----   $(notdir $(CRYPTCON)) -c2 -y -s -t -r $(notdir $(ROM_LAYOUT_EX)) -g 72A45C4B-F348-408f-B913-F7839F247CE2 -f $(notdir $(FVDXE_SIG_IMAGE)) -o serial.bin$(EOL)\
@echo ----3. Sign serial image and save signature in DER encoded PKCS7(or optionally PKCS1v1.5) signed certificate file (serial.bin.p7)$(EOL)\
@echo ----   using 3rd party sign utilities, e.g. Microsoft Signtool.exe or openSSL(MS or Linux).$(EOL)\
@echo ----   3.1 Using sign tools with access to local keys:$(EOL)\
@echo ----    signtool.exe sign /fd sha256 /p7 . /p7co 1.2.840.113549.1.7.1 /p7ce DetachedSignedData /p FW_PFX_Password /f FWpriv serial.bin$(EOL)\
@echo ----    openssl smime -sign -md sha256 -binary -noattr -in serial.bin -out serial.bin.p7 -outform DER -inkey FWpriv -signer FWpub -passin pass:FW_PFX_Password$(EOL)\
@echo ----   3.2 Using externally managed keys:$(EOL)\
@echo ----    a) A computer store is specified for the certification authority (CA) store. Certificate is identified by a Subject Name "My High Value Certificate"$(EOL)\
@echo ----    signtool sign /fd sha256 /p7 .\ /p7co 1.2.840.113549.1.7.1 /p7ce DetachedSignedData /sm /n "My High Value Certificate" serial.bin$(EOL)\
@echo ----    b) A private key is protected by a hardware cryptography module (HSM) with the HSM managed sign key is referenced in the certificate store by its SHA1 hash.$(EOL)\
@echo ----    signtool.exe sign /fd sha256 /p7 .\ /p7co 1.2.840.113549.1.7.1 /p7ce DetachedSignedData /a /sha1 "db314da0d0ef87d42b42f74b9c38a1f9173ef7a2" /sm serial.bin$(EOL)\
@echo ----4. Insert PKCS7 signed certificate into a target BIOS image$(EOL)\
@echo ----   $(notdir $(CRYPTCON)) -s -x serial.bin.p7 -t -y -c2 -r $(notdir $(ROM_LAYOUT_EX)) -g 72A45C4B-F348-408f-B913-F7839F247CE2 -f $(notdir $(FVDXE_SIG_IMAGE)) -o $(notdir $(FVDXE_SIG_IMAGE))$(EOL)\
@echo ----   Alternatively, if a FWCAPSULE_CERT_FORMAT is set to (0) RSA2048_SHA256 (vs. PKCS7), the signed digest field can be extracted from input PKCS7 file or$(EOL)\
@echo ----   if an input file is already formatted as PKCS1v1.5 padded signature, it will be copied to a FwCapsule Hdr as is:$(EOL)\
@echo ----   $(notdir $(CRYPTCON)) -y -c $(FWpub) -s -x serial.bin.p7 -f $(notdir $(FVDXE_SIG_IMAGE)) -o $(notdir $(FVDXE_SIG_IMAGE))$(EOL)"\
>> $(FVDXE_SIG_MAKE_BAT)
