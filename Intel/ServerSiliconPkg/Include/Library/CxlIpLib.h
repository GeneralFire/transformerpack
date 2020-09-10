/** @file

  This is the library class header file for CXL IP lib.

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 - 2020 Intel Corporation. <BR>

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

#ifndef __CXL_IP_LIB_H__
#define __CXL_IP_LIB_H__

#include <Uefi.h>
#include <Base.h>
#include <Upi/KtiSi.h>
#include <Library/UsraAccessApi.h>
#include <IndustryStandard/Pci.h>
#include <IndustryStandard/Cxl.h>

#define  CAPABILITY_SUPPORT    1

typedef enum {
  CxlOperationRead = 0,
  CxlOperationWrite,
  CxlOperationMax
} CXL_OPERATION_TYPE;

typedef enum {
  ZERO_HDM_RANGE   = 0,         // 0 HDM range
  ONE_HDM_RANGE,                // 1 HDM range
  TWO_HDM_RANGE                 // 2 HDM range
} HDM_RANGES_NUMBER;

typedef UINT8  CXL_PROTOCOL_LIST;
#define CXL_PROTOCOL_CACHE_TYPE    BIT0
#define CXL_PROTOCOL_MEM_TYPE      BIT1

typedef struct {
  UINTN             CxlDevPcieAddress;    // CXL device PCI configure space address
  UINT32            CxlPortRcrbBar;       // CXL DP RCRB BAR Address (mmiol, CXL UP RCRBBAR = CxlRcrbBar + 4K)
} CXL_ACCESS_INFO;

/**
  Routine to get the "Secondary/Subordinate Bus" from CXL Downstream Port RCRB register.

  @param[in]  SocId     - Socket ID
  @param[in]  CtrId     - Controller ID
  @param[in]  RegOff    - The register offset

  @retval               - Bus number or 0 if failed.
**/
UINT8
EFIAPI
GetCxlDownstreamBus (
  IN  UINT8        SocId,
  IN  UINT8        CtrId,
  IN  UINT16       RegOff
  );

/**
  Routine to set the "Secondary/Subordinate Bus" registers for CXL Downstream port.

  @param[in]  RcrbBaseAddr           CXL DP RCRB base address
  @param[in]  RegOff                 The register offset
  @param[in]  Bus                    The bus number to be programmed to the register

  @retval     EFI_SUCCESS            - Function return successfully.
              EFI_INVALID_PARAMETER  - Input parameter is invalid.
**/
EFI_STATUS
EFIAPI
SetCxlDownstreamBus (
  IN  UINTN      RcrbBaseAddr,
  IN  UINT16     RegOff,
  IN  UINT8      Bus
  );

/**
  This function obtains the extended PCI configuration space register offset for a
  specified Extended Capability for the specified PCI device.

  @param[in] Socket               Device's socket number.
  @param[in] Bus                  Device's bus number.
  @param[in] Device               Device's device number.
  @param[in] Function             Device's function number.
  @param[in] ExtCapabilityId      ID of the desired Extended Capability.
  @param[in] VendoreSpecificId    vendor ID that is at offset ExtCapabilityoffset + 4.

  @retval    Value                0         PCIe extended capability ID not found
                                  non-zero  Extended PCI configuration space offset of the specified Ext Capability block
**/
UINT16
EFIAPI
CxlDeviceGetExtCapOffset (
  IN      UINT8   Socket,
  IN      UINT8   Bus,
  IN      UINT8   Device,
  IN      UINT8   Function,
  IN      UINT16  ExtCapabilityId,
  IN      UINT16  VendorSpecificId,
  IN      UINT16  IntelVendorSpecificId
  );

/**
  This function will access(Read or Write) the DVSEC registers of a specified CXL Device.
  Note: The caller need to allocate a data buffer and provide the pointer to the data buffer.

  @param[in]     Socket                Device's socket number.
  @param[in]     Stack                 Box Instane, 0 based.
  @param[in]     DvsecRegOff           Offset relative to the beginning of DVSEC structure.
  @param[in]     UsraDataWidth         USRA data access width, please refer to the enum USRA_ACCESS_WIDTH.
  @param[in]     OperationType         Operation type, please refer to the enum CXL_OPERATION_TYPE.
  @param[in out] Buffer                The pointer to the data buffer.

  @retval        EFI_SUCCESS           The access is successful. If read, the data is returned in Buffer.
  @retval        EFI_INVALID_PARAMETER One of the input parameter is invalid or out of range.
  @retval        EFI_NOT_FOUND         Can't find DVSEC structure in CXL device configuration space
  @retval        EFI_UNSUPPORTED       Unsupported Operation type.

**/
EFI_STATUS
EFIAPI
CxlDeviceDvsecRegisterAccess (
  IN       UINT8              Socket,
  IN       UINT8              Stack,
  IN       UINT16             DvsecRegOff,
  IN       USRA_ACCESS_WIDTH  UsraDataWidth,
  IN       CXL_OPERATION_TYPE OperationType,
  IN  OUT  VOID               *Buffer
  );

/**
  This function will do bit field access to the DVSEC registers of a specified CXL Device.
  Note: This function only support 16 bit register.

  @param[in]     Socket                Device's socket number.
  @param[in]     Stack                 Box Instane, 0 based.
  @param[in]     DvsecRegOff           Offset relative to the beginning of DVSEC structure.
  @param[in]     AndData               The value to AND with the read value from the MMIO register.
  @param[in]     OrData                The value to OR with the result of the AND operation.

  @retval        EFI_SUCCESS           The access is successful.
  @retval        EFI_INVALID_PARAMETER One of the input parameter is invalid or out of range.
  @retval        EFI_NOT_FOUND         Can't find DVSEC structure in CXL device configuration space.

**/
EFI_STATUS
EFIAPI
CxlDeviceDvsecRegisterAndThenOr16 (
  IN       UINT8              Socket,
  IN       UINT8              Stack,
  IN       UINT16             DvsecRegOff,
  IN       UINT16             AndData,
  IN       UINT16             OrData
  );

/**
  This function obtains the RCRB offset for a specified Extended Capability
  for a specified CXL Port(Downstream port or Upstream port).

  @param[in] Socket               CXL's socket number.
  @param[in] Stack                Box Instane, 0 based.
  @param[in] UpstreamPort         FALSE - Downstram port; TRUE - Upstream port.
  @param[in] ExtCapabilityId      ID of the desired Extended Capability.
  @param[in] VendoreSpecificId    Vendor ID that is at offset ExtCapabilityoffset + 4.

  @retval    Value                0         Not found the desired Extended Capability structure
                                  non-zero  The offset of the desired Extended Capability structure
**/
UINT16
EFIAPI
CxlPortGetExtCapOffset (
  IN      UINT8   Socket,
  IN      UINT8   Stack,
  IN      BOOLEAN UpstreamPort,
  IN      UINT16  ExtCapabilityId,
  IN      UINT16  VendorSpecificId,
  IN      UINT16  IntelVendorSpecificId
  );

/**
  This function will access(Read or Write) the DVSEC registers of a specifid CXL DP or UP port.
  Note:
  1) The caller need to allocate a data buffer and provide the pointer to the data buffer.
  2) This function only support 16 bit register read/write.

  @param[in]     Socket                CXL's socket number.
  @param[in]     Stack                 Box Instane, 0 based.
  @param[in]     UpstreamPort          FALSE - Downstram port; TRUE - Upstream port.
  @param[in]     DvsecRegOff           Offset relative to the beginning of DVSEC structure.
  @param[in]     OperationType         Operation type, please refer to the enum CXL_OPERATION_TYPE.
  @param[in out] Buffer                The pointer to the data buffer.

  @retval        EFI_SUCCESS           The access is successful. If read, the data is returned in Buffer.
  @retval        EFI_INVALID_PARAMETER The access is failed.
  @retval        EFI_NOT_FOUND         Can't find DVSEC structure in Flex Bus port RCRB.
  @retval        EFI_UNSUPPORTED       Unsupported operation type.

**/
EFI_STATUS
EFIAPI
CxlPortDvsecRegisterAccess16 (
  IN       UINT8              Socket,
  IN       UINT8              Stack,
  IN       BOOLEAN            UpstreamPort,
  IN       UINT16             DvsecRegOff,
  IN       CXL_OPERATION_TYPE OperationType,
  IN  OUT  UINT16             *Buffer
  );

/**
  This function will do bit field access to the DVSEC registers of a specifid CXL DP or UP port.
  Note: This function only support 16 bit register .

  @param[in]     Socket                CXL's socket number.
  @param[in]     Stack                 Box Instane, 0 based.
  @param[in]     UpstreamPort          FALSE - Downstram port; TRUE - Upstream port.
  @param[in]     DvsecRegOff           Offset relative to the beginning of DVSEC structure.
  @param[in]     AndData               The value to AND with the read value from the MMIO register.
  @param[in]     OrData                The value to OR with the result of the AND operation.

  @retval        EFI_SUCCESS           The access is successful.
  @retval        EFI_INVALID_PARAMETER One of the input parameter is invalid.
  @retval        EFI_NOT_FOUND         Can't find DVSEC structure in Flex Bus port RCRB.

**/
EFI_STATUS
EFIAPI
CxlPortDvsecRegisterAndThenOr16 (
  IN       UINT8              Socket,
  IN       UINT8              Stack,
  IN       BOOLEAN            UpstreamPort,
  IN       UINT16             DvsecRegOff,
  IN       UINT16             AndData,
  IN       UINT16             OrData
  );

/**
  Get the CXL endpoint device's VID information

  @param SocId                - CPU Socket Node number (Socket ID)
  @param Stack                - Box Instance, 0 based

  @retval Data                - CXL endpoint device's VID or 0xFFFF which means no CXL devices

**/
UINT16
EFIAPI
GetCxlDevVid (
  UINT8    Socket,
  UINT8    Stack
  );

/**
  Get the CXL endpoint device's Serial number information

  @param SocId                - CPU Socket Node number (Socket ID)
  @param Stack                - Box Instance, 0 based

  @retval Data                - CXL endpoint device's Serial Number info or 0 which means no such info

**/
UINT64
EFIAPI
GetCxlDevSerialNum (
  UINT8    Socket,
  UINT8    Stack
  );

/**
  Get the CXL Ports and Device's access addresses

  @param SocId                   - CPU Socket Node number (Socket ID)
  @param Stack                   - Box Instance, 0 based
  @param CxlProtocolList         - The CXL protocol list which is going to check, [0:0] - 1: indicate to check the cxl.$ protocol, 0: don't care cxl.$
                                                                                  [1:1] - 1: indicate to check the cxl.mem protocol, 0: don't care cxl.mem
                                   If the field is 0, means there is no need to check the protocol supported. Then it will return the CxlAccessInfo if the
                                   stack works as CXL.
  @param CxlAccessInfo           - CXL Access output structure (CXL DP RCRB Base & CXL device PCI address)

  @retval EFI_SUCCESS            - The CXL Access info is successfully returned in the output data structure.
  @retval EFI_UNSUPPORTED        - The CXL doesn't not support the input protocols.
  @retval EFI_INVALID_PARAMETER  - One of the input parameter is invalid.
  @retval EFI_NOT_FOUND          - Failed to read the CXL device's Capability register.

**/
EFI_STATUS
EFIAPI
GetCxlAccessInfo (
  IN UINT8              Socket,
  IN UINT8              Stack,
  IN CXL_PROTOCOL_LIST  CxlProtocolList,
  OUT CXL_ACCESS_INFO   *CxlAccessInfo
  );

/**
  Routine to configure CXL security policy.

  @param[in]  Socket             - Socket number.
  @param[in]  Stack              - Box Instane, 0 based.
  @param[in]  SecurityLevel      - The security level to be set to register.

  @retval     EFI_SUCCESS            - Successfully configured security policy.
  @retval     EFI_INVALID_PARAMETER  - Failed to configure security policy.
**/
EFI_STATUS
EFIAPI
CxlConfigureSecurityPolicy (
  IN UINT8                      Socket,
  IN UINT8                      Stack,
  IN UINT8                      SecurityLevel
  );

/**
  CXL:  Set SNC base address

  @param SocId        - CPU Socket Node number (Socket ID)
  @param Stack        - Stack Id, 0 based
  @param ClusterIdx   - The cluster index
  @param ClMemBase    - The cluster memory base address

  @retval             - EFI_UNSUPPORTED: the function not implemented
                        EFI_INVALID_PARAMETER   A required parameter was NULL or is out of range.
                        EFI_SUCCESS: the function is excuted successfully

**/

EFI_STATUS
EFIAPI
CxlSetSncBaseAddr (
  IN UINT8  Socket,
  IN UINT8  Stack,
  IN UINT8  ClusterIdx,
  IN UINT32 ClMemBase
  );

/**
  CXL:  Set SNC upper base configuration

  @param SocId         - CPU Socket Node number (Socket ID)
  @param Stack         - Stack Id, 0 based
  @param SncUpperBase  - SNC upper base configuration

  @retval              - EFI_UNSUPPORTED: the function not implemented
                         EFI_SUCCESS: the function is excuted successfully

**/

EFI_STATUS
EFIAPI
CxlSetSncUpperBase (
  IN UINT8  Socket,
  IN UINT8  Stack,
  IN UINT32 SncUpperBase
  );

/**
  CXL:  Set Snc CFG

  @param SocId        - CPU Socket Node number (Socket ID)
  @param Stack        - Stack Id, 0 based
  @param SncConfig    - SNC configuration value

  @retval             - EFI_UNSUPPORTED: the function not implemented
                        EFI_SUCCESS: the function is excuted successfully

**/

EFI_STATUS
EFIAPI
CxlSetSncCfg (
  IN UINT8  Socket,
  IN UINT8  Stack,
  IN UINT32 SncConfig
  );

/**
  CXL:  Set UMA cluster configuration

  @param SocId          - CPU Socket Node number (Socket ID)
  @param Stack          - Stack Id, 0 based
  @param UmaClusterEn   - UMA clustering enable
  @param XorDefeature   - Enable for XOR defeature mode

  @retval               - EFI_UNSUPPORTED: the function not implemented
                          EFI_SUCCESS: the function is excuted successfully

**/

EFI_STATUS
EFIAPI
CxlSetUmaClusterCfg (
  IN UINT8  Socket,
  IN UINT8  Stack,
  IN UINT8  UmaClusterEn,
  IN UINT8  XorDefeatureg
  );

/**
  CXL:  Set Snc uncore configuration

  @param SocId            - CPU Socket Node number (Socket ID)
  @param Stack            - Stack Id, 0 based
  @param NumChaPerCluster - Number of CHAs per SNC Cluster
  @param BaseChaCluster1  - Base of CHA Cluster 1
  @param BaseChaCluster2  - Base of CHA Cluster 2
  @param BaseChaCluster3  - Base of CHA Cluster 3

  @retval                 - EFI_UNSUPPORTED: the function not implemented
                            EFI_SUCCESS: the function is excuted successfully

**/
EFI_STATUS
EFIAPI
CxlSetSncUncoreCfg (
  IN UINT8  Socket,
  IN UINT8  Stack,
  IN UINT8  NumChaPerCluster,
  IN UINT8  BaseChaCluster1,
  IN UINT8  BaseChaCluster2,
  IN UINT8  BaseChaCluster3
  );

/**
  CXL:  Set hash address mask

  @param SocId            - CPU Socket Node number (Socket ID)
  @param Stack            - Stack Id, 0 based
  @param HashAddrMask     - Hash Address [51:28] Mask
  @param ChaNumber        - Total number of CHA

  @retval                 - EFI_UNSUPPORTED: the function not implemented
                            EFI_SUCCESS: the function is excuted successfully

**/
EFI_STATUS
EFIAPI
CxlSetHashAddrMask (
  IN UINT8  Socket,
  IN UINT8  Stack,
  IN UINT32 HashAddrMask,
  IN UINT32 ChaNumber
  );

/**
  Routine to read, save and clear the error status registers for CXL.$M.

  @param[in]  Socket                 - Socket number.
  @param[in]  Stack                  - Box Instane, 0 based.

  @retval     EFI_SUCCESS            - Successfully collected and cleared error status.
  @retval     EFI_INVALID_PARAMETER  - Return fail due to input invalid parameter.
**/
EFI_STATUS
EFIAPI
CxlCollectAndClearErrors (
  IN UINT8                      Socket,
  IN UINT8                      Stack
  );

/**
  Set CXL DP Arb-Mux Clock Gating Enable bits: Stcge, Sdcge, Dlcge, Ltcge

  @param SocId                   - CPU Socket Node number (Socket ID)
  @param Stack                   - Box Instance, 0 based
  @param Stcge                   - Cxl Arb-Mux Stcge enable or disable
  @param Sdcge                   - Cxl Arb-Mux Sdcge enable or disable
  @param Dlcge                   - Cxl Arb-Mux Dlcge enable or disable
  @param Ltcge                   - Cxl Arb-Mux Ltcge enable or disable

  @retval

**/
VOID
EFIAPI
ConfigureCxlArbMuxCge (
  IN UINT8              Socket,
  IN UINT8              Stack,
  IN UINT8              Stcge,
  IN UINT8              Sdcge,
  IN UINT8              Dlcge,
  IN UINT8              Ltcge
  );

/**
  Set CXL DP Arb-Mux Secured Register Lock

  @param SocId                   - CPU Socket Node number (Socket ID)
  @param Stack                   - Box Instance, 0 based

  @retval

**/
VOID
EFIAPI
SetCxlArbMuxSrl (
  IN UINT8              Socket,
  IN UINT8              Stack
  );

/**
  Routine to program the PCICMD register in CXL DP/UP RCRB so as to enable or disable MMIO space.

  @param[in]  Socket             - Socket number (Socket ID)
  @param[in]  RcrbBaseAddr       - The base address of RCRB region
  @param[in]  EnableMse          - A flag to indicate enable or disable the mse bit. TRUE: Enable; FALSE: Disable.

  @retval     VOID

**/
VOID
EFIAPI
ProgramPciCommandRegister (
  IN  UINT8       Socket,
  IN  UINT32      RcrbBaseAddr,
  IN  BOOLEAN     EnableMse
  );

/**
  Program the Clock Gate Control register of CXL.CM IP.

  @param[in]  Socket             - Socket number (Socket ID)
  @param[in]  Stack              - Stack ID
  @param[in]  ResetPrepSxAutoAck - Input "1" indicates that CXLCM IP will respond back to ResetPrep.Sx
                                   without waiting for vLSM to enter into L2; otherwise input "0".

  @retval     VOID

**/
VOID
EFIAPI
ProgramCxlCmClkGateCtrlReg (
  IN  UINT8         Socket,
  IN  UINT8         Stack,
  IN  UINT8         ResetPrepSxAutoAck
  );

/**
  This routine will configrue the link power management registers.

  @param[in]  Socket             - Socket number (Socket ID)
  @param[in]  Stack              - Stack ID

  @retval     VOID
**/
VOID
EFIAPI
ConfigureLinkPowerManagement (
  IN  UINT8         Socket,
  IN  UINT8         Stack
  );
#endif // __CXL_IP_LIB_H__
