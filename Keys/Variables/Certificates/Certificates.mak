#***********************************************************************
#*                                                                     *
#*   Copyright (c) 1985-2020, American Megatrends International LLC.   *
#*                                                                     *
#*      All rights reserved. Subject to AMI licensing agreement.       *
#*                                                                     *
#***********************************************************************
## @file
#
# Create Signed Secure Variables PK, KEK, etc.
# from the list of Certificates: $(CertList_PK), $(CertList_KEK) etc.
# or Signature lists $(SigList_DB) etc. 
#
##
ifeq ($(call __ge, $(BUILD_TOOLS_VERSION), 29),no)
$(error This version of SecureBoot.Certificates requires build tools 29 or newer (you are using tools $(BUILD_TOOLS_VERSION)))
endif
ifeq ($(SILENT), 1)
SILENT_MAKE := @
endif
#dbg
SILENT_MAKE :=

#---------------------------------------------------------------------------
# Current list of generated Secure Boot policy variables
#---------------------------------------------------------------------------
SEC_VAR_LIST := PK KEK db dbt dbx dbr
#---------------------------------------------------------------------------
# Default policy settings
#---------------------------------------------------------------------------
PK_Append := 0

#PK_signerKey : defined in SDL
KEK_signerKey := $(PK_signerKey)
#db_signerKey : defined in SDL
dbt_signerKey := $(db_signerKey)
dbx_signerKey := $(db_signerKey)
dbr_signerKey := $(db_signerKey)

#PK_signerCert: defined in SDL
KEK_signerCert := $(PK_signerCert)
#db_signerCert: defined in SDL 
dbt_signerCert := $(db_signerCert)
dbx_signerCert := $(db_signerCert)
dbr_signerCert := $(db_signerCert)

#PK_signerKey_Password: defined in SDL
KEK_signerKey_Password := $(PK_signerKey_Password)
#db_signerKey_Password: defined in SDL
dbt_signerKey_Password := $(db_signerKey_Password)
dbx_signerKey_Password := $(db_signerKey_Password)
dbr_signerKey_Password := $(db_signerKey_Password)

# to maintain backward comp with versions of Secure Boot before label 17
PKVarFile := $(PkVarFile)
KEKVarFile:= $(KekVarFile)
#---------------------------------------------------------------------------
# Begin templates
#---------------------------------------------------------------------------
# $(1)    : variable name
# $(2)    : append attribute
# $(3)    : accumulated serialized input file
# $(4)    : serialized file to be signed
# $(4).p7 : output, Pkcs#7 signature file with an extension .p7
# $(5)    : private key file (.PFX format or PEM for openSSL)
# $(6)    : specifies a passphrase to unlock encrypted .PFX or .PEM key data
# $(7)    : signer certificate file (x509) in PEM format
ifeq ($(BUILD_OS), $(BUILD_OS_LINUX))
#------Linux----------------
define serialize_template =
$(ECHO) Serialize: '1-$(1)' '2-$(2)' '3-$(3)' '4-$(4)' '5-$(5)' '6-$(6)' '7-$(7)'
if [ -f "$(BUILD_DIR)$(PATH_SLASH)serialize.h" ]; then $(RM) $(BUILD_DIR)$(PATH_SLASH)serialize.h; fi
$(ECHO) "#define VarName $(1)\
$(EOL)#define VarAppend $(2)"\
> $(BUILD_DIR)$(PATH_SLASH)serialize.h
gcc -c $(Certificates_DIR)$(PATH_SLASH)serialize.c -o $(BUILD_DIR)$(PATH_SLASH)serialize.o -D EFI_SPECIFICATION_VERSION=0x2000A -D PI_SPECIFICATION_VERSION=0x00010014 -IMdePkg/Include/AArch64 -IMdePkg/Include -IAmiCompatibilityPkg/Include/ -IAmiCryptoPkg/Include -I $(BUILD_DIR)$(PATH_SLASH)
objcopy -O binary $(BUILD_DIR)$(PATH_SLASH)serialize.o $(BUILD_DIR)$(PATH_SLASH)serialize.bin
$(CAT) $(BUILD_DIR)$(PATH_SLASH)serialize.bin $(3) > $(4)
$(AUTHVAR_SIGNTOOL_CMND)

endef
define authhdr_template =
if [ -f "$(1)" ]; then stat --printf="#define CertSize %s\n" "$(1)" > $(BUILD_DIR)$(PATH_SLASH)authhdr.h; else echo "#define CertSize 0" > $(BUILD_DIR)$(PATH_SLASH)authhdr.h; fi
$(CAT) "$(BUILD_DIR)$(PATH_SLASH)authhdr.h"
gcc -c $(Certificates_DIR)$(PATH_SLASH)authhdr.c -o $(BUILD_DIR)$(PATH_SLASH)authhdr.o -D EFI_SPECIFICATION_VERSION=0x2000A -D PI_SPECIFICATION_VERSION=0x00010014 -IMdePkg/Include/AArch64 -IMdePkg/Include -IAmiCompatibilityPkg/Include/ -IAmiCryptoPkg/Include -I $(BUILD_DIR)$(PATH_SLASH)
objcopy -O binary $(BUILD_DIR)$(PATH_SLASH)authhdr.o $(BUILD_DIR)$(PATH_SLASH)authhdr.tmp
if [ -f "$(1)" ]; then $(CAT) $(BUILD_DIR)$(PATH_SLASH)authhdr.tmp $(1) > $(BUILD_DIR)$(PATH_SLASH)authhdr.bin; else $(CAT) $(BUILD_DIR)$(PATH_SLASH)authhdr.tmp $(BUILD_DIR)$(PATH_SLASH)authhdr.bin; fi

endef
define siglist_template =
stat --printf="#define CertSize %s\n" "$(1)" > $(BUILD_DIR)$(PATH_SLASH)siglist.h
$(CAT) "$(BUILD_DIR)$(PATH_SLASH)siglist.h"
gcc -c $(Certificates_DIR)$(PATH_SLASH)siglist.c -o $(BUILD_DIR)$(PATH_SLASH)siglist.o -D EFI_SPECIFICATION_VERSION=0x2000A -D PI_SPECIFICATION_VERSION=0x00010014 -I$(dir $(1)) -IMdePkg/Include/AArch64 -IMdePkg/Include -IAmiCompatibilityPkg/Include/ -IAmiCryptoPkg/Include -I $(BUILD_DIR)$(PATH_SLASH)
objcopy -O binary $(BUILD_DIR)$(PATH_SLASH)siglist.o $(BUILD_DIR)$(PATH_SLASH)siglist.bin
if [ -f "$(BUILD_DIR)$(PATH_SLASH)sig.dat" ]; then  $(CAT) $(BUILD_DIR)$(PATH_SLASH)sig.dat > $(BUILD_DIR)$(PATH_SLASH)sig.tmp; fi
if [ -f "$(BUILD_DIR)$(PATH_SLASH)sig.tmp" ]; then  $(CAT) $(BUILD_DIR)$(PATH_SLASH)siglist.bin $(1) $(BUILD_DIR)$(PATH_SLASH)sig.tmp > $(BUILD_DIR)$(PATH_SLASH)sig.dat; else $(CAT) $(BUILD_DIR)$(PATH_SLASH)siglist.bin $(1) > $(BUILD_DIR)$(PATH_SLASH)sig.dat; fi

endef
#--------Windows-----------
else
#--------------------------
define serialize_template =
$(ECHO) Serialize: '1-$(1)' '2-$(2)' '3-$(3)' '4-$(4)' '5-$(5)' '6-$(6)' '7-$(7)'
if exist $(BUILD_DIR)$(PATH_SLASH)serialize.h $(RM) $(BUILD_DIR)$(PATH_SLASH)serialize.h
$(ECHO) "#define VarName $(1)\
$(EOL)#define VarAppend $(2)"\
> $(BUILD_DIR)$(PATH_SLASH)serialize.h
$(CCX86DIR)$(PATH_SLASH)cl /Fo$(BUILD_DIR)$(PATH_SLASH) /nologo /c /Gy /W4 /WX /D EFI_SPECIFICATION_VERSION=0x2000A /D PI_SPECIFICATION_VERSION=0x00010014 /IMdePkg\Include\x64 /IMdePkg\Include /IAmiCompatibilityPkg\Include\ /IAmiCryptoPkg\Include /I Build /TC Keys\Variables\Certificates\serialize.c
$(CCX86DIR)$(PATH_SLASH)link /OUT:$(BUILD_DIR)$(PATH_SLASH)serialize.dll /DLL /SUBSYSTEM:NATIVE /NODEFAULTLIB /NOENTRY $(BUILD_DIR)$(PATH_SLASH)serialize.obj
genfw -b -o $(BUILD_DIR)$(PATH_SLASH)serialize.bin $(BUILD_DIR)$(PATH_SLASH)serialize.dll
$(CP) /b $(BUILD_DIR)$(PATH_SLASH)serialize.bin + $(3) $(4)
$(AUTHVAR_SIGNTOOL_CMND)

endef
define authhdr_template =
for %%i in ($(1)) do echo #define CertSize %%~zi > $(BUILD_DIR)$(PATH_SLASH)authhdr.h
type $(BUILD_DIR)$(PATH_SLASH)authhdr.h
$(CCX86DIR)$(PATH_SLASH)cl /Fo$(BUILD_DIR)$(PATH_SLASH) /nologo /c /D EFI_SPECIFICATION_VERSION=0x2000A /D PI_SPECIFICATION_VERSION=0x00010014 /IMdePkg\Include\x64 /IMdePkg\Include /IAmiCompatibilityPkg\Include\ /IAmiCryptoPkg\Include /I Build /TC Keys\Variables\Certificates\authhdr.c
$(CCX86DIR)$(PATH_SLASH)link /OUT:$(BUILD_DIR)$(PATH_SLASH)authhdr.dll /nologo /NODEFAULTLIB /NOENTRY /DLL /ALIGN:16 /DRIVER /FIXED $(BUILD_DIR)$(PATH_SLASH)authhdr.obj
genfw -b -o $(BUILD_DIR)$(PATH_SLASH)authhdr.bin $(BUILD_DIR)$(PATH_SLASH)authhdr.dll
$(CP) /b $(BUILD_DIR)$(PATH_SLASH)authhdr.bin + $(1) $(BUILD_DIR)$(PATH_SLASH)authhdr.bin

endef
define siglist_template =
@if exist $(BUILD_DIR)$(PATH_SLASH)siglist.h $(RM) $(BUILD_DIR)$(PATH_SLASH)siglist.h
for %%i in ($(1)) do echo #define CertSize %%~zi > $(BUILD_DIR)$(PATH_SLASH)siglist.h
type $(BUILD_DIR)$(PATH_SLASH)siglist.h
$(CCX86DIR)$(PATH_SLASH)cl /Fo$(BUILD_DIR)$(PATH_SLASH) /nologo /c /D EFI_SPECIFICATION_VERSION=0x2000A /D PI_SPECIFICATION_VERSION=0x00010014 /I$(dir $(1)) /IMdePkg\Include\x64 /IMdePkg\Include /IAmiCompatibilityPkg\Include\ /IAmiCryptoPkg\Include /I Build  /TC Keys\Variables\Certificates\siglist.c
$(CCX86DIR)$(PATH_SLASH)link /OUT:$(BUILD_DIR)$(PATH_SLASH)siglist.dll /nologo /NODEFAULTLIB /NOENTRY /DLL /ALIGN:16 /DRIVER /FIXED $(BUILD_DIR)$(PATH_SLASH)siglist.obj
genfw -b -o $(BUILD_DIR)$(PATH_SLASH)siglist.bin $(BUILD_DIR)$(PATH_SLASH)siglist.dll
if exist $(BUILD_DIR)$(PATH_SLASH)sig.dat $(CP) $(BUILD_DIR)$(PATH_SLASH)sig.dat $(BUILD_DIR)$(PATH_SLASH)sig.tmp
$(CP) /b $(BUILD_DIR)$(PATH_SLASH)siglist.bin + $(1) + $(BUILD_DIR)$(PATH_SLASH)sig.tmp $(BUILD_DIR)$(PATH_SLASH)sig.dat

endef
#-------------------
endif

#---------------------------------------------------------------------------
# Main Build rule templates
#---------------------------------------------------------------------------
ifneq ($(Make_SignedVariables), 1)
$(BUILD_DIR)$(PATH_SLASH)authhdr.bin: $(Certificates_DIR)$(PATH_SLASH)authhdr.c $(BUILD_DIR)$(PATH_SLASH)TimeStamp.h
	$(SILENT_MAKE)$(call authhdr_template, $(BUILD_DIR)$(PATH_SLASH)null)
endif

define SecVar_template =
ifneq ($(strip $$(CertList_$(1))),$(strip $$(SigList_$(1))))
Prepare: $$($(1)VarName)
$$($(1)VarName): $($(1)VarName).sig
ifeq ($(Make_SignedVariables), 1)
	$(SILENT_MAKE)$$(call serialize_template,$(1),$$($(1)_Append),$($(1)VarName).sig,$($(1)VarName).ser,$$($(1)_signerKey),$$($(1)_signerKey_Password), $$($(1)_signerCert) )
	$(SILENT_MAKE)$$(call authhdr_template,$($(1)VarName).ser.p7)
	$(SILENT_MAKE)$(CAT) $(BUILD_DIR)$(PATH_SLASH)authhdr.bin $$^ > $$@
else
$$($(1)VarName): $(BUILD_DIR)$(PATH_SLASH)authhdr.bin
	$(SILENT_MAKE)$(CAT) $$^ > $$@
endif

$$($(1)VarName).sig: $$(CertList_$(1))
ifeq ($(BUILD_OS), $(BUILD_OS_LINUX))
	@if [ -f "$(BUILD_DIR)$(PATH_SLASH)sig.dat" ]; then $(RM) $(BUILD_DIR)$(PATH_SLASH)sig.dat; fi
	@if [ -f "$(BUILD_DIR)$(PATH_SLASH)sig.tmp" ]; then $(RM) $(BUILD_DIR)$(PATH_SLASH)sig.tmp; fi
	$(SILENT_MAKE)$$(foreach cert,$$^, $$(call siglist_template,$$(cert)) )
	$(SILENT_MAKE)if [ -f $(BUILD_DIR)$(PATH_SLASH)sig.dat $(SILENT_MAKE)$(CAT) $(BUILD_DIR)$(PATH_SLASH)sig.dat > $$@
else
	@if exist $(BUILD_DIR)$(PATH_SLASH)sig.dat $(RM) $(BUILD_DIR)$(PATH_SLASH)sig.dat
	@if exist $(BUILD_DIR)$(PATH_SLASH)sig.tmp $(RM) $(BUILD_DIR)$(PATH_SLASH)sig.tmp
	$(SILENT_MAKE)$$(foreach cert,$$^, $$(call siglist_template,$$(cert)) )
	$(SILENT_MAKE)if exist $(BUILD_DIR)$(PATH_SLASH)sig.dat $(SILENT_MAKE)$(CP) $(BUILD_DIR)$(PATH_SLASH)sig.dat $$@
endif
ifneq ($$(SigList_$(1)),)
	$(SILENT_MAKE)$(CAT) $$(SigList_$(1)) >> $$@
endif	
else
$$(info '$(1)' = $$($(1)VarFile))
$$($(1)VarName): $$($(1)VarFile)
	$(SILENT_MAKE)$(CAT) $$^ > $$@
endif

endef

$(foreach var,$(SEC_VAR_LIST),$(eval $(call SecVar_template,$(var))) )
