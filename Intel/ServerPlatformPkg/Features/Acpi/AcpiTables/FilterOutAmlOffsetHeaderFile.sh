#!/bin/bash

#***********************************************************************
#*                                                                     *
#*   Copyright (c) 1985-2019, American Megatrends International LLC.   *
#*                                                                     *
#*      All rights reserved. Subject to AMI licensing agreement.       *
#*                                                                     *
#***********************************************************************

## @file FilterOutAmlOffsetHeader.sh
#   Shell Script file to filter AML offset headers and create 
#                        DsdtAsl.offset.h
##

function delIntermediateFiles() {
  #Delete intermediate files
  rm -fr ${BUILD_DIR}/$2.offset.h.tmp
  rm -fr ${BUILD_DIR}/$2.asl
  rm -fr ${BUILD_DIR}/$2.aml
  rm -fr ${BUILD_DIR}/$2.iii
  rm -fr ${BUILD_DIR}/$2.iiii

  exit 0
}

function genDsdtOffset() {
  echo Filtering DSDT $2.offset.h file
  #Create tmp file from $2.offset.h
  mv -f ${WORKSPACE}/${BUILD_DIR}/$2.offset.h ${WORKSPACE}/${BUILD_DIR}/$2.offset.h.tmp

  #Filter out everything except tool info, typedef, and start of structure definition.
cat ${WORKSPACE}/${BUILD_DIR}/$2.offset.h.tmp | grep -v -e "{\"" -e "{NULL," -e "};" > ${WORKSPACE}/${BUILD_DIR}/$2.offset.h

# Filter out everything except the AML operators that need fixup; this should match AcpiPlatformHooks.c BiosFixupAmlOffsetTable[] entries.
# When search string has " in it, must use ^" to indicate string quote for command processor, and \" as quote character in string.
# When search string has spaces in it, must use a literal search separate from the regex expressions.
# Filter out everything except structure terminator and end of structure definition.
# Copying Patch details to AMI File
cat ${WORKSPACE}/${BUILD_DIR}/$2.offset.h.tmp | grep -e "{NULL," -e "};" >> ${WORKSPACE}/${BUILD_DIR}/$2.offset.h
# Create new offset table for AMI to handle build time change in OEM table Id.
echo AML_OFFSET_TABLE_ENTRY DSDTPatchOffsetTable[] = { >> ${WORKSPACE}/${BUILD_DIR}/$2.offset.h
cat ${WORKSPACE}/${BUILD_DIR}/$2.offset.h.tmp | grep -G -e "\"PSYS\"" -e "\.MCTL\"" -e "\.FIX[0-9,A-Z]" -e "BBI[0]" -e "BBU[0]" -e "CRCM" -e "BAR0" -e "\.CCT[0-9,A-Z]"  -e "FXCD" -e "FXST" -e "FXIN" -e "FXOU" -e "FXBS"  -e "CENA" -e "DRVT" -e "CFIS">> ${WORKSPACE}/${BUILD_DIR}/$2.offset.h

  # Copying Patch details to AMI File
cat ${WORKSPACE}/${BUILD_DIR}/$2.offset.h.tmp | grep -e "{NULL," -e "};" >> ${WORKSPACE}/${BUILD_DIR}/$2.offset.h

  delIntermediateFiles $1 $2
  exit 0
}

function genSsdtOffset() {
  echo Filtering SSDT "$2".offset.h file
  #Create tmp file from "$2".offset.h


  # Move CpuPm.offset.h to AcpiPlatformHooks.c location so it can be included in C source, and use temp filename so it can be filtered.
  mv -f ${WORKSPACE}/${BUILD_DIR}/$2.offset.h ${WORKSPACE}/${BUILD_DIR}/$2.offset.h.tmp

  # Filter out everything except tool info, typedef, and start of structure definition.
  cat ${WORKSPACE}/${BUILD_DIR}/$2.offset.h.tmp | grep -v -e "{\"" -e "{NULL," -e "};" > ${WORKSPACE}/${BUILD_DIR}/$2.offset.h
# Copying Patch details to AMI File
# Filter out everything except structure terminator and end of structure definition.
cat ${WORKSPACE}/${BUILD_DIR}/$2.offset.h.tmp | grep -e "{NULL, };" >> ${WORKSPACE}/${BUILD_DIR}/$2.offset.h
#Create new offset table for AMI to handle build time change in OEM table Id.
  echo AML_OFFSET_TABLE_ENTRY SSDTPatchOffsetTable[] = { >> ${WORKSPACE}/${BUILD_DIR}/$2.offset.h
# Copying Patch details to AMI File
  cat ${WORKSPACE}/${BUILD_DIR}/$2.offset.h.tmp | grep -G -e "DOMN" -e "NCPU" -e "PSDC" -e "PSDD" -e "PSDE" -e "NPSS" -e "SPSS" >> ${WORKSPACE}/${BUILD_DIR}/$2.offset.h

  cat ${WORKSPACE}/${BUILD_DIR}/$2.offset.h.tmp | grep -e "{NULL," -e "};" >> ${WORKSPACE}/${BUILD_DIR}/$2.offset.h
  delIntermediateFiles $1 $2

  exit 0
}

if [ "$1" == "DSDT" ]; then
  genDsdtOffset $1 $2
else
  genSsdtOffset $1 $2
  exit 0
fi
