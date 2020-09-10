/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright 2019 Intel Corporation. <BR>

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

#include <Library/BaseMemoryLib.h>
#include <Library/MemPopulationPorLib.h>

// ICX:
//  Bitmap[0]:
//   D0 = C0D0
//   D1 = C0D1
//   D2 = C1D0
//   D3 = C1D1
//   D4 = C2D0
//   D5 = C2D1
//   D6 = C3D0
//   D7 = C3D1
//  Bitmap[1]:
//   D0 = C4D0
//   D1 = C4D1
//   D2 = C5D0
//   D3 = C5D1
//   D4 = C6D0
//   D5 = C6D1
//   D6 = C7D0
//   D7 = C7D1

MPOR_TBL IcxMemPorTable = {
  MPOR_SIGNATURE, // "MPOR"
  MPOR_REVISION,
  1,
  MAX_CH,
  MAX_DIMM,
#if MPOR_BITMAPS_8 > 1
  62,
#else
  0,
#endif // MPOR_BITMAPS_8 > 1
  {
#if MPOR_BITMAPS_8 > 1
//  Flags           DdrBitmap8[0]             DdrBitmap8[1]             DcpmmBitmap8[0] DcpmmBitmap8[1]
    {VALIDATED,     {D4,                      NO_DIMMS},                {NO_DIMMS,      NO_DIMMS}},//0
    {VALIDATED,     {D6,                      NO_DIMMS},                {NO_DIMMS,      NO_DIMMS}},
    {VALIDATED,     {D0,                      NO_DIMMS},                {NO_DIMMS,      NO_DIMMS}},
    {VALIDATED,     {D2,                      NO_DIMMS},                {NO_DIMMS,      NO_DIMMS}},
    {VALIDATED,     {NO_DIMMS,                D4},                      {NO_DIMMS,      NO_DIMMS}},
    {VALIDATED,     {NO_DIMMS,                D6},                      {NO_DIMMS,      NO_DIMMS}},
    {VALIDATED,     {NO_DIMMS,                D0},                      {NO_DIMMS,      NO_DIMMS}},
    {VALIDATED,     {NO_DIMMS,                D2},                      {NO_DIMMS,      NO_DIMMS}},

    {VALIDATED,     {D0,                      D0},                      {NO_DIMMS,      NO_DIMMS}},
    {VALIDATED,     {D4,                      D4},                      {NO_DIMMS,      NO_DIMMS}},
    {VALIDATED,     {D0+D4,                   NO_DIMMS},                {NO_DIMMS,      NO_DIMMS}},//10
    {VALIDATED,     {NO_DIMMS,                D0+D4},                   {NO_DIMMS,      NO_DIMMS}},
    {VALIDATED,     {D0+D6,                   NO_DIMMS},                {NO_DIMMS,      NO_DIMMS}},
    {NOT_VALIDATED, {D2,                      D2},                      {NO_DIMMS,      NO_DIMMS}},
    {NOT_VALIDATED, {D6,                      D6},                      {NO_DIMMS,      NO_DIMMS}},
    {NOT_VALIDATED, {D2+D6,                   NO_DIMMS},                {NO_DIMMS,      NO_DIMMS}},
    {NOT_VALIDATED, {NO_DIMMS,                D2+D6},                   {NO_DIMMS,      NO_DIMMS}},

    {VALIDATED,     {D0+D4,                   D0+D4},                   {NO_DIMMS,      NO_DIMMS}},
    {VALIDATED,     {D0+D6,                   D0+D6},                   {NO_DIMMS,      NO_DIMMS}},
    {NOT_VALIDATED, {D2+D6,                   D2+D6},                   {NO_DIMMS,      NO_DIMMS}},
    {NOT_VALIDATED, {D2+D4,                   D2+D4},                   {NO_DIMMS,      NO_DIMMS}},//20

    {VALIDATED,     {D0+D2+D4,                D0+D2+D4},                {NO_DIMMS,      NO_DIMMS}},
    {VALIDATED,     {D0+D4+D6,                D0+D4+D6},                {NO_DIMMS,      NO_DIMMS}},
    {NOT_VALIDATED, {D0+D2+D6,                D0+D2+D6},                {NO_DIMMS,      NO_DIMMS}},
    {NOT_VALIDATED, {D2+D4+D6,                D2+D4+D6},                {NO_DIMMS,      NO_DIMMS}},

    {VALIDATED,     {D0+D2+D4+D6,             D0+D2+D4+D6},             {NO_DIMMS,      NO_DIMMS}},

    {VALIDATED,     {D0+D1+D2+D3+D4+D5,       D0+D1+D2+D3+D4+D5},       {NO_DIMMS,      NO_DIMMS}},
    {VALIDATED,     {D0+D1+D4+D5+D6+D7,       D0+D1+D4+D5+D6+D7},       {NO_DIMMS,      NO_DIMMS}},
    {NOT_VALIDATED, {D0+D1+D2+D3+D6+D7,       D0+D1+D2+D3+D6+D7},       {NO_DIMMS,      NO_DIMMS}},
    {NOT_VALIDATED, {D2+D3+D4+D5+D6+D7,       D2+D3+D4+D5+D6+D7},       {NO_DIMMS,      NO_DIMMS}},
    {VALIDATED,     {D0+D1+D2+D4+D5+D6,       D0+D1+D2+D4+D5+D6},       {NO_DIMMS,      NO_DIMMS}},//30
    {NOT_VALIDATED, {D0+D1+D2+D4+D6+D7,       D0+D1+D2+D4+D6+D7},       {NO_DIMMS,      NO_DIMMS}},
    {NOT_VALIDATED, {D0+D2+D3+D4+D6+D7,       D0+D2+D3+D4+D6+D7},       {NO_DIMMS,      NO_DIMMS}},
    {NOT_VALIDATED, {D0+D2+D3+D4+D5+D6,       D0+D2+D3+D4+D5+D6},       {NO_DIMMS,      NO_DIMMS}},

    {VALIDATED,     {D0+D1+D2+D3+D4+D5+D6+D7, D0+D1+D2+D3+D4+D5+D6+D7}, {NO_DIMMS,      NO_DIMMS}},

    {VALIDATED,     {D0+D4,                   D0+D4},                   {D2+D6,         D2+D6}},
    {VALIDATED,     {D2+D6,                   D2+D6},                   {D0+D4,         D0+D4}},

    {VALIDATED,     {D0+D2+D4,                D0+D2+D4},                {D6,            NO_DIMMS}},
    {VALIDATED,     {D0+D4+D6,                D0+D4+D6},                {D2,            NO_DIMMS}},
    {VALIDATED,     {D0+D2+D4,                D0+D2+D4},                {NO_DIMMS,      D6}},
    {VALIDATED,     {D0+D4+D6,                D0+D4+D6},                {NO_DIMMS,      D2}},//40
    {NOT_VALIDATED, {D0+D2+D6,                D0+D2+D6},                {D4,            NO_DIMMS}},
    {NOT_VALIDATED, {D2+D4+D6,                D2+D4+D6},                {D0,            NO_DIMMS}},
    {NOT_VALIDATED, {D0+D2+D6,                D0+D2+D6},                {NO_DIMMS,      D4}},
    {NOT_VALIDATED, {D2+D4+D6,                D2+D4+D6},                {NO_DIMMS,      D0}},

    {VALIDATED,     {D0+D2+D4+D6,             D0+D2+D4+D6},             {D1,            NO_DIMMS}},
    {VALIDATED,     {D0+D2+D4+D6,             D0+D2+D4+D6},             {D5,            NO_DIMMS}},
    {VALIDATED,     {D0+D2+D4+D6,             D0+D2+D4+D6},             {NO_DIMMS,      D1}},
    {VALIDATED,     {D0+D2+D4+D6,             D0+D2+D4+D6},             {NO_DIMMS,      D5}},
    {NOT_VALIDATED, {D0+D2+D4+D6,             D0+D2+D4+D6},             {D7,            NO_DIMMS}},
    {NOT_VALIDATED, {D0+D2+D4+D6,             D0+D2+D4+D6},             {D3,            NO_DIMMS}},//50
    {NOT_VALIDATED, {D0+D2+D4+D6,             D0+D2+D4+D6},             {NO_DIMMS,      D3}},
    {NOT_VALIDATED, {D0+D2+D4+D6,             D0+D2+D4+D6},             {NO_DIMMS,      D7}},

    {VALIDATED,     {D0+D2+D4+D6,             D0+D2+D4+D6},             {D1+D5,         D1+D5}},
    {NOT_VALIDATED, {D0+D2+D4+D6,             D0+D2+D4+D6},             {D1+D7,         D1+D7}},
    {NOT_VALIDATED, {D0+D2+D4+D6,             D0+D2+D4+D6},             {D3+D5,         D3+D5}},
    {NOT_VALIDATED, {D0+D2+D4+D6,             D0+D2+D4+D6},             {D3+D7,         D3+D7}},

    {VALIDATED,     {D0+D2+D4+D6,             D0+D2+D4+D6},             {D1+D3+D5+D7,   D1+D3+D5+D7}},

    {VALIDATED,     {D0+D1+D4+D5+D6+D7,       D0+D1+D4+D5+D6+D7},       {D2,            D2}},
    {VALIDATED,     {D0+D1+D2+D3+D4+D5,       D0+D1+D2+D3+D4+D5},       {D6,            D6}},
    {NOT_VALIDATED, {D2+D3+D4+D5+D6+D7,       D2+D3+D4+D5+D6+D7},       {D0,            D0}},//60
    {NOT_VALIDATED, {D0+D1+D2+D3+D6+D7,       D0+D1+D2+D3+D6+D7},       {D4,            D4}},//61
#else
//  Flags           DdrBitmap8[0] DcpmmBitmap8[0]
    {NOT_VALIDATED, {NO_DIMMS},   {NO_DIMMS}}
#endif // MPOR_BITMAPS_8 > 1
  }
};

/**

Get the pointer to the POR table.

  @param Host  - Pointer to sysHost

  @retval      - Pointer to the POR table

**/
MPOR_TBL *
GetPorTablePtr (
  PSYSHOST Host
  )
{
  RcDebugPrint (SDBG_MAX, "GetPorTablePtr - Using ICX Matrix\n");
  return &IcxMemPorTable;
}
