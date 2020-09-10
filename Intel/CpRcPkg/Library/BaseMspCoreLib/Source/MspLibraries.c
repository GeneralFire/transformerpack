/** @file
  MspLibraries.c
  Modular Msp  libraries used throughout the Msp, these files
  are independent of the project.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2005 - 2018 Intel Corporation. <BR>

  The source code contained or described herein and all documents related to the
  source code ("Material") are owned by Intel Corporation or its suppliers or
  licensors. Title to the Material remains with Intel Corporation or its suppliers
  and licensors. The Material may contain trade secrets and proprietary    and
  confidential information of Intel Corporation and its suppliers and licensors,
  and is protected by worldwide copyright and trade secret laws and treaty
  provisions. No part of the Material may be used, copied, reproduced, modified,
  published, uploaded, posted, transmitted, distributed, or disclosed in any way
  without Intel's prior express written permission.

  No license under any patent, copyright, trade secret or other intellectual
  property right is granted to or conferred upon you by disclosure or delivery
  of the Materials, either expressly, by implication, inducement, estoppel or
  otherwise. Any license under such intellectual property rights must be
  express and approved by Intel in writing.

  Unless otherwise agreed by Intel in writing, you may not remove or alter
  this notice or any other notice embedded in Materials by Intel or
  Intel's suppliers or licensors in any way.
**/

#include "MspLibraries.h"
#include <Library/MspCoreLib.h>
#include <Library/MemoryCoreLib.h>
#include <Library/SysHostPointerLib.h>
#include <Library/SystemInfoLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/CteNetLib.h>
#include <Library/RcDebugLib.h>
#include <Library/ProcMemInitChipLib.h>

#include "RegAccess.h"
#include <Library/IoAccessLib.h>
#include <Library/MemoryServicesLib.h>
#include <Library/CheckpointLib.h>
#include <Library/PerformanceTrackerLib.h>
#include <Memory/MemoryCheckpointCodes.h>

/**

  Returns the MSP   Host structure from an MMX register.

  @param[in,out]    Destination Pointer
  @param[in]        Init Value
  @param[in]        Number of bytes to write

  @return           Destination Pointer

**/
VOID *
MspSet (
  VOID      *dst,
  UINT8     value,
  UINT32    cnt
  )
{
  UINT8 *dst8 = (UINT8 *) dst;

  SetMem (dst8, cnt, value);
  return dst;
}
/**
  This function detects the platform under test.

  @param[in]       *str1      Pointer to String 1
  @param[in]       *str2      Pointer to String 2
  @param[in]       cnt        Length for both strings
  @retval          TRUE       If both strings are the same
**/

BOOLEAN
MspStringCompare (
  IN    VOID    *str1,
  IN    VOID    *str2,
  IN    UINT32  cnt
  )
{

  UINT32      Index;
  UINT8       *StringA;
  UINT8       *StringB;

  StringA = (UINT8 *) str1;
  StringB = (UINT8 *) str2;

  for (Index = 0; Index < cnt; Index++) {
    if (StringA[Index] != StringB[Index]) {
      return FALSE;
    }
  }
  return TRUE;
}

UINT32
StringLength (
  UINT8 *String
  )
{
  UINT32 i;
  i = 0;
  while (String[i++] != 0);
  return i;
}

MSP_STATUS
MspDelay (
  IN        UINT8   Type,
  IN        UINT32  Delay
  )
{

  switch (Type) {
  case MILLI_DEL:
    FixedDelayMicroSecond (Delay * 1000);
    break;
  case MICRO_DEL:
    FixedDelayMicroSecond (Delay);
    break;
  case NANO_DEL:
    //
    // Minimum delay is 1us
    //
    if (Delay < 1000) {
      Delay = 1;
    }
    FixedDelayMicroSecond (Delay);
    break;
  default:
    break;
  }
  return MSP_SUCCESS;
}

UINT32
GetRegisterNumber (
  MSP_DATA *MspData,
  UINT8  Box,
  UINT32 Offset
  )
{
  UINT32      RegisterNumber;

  //
  // Grab the first register string for this boxtype.
  //
  RegisterNumber = INSTANCE_PORT_MAP(Box).StringIndex;

  if (RegisterNumber == 0xFFFFFFFF) {
    return RegisterNumber;
  }
  //
  // Loop through all registers for this box....
  //
  while (MspData->Registers[RegisterNumber].RegInfo != NULL) {
    if (MspData->Registers[RegisterNumber].RegInfo->Offset == Offset) {
      break;
    }
    RegisterNumber++;
  }
  return RegisterNumber;
}
/**
  Calculates the UINT32 value from the Byte enable input

  @param[in]        Be                  Byte Enable
  @param[in]        Value               Input Value
  @param[in,out]    ValueLength         Size of the Input Value

  @retval           FinalValue
**/
UINT32
ByteEnableToUintX (
  IN       UINT8    Be,
  IN       UINT32   Value,
  IN       UINT8    *ValueLength
  )
{
  UINT8  CurrentByte;
  UINT32 FinalValue;
  UINT8  TestByte;
  UINT8  MaxBit;

  MaxBit = 32;

  *ValueLength = 0;
  CurrentByte = 0;
  FinalValue = 0;
  for (TestByte = 0; TestByte < MaxBit; TestByte += 8) {
    if ((Be & 1) == 1) {
      (*ValueLength)++;
      FinalValue += ((Value & 0xff) << TestByte);
      Value >>= 8;
    }
    Be >>= 1;
  }
  //
  // In case of no byte enables (i.e. wake command or something), set to 1 byte of data.
  //
  if (*ValueLength == 0) {
    *ValueLength = 1;
  }
  return FinalValue;
}

/**
  Reconciles the PFCT restriction an returns it back

  @param[in, out]   MspData             Host structure for all data related to MSP.
  @param[in]        Socket              Socket under test
  @param[in]        Channel             Current Channel being examined.
  @param[in, out]   PFCTSelect          Return the mask created for the PFCT

  @retval           MSP_STATUS
**/
MSP_STATUS
CreatePFCTSel (
  IN  OUT   MSP_DATA         *MspData,
  IN        UINT8            Socket,
  IN        UINT8            Channel,
  IN        PFCT_VARIATIONS *PFCTSelect
  )
{
  UINT16  CurrentPlatform;
  UINT16  CurrentFrequency;
  UINT16  CurrentConfiguration;
  UINT16  CurrentType;
  UINT8   NumType;
  UINT8   NumConf;
  UINT8   NumFreq;
  UINT8   NumPlat;

  CurrentPlatform       = MspData->CurrentPlatform[Channel];
  CurrentFrequency      = MspData->CurrentFrequency[Channel];
  CurrentConfiguration  = MspData->CurrentConfiguration[Channel];
  CurrentType           = MspData->CurrentTechnology[Channel];

  NumType   = MspData->NumType;
  NumConf   = MspData->NumConf;
  NumFreq   = MspData->NumFreq;
  NumPlat   = MspData->NumPlat;

  PFCTSelect[Pfct].Pfct     = 0;
  PFCTSelect[PfctT].Pfct    = (1  << CurrentType);
  PFCTSelect[PfctC].Pfct    = (1  << CurrentConfiguration);
  PFCTSelect[PfctCT].Pfct   = ((1 << CurrentConfiguration) << (NumType)) | (1 << CurrentType);
  PFCTSelect[PfctF].Pfct    = (1  << CurrentFrequency);
  PFCTSelect[PfctFT].Pfct   = ((1 << CurrentFrequency) << (NumType)) | (1 << CurrentType);
  PFCTSelect[PfctFC].Pfct   = ((1 << CurrentFrequency) << (NumConf)) | (1 << CurrentConfiguration);
  PFCTSelect[PfctFCT].Pfct  = ((1 << CurrentFrequency) << (NumConf + NumType)) | ((1 << CurrentConfiguration) << (NumType)) | ((1 << CurrentType));
  PFCTSelect[PfctP].Pfct    = (1  << CurrentPlatform);
  PFCTSelect[PfctPT].Pfct   = ((1 << CurrentPlatform) << (NumType) ) | (1 << CurrentType);
  PFCTSelect[PfctPC].Pfct   = ((1 << CurrentPlatform) << (NumConf) ) | (1 << CurrentConfiguration);
  PFCTSelect[PfctPCT].Pfct  = ((1 << CurrentPlatform) << (NumConf + NumType) ) | ( (1 << CurrentConfiguration) << (NumType) ) | (1 << CurrentType);
  PFCTSelect[PfctPF].Pfct   = ((1 << CurrentPlatform) << (NumFreq) ) | (1 << CurrentFrequency);
  PFCTSelect[PfctPFT].Pfct  = ((1 << CurrentPlatform) << (NumFreq + NumType) ) | ( (1 << CurrentFrequency) << (NumType) ) | (1 << CurrentType);
  PFCTSelect[PfctPFC].Pfct  = ((1 << CurrentPlatform) << (NumFreq + NumConf) ) | ( (1 << CurrentFrequency) << (NumConf) ) | (1 << CurrentConfiguration);
  PFCTSelect[PfctPFCT].Pfct = ((1 << CurrentPlatform) << (NumFreq + NumConf + NumType) ) | ((1 << CurrentFrequency) << (NumConf + NumType)) | ((1 << CurrentConfiguration) << (NumType)) | (1 << CurrentType);

  PFCTSelect[Pfct].LengthVariation     = 0;
  PFCTSelect[PfctT].LengthVariation    = (NumType - 1) / 8 + 1;
  PFCTSelect[PfctC].LengthVariation    = (NumConf - 1) / 8 + 1;
  PFCTSelect[PfctCT].LengthVariation   = (NumConf + NumType - 1) / 8 + 1;
  PFCTSelect[PfctF].LengthVariation    = (NumFreq - 1) / 8 + 1;
  PFCTSelect[PfctFT].LengthVariation   = (NumFreq + NumType - 1) / 8 + 1;
  PFCTSelect[PfctFC].LengthVariation   = (NumFreq + NumConf - 1) / 8 + 1;
  PFCTSelect[PfctFCT].LengthVariation  = (NumFreq + NumConf + NumType - 1) / 8 + 1;
  PFCTSelect[PfctP].LengthVariation    = (NumPlat - 1) / 8 + 1;
  PFCTSelect[PfctPT].LengthVariation   = (NumPlat + NumType - 1) / 8 + 1;
  PFCTSelect[PfctPC].LengthVariation   = (NumPlat + NumConf - 1) / 8 + 1;
  PFCTSelect[PfctPCT].LengthVariation  = (NumPlat + NumConf + NumType - 1) / 8 + 1;
  PFCTSelect[PfctPF].LengthVariation   = (NumPlat + NumFreq - 1) / 8 + 1;
  PFCTSelect[PfctPFT].LengthVariation  = (NumPlat + NumFreq + NumType - 1) / 8 + 1;
  PFCTSelect[PfctPFC].LengthVariation  = (NumPlat + NumFreq + NumConf - 1) / 8 + 1;
  PFCTSelect[PfctPFCT].LengthVariation = (NumPlat + NumFreq + NumConf + NumType - 1) / 8 + 1;

  return MSP_SUCCESS;
}

/**
  Decodes and executes the Capsule initialization.
  The routine starts at a beginning index in the CapsuleInit
  compressed data structure and continues until a ASSIGN_DONE
  flag is detected.

  @param[in, out]   MspData             Host structure for all data related to MSP.
  @param[in]        Socket              Socket under test
  @param[in]        CapsuleStartIndex   Starting point within theCapsuleData structure where the Init should begin.
  @param[in]        StringIndex         When provided, this is the index into the string table to show the register names.
  @param[in]        Channel             Current Channel being examined.
  @retval           MSP_SUCCESS
**/
MSP_STATUS
MspExecuteTask (
  IN      MSP_DATA          *MspData,
  IN      UINT16            CapsuleStartIndex,
  IN      UINT16            StringIndex,
  IN      UINT8             Channel
  )
{
  UINT32   PfctValue;                                 // Based on the current PFCT, holds the Value for the current assignment.
  UINT8    PfctStatus;                                // Flag to determine if any of the PFCT conditions were met on the current assign.
  UINT8    Action;                                    // Holds the Action of the current assign.
  UINT8    Counter;                                   // Counter for displaying results on the output when enabled.
  UINT8    *CurrentEncLoc;                            // Holds the location of the current assign. being decoded.
  UINT32   Delay;                                     // Holds the Delay for the assignment.  0 if not needed.
  UINT16   ConditionalExecution;                      // Flag specifying if the assignment should take place.
  UINT8    ChannelToWrite;                            // Specific channel the assignment is directed to, if any.
  UINT8    InstanceToWrite;                           // Specific instance the assignment is directed to, if any.
  UINT16   CurrentPtr;                                // Pointer to the encoded data stream.
  UINT16   RegisterOffset;                            // Register offset provided in the assignment.
  UINT8    BoxType;                                   // Boxtype provided in the assignment.
  UINT8    ByteEnable;                                // Byte enable provided in the assignment.
  UINT32   Mask;                                      // Mask that may be provided in the assignment.
  UINT8    MaskValueLength;                           // Length of the MASK in bytes, when the mask is provided.
  UINT8    Pfct;                                      // PFCT flag provided in the assignment.
  UINT8    PfctDynamic;                               // Dynamic field in the dPFCT provided in the assignment.
  UINT8    NumberConditionalValues;                   // Number of PFCT Condition-Values provided in the assignment.
  UINT8    NumberConditionalBytes;                    // Number of bytes within the condition portion of the PFCT assignment.
  UINT8    ConditionalFlag;                           // When decoding PFCT values, when the condition is met, this value is 1.
  UINT8    TestVal;                                   // Used to determine if the condition is true in the PFCT assignment.
  UINT8    NumberInstances;                           // Number of instances for a specific BoxType.
  UINT8    Instance;                                  // Instance being specified when looping on total Instances.
  UINT8    SkipIosf;                                  // Flag specifying if the IOSF assignment should be skipped.  (Broadcasts)
  UINT32   readValue;                                 // When doing a poll, this is the value read back.
  UINT32   Value;                                     // 32-bit value that gets written to for the assignment.
  UINT8    sLoop;                                     // When DETAILED_PHYINIT is enabled, this is used to print the strings.
  UINT8    StartingBit;                               // Final starting position that the assignment is accessing.
  UINT8    EndingBit;                                 // Final ending position that the assignment is accessing.
  UINT8    PfctValueSize;                             // The size in bytes of the value portion of the PFCT CVs.
  UINT32   BaseOffset;                                // Base offset for the instance of the boxtype.
  UINT8    Port;                                      // Port ID for the instance of the boxtype.
  UINT8    Bus;                                       // When doing PCI Access, PCI Bus.
  UINT8    Device;                                    // When doing PCI Access, PCI Device.
  UINT8    Func;                                      // When doing PCI Access, PCI Function.
  UINT8    i;
  UINT32   NewValue;                                  // Used for temporary storage in DynamicAssignmentGenerated call.
  UINT16   DynamicIndex;                              // Used in DynamicAssignmentGenerated (the switch statement index).
  UINT16   InitialStringIndex;
  UINT8    Strobe;                                    // Used to loop on strobes.
  UINT8    Rank;                                      // Used to loop on ranks.
  UINT8    ThisChannel;                               // Used for strobe/rank looping.
  UINT8    ThisInstance;                              // Used for strobe/rank looping.
  UINT8    MaxStrobe;                                 // Used to loop on strobes.
  UINT8    MaxRank;                                   // Used to loop on ranks.
  UINT8    ThisBoxType;                               // Used for strobe/rank looping.
  UINT8    MaxBit;                                    // Are we using 32 bit or 64 bit registers?
  UINT8    NumStrings;                                // Number of strings we need to output to the log.
  UINT8    Zero;                                      // Temporary location for resetting structures to 0.
  UINT8    k;                                         // Temporary counter.
  UINT32   MsgLevel;                                  // Temporary storage for backing up the message level.
  UINT8    MaxBoxName;                                // Maximum length of box string to print.
  UINT8    MaxRegisterName;                           // Maximum length of register string to print.
  UINT32   RegisterNumber;                            // Index to register number which will be printed.
  BOOLEAN  ChannelBasedOffsetDetected;
  UINT16   CapsuleStrIndex;
  PFCT_VARIATIONS                 PFCTVariations[MaxPfct];  // Holds all permutations of PFCTs
  REGISTER_LOCATION_ATTRIBUTES    *RegisterAttributes;      // Bit fields for the flags portion of the MEMORYINIT.
  REGISTER_LOCATION_ATTRIBUTES2   *RegisterAttributes2;     // Bit fields for the 2nd byte of the flags portion of the MEMORYINIT.
  REGISTER_ACCESS                 Register;                 // Field that specifies the location of the register including offset and bits.
  UINT32   BarOffset;

  if (MspData->Use64bVariables) {
    MaxBit = 64;
  } else {
    MaxBit = 32;
  }

  InitialStringIndex  = StringIndex;
  ThisInstance        = 0;
  ThisChannel         = 0;
  MsgLevel            = GetDebugLevel ();

  //
  // Initializing the Variables.
  //
  Value                          = 1;
  ChannelToWrite                 = 0;
  PfctValue                      = 0;
  StartingBit                    = 0;
  EndingBit                      = 0;
  sLoop                          = 0;
  Counter                        = 0;
  RegisterOffset                 = 0;
  PfctValueSize                  = 0;
  Delay                          = 0;
  CurrentPtr                     = 0;
  ConditionalExecution           = 1;
  Bus                            = 0;
  Device                         = 0;
  Func                           = 0;
  BaseOffset                     = 0;
  Port                           = 0;
  ThisChannel                    = 0;
  ThisInstance                   = 0;
  StringIndex                    = InitialStringIndex;

  if (StringIndex == NO_PRINT) {
    SetDebugLevel (MsgLevel & ~SDBG_TRACE);
  }

  if (MspData->CapsuleStringsEn) {
    if (StringIndex < NO_PRINT) {
      RcDebugPrint (SDBG_TRACE, " %a for CH%d\n", MspData->CapsuleStrings[StringIndex], Channel);
    }
    RcDebugPrint (SDBG_TRACE, "    Box");
    if (MspData->MaxBoxNameLength < 3) {
      MaxBoxName = 0;
    } else {
      MaxBoxName = MspData->MaxBoxNameLength - 3;
    }
    for (i = 0; i < MaxBoxName; i++) {
      RcDebugPrint (SDBG_TRACE, " ");
    }
  }
  if (MspData->DumpRegisterNamesEn) {
    RcDebugPrint (SDBG_TRACE, "Register Symbol");
    if (MspData->MaxRegisterNameLength < 13) {
      MaxRegisterName = 0;
    } else {
      MaxRegisterName = MspData->MaxRegisterNameLength - 13;
    }
    for (i = 0; i < MaxRegisterName; i++) {
      RcDebugPrint (SDBG_TRACE, " ");
    }
  }
  RcDebugPrint (SDBG_TRACE, "   Port Offset     Mask               Action    Delay      Value\n");
  //
  // Given the current P, F, C, T provide all possible PFCT variations, this makes the comparison quicker
  // since this does not have to be derived for every assignment.  There are a total of 16 different PFCT
  // permutations.
  //
  CreatePFCTSel (MspData, 0, Channel, PFCTVariations);
  //
  // Begin by assigning the Current encoded location to the start of the assignments.
  //
  CurrentEncLoc = (UINT8 *) &MspData->InitData[CapsuleStartIndex];
  //
  // The last assignment of the assignment list has the MACRO "ASSIGNDONE" which is the signature to stop decoding.
  //
  while (1) {
    ChannelBasedOffsetDetected  = TRUE;
    InstanceToWrite =  0;
    ChannelToWrite  =  Channel;
    ConditionalExecution = 1;
    Zero = 0;
    //
    // Reset to 0.
    //
    RegisterAttributes2 = (REGISTER_LOCATION_ATTRIBUTES2 *) &Zero;
    //
    // The register attributes for the FLAGS starts at the beginning location, and the default length of this field
    // is one byte.
    //
    RegisterAttributes = (REGISTER_LOCATION_ATTRIBUTES *) &CurrentEncLoc[0];
    CurrentPtr = 1;
    if (RegisterAttributes->Done == 1) {
      break;
    }
    if (RegisterAttributes->NeedSecondByte == 1) {
      RegisterAttributes2 = (REGISTER_LOCATION_ATTRIBUTES2 *) &CurrentEncLoc[CurrentPtr];
      CurrentPtr++;
    }
    //
    // Check if the assignment should be executed based on the execution flag.
    //
    if (RegisterAttributes->CondExec == 1) {
      ConditionalExecution = (UINT16) DYNAMIC_VARS(Channel,*(UINT16 *) &CurrentEncLoc[CurrentPtr]);
      CurrentPtr += 2;
    }
    //
    // If the ChannelDependent or InstanceDependent flags are either set, then the flags will be one byte larger.
    // Also when not set, Channel/Instance to write are set to 0 but will not be used in the assignment.
    //
    if (RegisterAttributes2->ChannelDependent == 1) {
      ChannelToWrite  = CurrentEncLoc[CurrentPtr++];
    }
    if (RegisterAttributes2->InstanceDependent == 1) {
      InstanceToWrite = CurrentEncLoc[CurrentPtr++];
    }
    //
    // Following the flags is the register Offset.  This will be either a 8-bit or 16-bit value based on the
    // offset16 flag.
    //
    if (RegisterAttributes->Offset16 == 1) {
      RegisterOffset = *((UINT16 *) &CurrentEncLoc[CurrentPtr]);
      CurrentPtr +=2;
    } else {
      RegisterOffset = *((UINT8 *) &CurrentEncLoc[CurrentPtr]);
      CurrentPtr +=1;
    }
    //
    // Read either 2 bytes or 1 byte to get the boxtype and byte enables based on
    // the size of the boxtype number and the PFCT value to be written.
    //
    if (RegisterAttributes->UseBB2 == 1) {
      //
      // The Boxtype and ByteEnables are always provided in the next byte.  They are split into 4-bits each.
      //
      BoxType    = CurrentEncLoc[CurrentPtr];
      ByteEnable = CurrentEncLoc[CurrentPtr + 1];
      CurrentPtr += 2;
    } else {
      //
      // The Boxtype and ByteEnables are always provided in the next byte.  They are split into 4-bits each.
      //
      BoxType    = (CurrentEncLoc[CurrentPtr] >> 4) & 0x0f;
      ByteEnable = (CurrentEncLoc[CurrentPtr] >> 0) & 0x0f;
      CurrentPtr += 1;
    }
    //
    // If the MaskPresent bit in the flags is set, then the mask will be provided.  The length of the mask is based on the
    // byte enables.  Ex. If 2-bits in the BE (_0110) are set, then the mask will be 2-bytes long.
    //
    if (RegisterAttributes->MaskPresent == 1) {
      Mask = ByteEnableToUintX (ByteEnable, *((UINT32*) &CurrentEncLoc[CurrentPtr]), &MaskValueLength);
      CurrentPtr += MaskValueLength;
    } else {
      Mask = ByteEnableToUintX (ByteEnable, 0xffffffff, &MaskValueLength);
    }
    //
    // The Action/PFCT provides three distinct fields.  The action (get/set/poll), the PFCT, and the dynamic flag.
    // This is a 1-byte field and is split into [3-bits action, 1-bit dyn, 4-bits pfct].
    Action      = (CurrentEncLoc[CurrentPtr] >> 5) & 0x07;
    PfctDynamic = (CurrentEncLoc[CurrentPtr] >> 4) & 0x01;
    Pfct        = (CurrentEncLoc[CurrentPtr] >> 0) & 0x0f;
    CurrentPtr  += 1;
    //
    // If the action is MSP_SET_DELAY, then immediately following the ACTION will be a 4byte delay value.
    //
    if (Action == MSP_SET_DELAY) {
      Delay = *(UINT32 *) &CurrentEncLoc[CurrentPtr];
      CurrentPtr += 4;
    }
    //
    // The PFCT status will provide the flag if any of the PFCT conditions where true, the assumption is no
    // condition is true, and will be changed to TAKE_ACTION if one is determined.
    //
    PfctStatus = SKIP_ACTION;
    //
    // If the PFCT flag is set to 0, then the assignment is global/PFCT independent.  The assignment should then
    // provide one value.  If the dynamic bit is set, the value is an 8-bit index value, otherwise the value length
    // will be based on the byte-enables.
    //
    if (Pfct == PFCT_INDEPENDENT) {
      PfctStatus = TAKE_ACTION;
      if (PfctDynamic == 1) {
        PfctValue = *(UINT16 *) &CurrentEncLoc[CurrentPtr];
        CurrentPtr += 2;
      } else {
        PfctValue = ByteEnableToUintX (ByteEnable, *((UINT32*) &CurrentEncLoc[CurrentPtr]), &MaskValueLength);
        CurrentPtr += MaskValueLength;
      }
    } else {
      //
      // To reach this point, the PFCT Flag must have been non-zero so non-global.
      // The next byte is the total number of conditional-Value pairs..
      //
      NumberConditionalValues = CurrentEncLoc[CurrentPtr];
      CurrentPtr += 1;
      //
      // Loop through each Condition/Value pair looking for a match based on the current pfct.
      //
      for (Counter = 0; Counter < NumberConditionalValues; Counter++) {
        //
        // Determine the number of bytes are needed for the condition.  The system will
        // compare the PFCT condition byte-by-byte with the TRUE PFCT condition.  If all the
        // bits compare okay, then the condition is true.  ConditionalFlag is the byte-by-byte
        // comparison flag.
        //
        NumberConditionalBytes = PFCTVariations[Pfct].LengthVariation;
        ConditionalFlag = 1;
        while (NumberConditionalBytes > 0 && ConditionalFlag == 1) {
          TestVal = (UINT8) ((PFCTVariations[Pfct].Pfct >> ((NumberConditionalBytes - 1) * 8) ) & 0xff);
          if ((CurrentEncLoc[CurrentPtr+NumberConditionalBytes - 1]  & TestVal) != TestVal) {
             ConditionalFlag = 0;
          }
          NumberConditionalBytes--;
        }
        CurrentPtr += PFCTVariations[Pfct].LengthVariation;
        //
        // Read in the value which is also based on the byte-enables for its length, similar to the mask.
        //
        if (PfctDynamic == 1) {
          PfctValue = *(UINT16 *) &CurrentEncLoc[CurrentPtr];
          PfctValueSize = 2;
        } else {
          PfctValue = ByteEnableToUintX (ByteEnable, *((UINT32*) &CurrentEncLoc[CurrentPtr]), &MaskValueLength);
          PfctValueSize = MaskValueLength;
        }
        CurrentPtr += PfctValueSize;
        //
        // If the condition flag is still true, then move the currentPtr to the end of the remaining condition-values.
        // and set the status to take_action.
        if (ConditionalFlag == 1) {
          PfctStatus = TAKE_ACTION;
          CurrentPtr += (PFCTVariations[Pfct].LengthVariation + PfctValueSize) * (NumberConditionalValues - Counter - 1);
          break;
        }
      }  // for (Counter = 0; Counter < NumberConditionalValues; Counter++)
    } // if (Pfct == PFCT_INDEPENDENT) {
    //
    // Save the value which will be used as the index to the switch statement
    // in DynamicAssignmentGenerated.
    //
    DynamicIndex = (UINT16) PfctValue;
    if (RegisterAttributes2->StringPresent == 1) {
      NumStrings = CurrentEncLoc[CurrentPtr++];
    } else {
      NumStrings = 0;
    }
    //
    // Now that a value has been determined true, the value must be programmed based on the
    // loopings to multiple registers.
    //
    if (PfctStatus == TAKE_ACTION  && ConditionalExecution == 1) {
      if (Action == MSP_SUBTASK) {
        if (MspData->CapsuleStringsEn) {
          RcDebugPrint (SDBG_TRACE, "   +");
          for (k = 0; k < MspData->MaxBoxNameLength; k++) {
            RcDebugPrint (SDBG_TRACE, " ");
          }
        }

        if (MspData->DumpRegisterNamesEn) {
          for (k = 0; k < MspData->MaxRegisterNameLength + 2; k++) {
            RcDebugPrint (SDBG_TRACE, " ");
          }
        }

        RcDebugPrint (SDBG_TRACE, "   NA   NA         NA         ");
        RcDebugPrint (SDBG_TRACE, "SUBTASK   NA         ");
        //
        // Search for index in capsuleStrings
        //
        CapsuleStrIndex = (UINT16) -1;
        for (k = 0; k < MspData->MaxSubTasks; k++) {
          if (PfctValue == MspData->SubtaskOffsetIndex[k].FncOffset) {
            CapsuleStrIndex = MspData->SubtaskOffsetIndex[k].StrIndex;
            break;
          }
        }
        if (CapsuleStrIndex != -1 && MspData->CapsuleStringsEn) {
          RcDebugPrint (SDBG_TRACE, "%a", (CHAR8 *)MspData->CapsuleStrings[CapsuleStrIndex]);
        } else {
          RcDebugPrint (SDBG_TRACE, "ERROR");
        }
        if (MspData->CapsuleStringsEn) {
          if (RegisterAttributes2->StringPresent == 1) {
            for (k = 0; k < NumStrings; k++) {
              RcDebugPrint (SDBG_TRACE, " %a",(CHAR8 *)MspData->OutputStrings[CurrentEncLoc[CurrentPtr + k]]);
              if ((k + 1) < NumStrings) {
                RcDebugPrint (SDBG_TRACE, ",");
              }
            }
          }
        }

        RcDebugPrint (SDBG_TRACE, "\n");
        if (CapsuleStrIndex != -1) {
          MspExecuteTask (MspData, (UINT16) PfctValue, CapsuleStrIndex, Channel);
        } else {
          MspExecuteTask (MspData, (UINT16) PfctValue, 0, Channel);
        }
        //
        // Else, not a MSP_SUBTASK...so proceed to process normal action.
        //
      } else {
        if (RegisterAttributes->StrobeLoop == 1) {
          MaxStrobe = MspData->MaxStrobes;
        } else {
          MaxStrobe = 1;
        }
        if (RegisterAttributes->RankLoop == 1) {
          MaxRank = MspData->MaxRanks;
        } else {
          MaxRank = 1;
        }
        //
        // If the register should be written only for a specific channel, and this call is
        // not the correct channel, just skip this assignment and go to the next assignment.
        //
        if (RegisterAttributes2->ChannelDependent == 0 || (RegisterAttributes2->ChannelDependent == 1 && Channel == ChannelToWrite)) {
          //
          // If the InstanceDependent flag is set, then the total number of instances that will be written is 1,
          // otherwise, the total number of instances/channel for the given boxtype will be written. The other case
          // is if we are looping on rank and/or strobe, then we will ignore the instance looping since strobe/rank
          // looping is a higher priority.
          //
          if (RegisterAttributes2->InstanceDependent == 0 && Action != MSP_DELAY &&
              RegisterAttributes->StrobeLoop == 0 && RegisterAttributes->RankLoop == 0) {
              NumberInstances = INSTANCE_PORT_MAP(BoxType).Instances;
              if (NumberInstances == 0) {
                //
                // If this boxtype is not channel based, then use the total number of instances
                // instead of the instances per channel.
                //
                NumberInstances = INSTANCE_PORT_MAP(BoxType).TotalInstances;
                ChannelBasedOffsetDetected = FALSE;
              }
          } else {
            NumberInstances = 1;
          }
          //
          // The number of instances cannot be 0xff, if an 0xff is read this indicates
          // that the current Box Type is not valid for the current blueprint, so we want
          // to skip any subsequent action
          //
          if (NumberInstances == 0xff) {
            NumberInstances = 0;
          }
          //
          // Loop though all the instances determined earlier.
          //
          for (Instance = 0; Instance < NumberInstances; Instance++) {
            for (Rank = 0; Rank < MaxRank; Rank++) {
              for (Strobe = 0; Strobe < MaxStrobe; Strobe++) {
                //
                // If the register should be written only for a specific channel, and this call is
                // not the correct channel, just skip this assignment and go to the next assignment.
                //
                if (RegisterAttributes2->InstanceDependent == 1) {
                  Instance = InstanceToWrite;
                }
                //
                // If the broadcast feature within the IOSF is enabled, then only one write, the first, will occur as all the
                // instances will be written to.
                //
                if (BROADCAST_SUP && Instance > 0) {
                  SkipIosf = 1;
                } else {
                  SkipIosf = 0;
                }
                //
                // Determine the actual offset/mask for the register to be accessed.  The offset currently is based off the
                // offset for the assignment + the rank offset + bytelane offset. The mask and shift bit will also be used
                // for PCI accesses although the data for the PCI access is in PciReg which is filled in below.
                //
                Register.Offset   = RegisterOffset;
                Register.Mask     = Mask;
                Register.ShiftBit = 0;
                ThisBoxType = BoxType;
                //
                // If we're looping on strobe/rank we need to adjust the offset to the correct register location
                // for this strobe/rank.
                //
                if (RegisterAttributes->StrobeLoop == 0 && RegisterAttributes->RankLoop == 0) {
                  ThisChannel = Channel;
                  ThisInstance = Instance;
                } else {
                  if (RegisterAttributes->StrobeLoop == 1) {
                    Register.Offset += STROBE_TO_REGISTER(FLOOR_PLAN(Channel,Strobe).Strobelane,1);
                    ThisChannel = FLOOR_PLAN(Channel,Strobe).Channel;
                    ThisInstance = (UINT8)(Instance + STROBE_TO_REGISTER(FLOOR_PLAN(Channel,Strobe).Strobelane,0));
                  }
                  if (RegisterAttributes->RankLoop == 1) {
                    Register.Offset += RANK_TO_REGISTER(Rank,1);
                    ThisInstance += ((UINT8)(Instance + RANK_TO_REGISTER(Rank,0)));
                  }
                  //
                  // Select the new boxtype since we may have increased the sub-box instance to
                  // a subsequent boxtype to reach the correct register for this rank/strobe. An
                  // example would be if strobe 16's registers were in the next PCI bus/dev/func
                  // and we needed to point to the next boxtype to get to that location.
                  //
                  for (i = 0; i < MspData->MaxBoxes; i++) {
                    if (INSTANCE_PORT_MAP(i).InstanceStartIndex <= (INSTANCE_PORT_MAP(BoxType).InstanceStartIndex + ThisInstance)) {
                      ThisBoxType = i;
                    } else {
                      break;
                    }
                  }
                }
                //
                // When a Boxtype that is not channel based we need to hardcode 'ThisChannel' = 0
                //
                if (!ChannelBasedOffsetDetected) {
                  ThisChannel = 0;
                }
                //
                // If the action was a MSP_GET/MSP_SET/MSP_SET_DELAY/MSP_POLL, then the register offset, mask, delay, and value will be printed.
                //
                if (Action != MSP_DELAY) {
                  if (GetRegisterAccessInfo (MspData, BoxType, ThisChannel, ThisInstance, (UINT32 *)&readValue, &Port, &BaseOffset, &Bus, &Device, &Func, ModeRead) != NoError) {
                    continue;
                  }
                }
                //
                // The printout of the assignment will begin with either a *,/, or +.
                //  *-> the assignment was performed, but it completed the assignment of others as it had broadcast enabled.
                //  /-> the assignment was performed.
                //  +-> the assignment was skipeed as it was assigned with the broadcast.
                //
                if (Instance == 0 && RegisterAttributes2->InstanceDependent == 0 && BROADCAST_SUP) {
                  RcDebugPrint (SDBG_TRACE, "   *");
                } else {
                  RcDebugPrint (SDBG_TRACE, "   +");
                }
                //
                // Print out register information.
                //
                if (Action == MSP_GET || Action == MSP_SET || Action == MSP_SET_DELAY || Action == MSP_POLL) {
                  //
                  // Convert the PFCTValue to the dynamic value, if the flag is set.
                  //
                  if (PfctDynamic == 1 && Action != MSP_GET) {
                    Register.Mask     = 0xFFFFFFFF;
                    Register.ShiftBit = 0;
                    NewValue = MemRegRead (MspData, BoxType, ThisChannel, ThisInstance, Register);

                    (*MspData->DynamicPtr)(MspData, MspData->DynamicVars, ThisChannel, DynamicIndex, &NewValue);
                    PfctValue = NewValue;
                  }
                  if (MspData->CapsuleStringsEn) {
                    RcDebugPrint (SDBG_TRACE, "%a ", UNIT_STRINGS(ThisBoxType));
                  }
                  if (MspData->DumpRegisterNamesEn) {
                    RegisterNumber = GetRegisterNumber (MspData, ThisBoxType, Register.Offset);
                    //
                    // If the register has a name, print it out. Else, just pad with spaces.
                    //
                    if (RegisterNumber == 0xFFFFFFFF) {
                      i = 0;
                    } else {
                      RcDebugPrint (SDBG_TRACE, "%a",
                          MspData->Registers[GetRegisterNumber (MspData, ThisBoxType, Register.Offset)].RegInfo->RegisterName
                      );
                      i = (UINT8) StringLength ((UINT8 *)MspData->Registers[RegisterNumber].RegInfo->RegisterName);
                    }
                    for (; i <= MspData->MaxRegisterNameLength + 2; i++) {
                      RcDebugPrint (SDBG_TRACE, " ");
                    }
                  }

                  BarOffset = 0;
                  if (INSTANCE_PORT_MAP(ThisBoxType).AccessMethod == eSB) {
                    RcDebugPrint (SDBG_TRACE, "   0x%02X", Port);
                  } else if (INSTANCE_PORT_MAP(ThisBoxType).AccessMethod == eBAR) {
                    RcDebugPrint (SDBG_TRACE, "    MEM");
                    BarOffset = (UINT32) DYNAMIC_VARS(Channel,INSTANCE_PORT_MAP(BoxType).AccessBase);
                  } else if (INSTANCE_PORT_MAP(ThisBoxType).AccessMethod == ePCI) {
                    RcDebugPrint (SDBG_TRACE, "%02X/%02X/%01X", Bus, Device, Func);
                  }
                  RcDebugPrint (SDBG_TRACE, " 0x%08X 0x%08X%08X ",
                    BarOffset + ((Register.Offset + BaseOffset) & 0xFFFFFFFF), 0, (UINT32)Register.Mask
                  );
                }
                if (Action == MSP_GET) {
                  //
                  // Get the right shift bit to capture the variable being read.
                  //
                  for (i = 0; i < MaxBit; i++) {
                    if ((Register.Mask & (UINT32)(BIT0 << i)) != 0)
                    {
                      break;
                    }
                    Register.ShiftBit++;
                  }
                  DYNAMIC_VARS(ChannelToWrite,PfctValue) = MemFieldRead (MspData, BoxType, ThisChannel, ThisInstance, Register);

                  if (!ChannelBasedOffsetDetected) {
                    //
                    // Even if we execute this task on a Non-Channel Based Boxtype (NCBB), input parameter Channel
                    // might be equal '1', so when we execute a MSP_GET command over a register on a 'NCBB',
                    // the read value also needs to be stored on ThisChannel index, in case we want to restore it later.
                    // The restore value (MSP_SET Command) will be taken from the ThisChannel index in the function 'DynamicAssignmentGenerated'
                    //
                    DYNAMIC_VARS(ThisChannel,PfctValue) = DYNAMIC_VARS(ChannelToWrite,PfctValue);
                  }
                  RcDebugPrint (SDBG_TRACE, "GET       0x%08X 0x%08X%08X", 0,
                    0,
                    (UINT32)(DYNAMIC_VARS(ChannelToWrite,PfctValue))
                  );
                } else if (Action == MSP_SET || Action == MSP_SET_DELAY)  {
                  if (Action == MSP_SET) {
                    RcDebugPrint (SDBG_TRACE, "SET       0x%08X ", 0);
                  }
                  if (Action == MSP_SET_DELAY) {
                    RcDebugPrint (SDBG_TRACE, "SET_DELAY 0x%08X ", Delay);
                  }
                  RcDebugPrint (SDBG_TRACE, "0x%08X%08X", 0, (UINT32)(PfctValue));
                  MemRegWrite (MspData, BoxType, ThisChannel, ThisInstance, Register, PfctValue, ByteEnable);
                  //
                  // If the action has a delay, with the set, then delay between every assignment.
                  //
                  if (Action == MSP_SET_DELAY) {
                    MspDelay (NANO_DEL, Delay);
                    {
                      CteDelayQclk (Delay);
                    }
                  }
                } else if (Action == MSP_POLL) {
                  //
                  // If the action is a MSP_POLL, then print out the register/value to be polled for, and begin polling.
                  //
                  RcDebugPrint (SDBG_TRACE, "POLL      0x00000000 0x%08X%08X", 0, (UINT32)(PfctValue));
                  PfctValue ^= Mask;
                  do {
                    readValue = MemRegRead (MspData, BoxType, ThisChannel, ThisInstance, Register);

                    //
                    // Delay only if this is a CTE build, otherwise do nothing.
                    //

                    CteDelayQclk (50);

                    readValue &= Mask;
                    if ((GetEmulation() & SIMICS_FLAG) != 0) {
                      break;
                    }
                  } while (readValue == PfctValue);
                } else if (Action == MSP_DELAY) {
                  RcDebugPrint (SDBG_TRACE, "DELAY: %dns", PfctValue);
                  //
                  // Delay only if this is a CTE build, otherwise do nothing.
                  //
                  {
                    CteDelayQclk (PfctValue);
                  }

                  MspDelay (NANO_DEL, (UINT32) PfctValue);
                }
                //
                // Add the length of the current assignment to the current encoded location to the the location
                //  of the next assignment.
                //
                if (RegisterAttributes2->StringPresent == 1) {
                  for (k = 0; k < NumStrings; k++) {
                    if (MspData->CapsuleStringsEn) {
                      RcDebugPrint (SDBG_TRACE, " %a", (CHAR8 *)MspData->OutputStrings[CurrentEncLoc[CurrentPtr + k]]);
                      if ((k + 1) < NumStrings) {
                        RcDebugPrint (SDBG_TRACE, ",");
                      }
                    }
                  }
                }
                RcDebugPrint (SDBG_TRACE, "\n");
              } // Strobe loop
            } // Rank loop
          } // Instance loop ...
        }
      }
    } // TAKE_ACTION
    CurrentEncLoc += (CurrentPtr + NumStrings);
  } // *CurrentEncLoc != ASSIGNDONE loop ...

  SetDebugLevel (MsgLevel);

  return MSP_SUCCESS;
}
/**
  This routine converts a PFCT value from BITx to x.
  The Current* variables must not be in BITx format. For example
  if CurrentPlatform is set to BIT4, this routine sets
  CurrentPlatform to 4 instead.

  @param[in, out]  MspData         Host structure for all data related to Msp.
  @retval          NONE
**/
VOID
ConvertPfctFromBitXToX (
  IN        MSP_DATA         *MspData
  )
{
  UINT8 i;
  UINT8 Channel;
  //
  // Since Current* variables is setup as BITx, need to convert BITX into x for the variable
  // that Msp needs to do its platform comparisons in the CreatePfctSel.
  //
  for (Channel = 0; Channel < MspData->MaxChannels; Channel++) {
    for (i = 0; i < 16; i++) {
      if ((MspData->CurrentPlatform[Channel] & (1 << i)) != 0) {
        MspData->CurrentPlatform[Channel] = i;
      }
      if ((MspData->CurrentFrequency[Channel] & (1 << i)) != 0) {
        MspData->CurrentFrequency[Channel] = i;
      }
      if ((MspData->CurrentConfiguration[Channel] & (1 << i)) != 0) {
        MspData->CurrentConfiguration[Channel] = i;
      }
      if ((MspData->CurrentTechnology[Channel] & (1 << i)) != 0) {
        MspData->CurrentTechnology[Channel] = i;
      }
    }
  }
}

/**
  This function compares the current PFCT of this channel and
  match it to the list of allowable PFCTs for this Task in the
  call table.

  @param[in, out]  MspData         Host structure for all data related to Msp.
  @param[in]       j               Task Descriptor.
  @retval          NONE
**/
VOID
CreateChannelMask (
  IN        MSP_DATA             *MspData,
  IN        TASK_DESCRIPTOR      *j
  )
{
  UINT8                   Channel;

  for (Channel = 0; Channel < MspData->MaxChannels; Channel++) {
    if (((j->PList & (1 << MspData->CurrentPlatform[Channel]))      != 0) &&
        ((j->FList & (1 << MspData->CurrentFrequency[Channel]))     != 0) &&
        ((j->CList & (1 << MspData->CurrentConfiguration[Channel])) != 0) &&
        ((j->TList & (1 << MspData->CurrentTechnology[Channel]))       != 0)
      )
    {
      MspData->ExecuteOnThisChannel[Channel] = (MspData->ChannelEnabled[Channel]) ? TRUE : FALSE;
    } else {
      MspData->ExecuteOnThisChannel[Channel] = FALSE;
    }
  }
}
/**
  Single entry point to Msp. MSP calls this function
  and then Msp acts as a black box, performing the requested
  stage of DDRIO init and returns when complete or when
  an error is encountered.

  @param[in, out]  MspData         Host structure for all data related to Msp.
  @retval          Status
**/
MSP_STATUS
MspEntry (
  IN  OUT   MSP_DATA            *MspData
  )
{

  UINT32      ConditionalExec;
  UINT8       Channel;
  MSP_STATUS  Status;
  TASK_DESCRIPTOR   *j;


  Status        = MSP_SUCCESS;
  //
  // Convert BITx values to x instead for Current* variables which contain the current PFCT.
  //
  ConvertPfctFromBitXToX (MspData);
  //
  // Execute all steps for the given Stage.
  //
  for (j = (TASK_DESCRIPTOR   *)&MspData->InitTask[0]; j->Function != NULL; j++) {
    if ((MspData->BootMode & j->BootMode) != 0) {

      SetMemPhaseCP (j->PostCode, 0);

      OutputCheckpoint (CHECKPOINT_MAJOR_DDRIO_INIT, j->PostCode, ((PSYSHOST) GetSysHostPointer ())->var.mem.currentSocket);
      //
      // Mark the channels which will be executing this routine. Some channels might skip execution
      // based on their PFCT and Conditional Variable.
      //
      CreateChannelMask (MspData, j);
      if (j->Channel == ChAll) {
        for (Channel = 0; Channel < MspData->MaxChannels; Channel++) {
          if (!MspData->ExecuteOnThisChannel[Channel]) {
            continue;
          }
          if (j->CondExec < MspData->MaxDynVar) {
            ConditionalExec = (UINT32) DYNAMIC_VARS(Channel,j->CondExec);
            if (ConditionalExec == 0) {
              continue;
            }
          }
          if (j->CapsuleStartIndex == EXTERNAL && MspData->CapsuleStringsEn) {
#ifdef DEBUG_CODE_BLOCK
            AcquirePrintControl ();
            RcDebugPrintWithDevice (SDBG_MINMAX, ((PSYSHOST) GetSysHostPointer ())->var.mem.currentSocket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
              " Task: %a External\n", MspData->CapsuleStrings[j->StringIndex]);
            ReleasePrintControl ();
#endif // DEBUG_CODE_BLOCK
          }
          Status = (*(j->Function)) (MspData, j->CapsuleStartIndex, j->StringIndex, Channel);
          if (Status != MSP_SUCCESS) {
            return Status;
          }
        }
      } else if (j->Channel == ChNone) {
        if (j->CapsuleStartIndex == EXTERNAL && MspData->CapsuleStringsEn) {
          RcDebugPrint (SDBG_MINMAX, " %a\n", MspData->CapsuleStrings[j->StringIndex]);
        }
        if (j->CondExec < MspData->MaxDynVar) {
          ConditionalExec = (UINT32) DYNAMIC_VARS(0,j->CondExec);
          if (ConditionalExec == 0) {
            continue;
          }
        }
        Status = (*(j->Function)) (MspData, j->CapsuleStartIndex, j->StringIndex, 0);
        if (Status != MSP_SUCCESS) {
          return Status;
        }
      } else {
        if (j->CondExec < MspData->MaxDynVar) {
          ConditionalExec = (UINT32) DYNAMIC_VARS(j->Channel,j->CondExec);
          if (ConditionalExec == 0) {
            continue;
          }
        }
        if (j->CapsuleStartIndex == EXTERNAL && MspData->CapsuleStringsEn) {
#ifdef DEBUG_CODE_BLOCK
          AcquirePrintControl ();
          RcDebugPrintWithDevice (SDBG_MINMAX, ((PSYSHOST) GetSysHostPointer ())->var.mem.currentSocket, NO_CH, NO_DIMM, NO_SUBCH, NO_RANK, NO_STROBE, NO_BIT,
            " Task: %a External\n", MspData->CapsuleStrings[j->StringIndex]);
          ReleasePrintControl ();
#endif // DEBUG_CODE_BLOCK
        }
        //
        // We're here because a specific channel has been requested.
        //
        if (MspData->ExecuteOnThisChannel[j->Channel]) {
          Status = (*(j->Function)) (MspData, j->CapsuleStartIndex, j->StringIndex, j->Channel);
          if (Status != MSP_SUCCESS) {
            return Status;
          }
        }
      }
    } // if ((MspData->BootMode & j->BootMode) != 0) {
  } // for (j; j->Function != NULL; j++) {

  return Status;
}

VOID
VerifyMspData(
IN    MSP_DATA         *MspData
)
{
  UINT8     Channel;
  //
  // Verify Misc Data
  //
  RcDebugPrint (SDBG_MINMAX, "BootMode              = 0x%02x\n", MspData->BootMode);
  RcDebugPrint (SDBG_MINMAX, "SerialDebugMsgLvl     = 0x%02x\n", GetDebugLevel ());
  //
  // Verify Common Data
  //
  RcDebugPrint (SDBG_MINMAX, "CurrentBlueprint      = %d\n", MspData->CurrentBlueprint);
  RcDebugPrint (SDBG_MINMAX, "MaxBlueprints         = %d\n", MspData->MaxBlueprints);
  if (MspData->MaxBlueprints <= 0) {
    MspAssert(MspData, "MspData->MaxBlueprints has not been specified", (UINT8 *)__FUNCTION__, __LINE__);
  } else if (MspData->CurrentBlueprint >= MspData->MaxBlueprints) {
    MspAssert(MspData, "MspData->CurrentBlueprint >= MspData->MaxBlueprints", (UINT8 *)__FUNCTION__, __LINE__);
  }
  RcDebugPrint (SDBG_MINMAX, "MaxChannels           = %d\n", MspData->MaxChannels);
  if (MspData->MaxChannels <= 0) {
    MspAssert(MspData, "MspData->MaxChannels has not been specified", (UINT8 *)__FUNCTION__, __LINE__);
  }
  RcDebugPrint (SDBG_MINMAX, "MaxDynVar             = %d\n", MspData->MaxDynVar);
  if (MspData->MaxDynVar <= 0) {
    MspAssert(MspData, "MspData->MaxDynVar has not been specified", (UINT8 *)__FUNCTION__, __LINE__);
  }
  if (MspData->MaxDynVar > MspData->MaxDynamicVarsX) {
    MspAssert(MspData, "MspData->MaxDynVar > MaxDynamicVarsX", (UINT8 *)__FUNCTION__, __LINE__);
  }
  RcDebugPrint (SDBG_MINMAX, "MaxDynamicVarsX       = %d\n", MspData->MaxDynamicVarsX);
  if (MspData->MaxDynamicVarsX < MspData->MaxDynVar) {
    MspAssert (MspData, "MspData->MaxDynamicVarsX < MspData->MaxDynVar", (UINT8 *)__FUNCTION__, __LINE__);
  }
  RcDebugPrint (SDBG_MINMAX, "MaxBoxes              = %d\n", MspData->MaxBoxes);
  if (MspData->MaxBoxes <= 0) {
    MspAssert(MspData, "MspData->MaxBoxes has not been specified", (UINT8 *)__FUNCTION__, __LINE__);
  }
  RcDebugPrint (SDBG_MINMAX, "MaxStrobes            = %d\n", MspData->MaxStrobes);
  if (MspData->MaxStrobes == 0) {
    MspAssert(MspData, "MspData->MaxStrobes has a value of '0'", (UINT8 *)__FUNCTION__, __LINE__);
  }
  RcDebugPrint (SDBG_MINMAX, "MaxRanks              = %d\n", MspData->MaxRanks);
  if (MspData->MaxRanks == 0) {
    MspAssert(MspData, "MspData->MaxRanks has a value of '0'", (UINT8 *)__FUNCTION__, __LINE__);
  }
  RcDebugPrint (SDBG_MINMAX, "NumType               = %d\n", MspData->NumType);
  if (MspData->NumType <= 0) {
    MspAssert(MspData, "MspData->NumType has not been specified", (UINT8 *)__FUNCTION__, __LINE__);
  }
  RcDebugPrint (SDBG_MINMAX, "NumConf               = %d\n", MspData->NumConf);
  if (MspData->NumConf <= 0) {
    MspAssert(MspData, "MspData->NumConf has not been specified", (UINT8 *)__FUNCTION__, __LINE__);
  }
  RcDebugPrint (SDBG_MINMAX, "NumFreq               = %d\n", MspData->NumFreq);
  if (MspData->NumFreq <= 0) {
    MspAssert(MspData, "MspData->NumFreq has not been specified", (UINT8 *)__FUNCTION__, __LINE__);
  }
  RcDebugPrint (SDBG_MINMAX, "NumPlat               = %d\n", MspData->NumPlat);
  if (MspData->NumPlat <= 0) {
    MspAssert(MspData, "MspData->NumPlat has not been specified", (UINT8 *)__FUNCTION__, __LINE__);
  }
  RcDebugPrint (SDBG_MINMAX, "MaxInstancePorts      = %d\n", MspData->MaxInstancePorts);
  if (MspData->MaxInstancePorts <= 0) {
    MspAssert(MspData, "MspData->MaxInstancePorts has not been specified", (UINT8 *)__FUNCTION__, __LINE__);
  }
  RcDebugPrint (SDBG_MINMAX, "MaxBoxNameLength      = %d\n", MspData->MaxBoxNameLength);
  if (MspData->MaxBoxNameLength <= 0) {
    MspAssert(MspData, "MspData->MaxBoxNameLength has not been specified", (UINT8 *)__FUNCTION__, __LINE__);
  }
  RcDebugPrint (SDBG_MINMAX, "MaxRegisterNameLength = %d\n", MspData->MaxRegisterNameLength);
  if (MspData->MaxRegisterNameLength <= 0) {
    MspAssert(MspData, "MspData->MaxRegisterNameLength has not been specified", (UINT8 *)__FUNCTION__, __LINE__);
  }
  RcDebugPrint (SDBG_MINMAX, "Use64bVariables       = %d\n", MspData->Use64bVariables);
  if (MspData->Use64bVariables != 0) {
    RcDebugPrint (SDBG_MINMAX, "Warning Use64bVariables is set to TRUE\n");
  }
  if (MspData->CapsuleStrings != NULL) {
    RcDebugPrint (SDBG_MINMAX, "CapsuleStrings        = %a\n", MspData->CapsuleStrings[0]);
  } else if (MspData->CapsuleStringsEn) {
    MspAssert(MspData, "**CapsuleStrings has not been specified", (UINT8 *)__FUNCTION__, __LINE__);
  }
  if (MspData->OutputStrings != NULL && MspData->OutputStrings[0] != NULL) {
    RcDebugPrint (SDBG_MINMAX, "OutputStrings         = %a\n", MspData->OutputStrings[0]);
  } else if (MspData->OutputStrings != NULL && MspData->OutputStrings[0] == NULL) {
    RcDebugPrint (SDBG_MINMAX, "OutputStrings         = <Empty>\n");
  } else if (MspData->CapsuleStringsEn) {
    MspAssert (MspData, "**OutputStrings has not been specified", (UINT8 *)__FUNCTION__, __LINE__);
  }
  if (MspData->UnitStrings != NULL) {
   RcDebugPrint (SDBG_MINMAX, "UnitStrings           = %a\n", UNIT_STRINGS(0)             );
  } else if (MspData->CapsuleStringsEn) {
    MspAssert (MspData, "*UnitStrings has not been specified", (UINT8 *)__FUNCTION__, __LINE__);
  }
  RcDebugPrint (SDBG_MINMAX, "CapsuleStringsEn      = %d\n", MspData->CapsuleStringsEn     );
  RcDebugPrint (SDBG_MINMAX, "DumpRegisterNamesEn   = %d\n", MspData->DumpRegisterNamesEn  );
  if (MspData->ExecuteOnThisChannel == NULL) {
    MspAssert (MspData, "MspData->ExecuteOnThisChannel has not been specified", (UINT8 *)__FUNCTION__, __LINE__);
  }
  //
  // Verify Data (PFCT)
  //
   RcDebugPrint (SDBG_MINMAX, "      Channels enabled 0 --> %d\n", MspData->MaxChannels-1);
  if (MspData->ChannelEnabled == NULL) {
    MspAssert (MspData, "MspData->ChannelEnabled has not been specified", (UINT8 *)__FUNCTION__, __LINE__);
  } else {
    RcDebugPrint (SDBG_MINMAX, "CHEN: ");
    for (Channel = 0; Channel < MspData->MaxChannels; Channel ++) {
      RcDebugPrint (SDBG_MINMAX, "%d,", MspData->ChannelEnabled[Channel]);
    }
    RcDebugPrint (SDBG_MINMAX, "\n");
  }
  if (MspData->CurrentPlatform == NULL) {
    MspAssert (MspData, "MspData->CurrentPlatform has not been specified", (UINT8 *)__FUNCTION__, __LINE__);
  } else {
    RcDebugPrint (SDBG_MINMAX, "CHPL: ");
    for (Channel = 0; Channel < MspData->MaxChannels; Channel ++) {
      RcDebugPrint (SDBG_MINMAX, "%02x,", MspData->CurrentPlatform[Channel]);
    }
    RcDebugPrint (SDBG_MINMAX, "\n");
  }
  if (MspData->CurrentFrequency == NULL) {
    MspAssert (MspData, "MspData->CurrentFrequency has not been specified", (UINT8 *)__FUNCTION__, __LINE__);
  } else {
    RcDebugPrint (SDBG_MINMAX, "CHFR: ");
    for (Channel = 0; Channel < MspData->MaxChannels; Channel ++) {
      RcDebugPrint (SDBG_MINMAX, "%02x,", MspData->CurrentFrequency[Channel]);
    }
    RcDebugPrint (SDBG_MINMAX, "\n");
  }
  if (MspData->CurrentConfiguration == NULL) {
    MspAssert (MspData, "MspData->CurrentConfiguration has not been specified", (UINT8 *)__FUNCTION__, __LINE__);
  } else {
    RcDebugPrint (SDBG_MINMAX, "CHCN: ");
    for (Channel = 0; Channel < MspData->MaxChannels; Channel ++) {
      RcDebugPrint (SDBG_MINMAX, "%02x,", MspData->CurrentConfiguration[Channel]);
    }
    RcDebugPrint (SDBG_MINMAX, "\n");
  }
  if (MspData->CurrentTechnology == NULL) {
    MspAssert (MspData, "MspData->CurrentTechnology has not been specified", (UINT8 *)__FUNCTION__, __LINE__);
  } else {
    RcDebugPrint (SDBG_MINMAX, "CHTE: ");
    for (Channel = 0; Channel < MspData->MaxChannels; Channel ++) {
      RcDebugPrint (SDBG_MINMAX, "%02x,", MspData->CurrentTechnology[Channel]);
    }
    RcDebugPrint (SDBG_MINMAX, "\n");
  }
  //
  // Verify:
  // InitTask and InitData
  // Different Arrays
  //
  if (MspData->InitTask == NULL) {
    MspAssert (MspData, "MspData->InitTask has not been specified", (UINT8 *)__FUNCTION__, __LINE__);
  } else {
    RcDebugPrint (SDBG_MINMAX, "InitTask              = %04x|%04x\n", MspData->InitTask[0].PostCode, MspData->InitTask[0].CapsuleStartIndex     );
  }
  if (MspData->InitData == NULL) {
    MspAssert (MspData, "MspData->InitData has not been specified", (UINT8 *)__FUNCTION__, __LINE__);
  } else {
    RcDebugPrint (SDBG_MINMAX, "InitData              = %02x,%02x,%02x,%02x\n",
      MspData->InitData[0],
      MspData->InitData[1],
      MspData->InitData[2],
      MspData->InitData[3]
    );
  }
  if (MspData->SubtaskOffsetIndex == NULL) {
    MspAssert (MspData, "MspData->SubtaskOffsetIndex has not been specified", (UINT8 *)__FUNCTION__, __LINE__);
  } else {
    RcDebugPrint (SDBG_MINMAX, "SubtaskOffsetIndex    = %08x::%04x\n", MspData->SubtaskOffsetIndex[0].FncOffset,MspData->SubtaskOffsetIndex[0].StrIndex);
  }
  RcDebugPrint (SDBG_MINMAX, "MaxSubTasks           = %d\n", MspData->MaxSubTasks     );
  if (MspData->DynamicVars == NULL) {
    MspAssert (MspData, "MspData->DynamicVars has not been specified", (UINT8 *)__FUNCTION__, __LINE__);
  }
  if (MspData->InstancePortMap == NULL) {
    MspAssert (MspData, "MspData->InstancePortMap has not been specified", (UINT8 *)__FUNCTION__, __LINE__);
  } else {
    RcDebugPrint (SDBG_MINMAX, "InstancePortMap       = %d|%d\n", INSTANCE_PORT_MAP(0).TotalInstances, INSTANCE_PORT_MAP(0).Instances);
  }
  if (MspData->StrobeToRegister == NULL) {
    MspAssert (MspData, "MspData->StrobeToRegister has not been specified", (UINT8 *)__FUNCTION__, __LINE__);
  } else {
    RcDebugPrint (SDBG_MINMAX, "StrobeToRegister      = %d|%d\n", STROBE_TO_REGISTER(0,0), STROBE_TO_REGISTER(0,0));
  }
  if (MspData->FloorPlan == NULL) {
    MspAssert (MspData, "MspData->FloorPlan has not been specified", (UINT8 *)__FUNCTION__, __LINE__);
  } else {
    RcDebugPrint (SDBG_MINMAX, "FloorPlan             = %d|%d\n", FLOOR_PLAN(0,0).Channel, FLOOR_PLAN(0,0).Strobelane);
  }
  if (MspData->Registers != NULL && MspData->Registers[0].RegInfo != NULL) {
    RcDebugPrint (SDBG_MINMAX, "Registers             = %d|%d\n", MspData->Registers[0].RegFields[0].StartBit, MspData->Registers[0].RegFields[0].EndBit);
  } else if (MspData->Registers != NULL && MspData->Registers[0].RegInfo != NULL) {
    RcDebugPrint (SDBG_MINMAX, "Registers             = <Empty>\n");
  } else if (MspData->DumpRegisterNamesEn) {
    MspAssert (MspData, "MspData->Registers has not been specified", (UINT8 *)__FUNCTION__, __LINE__);
  }
  if (MspData->RankToRegister == NULL) {
    MspAssert (MspData, "MspData->UnitStrings has not been specified", (UINT8 *)__FUNCTION__, __LINE__);
  } else {
    RcDebugPrint (SDBG_MINMAX, "RankToRegister        = %d\n", RANK_TO_REGISTER(0,0));
  }
  if (MspData->ChannelToInstanceMap == NULL) {
    MspAssert (MspData, "MspData->ChannelToInstanceMap has not been specified", (UINT8 *)__FUNCTION__, __LINE__);
  } else {
    RcDebugPrint (SDBG_MINMAX, "ChannelToInstanceMap  = %d\n", CHANNEL_TO_INSTANCE(0,1));
  }
  if (MspData->InstancePortOffset == NULL) {
    MspAssert (MspData, "MspData->InstancePortOffset has not been specified", (UINT8 *)__FUNCTION__, __LINE__);
  } else {
    RcDebugPrint (SDBG_MINMAX, "InstancePortOffset    = %04x::%04x\n", INSTANCE_PORT_OFFSET(0).Port, INSTANCE_PORT_OFFSET(0).Offset);
  }
  if (MspData->DynamicPtr == NULL) {
    MspAssert (MspData, "MspData->DynamicPtr has not been specified", (UINT8 *)__FUNCTION__, __LINE__);
  }
}

/**
  Initialize dynamically all the pointer to the structures for the MSP Library

  @param[in]              MspData                 Host structure for all data related to Msp.

  @param[in]              MaxBlueprints           Max number of blueprints supported
  @param[in]              MaxChannels             Max number of channels supported
  @param[in]              MaxDynVar               Max number of dynamic variables supported
  @param[in]              MaxBoxes                Max number of box types supported
  @param[in]              MaxStrobes              Max number of strobes supported
  @param[in]              MaxRanks                Max number of ranks supported

  @param[in]              NumType                 Max number of technologies supported (DDR3, DDR4,...)
  @param[in]              NumConf                 Max number of configurations supported (A0, B0, C0,...)
  @param[in]              NumFreq                 Max number of frequencies supported (1600, 1866, 2133,...)
  @param[in]              NumPlat                 Max number of platforms supported (CRB, SV, SIMICS,...)

  @param[in]              MaxInstancePorts        Max number of instances supported
  @param[in]              MaxBoxNameLength        Max string length for box names
  @param[in]              MaxRegisterNameLength   Max string length for register names
  @param[in]              Use64bVariables

  @param[in]              **CapsuleStrings        Pointer to tasks names strings
  @param[in]              **OutputStrings         Pointer to output log strings
  @param[in]              *UnitStrings            Pointer to box name strings
  @param[in]              CapsuleStringsEn        Specifies if information about boxes names,
                                                  output logs and/or task names have to be printed
  @param[in]              DumpRegisterNamesEn     Specifies if information about register names have to be printed

  @param[in]              *InitTask               Pointer to array that contains the table that describes order of execution
  @param[in]              *InitData               Pointer to array that contains all tasks
  @param[in]              *SubtaskOffsetIndex     Pointer to array that contains all subtasks
  @param[in]              MaxSubTasks             Max number of subtasks

  @param[in]              *InstancePortMap        Pointer to array that contains the instance port mapping
  @param[in]              *StrobeToRegister       Pointer to array that contains Strobe to Register relationship
  @param[in]              *FloorPlan              Pointer to array that contains floorplan
  @param[in]              *Registers              Pointer to array for all defines registers
  @param[in]              *RankToRegister         Pointer to array that contains Rank to Register relationship
  @param[in]              *ChannelToInstanceMap   Pointer to array that contains Channel to Instance relationship
  @param[in]              *InstancePortOffset     Pointer to array that contains the instance port offset
  @param[in]              DynamicPtr              Pointer to function that will be used to execute dynamic assignments during the MSP execution

  @param[in]              *ChannelEnabled         Pointer to array that contains information about which channels are enabled
  @param[in]              *ExecuteOnThisChannel   Pointer to array that contains information about which channels are under test

  @param[in]              *CurrentPlatform        Pointer to array that contains information about current platform
  @param[in]              *CurrentFrequency       Pointer to array that contains information about current frequency
  @param[in]              *CurrentConfiguration   Pointer to array that contains information about current configuration
  @param[in]              *CurrentTechnology      Pointer to array that contains information about current technology

  @param[in]              *DynamicVars            Pointer to array that contains information about dynamic variables
  @param[in]              MaxDynamicVarsX         Max number of the dynamic variables array

**/
VOID
InitializeMspDataPointers (
  IN    MSP_DATA          *MspData,

  IN    UINT8             MaxBlueprints,
  IN    UINT8             MaxChannels,
  IN    UINT16            MaxDynVar,
  IN    UINT16            MaxBoxes,
  IN    UINT8             MaxStrobes,
  IN    UINT8             MaxRanks,

  IN    UINT8             NumType,
  IN    UINT8             NumConf,
  IN    UINT8             NumFreq,
  IN    UINT8             NumPlat,

  IN    UINT16            MaxInstancePorts,
  IN    UINT8             MaxBoxNameLength,
  IN    UINT8             MaxRegisterNameLength,
  IN    BOOLEAN           Use64bVariables,

  IN    UINT8             **CapsuleStrings,
  IN    UINT8             **OutputStrings,
  IN    UINT8             *UnitStrings,
  IN    BOOLEAN           CapsuleStringsEn,
  IN    BOOLEAN           DumpRegisterNamesEn,

  IN    TASK_DESCRIPTOR   *InitTask,
  IN    UINT8             *InitData,
  IN    SUBTASK_INDEX_OFFSET  *SubtaskOffsetIndex,
  IN    UINT16            MaxSubTasks,

  IN    INSTANCE_PORT_MAP *InstancePortMap,
  IN    INT16             *StrobeToRegister,
  IN    FLOORPLAN         *FloorPlan,
  IN    Register          *Registers,
  IN    INT16             *RankToRegister,
  IN    UINT8             *ChannelToInstanceMap,
  IN    INSTANCE_PORT_OFFSET  *InstancePortOffset,
  IN    DynamicFunctionPtr DynamicPtr,

  IN    BOOLEAN           *ChannelEnabled,
  IN    BOOLEAN           *ExecuteOnThisChannel,

  IN    UINT16            *CurrentPlatform,
  IN    UINT16            *CurrentFrequency,
  IN    UINT16            *CurrentConfiguration,
  IN    UINT16            *CurrentTechnology,

  IN    UINT32            *DynamicVars,
  IN    UINT16            MaxDynamicVarsX
)
{
  //
  // Init Common Data
  //
  MspData->MaxBlueprints          = MaxBlueprints;
  MspData->MaxChannels            = MaxChannels;
  MspData->MaxDynVar              = MaxDynVar;
  MspData->MaxBoxes               = MaxBoxes;
  MspData->MaxStrobes             = MaxStrobes;
  MspData->MaxRanks               = MaxRanks;
  MspData->NumType                = NumType;
  MspData->NumConf                = NumConf;
  MspData->NumFreq                = NumFreq;
  MspData->NumPlat                = NumPlat;
  MspData->MaxInstancePorts       = MaxInstancePorts;
  MspData->MaxBoxNameLength       = MaxBoxNameLength;
  MspData->MaxRegisterNameLength  = MaxRegisterNameLength;
  MspData->Use64bVariables        = Use64bVariables;
  MspData->CapsuleStrings         = CapsuleStrings;
  MspData->OutputStrings          = OutputStrings;
  MspData->UnitStrings            = UnitStrings;
  MspData->CapsuleStringsEn       = CapsuleStringsEn;
  MspData->DumpRegisterNamesEn    = DumpRegisterNamesEn;
  MspData->ExecuteOnThisChannel   = ExecuteOnThisChannel;
  //
  // Init Data (PFCT)
  //
  MspData->ChannelEnabled         = ChannelEnabled;
  MspData->CurrentPlatform        = CurrentPlatform;
  MspData->CurrentFrequency       = CurrentFrequency;
  MspData->CurrentConfiguration   = CurrentConfiguration;
  MspData->CurrentTechnology      = CurrentTechnology;
  //
  // InitTask and InitData
  // Different Arrays
  //
  MspData->InitTask               = InitTask;
  MspData->InitData               = InitData;

  MspData->SubtaskOffsetIndex     = SubtaskOffsetIndex;
  MspData->MaxSubTasks            = MaxSubTasks;

  MspData->DynamicVars            = DynamicVars;
  MspData->MaxDynamicVarsX        = MaxDynamicVarsX;

  MspData->InstancePortMap        = InstancePortMap;
  MspData->StrobeToRegister       = StrobeToRegister;
  MspData->FloorPlan              = FloorPlan;
  MspData->Registers              = Registers;
  MspData->RankToRegister         = RankToRegister;
  MspData->ChannelToInstanceMap   = ChannelToInstanceMap;
  MspData->InstancePortOffset     = InstancePortOffset;
  MspData->DynamicPtr             = DynamicPtr;
  //
  // Verify the data populated is correct
  //
  //
  // Commented out to save binary space. Routine left to enable future debug
  // VerifyMspData (MspData);
  //

}
