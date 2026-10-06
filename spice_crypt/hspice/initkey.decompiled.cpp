// Decompiled output. Not intended to be compiled

/* Randkeydecipher::InitKey(std::istream&) */

#include <cstdint>
#include <iostream>
#include "hspice.h"

void des__select1(uchar *param_1,uchar *param_2)

{
  byte *pbVar1;
  byte *pbVar2;
  byte bVar3;
  
  pbVar2 = sel_table1;
  do {
    *param_2 = 0;
    bVar3 = (byte)((int)(uint)param_1[*pbVar2 >> 3] >> (*pbVar2 & 7)) & 1;
    *param_2 = bVar3;
    bVar3 = bVar3 | ((byte)((int)(uint)param_1[pbVar2[1] >> 3] >> (pbVar2[1] & 7)) & 1) * '\x02';
    *param_2 = bVar3;
    bVar3 = bVar3 | ((byte)((int)(uint)param_1[pbVar2[2] >> 3] >> (pbVar2[2] & 7)) & 1) * '\x04';
    *param_2 = bVar3;
    bVar3 = (byte)(((int)(uint)param_1[pbVar2[3] >> 3] >> (pbVar2[3] & 7) & 1U) << 3) | bVar3;
    *param_2 = bVar3;
    bVar3 = bVar3 | (byte)(((int)(uint)param_1[pbVar2[4] >> 3] >> (pbVar2[4] & 7) & 1U) << 4);
    *param_2 = bVar3;
    bVar3 = bVar3 | (byte)(((int)(uint)param_1[pbVar2[5] >> 3] >> (pbVar2[5] & 7) & 1U) << 5);
    *param_2 = bVar3;
    bVar3 = bVar3 | (byte)(((int)(uint)param_1[pbVar2[6] >> 3] >> (pbVar2[6] & 7) & 1U) << 6);
    *param_2 = bVar3;
    pbVar1 = pbVar2 + 7;
    pbVar2 = pbVar2 + 8;
    *param_2 = (byte)(((int)(uint)param_1[*pbVar1 >> 3] >> (*pbVar1 & 7) & 1U) << 7) | bVar3;
    param_2 = param_2 + 1;
  } while (pbVar2 != &DAT_06176f58);
  return;
}

void des__cyc_move(uchar *param_1,int param_2)

{
  uint uVar1;
  uint uVar2;
  uint uVar3;
  uint uVar4;
  int iVar5;
  uint uVar6;
  uint uVar7;
  
  if (0 < param_2) {
    uVar1 = (uint)*param_1;
    uVar2 = (uint)param_1[1];
    iVar5 = 0;
    uVar4 = (uint)param_1[2];
    uVar6 = (uint)param_1[3];
    do {
      uVar3 = uVar2 & 1;
      uVar2 = (uVar1 & 1) << 7 | uVar2 >> 1;
      uVar7 = uVar6 >> 1 | (uVar4 & 1) << 3;
      iVar5 = iVar5 + 1;
      uVar4 = uVar3 << 7 | uVar4 >> 1;
      uVar1 = (uVar6 & 1) << 7 | uVar1 >> 1;
      uVar6 = uVar7;
    } while (iVar5 != param_2);
    *param_1 = (uchar)uVar1;
    param_1[1] = (uchar)uVar2;
    param_1[2] = (uchar)uVar4;
    param_1[3] = (uchar)uVar7;
  }
  return;
}

void des__change1(uchar *param_1,int param_2)

{
  uchar local_38 [4];
  byte local_28 [4];
  
  local_38[0] = *param_1;
  local_38[1] = param_1[1];
  local_38[2] = param_1[2];
  local_38[3] = (uchar)((int)(param_1[3] & 0xf0) >> 4);
  local_28[0] = param_1[3] << 4 | param_1[4] >> 4;
  local_28[1] = param_1[4] << 4 | param_1[5] >> 4;
  local_28[3] = param_1[6] & 0xf;
  local_28[2] = param_1[5] << 4 | param_1[6] >> 4;
  des__cyc_move(local_38,param_2);
  des__cyc_move(local_28,param_2);
  *param_1 = local_38[0];
  param_1[1] = local_38[1];
  param_1[2] = local_38[2];
  param_1[3] = local_28[0] >> 4 | local_38[3] << 4;
  param_1[4] = local_28[0] << 4 | local_28[1] >> 4;
  param_1[5] = local_28[1] << 4 | local_28[2] >> 4;
  param_1[6] = local_28[2] << 4 | local_28[3] & 0xf;
  return;
}

void des__select2(uchar *param_1,uchar *param_2)

{
  byte bVar1;
  byte bVar2;
  byte bVar3;
  
  *param_2 = '\0';
  bVar2 = (byte)((int)(uint)param_1[sel_table2[0] >> 3] >> (sel_table2[0] & 7)) & 1;
  *param_2 = bVar2;
  bVar2 = bVar2 | ((byte)((int)(uint)param_1[sel_table2[1] >> 3] >> (sel_table2[1] & 7)) & 1) *
                  '\x02';
  *param_2 = bVar2;
  bVar2 = bVar2 | (byte)(((int)(uint)param_1[sel_table2[2] >> 3] >> (sel_table2[2] & 7) & 1U) << 2);
  *param_2 = bVar2;
  bVar2 = (byte)(((int)(uint)param_1[sel_table2[3] >> 3] >> (sel_table2[3] & 7) & 1U) << 3) | bVar2;
  *param_2 = bVar2;
  bVar2 = bVar2 | (byte)(((int)(uint)param_1[sel_table2[4] >> 3] >> (sel_table2[4] & 7) & 1U) << 4);
  *param_2 = bVar2;
  bVar2 = bVar2 | (byte)(((int)(uint)param_1[sel_table2[5] >> 3] >> (sel_table2[5] & 7) & 1U) << 5);
  *param_2 = bVar2;
  bVar2 = bVar2 | (byte)(((int)(uint)param_1[sel_table2[6] >> 3] >> (sel_table2[6] & 7) & 1U) << 6);
  *param_2 = bVar2;
  bVar1 = sel_table2[7] & 7;
  bVar3 = param_1[sel_table2[7] >> 3];
  param_2[1] = '\0';
  *param_2 = (byte)(((int)(uint)bVar3 >> bVar1 & 1U) << 7) | bVar2;
  bVar2 = (byte)((int)(uint)param_1[sel_table2[8] >> 3] >> (sel_table2[8] & 7)) & 1;
  param_2[1] = bVar2;
  bVar2 = bVar2 | ((byte)((int)(uint)param_1[sel_table2[9] >> 3] >> (sel_table2[9] & 7)) & 1) *
                  '\x02';
  param_2[1] = bVar2;
  bVar2 = bVar2 | (byte)(((int)(uint)param_1[sel_table2[10] >> 3] >> (sel_table2[10] & 7) & 1U) << 2
                        );
  param_2[1] = bVar2;
  bVar2 = (byte)(((int)(uint)param_1[sel_table2[0xb] >> 3] >> (sel_table2[0xb] & 7) & 1U) << 3) |
          bVar2;
  param_2[1] = bVar2;
  bVar2 = bVar2 | (byte)(((int)(uint)param_1[sel_table2[0xc] >> 3] >> (sel_table2[0xc] & 7) & 1U) <<
                        4);
  param_2[1] = bVar2;
  bVar2 = bVar2 | (byte)(((int)(uint)param_1[sel_table2[0xd] >> 3] >> (sel_table2[0xd] & 7) & 1U) <<
                        5);
  param_2[1] = bVar2;
  bVar2 = bVar2 | (byte)(((int)(uint)param_1[sel_table2[0xe] >> 3] >> (sel_table2[0xe] & 7) & 1U) <<
                        6);
  param_2[1] = bVar2;
  bVar1 = sel_table2[0xf] & 7;
  bVar3 = param_1[sel_table2[0xf] >> 3];
  param_2[2] = '\0';
  param_2[1] = (byte)(((int)(uint)bVar3 >> bVar1 & 1U) << 7) | bVar2;
  bVar2 = (byte)((int)(uint)param_1[sel_table2[0x10] >> 3] >> (sel_table2[0x10] & 7)) & 1;
  param_2[2] = bVar2;
  bVar2 = bVar2 | ((byte)((int)(uint)param_1[sel_table2[0x11] >> 3] >> (sel_table2[0x11] & 7)) & 1)
                  * '\x02';
  param_2[2] = bVar2;
  bVar2 = bVar2 | (byte)(((int)(uint)param_1[sel_table2[0x12] >> 3] >> (sel_table2[0x12] & 7) & 1U)
                        << 2);
  param_2[2] = bVar2;
  bVar2 = (byte)(((int)(uint)param_1[sel_table2[0x13] >> 3] >> (sel_table2[0x13] & 7) & 1U) << 3) |
          bVar2;
  param_2[2] = bVar2;
  bVar2 = bVar2 | (byte)(((int)(uint)param_1[sel_table2[0x14] >> 3] >> (sel_table2[0x14] & 7) & 1U)
                        << 4);
  param_2[2] = bVar2;
  bVar2 = bVar2 | (byte)(((int)(uint)param_1[sel_table2[0x15] >> 3] >> (sel_table2[0x15] & 7) & 1U)
                        << 5);
  param_2[2] = bVar2;
  bVar2 = bVar2 | (byte)(((int)(uint)param_1[sel_table2[0x16] >> 3] >> (sel_table2[0x16] & 7) & 1U)
                        << 6);
  param_2[2] = bVar2;
  bVar1 = sel_table2[0x17] & 7;
  bVar3 = param_1[sel_table2[0x17] >> 3];
  param_2[3] = '\0';
  param_2[2] = (byte)(((int)(uint)bVar3 >> bVar1 & 1U) << 7) | bVar2;
  bVar3 = (byte)((int)(uint)param_1[sel_table2[0x18] >> 3] >> (sel_table2[0x18] & 7)) & 1;
  param_2[3] = bVar3;
  bVar3 = bVar3 | ((byte)((int)(uint)param_1[sel_table2[0x19] >> 3] >> (sel_table2[0x19] & 7)) & 1)
                  * '\x02';
  param_2[3] = bVar3;
  bVar3 = bVar3 | (byte)(((int)(uint)param_1[sel_table2[0x1a] >> 3] >> (sel_table2[0x1a] & 7) & 1U)
                        << 2);
  param_2[3] = bVar3;
  bVar3 = (byte)(((int)(uint)param_1[sel_table2[0x1b] >> 3] >> (sel_table2[0x1b] & 7) & 1U) << 3) |
          bVar3;
  param_2[3] = bVar3;
  bVar3 = bVar3 | (byte)(((int)(uint)param_1[sel_table2[0x1c] >> 3] >> (sel_table2[0x1c] & 7) & 1U)
                        << 4);
  param_2[3] = bVar3;
  bVar3 = bVar3 | (byte)(((int)(uint)param_1[sel_table2[0x1d] >> 3] >> (sel_table2[0x1d] & 7) & 1U)
                        << 5);
  param_2[3] = bVar3;
  bVar3 = bVar3 | (byte)(((int)(uint)param_1[sel_table2[0x1e] >> 3] >> (sel_table2[0x1e] & 7) & 1U)
                        << 6);
  param_2[3] = bVar3;
  param_2[3] = (byte)(((int)(uint)param_1[sel_table2[0x1f] >> 3] >> (sel_table2[0x1f] & 7) & 1U) <<
                     7) | bVar3;
  param_2[4] = '\0';
  bVar2 = (byte)((int)(uint)param_1[sel_table2[0x20] >> 3] >> (sel_table2[0x20] & 7)) & 1;
  param_2[4] = bVar2;
  bVar2 = bVar2 | ((byte)((int)(uint)param_1[sel_table2[0x21] >> 3] >> (sel_table2[0x21] & 7)) & 1)
                  * '\x02';
  param_2[4] = bVar2;
  bVar2 = bVar2 | (byte)(((int)(uint)param_1[sel_table2[0x22] >> 3] >> (sel_table2[0x22] & 7) & 1U)
                        << 2);
  param_2[4] = bVar2;
  bVar2 = (byte)(((int)(uint)param_1[sel_table2[0x23] >> 3] >> (sel_table2[0x23] & 7) & 1U) << 3) |
          bVar2;
  param_2[4] = bVar2;
  bVar2 = bVar2 | (byte)(((int)(uint)param_1[sel_table2[0x24] >> 3] >> (sel_table2[0x24] & 7) & 1U)
                        << 4);
  param_2[4] = bVar2;
  bVar2 = bVar2 | (byte)(((int)(uint)param_1[sel_table2[0x25] >> 3] >> (sel_table2[0x25] & 7) & 1U)
                        << 5);
  param_2[4] = bVar2;
  bVar2 = bVar2 | (byte)(((int)(uint)param_1[sel_table2[0x26] >> 3] >> (sel_table2[0x26] & 7) & 1U)
                        << 6);
  param_2[4] = bVar2;
  bVar1 = sel_table2[0x27] & 7;
  bVar3 = param_1[sel_table2[0x27] >> 3];
  param_2[5] = '\0';
  param_2[4] = (byte)(((int)(uint)bVar3 >> bVar1 & 1U) << 7) | bVar2;
  bVar3 = (byte)((int)(uint)param_1[sel_table2[0x28] >> 3] >> (sel_table2[0x28] & 7)) & 1;
  param_2[5] = bVar3;
  bVar3 = bVar3 | ((byte)((int)(uint)param_1[sel_table2[0x29] >> 3] >> (sel_table2[0x29] & 7)) & 1)
                  * '\x02';
  param_2[5] = bVar3;
  bVar3 = bVar3 | (byte)(((int)(uint)param_1[sel_table2[0x2a] >> 3] >> (sel_table2[0x2a] & 7) & 1U)
                        << 2);
  param_2[5] = bVar3;
  bVar3 = (byte)(((int)(uint)param_1[sel_table2[0x2b] >> 3] >> (sel_table2[0x2b] & 7) & 1U) << 3) |
          bVar3;
  param_2[5] = bVar3;
  bVar3 = bVar3 | (byte)(((int)(uint)param_1[sel_table2[0x2c] >> 3] >> (sel_table2[0x2c] & 7) & 1U)
                        << 4);
  param_2[5] = bVar3;
  bVar3 = bVar3 | (byte)(((int)(uint)param_1[sel_table2[0x2d] >> 3] >> (sel_table2[0x2d] & 7) & 1U)
                        << 5);
  param_2[5] = bVar3;
  bVar3 = (byte)(((int)(uint)param_1[sel_table2[0x2e] >> 3] >> (sel_table2[0x2e] & 7) & 1U) << 6) |
          bVar3;
  param_2[5] = bVar3;
  param_2[5] = bVar3 | (byte)(((int)(uint)param_1[sel_table2[0x2f] >> 3] >> (sel_table2[0x2f] & 7) &
                              1U) << 7);
  return;
}

void init_transform(uchar *param_1)

{
  byte *pbVar1;
  byte *pbVar2;
  byte *pbVar3;
  byte *pbVar4;
  byte *pbVar5;
  byte *pbVar6;
  byte *pbVar7;
  byte bVar8;
  byte *pbVar9;
  byte *pbVar10;
  byte local_18;
  uchar uStack_17;
  uchar uStack_16;
  uchar uStack_15;
  uchar uStack_14;
  uchar uStack_13;
  uchar uStack_12;
  uchar uStack_11;
  
  pbVar9 = init_tran;
  pbVar10 = &local_18;
  _local_18 = 0;
  do {
    bVar8 = *pbVar9;
    pbVar1 = pbVar9 + 1;
    pbVar2 = pbVar9 + 2;
    pbVar3 = pbVar9 + 3;
    pbVar4 = pbVar9 + 4;
    pbVar5 = pbVar9 + 5;
    pbVar6 = pbVar9 + 6;
    pbVar7 = pbVar9 + 7;
    pbVar9 = pbVar9 + 8;
    *pbVar10 = (byte)((int)(uint)param_1[(int)(uint)bVar8 >> 3] >> (bVar8 & 7)) & 1 | *pbVar10 |
               ((byte)((int)(uint)param_1[(int)(uint)*pbVar1 >> 3] >> (*pbVar1 & 7)) & 1) * '\x02' |
               (byte)(((int)(uint)param_1[(int)(uint)*pbVar2 >> 3] >> (*pbVar2 & 7) & 1U) << 2) |
               (byte)(((int)(uint)param_1[(int)(uint)*pbVar3 >> 3] >> (*pbVar3 & 7) & 1U) << 3) |
               (byte)(((int)(uint)param_1[(int)(uint)*pbVar4 >> 3] >> (*pbVar4 & 7) & 1U) << 4) |
               (byte)(((int)(uint)param_1[(int)(uint)*pbVar5 >> 3] >> (*pbVar5 & 7) & 1U) << 5) |
               (byte)(((int)(uint)param_1[(int)(uint)*pbVar6 >> 3] >> (*pbVar6 & 7) & 1U) << 6) |
               (byte)(((int)(uint)param_1[(int)(uint)*pbVar7 >> 3] >> (*pbVar7 & 7) & 1U) << 7);
    pbVar10 = pbVar10 + 1;
  } while (pbVar9 != uninit_tran);
  *param_1 = local_18;
  param_1[1] = uStack_17;
  param_1[2] = uStack_16;
  param_1[3] = uStack_15;
  param_1[4] = uStack_14;
  param_1[5] = uStack_13;
  param_1[6] = uStack_12;
  param_1[7] = uStack_11;
  return;
}

void des__select3(uchar *param_1,uchar *param_2)

{
  byte bVar1;
  byte bVar2;
  byte bVar3;
  
  *param_2 = '\0';
  bVar2 = (byte)((int)(uint)param_1[sel_table3[0] >> 3] >> (sel_table1[0] & 7)) & 1;
  *param_2 = bVar2;
  bVar2 = bVar2 | ((byte)((int)(uint)param_1[sel_table3[1] >> 3] >> (sel_table1[1] & 7)) & 1) *
                  '\x02';
  *param_2 = bVar2;
  bVar2 = bVar2 | (byte)(((int)(uint)param_1[sel_table3[2] >> 3] >> (sel_table1[2] & 7) & 1U) << 2);
  *param_2 = bVar2;
  bVar2 = (byte)(((int)(uint)param_1[sel_table3[3] >> 3] >> (sel_table1[3] & 7) & 1U) << 3) | bVar2;
  *param_2 = bVar2;
  bVar2 = bVar2 | (byte)(((int)(uint)param_1[sel_table3[4] >> 3] >> (sel_table1[4] & 7) & 1U) << 4);
  *param_2 = bVar2;
  bVar2 = bVar2 | (byte)(((int)(uint)param_1[sel_table3[5] >> 3] >> (sel_table1[5] & 7) & 1U) << 5);
  *param_2 = bVar2;
  bVar2 = bVar2 | (byte)(((int)(uint)param_1[sel_table3[6] >> 3] >> (sel_table1[6] & 7) & 1U) << 6);
  *param_2 = bVar2;
  *param_2 = (byte)(((int)(uint)param_1[sel_table3[7] >> 3] >> (sel_table1[7] & 7) & 1U) << 7) |
             bVar2;
  param_2[1] = '\0';
  bVar2 = (byte)((int)(uint)param_1[sel_table3[8] >> 3] >> (sel_table1[8] & 7)) & 1;
  param_2[1] = bVar2;
  bVar2 = bVar2 | ((byte)((int)(uint)param_1[sel_table3[9] >> 3] >> (sel_table1[9] & 7)) & 1) *
                  '\x02';
  param_2[1] = bVar2;
  bVar2 = bVar2 | (byte)(((int)(uint)param_1[sel_table3[10] >> 3] >> (sel_table1[10] & 7) & 1U) << 2
                        );
  param_2[1] = bVar2;
  bVar2 = (byte)(((int)(uint)param_1[sel_table3[0xb] >> 3] >> (sel_table1[0xb] & 7) & 1U) << 3) |
          bVar2;
  param_2[1] = bVar2;
  bVar2 = bVar2 | (byte)(((int)(uint)param_1[sel_table3[0xc] >> 3] >> (sel_table1[0xc] & 7) & 1U) <<
                        4);
  param_2[1] = bVar2;
  bVar2 = bVar2 | (byte)(((int)(uint)param_1[sel_table3[0xd] >> 3] >> (sel_table1[0xd] & 7) & 1U) <<
                        5);
  param_2[1] = bVar2;
  bVar2 = bVar2 | (byte)(((int)(uint)param_1[sel_table3[0xe] >> 3] >> (sel_table1[0xe] & 7) & 1U) <<
                        6);
  param_2[1] = bVar2;
  param_2[1] = (byte)(((int)(uint)param_1[sel_table3[0xf] >> 3] >> (sel_table1[0xf] & 7) & 1U) << 7)
               | bVar2;
  param_2[2] = '\0';
  bVar3 = (byte)((int)(uint)param_1[sel_table3[0x10] >> 3] >> (sel_table1[0x10] & 7)) & 1;
  param_2[2] = bVar3;
  bVar3 = bVar3 | ((byte)((int)(uint)param_1[sel_table3[0x11] >> 3] >> (sel_table1[0x11] & 7)) & 1)
                  * '\x02';
  param_2[2] = bVar3;
  bVar3 = bVar3 | (byte)(((int)(uint)param_1[sel_table3[0x12] >> 3] >> (sel_table1[0x12] & 7) & 1U)
                        << 2);
  param_2[2] = bVar3;
  bVar3 = (byte)(((int)(uint)param_1[sel_table3[0x13] >> 3] >> (sel_table1[0x13] & 7) & 1U) << 3) |
          bVar3;
  param_2[2] = bVar3;
  bVar3 = bVar3 | (byte)(((int)(uint)param_1[sel_table3[0x14] >> 3] >> (sel_table1[0x14] & 7) & 1U)
                        << 4);
  param_2[2] = bVar3;
  bVar3 = bVar3 | (byte)(((int)(uint)param_1[sel_table3[0x15] >> 3] >> (sel_table1[0x15] & 7) & 1U)
                        << 5);
  param_2[2] = bVar3;
  bVar3 = bVar3 | (byte)(((int)(uint)param_1[sel_table3[0x16] >> 3] >> (sel_table1[0x16] & 7) & 1U)
                        << 6);
  param_2[2] = bVar3;
  bVar1 = sel_table1[0x17];
  bVar2 = param_1[sel_table3[0x17] >> 3];
  param_2[3] = '\0';
  param_2[2] = (byte)(((int)(uint)bVar2 >> (bVar1 & 7) & 1U) << 7) | bVar3;
  bVar3 = (byte)((int)(uint)param_1[sel_table3[0x18] >> 3] >> (sel_table1[0x18] & 7)) & 1;
  param_2[3] = bVar3;
  bVar3 = bVar3 | ((byte)((int)(uint)param_1[sel_table3[0x19] >> 3] >> (sel_table1[0x19] & 7)) & 1)
                  * '\x02';
  param_2[3] = bVar3;
  bVar3 = bVar3 | (byte)(((int)(uint)param_1[sel_table3[0x1a] >> 3] >> (sel_table1[0x1a] & 7) & 1U)
                        << 2);
  param_2[3] = bVar3;
  bVar3 = (byte)(((int)(uint)param_1[sel_table3[0x1b] >> 3] >> (sel_table1[0x1b] & 7) & 1U) << 3) |
          bVar3;
  param_2[3] = bVar3;
  bVar3 = bVar3 | (byte)(((int)(uint)param_1[sel_table3[0x1c] >> 3] >> (sel_table1[0x1c] & 7) & 1U)
                        << 4);
  param_2[3] = bVar3;
  bVar3 = bVar3 | (byte)(((int)(uint)param_1[sel_table3[0x1d] >> 3] >> (sel_table1[0x1d] & 7) & 1U)
                        << 5);
  param_2[3] = bVar3;
  bVar3 = bVar3 | (byte)(((int)(uint)param_1[sel_table3[0x1e] >> 3] >> (sel_table1[0x1e] & 7) & 1U)
                        << 6);
  param_2[3] = bVar3;
  bVar1 = sel_table1[0x1f];
  bVar2 = param_1[sel_table3[0x1f] >> 3];
  param_2[4] = '\0';
  param_2[3] = (byte)(((int)(uint)bVar2 >> (bVar1 & 7) & 1U) << 7) | bVar3;
  bVar3 = (byte)((int)(uint)param_1[sel_table3[0x20] >> 3] >> (sel_table1[0x20] & 7)) & 1;
  param_2[4] = bVar3;
  bVar3 = bVar3 | ((byte)((int)(uint)param_1[sel_table3[0x21] >> 3] >> (sel_table1[0x21] & 7)) & 1)
                  * '\x02';
  param_2[4] = bVar3;
  bVar3 = bVar3 | (byte)(((int)(uint)param_1[sel_table3[0x22] >> 3] >> (sel_table1[0x22] & 7) & 1U)
                        << 2);
  param_2[4] = bVar3;
  bVar3 = (byte)(((int)(uint)param_1[sel_table3[0x23] >> 3] >> (sel_table1[0x23] & 7) & 1U) << 3) |
          bVar3;
  param_2[4] = bVar3;
  bVar3 = bVar3 | (byte)(((int)(uint)param_1[sel_table3[0x24] >> 3] >> (sel_table1[0x24] & 7) & 1U)
                        << 4);
  param_2[4] = bVar3;
  bVar3 = bVar3 | (byte)(((int)(uint)param_1[sel_table3[0x25] >> 3] >> (sel_table1[0x25] & 7) & 1U)
                        << 5);
  param_2[4] = bVar3;
  bVar3 = bVar3 | (byte)(((int)(uint)param_1[sel_table3[0x26] >> 3] >> (sel_table1[0x26] & 7) & 1U)
                        << 6);
  param_2[4] = bVar3;
  bVar1 = sel_table1[0x27];
  bVar2 = param_1[sel_table3[0x27] >> 3];
  param_2[5] = '\0';
  param_2[4] = (byte)(((int)(uint)bVar2 >> (bVar1 & 7) & 1U) << 7) | bVar3;
  bVar2 = (byte)((int)(uint)param_1[sel_table3[0x28] >> 3] >> (sel_table1[0x28] & 7)) & 1;
  param_2[5] = bVar2;
  bVar2 = bVar2 | ((byte)((int)(uint)param_1[sel_table3[0x29] >> 3] >> (sel_table1[0x29] & 7)) & 1)
                  * '\x02';
  param_2[5] = bVar2;
  bVar2 = bVar2 | (byte)(((int)(uint)param_1[sel_table3[0x2a] >> 3] >> (sel_table1[0x2a] & 7) & 1U)
                        << 2);
  param_2[5] = bVar2;
  bVar2 = (byte)(((int)(uint)param_1[sel_table3[0x2b] >> 3] >> (sel_table1[0x2b] & 7) & 1U) << 3) |
          bVar2;
  param_2[5] = bVar2;
  bVar2 = bVar2 | (byte)(((int)(uint)param_1[sel_table3[0x2c] >> 3] >> (sel_table1[0x2c] & 7) & 1U)
                        << 4);
  param_2[5] = bVar2;
  bVar2 = bVar2 | (byte)(((int)(uint)param_1[sel_table3[0x2d] >> 3] >> (sel_table1[0x2d] & 7) & 1U)
                        << 5);
  param_2[5] = bVar2;
  bVar2 = (byte)(((int)(uint)param_1[sel_table3[0x2e] >> 3] >> (sel_table1[0x2e] & 7) & 1U) << 6) |
          bVar2;
  param_2[5] = bVar2;
  param_2[5] = bVar2 | (byte)(((int)(uint)param_1[sel_table3[0x2f] >> 3] >> (sel_table1[0x2f] & 7) &
                              1U) << 7);
  return;
}

void des__select4(uchar *param_1,uchar *param_2)

{
  uint uVar1;
  sbyte sVar2;
  int iVar3;
  byte bVar4;
  uint uVar5;
  int iVar6;
  int iVar7;
  uint uVar8;
  
  *param_2 = '\0';
  param_2[1] = '\0';
  param_2[2] = '\0';
  uVar1 = 0;
  param_2[3] = '\0';
  bVar4 = *param_1;
  iVar7 = 1;
  iVar3 = 0;
  uVar8 = bVar4 & 1;
  do {
    uVar5 = (uint)bVar4;
    uVar8 = uVar8 | ((int)(uint)bVar4 >> ((byte)iVar7 & 0x1f) & 1U) * 2;
    if (iVar7 + 1 == 8) {
      iVar3 = iVar3 + 1;
      iVar6 = 2;
      uVar5 = (uint)param_1[iVar3];
      uVar8 = uVar8 | (uVar5 & 1) << 2 | uVar5 * 4 & 8;
LAB_00cf6903:
      iVar7 = iVar6 + 1;
      uVar8 = uVar8 | ((int)uVar5 >> ((byte)iVar6 & 0x1f) & 1U) << 4;
      if (iVar7 == 8) {
        iVar3 = iVar3 + 1;
        iVar6 = 1;
        uVar5 = (uint)param_1[iVar3];
        sVar2 = 0;
        goto LAB_00cf681f;
      }
LAB_00cf696b:
      iVar6 = iVar7 + 1;
      uVar8 = ((int)uVar5 >> ((byte)iVar7 & 0x1f) & 1U) << 5 | uVar8;
      if (iVar6 == 8) {
        iVar3 = iVar3 + 1;
        iVar6 = 0;
      }
    }
    else {
      uVar8 = uVar8 | ((int)(uint)bVar4 >> ((byte)(iVar7 + 1) & 0x1f) & 1U) << 2;
      if (iVar7 + 2 == 8) {
        iVar3 = iVar3 + 1;
        iVar7 = 2;
        uVar5 = (uint)param_1[iVar3];
        uVar8 = uVar8 | (uVar5 & 1) << 3 | uVar5 * 8 & 0x10;
        goto LAB_00cf696b;
      }
      iVar6 = iVar7 + 3;
      uVar8 = uVar8 | ((int)uVar5 >> ((byte)(iVar7 + 2) & 0x1f) & 1U) << 3;
      if (iVar6 != 8) goto LAB_00cf6903;
      iVar3 = iVar3 + 1;
      iVar6 = 2;
      uVar5 = (uint)param_1[iVar3];
      uVar8 = uVar8 | (uVar5 & 1) << 4;
      sVar2 = 1;
LAB_00cf681f:
      uVar8 = ((int)uVar5 >> sVar2 & 1U) << 5 | uVar8;
    }
    if ((uVar1 & 1) == 0) {
      param_2[(int)uVar1 >> 1] =
           param_2[(int)uVar1 >> 1] |
           sel_table4
           [((long)(int)((int)(uVar8 & 0x20) >> 4 | uVar8 & 1) + (long)(int)uVar1 * 4) * 0x10 +
            (ulong)(uVar8 >> 1 & 0xf)] << 4;
    }
    else {
      param_2[(int)uVar1 >> 1] =
           param_2[(int)uVar1 >> 1] |
           sel_table4
           [((long)(int)((int)(uVar8 & 0x20) >> 4 | uVar8 & 1) + (long)(int)uVar1 * 4) * 0x10 +
            (ulong)(uVar8 >> 1 & 0xf)];
    }
    if (uVar1 == 7) {
      return;
    }
    uVar1 = uVar1 + 1;
    bVar4 = param_1[iVar3];
    iVar7 = iVar6 + 1;
    uVar8 = (int)(uint)bVar4 >> ((byte)iVar6 & 0x1f) & 1;
    if (iVar7 == 8) {
      iVar3 = iVar3 + 1;
      iVar7 = 0;
      bVar4 = param_1[iVar3];
    }
  } while( true );
}

void uninit_transform(uchar *param_1)

{
  byte *pbVar1;
  byte *pbVar2;
  byte *pbVar3;
  byte *pbVar4;
  byte *pbVar5;
  byte *pbVar6;
  byte *pbVar7;
  byte bVar8;
  byte *pbVar9;
  byte *pbVar10;
  byte local_18 [8];
  
  pbVar9 = uninit_tran;
  pbVar10 = local_18;
  local_18[0] = 0;
  local_18[1] = 0;
  local_18[2] = 0;
  local_18[3] = 0;
  local_18[4] = 0;
  local_18[5] = 0;
  local_18[6] = 0;
  local_18[7] = 0;
  do {
    bVar8 = *pbVar9;
    pbVar1 = pbVar9 + 1;
    pbVar2 = pbVar9 + 2;
    pbVar3 = pbVar9 + 3;
    pbVar4 = pbVar9 + 4;
    pbVar5 = pbVar9 + 5;
    pbVar6 = pbVar9 + 6;
    pbVar7 = pbVar9 + 7;
    pbVar9 = pbVar9 + 8;
    *pbVar10 = (byte)((int)(uint)param_1[(int)(uint)bVar8 >> 3] >> (bVar8 & 7)) & 1 | *pbVar10 |
               ((byte)((int)(uint)param_1[(int)(uint)*pbVar1 >> 3] >> (*pbVar1 & 7)) & 1) * '\x02' |
               (byte)(((int)(uint)param_1[(int)(uint)*pbVar2 >> 3] >> (*pbVar2 & 7) & 1U) << 2) |
               (byte)(((int)(uint)param_1[(int)(uint)*pbVar3 >> 3] >> (*pbVar3 & 7) & 1U) << 3) |
               (byte)(((int)(uint)param_1[(int)(uint)*pbVar4 >> 3] >> (*pbVar4 & 7) & 1U) << 4) |
               (byte)(((int)(uint)param_1[(int)(uint)*pbVar5 >> 3] >> (*pbVar5 & 7) & 1U) << 5) |
               (byte)(((int)(uint)param_1[(int)(uint)*pbVar6 >> 3] >> (*pbVar6 & 7) & 1U) << 6) |
               (byte)(((int)(uint)param_1[(int)(uint)*pbVar7 >> 3] >> (*pbVar7 & 7) & 1U) << 7);
    pbVar10 = pbVar10 + 1;
  } while (pbVar9 != cyc);
  *param_1 = local_18[0];
  param_1[1] = local_18[1];
  param_1[2] = local_18[2];
  param_1[3] = local_18[3];
  param_1[4] = local_18[4];
  param_1[5] = local_18[5];
  param_1[6] = local_18[6];
  param_1[7] = local_18[7];
  return;
}

void des__mod2add(uchar *param_1,uchar *param_2,int param_3)

{
  byte bVar1;
  ulong uVar2;
  ulong uVar3;
  ulong *puVar4;
  uint uVar5;
  byte *pbVar6;
  byte *pbVar7;
  int iVar8;
  ulong *puVar9;
  uint uVar10;
  
  if (0 < param_3) {
    uVar10 = (uint)param_3 >> 4;
    iVar8 = uVar10 << 4;
    if ((uVar10 == 0) ||
       (param_1 <= param_2 + 0x10 && param_2 <= param_1 + 0x10 || (uint)param_3 < 0x10)) {
      iVar8 = 0;
    }
    else {
      uVar5 = 0;
      puVar4 = (ulong *)param_1;
      puVar9 = (ulong *)param_2;
      do {
        uVar2 = *puVar9;
        uVar3 = puVar9[1];
        uVar5 = uVar5 + 1;
        puVar9 = puVar9 + 2;
        *puVar4 = ~uVar2 & *puVar4 | ~*puVar4 & uVar2;
        puVar4[1] = ~uVar3 & puVar4[1] | ~puVar4[1] & uVar3;
        puVar4 = puVar4 + 2;
      } while (uVar5 < uVar10);
      if (param_3 == iVar8) {
        return;
      }
    }
    pbVar7 = param_1 + iVar8;
    pbVar6 = param_2 + iVar8;
    do {
      bVar1 = *pbVar6;
      iVar8 = iVar8 + 1;
      pbVar6 = pbVar6 + 1;
      *pbVar7 = ~bVar1 & *pbVar7 | ~*pbVar7 & bVar1;
      pbVar7 = pbVar7 + 1;
    } while (iVar8 < param_3);
  }
  return;
}


void des__desdecode(des *self,uchar *param_1)

{
  uchar uVar1;
  uchar uVar2;
  uchar uVar3;
  uchar uVar4;
  uchar uVar5;
  uchar uVar6;
  uchar uVar7;
  uchar uVar8;
  uchar auStack_68 [16];
  uchar local_58 [4];
  uchar local_48 [4];
  uchar dVar2;
  uchar dVar1;
  
  init_transform(self->key);
  local_58[3] = self->key[4];
  dVar1 = self->key[0];
  local_58[2] = self->key[5];
  dVar2 = self->key[1];
  local_58[1] = self->key[6];
  uVar1 = self->key[2];
  local_58[0] = self->key[7];
  uVar2 = self->key[3];
  local_48[0] = uVar2;
  local_48[1] = uVar1;
  local_48[2] = dVar2;
  local_48[3] = dVar1;
  des__select3(local_48,auStack_68);
  des__mod2add(auStack_68,param_1 + 0x5a,6);
  des__select4(auStack_68,local_48);
  des__mod2add(local_48,local_58,4);
  uVar6 = local_48[3];
  uVar4 = local_48[1];
  uVar8 = local_48[0];
  uVar3 = local_48[2];
  local_58[0] = uVar2;
  local_58[1] = uVar1;
  local_58[2] = dVar2;
  local_58[3] = dVar1;
  des__select3(local_48,auStack_68);
  des__mod2add(auStack_68,param_1 + 0x54,6);
  des__select4(auStack_68,local_48);
  des__mod2add(local_48,local_58,4);
  uVar7 = local_48[3];
  uVar5 = local_48[1];
  uVar2 = local_48[0];
  local_58[0] = uVar8;
  local_58[1] = uVar4;
  local_58[3] = uVar6;
  uVar1 = local_48[2];
  local_58[2] = uVar3;
  des__select3(local_48,auStack_68);
  des__mod2add(auStack_68,param_1 + 0x4e,6);
  des__select4(auStack_68,local_48);
  des__mod2add(local_48,local_58,4);
  uVar6 = local_48[3];
  uVar4 = local_48[1];
  uVar3 = local_48[0];
  local_58[0] = uVar2;
  local_58[1] = uVar5;
  local_58[3] = uVar7;
  uVar2 = local_48[2];
  local_58[2] = uVar1;
  des__select3(local_48,auStack_68);
  des__mod2add(auStack_68,param_1 + 0x48,6);
  des__select4(auStack_68,local_48);
  des__mod2add(local_48,local_58,4);
  uVar7 = local_48[3];
  uVar5 = local_48[1];
  uVar8 = local_48[0];
  local_58[0] = uVar3;
  local_58[1] = uVar4;
  local_58[3] = uVar6;
  uVar1 = local_48[2];
  local_58[2] = uVar2;
  des__select3(local_48,auStack_68);
  des__mod2add(auStack_68,param_1 + 0x42,6);
  des__select4(auStack_68,local_48);
  des__mod2add(local_48,local_58,4);
  uVar6 = local_48[3];
  uVar4 = local_48[1];
  uVar3 = local_48[0];
  local_58[0] = uVar8;
  local_58[1] = uVar5;
  local_58[3] = uVar7;
  uVar2 = local_48[2];
  local_58[2] = uVar1;
  des__select3(local_48,auStack_68);
  des__mod2add(auStack_68,param_1 + 0x3c,6);
  des__select4(auStack_68,local_48);
  des__mod2add(local_48,local_58,4);
  uVar7 = local_48[3];
  uVar5 = local_48[1];
  uVar8 = local_48[0];
  local_58[0] = uVar3;
  local_58[1] = uVar4;
  local_58[3] = uVar6;
  uVar1 = local_48[2];
  local_58[2] = uVar2;
  des__select3(local_48,auStack_68);
  des__mod2add(auStack_68,param_1 + 0x36,6);
  des__select4(auStack_68,local_48);
  des__mod2add(local_48,local_58,4);
  uVar6 = local_48[3];
  uVar4 = local_48[1];
  uVar3 = local_48[0];
  local_58[0] = uVar8;
  local_58[1] = uVar5;
  local_58[3] = uVar7;
  uVar2 = local_48[2];
  local_58[2] = uVar1;
  des__select3(local_48,auStack_68);
  des__mod2add(auStack_68,param_1 + 0x30,6);
  des__select4(auStack_68,local_48);
  des__mod2add(local_48,local_58,4);
  uVar7 = local_48[3];
  uVar5 = local_48[1];
  uVar8 = local_48[0];
  local_58[0] = uVar3;
  local_58[1] = uVar4;
  local_58[3] = uVar6;
  uVar1 = local_48[2];
  local_58[2] = uVar2;
  des__select3(local_48,auStack_68);
  des__mod2add(auStack_68,param_1 + 0x2a,6);
  des__select4(auStack_68,local_48);
  des__mod2add(local_48,local_58,4);
  uVar6 = local_48[3];
  uVar4 = local_48[1];
  uVar3 = local_48[0];
  local_58[0] = uVar8;
  local_58[1] = uVar5;
  local_58[3] = uVar7;
  uVar2 = local_48[2];
  local_58[2] = uVar1;
  des__select3(local_48,auStack_68);
  des__mod2add(auStack_68,param_1 + 0x24,6);
  des__select4(auStack_68,local_48);
  des__mod2add(local_48,local_58,4);
  uVar7 = local_48[3];
  uVar5 = local_48[1];
  uVar8 = local_48[0];
  local_58[0] = uVar3;
  local_58[1] = uVar4;
  local_58[3] = uVar6;
  uVar1 = local_48[2];
  local_58[2] = uVar2;
  des__select3(local_48,auStack_68);
  des__mod2add(auStack_68,param_1 + 0x1e,6);
  des__select4(auStack_68,local_48);
  des__mod2add(local_48,local_58,4);
  uVar6 = local_48[3];
  uVar4 = local_48[1];
  uVar3 = local_48[0];
  local_58[0] = uVar8;
  local_58[1] = uVar5;
  local_58[3] = uVar7;
  uVar2 = local_48[2];
  local_58[2] = uVar1;
  des__select3(local_48,auStack_68);
  des__mod2add(auStack_68,param_1 + 0x18,6);
  des__select4(auStack_68,local_48);
  des__mod2add(local_48,local_58,4);
  uVar7 = local_48[3];
  uVar5 = local_48[1];
  uVar8 = local_48[0];
  local_58[0] = uVar3;
  local_58[1] = uVar4;
  local_58[3] = uVar6;
  uVar1 = local_48[2];
  local_58[2] = uVar2;
  des__select3(local_48,auStack_68);
  des__mod2add(auStack_68,param_1 + 0x12,6);
  des__select4(auStack_68,local_48);
  des__mod2add(local_48,local_58,4);
  uVar6 = local_48[3];
  uVar4 = local_48[1];
  uVar3 = local_48[0];
  local_58[0] = uVar8;
  local_58[1] = uVar5;
  local_58[3] = uVar7;
  uVar2 = local_48[2];
  local_58[2] = uVar1;
  des__select3(local_48,auStack_68);
  des__mod2add(auStack_68,param_1 + 0xc,6);
  des__select4(auStack_68,local_48);
  des__mod2add(local_48,local_58,4);
  uVar7 = local_48[3];
  uVar5 = local_48[1];
  uVar8 = local_48[0];
  local_58[0] = uVar3;
  local_58[1] = uVar4;
  local_58[3] = uVar6;
  uVar1 = local_48[2];
  local_58[2] = uVar2;
  des__select3(local_48,auStack_68);
  des__mod2add(auStack_68,param_1 + 6,6);
  des__select4(auStack_68,local_48);
  des__mod2add(local_48,local_58,4);
  uVar6 = local_48[3];
  uVar4 = local_48[2];
  uVar3 = local_48[1];
  uVar2 = local_48[0];
  local_58[0] = uVar8;
  local_58[1] = uVar5;
  local_58[3] = uVar7;
  uVar8 = local_48[2];
  local_58[2] = uVar1;
  des__select3(local_48,auStack_68);
  des__mod2add(auStack_68,param_1,6);
  des__select4(auStack_68,local_48);
  des__mod2add(local_48,local_58,4);
  local_58[1] = uVar3;
  local_58[3] = uVar6;
  self->key[4] = uVar6;
  self->key[5] = uVar4;
  self->key[6] = uVar3;
  local_58[0] = uVar2;
  self->key[7] = uVar2;
  self->key[0] = local_48[3];
  self->key[1] = local_48[2];
  self->key[2] = local_48[1];
  self->key[3] = local_48[0];
  local_58[2] = uVar8;
  uninit_transform(self->key);
  return;
}

void des__descode(des *self,uchar *param_1)

{
  uchar uVar1;
  uchar uVar2;
  uchar uVar3;
  uchar uVar4;
  uchar uVar5;
  uchar uVar6;
  uchar uVar7;
  uchar uVar8;
  uchar uVar9;
  uchar auStack_68 [16];
  uchar local_58;
  uchar local_57;
  uchar local_56;
  uchar local_55;
  uchar local_48;
  uchar local_47;
  uchar local_46;
  uchar local_45;
  
  init_transform(self->key);
  local_45 = self->key[0];
  uVar1 = self->key[4];
  local_46 = self->key[1];
  uVar2 = self->key[5];
  local_47 = self->key[2];
  uVar3 = self->key[6];
  local_48 = self->key[3];
  uVar9 = self->key[7];
  local_58 = uVar9;
  local_57 = uVar3;
  local_56 = uVar2;
  local_55 = uVar1;
  des__select3(&local_58,auStack_68);
  des__mod2add(auStack_68,param_1,6);
  des__select4(auStack_68,&local_58);
  des__mod2add(&local_58,&local_48,4);
  uVar7 = local_55;
  uVar6 = local_57;
  uVar5 = local_58;
  uVar4 = local_56;
  local_48 = uVar9;
  local_47 = uVar3;
  local_46 = uVar2;
  local_45 = uVar1;
  des__select3(&local_58,auStack_68);
  des__mod2add(auStack_68,param_1 + 6,6);
  des__select4(auStack_68,&local_58);
  des__mod2add(&local_58,&local_48,4);
  uVar8 = local_55;
  uVar9 = local_57;
  uVar2 = local_58;
  local_48 = uVar5;
  local_47 = uVar6;
  local_45 = uVar7;
  uVar1 = local_56;
  local_46 = uVar4;
  des__select3(&local_58,auStack_68);
  des__mod2add(auStack_68,param_1 + 0xc,6);
  des__select4(auStack_68,&local_58);
  des__mod2add(&local_58,&local_48,4);
  uVar6 = local_55;
  uVar4 = local_57;
  uVar3 = local_58;
  local_48 = uVar2;
  local_47 = uVar9;
  local_45 = uVar8;
  uVar2 = local_56;
  local_46 = uVar1;
  des__select3(&local_58,auStack_68);
  des__mod2add(auStack_68,param_1 + 0x12,6);
  des__select4(auStack_68,&local_58);
  des__mod2add(&local_58,&local_48,4);
  uVar7 = local_55;
  uVar5 = local_57;
  uVar9 = local_58;
  local_48 = uVar3;
  local_47 = uVar4;
  local_45 = uVar6;
  uVar1 = local_56;
  local_46 = uVar2;
  des__select3(&local_58,auStack_68);
  des__mod2add(auStack_68,param_1 + 0x18,6);
  des__select4(auStack_68,&local_58);
  des__mod2add(&local_58,&local_48,4);
  uVar6 = local_55;
  uVar4 = local_57;
  uVar3 = local_58;
  local_48 = uVar9;
  local_47 = uVar5;
  local_45 = uVar7;
  uVar2 = local_56;
  local_46 = uVar1;
  des__select3(&local_58,auStack_68);
  des__mod2add(auStack_68,param_1 + 0x1e,6);
  des__select4(auStack_68,&local_58);
  des__mod2add(&local_58,&local_48,4);
  uVar7 = local_55;
  uVar5 = local_57;
  uVar9 = local_58;
  local_48 = uVar3;
  local_47 = uVar4;
  local_45 = uVar6;
  uVar1 = local_56;
  local_46 = uVar2;
  des__select3(&local_58,auStack_68);
  des__mod2add(auStack_68,param_1 + 0x24,6);
  des__select4(auStack_68,&local_58);
  des__mod2add(&local_58,&local_48,4);
  uVar6 = local_55;
  uVar4 = local_57;
  uVar3 = local_58;
  local_48 = uVar9;
  local_47 = uVar5;
  local_45 = uVar7;
  uVar2 = local_56;
  local_46 = uVar1;
  des__select3(&local_58,auStack_68);
  des__mod2add(auStack_68,param_1 + 0x2a,6);
  des__select4(auStack_68,&local_58);
  des__mod2add(&local_58,&local_48,4);
  uVar7 = local_55;
  uVar5 = local_57;
  uVar9 = local_58;
  local_48 = uVar3;
  local_47 = uVar4;
  local_45 = uVar6;
  uVar1 = local_56;
  local_46 = uVar2;
  des__select3(&local_58,auStack_68);
  des__mod2add(auStack_68,param_1 + 0x30,6);
  des__select4(auStack_68,&local_58);
  des__mod2add(&local_58,&local_48,4);
  uVar6 = local_55;
  uVar4 = local_57;
  uVar3 = local_58;
  local_48 = uVar9;
  local_47 = uVar5;
  local_45 = uVar7;
  uVar2 = local_56;
  local_46 = uVar1;
  des__select3(&local_58,auStack_68);
  des__mod2add(auStack_68,param_1 + 0x36,6);
  des__select4(auStack_68,&local_58);
  des__mod2add(&local_58,&local_48,4);
  uVar7 = local_55;
  uVar5 = local_57;
  uVar9 = local_58;
  local_48 = uVar3;
  local_47 = uVar4;
  local_45 = uVar6;
  uVar1 = local_56;
  local_46 = uVar2;
  des__select3(&local_58,auStack_68);
  des__mod2add(auStack_68,param_1 + 0x3c,6);
  des__select4(auStack_68,&local_58);
  des__mod2add(&local_58,&local_48,4);
  uVar6 = local_55;
  uVar4 = local_57;
  uVar3 = local_58;
  local_48 = uVar9;
  local_47 = uVar5;
  local_45 = uVar7;
  uVar2 = local_56;
  local_46 = uVar1;
  des__select3(&local_58,auStack_68);
  des__mod2add(auStack_68,param_1 + 0x42,6);
  des__select4(auStack_68,&local_58);
  des__mod2add(&local_58,&local_48,4);
  uVar7 = local_55;
  uVar5 = local_57;
  uVar9 = local_58;
  local_48 = uVar3;
  local_47 = uVar4;
  local_45 = uVar6;
  uVar1 = local_56;
  local_46 = uVar2;
  des__select3(&local_58,auStack_68);
  des__mod2add(auStack_68,param_1 + 0x48,6);
  des__select4(auStack_68,&local_58);
  des__mod2add(&local_58,&local_48,4);
  uVar6 = local_55;
  uVar4 = local_57;
  uVar3 = local_58;
  local_48 = uVar9;
  local_47 = uVar5;
  local_45 = uVar7;
  uVar2 = local_56;
  local_46 = uVar1;
  des__select3(&local_58,auStack_68);
  des__mod2add(auStack_68,param_1 + 0x4e,6);
  des__select4(auStack_68,&local_58);
  des__mod2add(&local_58,&local_48,4);
  uVar7 = local_55;
  uVar5 = local_57;
  uVar9 = local_58;
  local_48 = uVar3;
  local_47 = uVar4;
  local_45 = uVar6;
  uVar1 = local_56;
  local_46 = uVar2;
  des__select3(&local_58,auStack_68);
  des__mod2add(auStack_68,param_1 + 0x54,6);
  des__select4(auStack_68,&local_58);
  des__mod2add(&local_58,&local_48,4);
  uVar6 = local_55;
  uVar4 = local_56;
  uVar3 = local_57;
  uVar2 = local_58;
  local_48 = uVar9;
  local_47 = uVar5;
  local_45 = uVar7;
  uVar9 = local_56;
  local_46 = uVar1;
  des__select3(&local_58,auStack_68);
  des__mod2add(auStack_68,param_1 + 0x5a,6);
  des__select4(auStack_68,&local_58);
  des__mod2add(&local_58,&local_48,4);
  local_47 = uVar3;
  self->key[2] = uVar3;
  local_45 = uVar6;
  self->key[0] = uVar6;
  self->key[1] = uVar4;
  local_48 = uVar2;
  self->key[3] = uVar2;
  self->key[6] = local_57;
  self->key[4] = local_55;
  self->key[5] = local_56;
  self->key[7] = local_58;
  local_46 = uVar9;
  uninit_transform(self->key);
  return;
}

void Randkeydecipher__InitKey(Randkeydecipher *self,istream *param_1)
{
  byte (*pabVar1) [6];
  byte (*pabVar2) [7];
  byte (*pabVar3) [7];
  byte (*pabVar4) [7];
  byte (*pabVar5) [6];
  byte (*pabVar6) [6];
  uchar uVar7;
  uchar uVar8;
  uchar uVar9;
  uchar uVar10;
  uchar uVar11;
  uchar uVar12;
  uchar uVar13;
  uchar uVar14;
  des local_58 [2];
  byte local_48 [8];
  
  pabVar2 = self->select1_dest;
  pabVar1 = self->select2_dest;
  pabVar3 = self->select1_dest + 1;
  pabVar4 = self->select1_dest + 2;
  des__select1(self->sha_rand_key + 0x40,*pabVar2);
  des__change1(*pabVar2,(uint)des__cyc[0]);
  des__select2(*pabVar2,*pabVar1);
  des__change1(*pabVar2,(uint)des__cyc[1]);
  des__select2(*pabVar2,self->select2_dest[1]);
  des__change1(*pabVar2,(uint)des__cyc[2]);
  des__select2(*pabVar2,self->select2_dest[2]);
  des__change1(*pabVar2,(uint)des__cyc[3]);
  des__select2(*pabVar2,self->select2_dest[3]);
  des__change1(*pabVar2,(uint)des__cyc[4]);
  des__select2(*pabVar2,self->select2_dest[4]);
  des__change1(*pabVar2,(uint)des__cyc[5]);
  des__select2(*pabVar2,self->select2_dest[5]);
  des__change1(*pabVar2,(uint)des__cyc[6]);
  des__select2(*pabVar2,self->select2_dest[6]);
  des__change1(*pabVar2,(uint)des__cyc[7]);
  des__select2(*pabVar2,self->select2_dest[7]);
  des__change1(*pabVar2,(uint)des__cyc[8]);
  des__select2(*pabVar2,self->select2_dest[8]);
  des__change1(*pabVar2,(uint)des__cyc[9]);
  des__select2(*pabVar2,self->select2_dest[9]);
  des__change1(*pabVar2,(uint)des__cyc[10]);
  des__select2(*pabVar2,self->select2_dest[10]);
  des__change1(*pabVar2,(uint)des__cyc[0xb]);
  des__select2(*pabVar2,self->select2_dest[0xb]);
  des__change1(*pabVar2,(uint)des__cyc[0xc]);
  des__select2(*pabVar2,self->select2_dest[0xc]);
  des__change1(*pabVar2,(uint)des__cyc[0xd]);
  des__select2(*pabVar2,self->select2_dest[0xd]);
  des__change1(*pabVar2,(uint)des__cyc[0xe]);
  des__select2(*pabVar2,self->select2_dest[0xe]);
  des__change1(*pabVar2,(uint)des__cyc[0xf]);
  pabVar5 = self->select2_dest + 0x10;
  des__select2(*pabVar2,self->select2_dest[0xf]);
  des__select1(self->sha_rand_key + 0x38,*pabVar3);
  des__change1(*pabVar3,(uint)des__cyc[0]);
  des__select2(*pabVar3,*pabVar5);
  des__change1(*pabVar3,(uint)des__cyc[1]);
  des__select2(*pabVar3,self->select2_dest[0x11]);
  des__change1(*pabVar3,(uint)des__cyc[2]);
  des__select2(*pabVar3,self->select2_dest[0x12]);
  des__change1(*pabVar3,(uint)des__cyc[3]);
  des__select2(*pabVar3,self->select2_dest[0x13]);
  des__change1(*pabVar3,(uint)des__cyc[4]);
  des__select2(*pabVar3,self->select2_dest[0x14]);
  des__change1(*pabVar3,(uint)des__cyc[5]);
  des__select2(*pabVar3,self->select2_dest[0x15]);
  des__change1(*pabVar3,(uint)des__cyc[6]);
  des__select2(*pabVar3,self->select2_dest[0x16]);
  des__change1(*pabVar3,(uint)des__cyc[7]);
  des__select2(*pabVar3,self->select2_dest[0x17]);
  des__change1(*pabVar3,(uint)des__cyc[8]);
  des__select2(*pabVar3,self->select2_dest[0x18]);
  des__change1(*pabVar3,(uint)des__cyc[9]);
  des__select2(*pabVar3,self->select2_dest[0x19]);
  des__change1(*pabVar3,(uint)des__cyc[10]);
  des__select2(*pabVar3,self->select2_dest[0x1a]);
  des__change1(*pabVar3,(uint)des__cyc[0xb]);
  des__select2(*pabVar3,self->select2_dest[0x1b]);
  des__change1(*pabVar3,(uint)des__cyc[0xc]);
  des__select2(*pabVar3,self->select2_dest[0x1c]);
  des__change1(*pabVar3,(uint)des__cyc[0xd]);
  des__select2(*pabVar3,self->select2_dest[0x1d]);
  des__change1(*pabVar3,(uint)des__cyc[0xe]);
  des__select2(*pabVar3,self->select2_dest[0x1e]);
  des__change1(*pabVar3,(uint)des__cyc[0xf]);
  pabVar6 = self->select2_dest + 0x20;
  des__select2(*pabVar3,self->select2_dest[0x1f]);
  des__select1(self->sha_rand_key + 0x30,*pabVar4);
  des__change1(*pabVar4,(uint)des__cyc[0]);
  des__select2(*pabVar4,*pabVar6);
  des__change1(*pabVar4,(uint)des__cyc[1]);
  des__select2(*pabVar4,self->select2_dest[0x21]);
  des__change1(*pabVar4,(uint)des__cyc[2]);
  des__select2(*pabVar4,self->select2_dest[0x22]);
  des__change1(*pabVar4,(uint)des__cyc[3]);
  des__select2(*pabVar4,self->select2_dest[0x23]);
  des__change1(*pabVar4,(uint)des__cyc[4]);
  des__select2(*pabVar4,self->select2_dest[0x24]);
  des__change1(*pabVar4,(uint)des__cyc[5]);
  des__select2(*pabVar4,self->select2_dest[0x25]);
  des__change1(*pabVar4,(uint)des__cyc[6]);
  des__select2(*pabVar4,self->select2_dest[0x26]);
  des__change1(*pabVar4,(uint)des__cyc[7]);
  des__select2(*pabVar4,self->select2_dest[0x27]);
  des__change1(*pabVar4,(uint)des__cyc[8]);
  des__select2(*pabVar4,self->select2_dest[0x28]);
  des__change1(*pabVar4,(uint)des__cyc[9]);
  des__select2(*pabVar4,self->select2_dest[0x29]);
  des__change1(*pabVar4,(uint)des__cyc[10]);
  des__select2(*pabVar4,self->select2_dest[0x2a]);
  des__change1(*pabVar4,(uint)des__cyc[0xb]);
  des__select2(*pabVar4,self->select2_dest[0x2b]);
  des__change1(*pabVar4,(uint)des__cyc[0xc]);
  des__select2(*pabVar4,self->select2_dest[0x2c]);
  des__change1(*pabVar4,(uint)des__cyc[0xd]);
  des__select2(*pabVar4,self->select2_dest[0x2d]);
  des__change1(*pabVar4,(uint)des__cyc[0xe]);
  des__select2(*pabVar4,self->select2_dest[0x2e]);
  des__change1(*pabVar4,(uint)des__cyc[0xf]);
  des__select2(*pabVar4,self->select2_dest[0x2f]);
  local_48[0] = self->sha_rand_key[0x28];
  local_48[1] = self->sha_rand_key[0x29];
  local_48[2] = self->sha_rand_key[0x2a];
  local_48[3] = self->sha_rand_key[0x2b];
  local_48[4] = self->sha_rand_key[0x2c];
  local_48[5] = self->sha_rand_key[0x2d];
  local_48[6] = self->sha_rand_key[0x2e];
  local_48[7] = self->sha_rand_key[0x2f];
  std::istream::read(param_1,(char *)local_58,8);
  uVar14 = local_58[0].key[7];
  uVar13 = local_58[0].key[6];
  uVar12 = local_58[0].key[5];
  uVar11 = local_58[0].key[4];
  uVar10 = local_58[0].key[3];
  uVar9 = local_58[0].key[2];
  uVar8 = local_58[0].key[1];
  uVar7 = local_58[0].key[0];
  des__desdecode(local_58,*pabVar6);
  des__descode(local_58,*pabVar5);
  des__desdecode(local_58,*pabVar1);
  des__mod2add(local_58[0].key,local_48,8);
  local_48[1] = uVar8;
  local_48[7] = uVar14;
  local_48[0] = uVar7;
  local_48[2] = uVar9;
  local_48[3] = uVar10;
  local_48[4] = uVar11;
  local_48[5] = uVar12;
  local_48[6] = uVar13;
  self->subkey[0][0] = local_58[0].key[0];
  self->subkey[0][1] = local_58[0].key[1];
  self->subkey[0][2] = local_58[0].key[2];
  self->subkey[0][3] = local_58[0].key[3];
  self->subkey[0][4] = local_58[0].key[4];
  self->subkey[0][5] = local_58[0].key[5];
  self->subkey[0][6] = local_58[0].key[6];
  self->subkey[0][7] = local_58[0].key[7];
  std::istream::read(param_1,(char *)local_58,8);
  uVar14 = local_58[0].key[7];
  uVar13 = local_58[0].key[6];
  uVar12 = local_58[0].key[5];
  uVar11 = local_58[0].key[4];
  uVar10 = local_58[0].key[3];
  uVar9 = local_58[0].key[2];
  uVar8 = local_58[0].key[1];
  uVar7 = local_58[0].key[0];
  des__desdecode(local_58,*pabVar6);
  des__descode(local_58,*pabVar5);
  des__desdecode(local_58,*pabVar1);
  des__mod2add(local_58[0].key,local_48,8);
  local_48[1] = uVar8;
  local_48[7] = uVar14;
  local_48[0] = uVar7;
  local_48[2] = uVar9;
  local_48[3] = uVar10;
  local_48[4] = uVar11;
  local_48[5] = uVar12;
  local_48[6] = uVar13;
  self->subkey[1][0] = local_58[0].key[0];
  self->subkey[1][1] = local_58[0].key[1];
  self->subkey[1][2] = local_58[0].key[2];
  self->subkey[1][3] = local_58[0].key[3];
  self->subkey[1][4] = local_58[0].key[4];
  self->subkey[1][5] = local_58[0].key[5];
  self->subkey[1][6] = local_58[0].key[6];
  self->subkey[1][7] = local_58[0].key[7];
  std::istream::read(param_1,(char *)local_58,8);
  des__desdecode(local_58,*pabVar6);
  des__descode(local_58,*pabVar5);
  des__desdecode(local_58,*pabVar1);
  des__mod2add(local_58[0].key,local_48,8);
  self->subkey[2][0] = local_58[0].key[0];
  self->subkey[2][1] = local_58[0].key[1];
  self->subkey[2][2] = local_58[0].key[2];
  self->subkey[2][3] = local_58[0].key[3];
  self->subkey[2][4] = local_58[0].key[4];
  self->subkey[2][5] = local_58[0].key[5];
  self->subkey[2][6] = local_58[0].key[6];
  self->subkey[2][7] = local_58[0].key[7];
  return;
}

undefined8 
TDESdecipher__InitHead(TDESdecipher *self,istream *param_1,int param_2,char *param_3,int *param_4)

{
  byte bVar1;
  uint uVar2;
  uint uVar3;
  undefined8 uVar4;
  long *plVar5;
  undefined4 *puVar5;
  byte *pbVar6;
  long lVar7;
  uint uVar8;
  int iVar9;
  char *pcVar10;
  char *pcVar11;
  undefined1 uVar12;
  byte bVar13;
  byte local_b8 [32];
  char local_98 [32];
  char local_78 [32];
  undefined1 local_58 [16];
  byte local_48 [8];
  char local_39 [9];
  
  bVar13 = 0;
  std::istream::read(param_1,(char *)local_48,8);
  bVar1 = local_48[7];
  sha256_hash_byte(self->sha_obj,local_48[0]);
  uVar2 = (uint)local_48[6];
  sha256_hash_byte(self->sha_obj,local_48[1]);
  uVar8 = (uint)local_48[5];
  sha256_hash_byte(self->sha_obj,local_48[2]);
  uVar3 = (uint)local_48[4];
  sha256_hash_byte(self->sha_obj,local_48[3]);
  sha256_hash_byte(self->sha_obj,local_48[4]);
  sha256_hash_byte(self->sha_obj,local_48[5]);
  sha256_hash_byte(self->sha_obj,local_48[6]);
  uVar2 = (uint)local_48[0] +
          ((uint)local_48[1] +
          ((uint)local_48[2] +
          ((uint)local_48[3] +
          (uVar3 + (uVar8 + (uVar2 + ((uint)bVar1 + (uint)bVar1 * 4) * 2) * 10) * 10) * 10) * 10) *
          10) * 10;
  sha256_hash_byte(self->sha_obj,local_48[7]);
  if (param_2 < (int)uVar2) {
    puVar5 = (undefined4 *)__cxa_allocate_exception(4);
    *puVar5 = 4;
                    /* WARNING: Subroutine does not return */
    __cxa_throw(puVar5,&HEDRStatus::typeinfo,0);
  }
  if (0x131ccc3 < (int)uVar2) {
    std::istream::read(param_1,local_78,0x20);
    local_58 = std::istream::tellg(param_1);
    uVar4 = local_58._0_8_;
    while( true ) {
      plVar5 = (long *)std::istream::read(param_1,local_39,1);
      uVar12 = (*(byte *)((long)plVar5 + *(long *)(*plVar5 + -0x18) + 0x20) & 5) == 0;
      if (!(bool)uVar12) break;
      sha256_hash_byte(self->sha_obj,(int)local_39[0]);
    }
    sha256_finish(self->sha_obj,local_98);
    lVar7 = 0x20;
    pcVar10 = local_78;
    pcVar11 = local_98;
    do {
      if (lVar7 == 0) break;
      lVar7 = lVar7 + -1;
      uVar12 = *pcVar10 == *pcVar11;
      pcVar10 = pcVar10 + (ulong)bVar13 * -2 + 1;
      pcVar11 = pcVar11 + (ulong)bVar13 * -2 + 1;
    } while ((bool)uVar12);
    if (!(bool)uVar12) {
      puVar5 = (undefined4 *)__cxa_allocate_exception(4);
      *puVar5 = 3;
                    /* WARNING: Subroutine does not return */
      __cxa_throw(puVar5,&HEDRStatus::typeinfo,0);
    }
    std::ios::clear((ios *)(param_1 + *(long *)(*(long *)param_1 + -0x18)),0);
    std::istream::seekg(param_1,uVar4,0);
  }
  local_b8[2] = 0;
  local_b8[3] = 0;
  local_b8[4] = 0;
  local_b8[5] = 0;
  local_b8[6] = 0;
  local_b8[7] = 0;
  local_b8[8] = 0;
  local_b8[9] = 0;
  local_b8[10] = 0;
  local_b8[0xb] = 0;
  local_b8[0xc] = 0;
  local_b8[0xd] = 0;
  local_b8[0xe] = 0;
  local_b8[0xf] = 0;
  local_b8[0x10] = 0;
  local_b8[0x11] = 0;
  uVar3 = uVar2 & 0xff;
  local_b8[0x12] = 0;
  local_b8[0x13] = 0;
  local_b8[0x14] = 0;
  local_b8[0x15] = 0;
  local_b8[0x16] = 0;
  local_b8[0x17] = 0;
  local_b8[0x18] = 0;
  local_b8[0x19] = 0;
  local_b8[0x1a] = 0;
  local_b8[0x1b] = 0;
  local_b8[0x1c] = 0;
  local_b8[0x1d] = 0;
  local_b8[0x1e] = 0;
  local_b8[0x1f] = 0;
  local_b8[0] = 0;
  local_b8[1] = 0;
  pbVar6 = randrsl;
  uVar8 = 0;
  do {
    uVar8 = uVar8 + 1;
    *(uint *)pbVar6 = uVar3;
    *(uint *)((long)pbVar6 + 4) = uVar3;
    *(uint *)((long)pbVar6 + 8) = uVar3;
    *(uint *)((long)pbVar6 + 0xc) = uVar3;
    pbVar6 = (byte *)((long)pbVar6 + 0x10);
  } while (uVar8 < 0x40);
  iVar9 = 0;
  randinit(1);
  do {
    iVar9 = iVar9 + 1;
    isaac();
  } while (iVar9 <= (int)(uint)(byte)((byte)uVar2 % 10));
  sha256(local_b8,0x100 - (uVar2 & 0xff));
  self->sha_rand_key[1] = local_b8[1];
  self->sha_rand_key[2] = local_b8[2];
  self->sha_rand_key[4] = local_b8[4];
  self->sha_rand_key[0] = local_b8[0];
  self->sha_rand_key[3] = local_b8[3];
  self->sha_rand_key[6] = local_b8[6];
  self->sha_rand_key[9] = local_b8[9];
  self->sha_rand_key[5] = local_b8[5];
  self->sha_rand_key[7] = local_b8[7];
  self->sha_rand_key[8] = local_b8[8];
  self->sha_rand_key[0xc] = local_b8[0xc];
  self->sha_rand_key[10] = local_b8[10];
  self->sha_rand_key[0xb] = local_b8[0xb];
  self->sha_rand_key[0xd] = local_b8[0xd];
  self->sha_rand_key[0xe] = local_b8[0xe];
  self->sha_rand_key[0xf] = local_b8[0xf];
  self->sha_rand_key[0x10] = local_b8[0x10];
  self->sha_rand_key[0x11] = local_b8[0x11];
  self->sha_rand_key[0x13] = local_b8[0x13];
  self->sha_rand_key[0x18] = local_b8[0x18];
  self->sha_rand_key[0x19] = local_b8[0x19];
  self->sha_rand_key[0x14] = local_b8[0x14];
  self->sha_rand_key[0x1a] = local_b8[0x1a];
  self->sha_rand_key[0x1b] = local_b8[0x1b];
  self->sha_rand_key[0x15] = local_b8[0x15];
  self->sha_rand_key[0x1c] = local_b8[0x1c];
  self->sha_rand_key[0x20] = local_b8[0x18];
  self->sha_rand_key[0x21] = local_b8[0x19];
  self->sha_rand_key[0x16] = local_b8[0x16];
  self->sha_rand_key[0x22] = local_b8[0x1a];
  self->sha_rand_key[0x12] = local_b8[0x12];
  self->sha_rand_key[0x1d] = local_b8[0x1d];
  self->sha_rand_key[0x17] = local_b8[0x17];
  self->sha_rand_key[0x1e] = local_b8[0x1e];
  self->sha_rand_key[0x1f] = local_b8[0x1f];
  self->sha_rand_key[0x23] = local_b8[0x1b];
  self->sha_rand_key[0x24] = local_b8[0x1c];
  self->sha_rand_key[0x2d] = local_b8[0x15];
  self->sha_rand_key[0x25] = local_b8[0x1d];
  self->sha_rand_key[0x2a] = local_b8[0x12];
  self->sha_rand_key[0x31] = local_b8[9];
  self->sha_rand_key[0x34] = local_b8[0xc];
  self->sha_rand_key[0x26] = local_b8[0x1e];
  self->sha_rand_key[0x27] = local_b8[0x1f];
  self->sha_rand_key[0x28] = local_b8[0x10];
  self->sha_rand_key[0x29] = local_b8[0x11];
  self->sha_rand_key[0x2b] = local_b8[0x13];
  self->sha_rand_key[0x2c] = local_b8[0x14];
  self->sha_rand_key[0x2e] = local_b8[0x16];
  self->sha_rand_key[0x2f] = local_b8[0x17];
  self->sha_rand_key[0x30] = local_b8[8];
  self->sha_rand_key[0x32] = local_b8[10];
  self->sha_rand_key[0x33] = local_b8[0xb];
  self->sha_rand_key[0x35] = local_b8[0xd];
  self->sha_rand_key[0x36] = local_b8[0xe];
  self->sha_rand_key[0x37] = local_b8[0xf];
  self->sha_rand_key[0x47] = local_b8[0x15];
  self->sha_rand_key[0x38] = local_b8[0];
  self->sha_rand_key[0x3b] = local_b8[3];
  self->sha_rand_key[0x3e] = local_b8[6];
  self->sha_rand_key[0x40] = local_b8[0];
  self->sha_rand_key[0x41] = local_b8[3];
  self->sha_rand_key[0x39] = local_b8[1];
  self->sha_rand_key[0x42] = local_b8[6];
  self->sha_rand_key[0x43] = local_b8[9];
  self->sha_rand_key[0x44] = local_b8[0xc];
  self->sha_rand_key[0x46] = local_b8[0x12];
  self->sha_rand_key[0x3a] = local_b8[2];
  self->sha_rand_key[0x3c] = local_b8[4];
  self->sha_rand_key[0x3d] = local_b8[5];
  self->sha_rand_key[0x3f] = local_b8[7];
  self->sha_rand_key[0x45] = local_b8[0xf];
  return 1;
}

byte * sha256_create(void)

{
  byte *pbVar1;
  
  pbVar1 = malloc(0x70);
  pbVar1[0x6c] = 0;
  pbVar1[0x6d] = 0;
  pbVar1[0x6e] = 0;
  pbVar1[0x6f] = 0;
  pbVar1[0x40] = 0;
  pbVar1[0x41] = 0;
  pbVar1[0x42] = 0;
  pbVar1[0x43] = 0;
  pbVar1[0x44] = 0;
  pbVar1[0x45] = 0;
  pbVar1[0x46] = 0;
  pbVar1[0x47] = 0;
  pbVar1[0x48] = 0x67;
  pbVar1[0x49] = 0xe6;
  pbVar1[0x4a] = 9;
  pbVar1[0x4b] = 0x6a;
  pbVar1[0x4c] = 0x85;
  pbVar1[0x4d] = 0xae;
  pbVar1[0x4e] = 0x67;
  pbVar1[0x4f] = 0xbb;
  pbVar1[0x50] = 0x72;
  pbVar1[0x51] = 0xf3;
  pbVar1[0x52] = 0x6e;
  pbVar1[0x53] = 0x3c;
  pbVar1[0x54] = 0x3a;
  pbVar1[0x55] = 0xf5;
  pbVar1[0x56] = 0x4f;
  pbVar1[0x57] = 0xa5;
  pbVar1[0x58] = 0x7f;
  pbVar1[0x59] = 0x52;
  pbVar1[0x5a] = 0xe;
  pbVar1[0x5b] = 0x51;
  pbVar1[0x5c] = 0x8c;
  pbVar1[0x5d] = 0x68;
  pbVar1[0x5e] = 5;
  pbVar1[0x5f] = 0x9b;
  pbVar1[0x60] = 0xab;
  pbVar1[0x61] = 0xd9;
  pbVar1[0x62] = 0x83;
  pbVar1[99] = 0x1f;
  pbVar1[100] = 0x19;
  pbVar1[0x65] = 0xcd;
  pbVar1[0x66] = 0xe0;
  pbVar1[0x67] = 0x5b;
  pbVar1[0x68] = 1;
  pbVar1[0x69] = 0;
  pbVar1[0x6a] = 0;
  pbVar1[0x6b] = 0;
  return pbVar1;
}

void sha256(char *param_1,int param_2)

{
  uint uVar1;
  byte *__ptr;
  byte *pbVar2;
  int iVar3;
  
  __ptr = sha256_create();
  if (0 < param_2) {
    pbVar2 = randrsl;
    iVar3 = 0;
    do {
      iVar3 = iVar3 + 1;
      sha256_hash_byte(__ptr,*(uint *)pbVar2 & 0xf);
      sha256_hash_byte(__ptr,*(uint *)pbVar2 >> 8 & 0xf);
      sha256_hash_byte(__ptr,*(uint *)pbVar2 >> 0x10 & 0xf);
      uVar1 = *(uint *)pbVar2;
      pbVar2 = (byte *)((long)pbVar2 + 4);
      sha256_hash_byte(__ptr,uVar1 >> 0x18 & 0xf);
    } while (iVar3 < param_2);
  }
  sha256_finish(__ptr,param_1);
  delete_sha256(__ptr);
  return;
}

void sha256_hash_byte(uint *param_1,undefined1 param_2)

{
  uint uVar1;
  uint uVar2;
  uint uVar3;
  uint uVar4;
  int iVar5;
  uint uVar6;
  uint uVar7;
  uint uVar8;
  uint uVar9;
  uint uVar10;
  uint uVar11;
  uint uVar12;
  uint uVar13;
  uint uVar14;
  uint uVar15;
  uint uVar16;
  uint uVar17;
  uint uVar18;
  uint uVar19;
  uint uVar20;
  uint uVar21;
  uint uVar22;
  uint uVar23;
  uint uVar24;
  uint uVar25;
  uint uVar26;
  uint uVar27;
  uint uVar28;
  uint uVar29;
  uint uVar30;
  undefined8 local_78;
  undefined8 uStack_70;
  undefined8 local_68;
  undefined8 uStack_60;
  undefined8 local_58;
  undefined8 uStack_50;
  undefined8 local_48;
  undefined8 uStack_40;
  
  uVar1 = param_1[0x1b] + 1;
  *(undefined1 *)((long)param_1 + (long)(int)param_1[0x1b]) = param_2;
  param_1[0x1b] = uVar1;
  if ((int)uVar1 < 0x40) {
    return;
  }
  uVar1 = param_1[0x10];
  param_1[0x10] = uVar1 + 0x200;
  if (uVar1 + 0x200 < 0x200) {
    param_1[0x11] = param_1[0x11] + 1;
  }
  if (param_1[0x1a] == 0) {
    local_78 = *(undefined8 *)param_1;
    uStack_70 = *(undefined8 *)(param_1 + 2);
    local_68 = *(undefined8 *)(param_1 + 4);
    uStack_60 = *(undefined8 *)(param_1 + 6);
    local_58 = *(undefined8 *)(param_1 + 8);
    uStack_50 = *(undefined8 *)(param_1 + 10);
    local_48 = *(undefined8 *)(param_1 + 0xc);
    uStack_40 = *(undefined8 *)(param_1 + 0xe);
  }
  else {
    uVar1 = *param_1;
    uVar6 = param_1[1];
    local_78 = CONCAT44(uVar6 >> 0x18 | (uVar6 & 0xff0000) >> 8 | (uVar6 & 0xff00) << 8 |
                        uVar6 << 0x18,
                        uVar1 >> 0x18 | (uVar1 & 0xff0000) >> 8 | (uVar1 & 0xff00) << 8 |
                        uVar1 << 0x18);
    uVar1 = param_1[2];
    uVar6 = param_1[3];
    uStack_70 = CONCAT44(uVar6 >> 0x18 | (uVar6 & 0xff0000) >> 8 | (uVar6 & 0xff00) << 8 |
                         uVar6 << 0x18,
                         uVar1 >> 0x18 | (uVar1 & 0xff0000) >> 8 | (uVar1 & 0xff00) << 8 |
                         uVar1 << 0x18);
    uVar1 = param_1[4];
    uVar6 = param_1[5];
    local_68 = CONCAT44(uVar6 >> 0x18 | (uVar6 & 0xff0000) >> 8 | (uVar6 & 0xff00) << 8 |
                        uVar6 << 0x18,
                        uVar1 >> 0x18 | (uVar1 & 0xff0000) >> 8 | (uVar1 & 0xff00) << 8 |
                        uVar1 << 0x18);
    uVar1 = param_1[6];
    uVar6 = param_1[7];
    uStack_60 = CONCAT44(uVar6 >> 0x18 | (uVar6 & 0xff0000) >> 8 | (uVar6 & 0xff00) << 8 |
                         uVar6 << 0x18,
                         uVar1 >> 0x18 | (uVar1 & 0xff0000) >> 8 | (uVar1 & 0xff00) << 8 |
                         uVar1 << 0x18);
    uVar1 = param_1[8];
    uVar6 = param_1[9];
    local_58 = CONCAT44(uVar6 >> 0x18 | (uVar6 & 0xff0000) >> 8 | (uVar6 & 0xff00) << 8 |
                        uVar6 << 0x18,
                        uVar1 >> 0x18 | (uVar1 & 0xff0000) >> 8 | (uVar1 & 0xff00) << 8 |
                        uVar1 << 0x18);
    uVar1 = param_1[10];
    uVar6 = param_1[0xb];
    uStack_50 = CONCAT44(uVar6 >> 0x18 | (uVar6 & 0xff0000) >> 8 | (uVar6 & 0xff00) << 8 |
                         uVar6 << 0x18,
                         uVar1 >> 0x18 | (uVar1 & 0xff0000) >> 8 | (uVar1 & 0xff00) << 8 |
                         uVar1 << 0x18);
    uVar1 = param_1[0xc];
    uVar6 = param_1[0xd];
    local_48 = CONCAT44(uVar6 >> 0x18 | (uVar6 & 0xff0000) >> 8 | (uVar6 & 0xff00) << 8 |
                        uVar6 << 0x18,
                        uVar1 >> 0x18 | (uVar1 & 0xff0000) >> 8 | (uVar1 & 0xff00) << 8 |
                        uVar1 << 0x18);
    uVar1 = param_1[0xe];
    uVar6 = param_1[0xf];
    uStack_40 = CONCAT44(uVar6 >> 0x18 | (uVar6 & 0xff0000) >> 8 | (uVar6 & 0xff00) << 8 |
                         uVar6 << 0x18,
                         uVar1 >> 0x18 | (uVar1 & 0xff0000) >> 8 | (uVar1 & 0xff00) << 8 |
                         uVar1 << 0x18);
  }
  iVar5 = param_1[0x19] + 0x428a2f98 + (int)local_78 +
          ((param_1[0x16] >> 0xb | param_1[0x16] << 0x15) ^
           (param_1[0x16] >> 6 | param_1[0x16] << 0x1a) ^
          (param_1[0x16] >> 0x19 | param_1[0x16] << 7)) +
          (~param_1[0x16] & param_1[0x18] | param_1[0x17] & param_1[0x16]);
  uVar1 = iVar5 + param_1[0x15];
  uVar27 = ((param_1[0x14] | param_1[0x13]) & param_1[0x12] | param_1[0x14] & param_1[0x13]) +
           ((param_1[0x12] >> 0xd | param_1[0x12] << 0x13) ^
            (param_1[0x12] >> 2 | param_1[0x12] << 0x1e) ^
           (param_1[0x12] >> 0x16 | param_1[0x12] << 10)) + iVar5;
  iVar5 = param_1[0x18] + 0x71374491 + local_78._4_4_ +
          ((uVar1 >> 0xb | uVar1 * 0x200000) ^ (uVar1 >> 6 | uVar1 * 0x4000000) ^
          (uVar1 >> 0x19 | uVar1 * 0x80)) + (~uVar1 & param_1[0x17] | param_1[0x16] & uVar1);
  uVar6 = iVar5 + param_1[0x14];
  uVar25 = ((uVar27 >> 0xd | uVar27 * 0x80000) ^ (uVar27 >> 2 | uVar27 * 0x40000000) ^
           (uVar27 >> 0x16 | uVar27 * 0x400)) +
           ((param_1[0x13] | param_1[0x12]) & uVar27 | param_1[0x13] & param_1[0x12]) + iVar5;
  iVar5 = param_1[0x17] + 0xb5c0fbcf + (uint)uStack_70 +
          ((uVar6 >> 0xb | uVar6 * 0x200000) ^ (uVar6 >> 6 | uVar6 * 0x4000000) ^
          (uVar6 >> 0x19 | uVar6 * 0x80)) + (~uVar6 & param_1[0x16] | uVar6 & uVar1);
  uVar10 = iVar5 + param_1[0x13];
  uVar23 = ((uVar25 >> 0xd | uVar25 * 0x80000) ^ (uVar25 >> 2 | uVar25 * 0x40000000) ^
           (uVar25 >> 0x16 | uVar25 * 0x400)) +
           ((param_1[0x12] | uVar27) & uVar25 | param_1[0x12] & uVar27) + iVar5;
  iVar5 = param_1[0x16] + 0xe9b5dba5 + uStack_70._4_4_ +
          ((uVar10 >> 0xb | uVar10 * 0x200000) ^ (uVar10 >> 6 | uVar10 * 0x4000000) ^
          (uVar10 >> 0x19 | uVar10 * 0x80)) + (~uVar10 & uVar1 | uVar10 & uVar6);
  uVar2 = iVar5 + param_1[0x12];
  uVar20 = ((uVar23 >> 0xd | uVar23 * 0x80000) ^ (uVar23 >> 2 | uVar23 * 0x40000000) ^
           (uVar23 >> 0x16 | uVar23 * 0x400)) + ((uVar25 | uVar27) & uVar23 | uVar25 & uVar27) +
           iVar5;
  iVar5 = uVar1 + 0x3956c25b + (uint)local_68 +
          ((uVar2 >> 0xb | uVar2 * 0x200000) ^ (uVar2 >> 6 | uVar2 * 0x4000000) ^
          (uVar2 >> 0x19 | uVar2 * 0x80)) + (~uVar2 & uVar6 | uVar2 & uVar10);
  uVar27 = iVar5 + uVar27;
  uVar1 = ((uVar20 >> 0xd | uVar20 * 0x80000) ^ (uVar20 >> 2 | uVar20 * 0x40000000) ^
          (uVar20 >> 0x16 | uVar20 * 0x400)) + ((uVar23 | uVar25) & uVar20 | uVar23 & uVar25) +
          iVar5;
  iVar5 = uVar6 + 0x59f111f1 + local_68._4_4_ +
          ((uVar27 >> 0xb | uVar27 * 0x200000) ^ (uVar27 >> 6 | uVar27 * 0x4000000) ^
          (uVar27 >> 0x19 | uVar27 * 0x80)) + (~uVar27 & uVar10 | uVar27 & uVar2);
  uVar25 = iVar5 + uVar25;
  uVar6 = ((uVar1 >> 0xd | uVar1 * 0x80000) ^ (uVar1 >> 2 | uVar1 * 0x40000000) ^
          (uVar1 >> 0x16 | uVar1 * 0x400)) + ((uVar20 | uVar23) & uVar1 | uVar20 & uVar23) + iVar5;
  iVar5 = uVar10 + 0x923f82a4 + (uint)uStack_60 +
          ((uVar25 >> 0xb | uVar25 * 0x200000) ^ (uVar25 >> 6 | uVar25 * 0x4000000) ^
          (uVar25 >> 0x19 | uVar25 * 0x80)) + (~uVar25 & uVar2 | uVar25 & uVar27);
  uVar23 = iVar5 + uVar23;
  uVar10 = ((uVar6 >> 0xd | uVar6 * 0x80000) ^ (uVar6 >> 2 | uVar6 * 0x40000000) ^
           (uVar6 >> 0x16 | uVar6 * 0x400)) + ((uVar1 | uVar20) & uVar6 | uVar1 & uVar20) + iVar5;
  iVar5 = uVar2 + 0xab1c5ed5 + uStack_60._4_4_ +
          ((uVar23 >> 0xb | uVar23 * 0x200000) ^ (uVar23 >> 6 | uVar23 * 0x4000000) ^
          (uVar23 >> 0x19 | uVar23 * 0x80)) + (~uVar23 & uVar27 | uVar23 & uVar25);
  uVar20 = iVar5 + uVar20;
  uVar2 = ((uVar10 >> 0xd | uVar10 * 0x80000) ^ (uVar10 >> 2 | uVar10 * 0x40000000) ^
          (uVar10 >> 0x16 | uVar10 * 0x400)) + ((uVar6 | uVar1) & uVar10 | uVar6 & uVar1) + iVar5;
  iVar5 = uVar27 + 0xd807aa98 + (uint)local_58 +
          ((uVar20 >> 0xb | uVar20 * 0x200000) ^ (uVar20 >> 6 | uVar20 * 0x4000000) ^
          (uVar20 >> 0x19 | uVar20 * 0x80)) + (~uVar20 & uVar25 | uVar20 & uVar23);
  uVar1 = iVar5 + uVar1;
  uVar27 = ((uVar2 >> 0xd | uVar2 * 0x80000) ^ (uVar2 >> 2 | uVar2 * 0x40000000) ^
           (uVar2 >> 0x16 | uVar2 * 0x400)) + ((uVar10 | uVar6) & uVar2 | uVar10 & uVar6) + iVar5;
  iVar5 = uVar25 + 0x12835b01 + local_58._4_4_ +
          ((uVar1 >> 0xb | uVar1 * 0x200000) ^ (uVar1 >> 6 | uVar1 * 0x4000000) ^
          (uVar1 >> 0x19 | uVar1 * 0x80)) + (~uVar1 & uVar23 | uVar1 & uVar20);
  uVar6 = iVar5 + uVar6;
  uVar25 = ((uVar27 >> 0xd | uVar27 * 0x80000) ^ (uVar27 >> 2 | uVar27 * 0x40000000) ^
           (uVar27 >> 0x16 | uVar27 * 0x400)) + ((uVar2 | uVar10) & uVar27 | uVar2 & uVar10) + iVar5
  ;
  iVar5 = uVar23 + 0x243185be + (uint)uStack_50 +
          ((uVar6 >> 0xb | uVar6 * 0x200000) ^ (uVar6 >> 6 | uVar6 * 0x4000000) ^
          (uVar6 >> 0x19 | uVar6 * 0x80)) + (~uVar6 & uVar20 | uVar6 & uVar1);
  uVar10 = iVar5 + uVar10;
  uVar23 = ((uVar25 >> 0xd | uVar25 * 0x80000) ^ (uVar25 >> 2 | uVar25 * 0x40000000) ^
           (uVar25 >> 0x16 | uVar25 * 0x400)) + ((uVar27 | uVar2) & uVar25 | uVar27 & uVar2) + iVar5
  ;
  iVar5 = uVar20 + 0x550c7dc3 + uStack_50._4_4_ +
          ((uVar10 >> 0xb | uVar10 * 0x200000) ^ (uVar10 >> 6 | uVar10 * 0x4000000) ^
          (uVar10 >> 0x19 | uVar10 * 0x80)) + (~uVar10 & uVar1 | uVar10 & uVar6);
  uVar2 = iVar5 + uVar2;
  uVar20 = ((uVar23 >> 0xd | uVar23 * 0x80000) ^ (uVar23 >> 2 | uVar23 * 0x40000000) ^
           (uVar23 >> 0x16 | uVar23 * 0x400)) + ((uVar25 | uVar27) & uVar23 | uVar25 & uVar27) +
           iVar5;
  iVar5 = uVar1 + 0x72be5d74 + (uint)local_48 +
          ((uVar2 >> 0xb | uVar2 * 0x200000) ^ (uVar2 >> 6 | uVar2 * 0x4000000) ^
          (uVar2 >> 0x19 | uVar2 * 0x80)) + (~uVar2 & uVar6 | uVar2 & uVar10);
  uVar27 = iVar5 + uVar27;
  uVar21 = ((uVar20 >> 0xd | uVar20 * 0x80000) ^ (uVar20 >> 2 | uVar20 * 0x40000000) ^
           (uVar20 >> 0x16 | uVar20 * 0x400)) + ((uVar23 | uVar25) & uVar20 | uVar23 & uVar25) +
           iVar5;
  iVar5 = uVar6 + 0x80deb1fe + local_48._4_4_ +
          ((uVar27 >> 0xb | uVar27 * 0x200000) ^ (uVar27 >> 6 | uVar27 * 0x4000000) ^
          (uVar27 >> 0x19 | uVar27 * 0x80)) + (~uVar27 & uVar10 | uVar27 & uVar2);
  uVar25 = iVar5 + uVar25;
  uVar16 = ((uVar21 >> 0xd | uVar21 * 0x80000) ^ (uVar21 >> 2 | uVar21 * 0x40000000) ^
           (uVar21 >> 0x16 | uVar21 * 0x400)) + ((uVar20 | uVar23) & uVar21 | uVar20 & uVar23) +
           iVar5;
  iVar5 = uVar10 + 0x9bdc06a7 + (uint)uStack_40 +
          ((uVar25 >> 0xb | uVar25 * 0x200000) ^ (uVar25 >> 6 | uVar25 * 0x4000000) ^
          (uVar25 >> 0x19 | uVar25 * 0x80)) + (~uVar25 & uVar2 | uVar25 & uVar27);
  uVar23 = iVar5 + uVar23;
  uVar7 = ((uVar16 >> 0xd | uVar16 * 0x80000) ^ (uVar16 >> 2 | uVar16 * 0x40000000) ^
          (uVar16 >> 0x16 | uVar16 * 0x400)) + ((uVar21 | uVar20) & uVar16 | uVar21 & uVar20) +
          iVar5;
  iVar5 = uVar2 + 0xc19bf174 + uStack_40._4_4_ +
          ((uVar23 >> 0xb | uVar23 * 0x200000) ^ (uVar23 >> 6 | uVar23 * 0x4000000) ^
          (uVar23 >> 0x19 | uVar23 * 0x80)) + (~uVar23 & uVar27 | uVar23 & uVar25);
  uVar20 = iVar5 + uVar20;
  uVar6 = ((uVar7 >> 0xd | uVar7 * 0x80000) ^ (uVar7 >> 2 | uVar7 * 0x40000000) ^
          (uVar7 >> 0x16 | uVar7 * 0x400)) + ((uVar16 | uVar21) & uVar7 | uVar16 & uVar21) + iVar5;
  uVar3 = local_58._4_4_ + (int)local_78 +
          (((uint)uStack_40 >> 0x13 | (uint)uStack_40 << 0xd) ^
           ((uint)uStack_40 >> 0x11 | (uint)uStack_40 << 0xf) ^ (uint)uStack_40 >> 10) +
          ((local_78._4_4_ >> 0x12 | local_78._4_4_ << 0xe) ^
           (local_78._4_4_ >> 7 | local_78._4_4_ << 0x19) ^ local_78._4_4_ >> 3);
  iVar5 = uVar27 + 0xe49b69c1 + uVar3 +
          ((uVar20 >> 0xb | uVar20 * 0x200000) ^ (uVar20 >> 6 | uVar20 * 0x4000000) ^
          (uVar20 >> 0x19 | uVar20 * 0x80)) + (~uVar20 & uVar25 | uVar20 & uVar23);
  uVar21 = uVar21 + iVar5;
  uVar30 = ((uVar6 >> 0xd | uVar6 * 0x80000) ^ (uVar6 >> 2 | uVar6 * 0x40000000) ^
           (uVar6 >> 0x16 | uVar6 * 0x400)) + ((uVar7 | uVar16) & uVar6 | uVar7 & uVar16) + iVar5;
  uVar29 = local_78._4_4_ + (uint)uStack_50 +
           ((uStack_40._4_4_ >> 0x13 | uStack_40._4_4_ << 0xd) ^
            (uStack_40._4_4_ >> 0x11 | uStack_40._4_4_ << 0xf) ^ uStack_40._4_4_ >> 10) +
           (((uint)uStack_70 >> 0x12 | (uint)uStack_70 << 0xe) ^
            ((uint)uStack_70 >> 7 | (uint)uStack_70 << 0x19) ^ (uint)uStack_70 >> 3);
  iVar5 = uVar25 + 0xefbe4786 + uVar29 +
          ((uVar21 >> 0xb | uVar21 * 0x200000) ^ (uVar21 >> 6 | uVar21 * 0x4000000) ^
          (uVar21 >> 0x19 | uVar21 * 0x80)) + (~uVar21 & uVar23 | uVar21 & uVar20);
  uVar16 = uVar16 + iVar5;
  uVar28 = ((uVar30 >> 0xd | uVar30 * 0x80000) ^ (uVar30 >> 2 | uVar30 * 0x40000000) ^
           (uVar30 >> 0x16 | uVar30 * 0x400)) + ((uVar6 | uVar7) & uVar30 | uVar6 & uVar7) + iVar5;
  uVar14 = (uint)uStack_70 + uStack_50._4_4_ +
           ((uStack_70._4_4_ >> 0x12 | uStack_70._4_4_ << 0xe) ^
            (uStack_70._4_4_ >> 7 | uStack_70._4_4_ << 0x19) ^ uStack_70._4_4_ >> 3) +
           ((uVar3 >> 0x13 | uVar3 * 0x2000) ^ (uVar3 >> 0x11 | uVar3 * 0x8000) ^ uVar3 >> 10);
  iVar5 = uVar23 + 0xfc19dc6 + uVar14 +
          ((uVar16 >> 0xb | uVar16 * 0x200000) ^ (uVar16 >> 6 | uVar16 * 0x4000000) ^
          (uVar16 >> 0x19 | uVar16 * 0x80)) + (~uVar16 & uVar20 | uVar16 & uVar21);
  uVar7 = uVar7 + iVar5;
  uVar26 = ((uVar28 >> 0xd | uVar28 * 0x80000) ^ (uVar28 >> 2 | uVar28 * 0x40000000) ^
           (uVar28 >> 0x16 | uVar28 * 0x400)) + ((uVar30 | uVar6) & uVar28 | uVar30 & uVar6) + iVar5
  ;
  uVar1 = uStack_70._4_4_ + (uint)local_48 +
          (((uint)local_68 >> 0x12 | (uint)local_68 << 0xe) ^
           ((uint)local_68 >> 7 | (uint)local_68 << 0x19) ^ (uint)local_68 >> 3) +
          ((uVar29 >> 0x13 | uVar29 * 0x2000) ^ (uVar29 >> 0x11 | uVar29 * 0x8000) ^ uVar29 >> 10);
  iVar5 = uVar20 + 0x240ca1cc + uVar1 +
          ((uVar7 >> 0xb | uVar7 * 0x200000) ^ (uVar7 >> 6 | uVar7 * 0x4000000) ^
          (uVar7 >> 0x19 | uVar7 * 0x80)) + (~uVar7 & uVar21 | uVar7 & uVar16);
  uVar6 = iVar5 + uVar6;
  uVar20 = ((uVar26 >> 0xd | uVar26 * 0x80000) ^ (uVar26 >> 2 | uVar26 * 0x40000000) ^
           (uVar26 >> 0x16 | uVar26 * 0x400)) + ((uVar28 | uVar30) & uVar26 | uVar28 & uVar30) +
           iVar5;
  uVar25 = local_48._4_4_ + (uint)local_68 +
           ((local_68._4_4_ >> 0x12 | local_68._4_4_ << 0xe) ^
            (local_68._4_4_ >> 7 | local_68._4_4_ << 0x19) ^ local_68._4_4_ >> 3) +
           ((uVar14 >> 0x13 | uVar14 * 0x2000) ^ (uVar14 >> 0x11 | uVar14 * 0x8000) ^ uVar14 >> 10);
  iVar5 = uVar21 + 0x2de92c6f + uVar25 +
          ((uVar6 >> 0xb | uVar6 * 0x200000) ^ (uVar6 >> 6 | uVar6 * 0x4000000) ^
          (uVar6 >> 0x19 | uVar6 * 0x80)) + (~uVar6 & uVar16 | uVar6 & uVar7);
  uVar30 = iVar5 + uVar30;
  uVar27 = ((uVar20 >> 0xd | uVar20 * 0x80000) ^ (uVar20 >> 2 | uVar20 * 0x40000000) ^
           (uVar20 >> 0x16 | uVar20 * 0x400)) + ((uVar26 | uVar28) & uVar20 | uVar26 & uVar28) +
           iVar5;
  uVar10 = (uint)uStack_40 + local_68._4_4_ +
           (((uint)uStack_60 >> 0x12 | (uint)uStack_60 << 0xe) ^
            ((uint)uStack_60 >> 7 | (uint)uStack_60 << 0x19) ^ (uint)uStack_60 >> 3) +
           ((uVar1 >> 0x13 | uVar1 * 0x2000) ^ (uVar1 >> 0x11 | uVar1 * 0x8000) ^ uVar1 >> 10);
  iVar5 = uVar16 + 0x4a7484aa + uVar10 +
          ((uVar30 >> 0xb | uVar30 * 0x200000) ^ (uVar30 >> 6 | uVar30 * 0x4000000) ^
          (uVar30 >> 0x19 | uVar30 * 0x80)) + (~uVar30 & uVar7 | uVar30 & uVar6);
  uVar28 = iVar5 + uVar28;
  uVar16 = ((uVar27 >> 0xd | uVar27 * 0x80000) ^ (uVar27 >> 2 | uVar27 * 0x40000000) ^
           (uVar27 >> 0x16 | uVar27 * 0x400)) + ((uVar20 | uVar26) & uVar27 | uVar20 & uVar26) +
           iVar5;
  uVar23 = uStack_40._4_4_ + (uint)uStack_60 +
           ((uStack_60._4_4_ >> 0x12 | uStack_60._4_4_ << 0xe) ^
            (uStack_60._4_4_ >> 7 | uStack_60._4_4_ << 0x19) ^ uStack_60._4_4_ >> 3) +
           ((uVar25 >> 0x13 | uVar25 * 0x2000) ^ (uVar25 >> 0x11 | uVar25 * 0x8000) ^ uVar25 >> 10);
  iVar5 = uVar7 + 0x5cb0a9dc + uVar23 +
          ((uVar28 >> 0xb | uVar28 * 0x200000) ^ (uVar28 >> 6 | uVar28 * 0x4000000) ^
          (uVar28 >> 0x19 | uVar28 * 0x80)) + (~uVar28 & uVar6 | uVar28 & uVar30);
  uVar26 = iVar5 + uVar26;
  uVar7 = ((uVar16 >> 0xd | uVar16 * 0x80000) ^ (uVar16 >> 2 | uVar16 * 0x40000000) ^
          (uVar16 >> 0x16 | uVar16 * 0x400)) + ((uVar27 | uVar20) & uVar16 | uVar27 & uVar20) +
          iVar5;
  uVar2 = (((uint)local_58 >> 0x12 | (uint)local_58 << 0xe) ^
           ((uint)local_58 >> 7 | (uint)local_58 << 0x19) ^ (uint)local_58 >> 3) + uStack_60._4_4_ +
          uVar3 + ((uVar10 >> 0x13 | uVar10 * 0x2000) ^ (uVar10 >> 0x11 | uVar10 * 0x8000) ^
                  uVar10 >> 10);
  iVar5 = uVar6 + 0x76f988da + uVar2 +
          ((uVar26 >> 0xb | uVar26 * 0x200000) ^ (uVar26 >> 6 | uVar26 * 0x4000000) ^
          (uVar26 >> 0x19 | uVar26 * 0x80)) + (~uVar26 & uVar30 | uVar26 & uVar28);
  uVar20 = iVar5 + uVar20;
  uVar21 = ((uVar7 >> 0xd | uVar7 * 0x80000) ^ (uVar7 >> 2 | uVar7 * 0x40000000) ^
           (uVar7 >> 0x16 | uVar7 * 0x400)) + ((uVar16 | uVar27) & uVar7 | uVar16 & uVar27) + iVar5;
  uVar6 = ((local_58._4_4_ >> 0x12 | local_58._4_4_ << 0xe) ^
           (local_58._4_4_ >> 7 | local_58._4_4_ << 0x19) ^ local_58._4_4_ >> 3) + (uint)local_58 +
          uVar29 + ((uVar23 >> 0x13 | uVar23 * 0x2000) ^ (uVar23 >> 0x11 | uVar23 * 0x8000) ^
                   uVar23 >> 10);
  iVar5 = uVar30 + 0x983e5152 + uVar6 +
          ((uVar20 >> 0xb | uVar20 * 0x200000) ^ (uVar20 >> 6 | uVar20 * 0x4000000) ^
          (uVar20 >> 0x19 | uVar20 * 0x80)) + (~uVar20 & uVar28 | uVar20 & uVar26);
  uVar27 = iVar5 + uVar27;
  uVar22 = ((uVar21 >> 0xd | uVar21 * 0x80000) ^ (uVar21 >> 2 | uVar21 * 0x40000000) ^
           (uVar21 >> 0x16 | uVar21 * 0x400)) + ((uVar7 | uVar16) & uVar21 | uVar7 & uVar16) + iVar5
  ;
  uVar30 = (((uint)uStack_50 >> 0x12 | (uint)uStack_50 << 0xe) ^
            ((uint)uStack_50 >> 7 | (uint)uStack_50 << 0x19) ^ (uint)uStack_50 >> 3) +
           local_58._4_4_ + uVar14 +
           ((uVar2 >> 0x13 | uVar2 * 0x2000) ^ (uVar2 >> 0x11 | uVar2 * 0x8000) ^ uVar2 >> 10);
  iVar5 = uVar28 + 0xa831c66d + uVar30 +
          ((uVar27 >> 0xb | uVar27 * 0x200000) ^ (uVar27 >> 6 | uVar27 * 0x4000000) ^
          (uVar27 >> 0x19 | uVar27 * 0x80)) + (~uVar27 & uVar26 | uVar27 & uVar20);
  uVar16 = iVar5 + uVar16;
  uVar8 = ((uVar22 >> 0xd | uVar22 * 0x80000) ^ (uVar22 >> 2 | uVar22 * 0x40000000) ^
          (uVar22 >> 0x16 | uVar22 * 0x400)) + ((uVar21 | uVar7) & uVar22 | uVar21 & uVar7) + iVar5;
  uVar28 = ((uStack_50._4_4_ >> 0x12 | uStack_50._4_4_ << 0xe) ^
            (uStack_50._4_4_ >> 7 | uStack_50._4_4_ << 0x19) ^ uStack_50._4_4_ >> 3) +
           (uint)uStack_50 + uVar1 +
           ((uVar6 >> 0x13 | uVar6 * 0x2000) ^ (uVar6 >> 0x11 | uVar6 * 0x8000) ^ uVar6 >> 10);
  iVar5 = uVar26 + 0xb00327c8 + uVar28 +
          ((uVar16 >> 0xb | uVar16 * 0x200000) ^ (uVar16 >> 6 | uVar16 * 0x4000000) ^
          (uVar16 >> 0x19 | uVar16 * 0x80)) + (~uVar16 & uVar20 | uVar16 & uVar27);
  uVar7 = iVar5 + uVar7;
  uVar11 = ((uVar8 >> 0xd | uVar8 * 0x80000) ^ (uVar8 >> 2 | uVar8 * 0x40000000) ^
           (uVar8 >> 0x16 | uVar8 * 0x400)) + ((uVar22 | uVar21) & uVar8 | uVar22 & uVar21) + iVar5;
  uVar26 = (((uint)local_48 >> 0x12 | (uint)local_48 << 0xe) ^
            ((uint)local_48 >> 7 | (uint)local_48 << 0x19) ^ (uint)local_48 >> 3) + uStack_50._4_4_
           + uVar25 +
           ((uVar30 >> 0x13 | uVar30 * 0x2000) ^ (uVar30 >> 0x11 | uVar30 * 0x8000) ^ uVar30 >> 10);
  iVar5 = uVar20 + 0xbf597fc7 + uVar26 +
          ((uVar7 >> 0xb | uVar7 * 0x200000) ^ (uVar7 >> 6 | uVar7 * 0x4000000) ^
          (uVar7 >> 0x19 | uVar7 * 0x80)) + (~uVar7 & uVar27 | uVar7 & uVar16);
  uVar21 = iVar5 + uVar21;
  uVar17 = ((uVar11 >> 0xd | uVar11 * 0x80000) ^ (uVar11 >> 2 | uVar11 * 0x40000000) ^
           (uVar11 >> 0x16 | uVar11 * 0x400)) + ((uVar8 | uVar22) & uVar11 | uVar8 & uVar22) + iVar5
  ;
  uVar20 = ((local_48._4_4_ >> 0x12 | local_48._4_4_ << 0xe) ^
            (local_48._4_4_ >> 7 | local_48._4_4_ << 0x19) ^ local_48._4_4_ >> 3) + (uint)local_48 +
           uVar10 + ((uVar28 >> 0x13 | uVar28 * 0x2000) ^ (uVar28 >> 0x11 | uVar28 * 0x8000) ^
                    uVar28 >> 10);
  iVar5 = uVar27 + 0xc6e00bf3 + uVar20 +
          ((uVar21 >> 0xb | uVar21 * 0x200000) ^ (uVar21 >> 6 | uVar21 * 0x4000000) ^
          (uVar21 >> 0x19 | uVar21 * 0x80)) + (~uVar21 & uVar16 | uVar21 & uVar7);
  uVar22 = iVar5 + uVar22;
  uVar13 = ((uVar17 >> 0xd | uVar17 * 0x80000) ^ (uVar17 >> 2 | uVar17 * 0x40000000) ^
           (uVar17 >> 0x16 | uVar17 * 0x400)) + ((uVar11 | uVar8) & uVar17 | uVar11 & uVar8) + iVar5
  ;
  uVar27 = (((uint)uStack_40 >> 0x12 | (uint)uStack_40 << 0xe) ^
            ((uint)uStack_40 >> 7 | (uint)uStack_40 << 0x19) ^ (uint)uStack_40 >> 3) +
           local_48._4_4_ + uVar23 +
           ((uVar26 >> 0x13 | uVar26 * 0x2000) ^ (uVar26 >> 0x11 | uVar26 * 0x8000) ^ uVar26 >> 10);
  iVar5 = uVar16 + 0xd5a79147 + uVar27 +
          ((uVar22 >> 0xb | uVar22 * 0x200000) ^ (uVar22 >> 6 | uVar22 * 0x4000000) ^
          (uVar22 >> 0x19 | uVar22 * 0x80)) + (~uVar22 & uVar7 | uVar22 & uVar21);
  uVar8 = iVar5 + uVar8;
  uVar16 = ((uVar13 >> 0xd | uVar13 * 0x80000) ^ (uVar13 >> 2 | uVar13 * 0x40000000) ^
           (uVar13 >> 0x16 | uVar13 * 0x400)) + ((uVar17 | uVar11) & uVar13 | uVar17 & uVar11) +
           iVar5;
  uVar19 = ((uStack_40._4_4_ >> 0x12 | uStack_40._4_4_ << 0xe) ^
            (uStack_40._4_4_ >> 7 | uStack_40._4_4_ << 0x19) ^ uStack_40._4_4_ >> 3) +
           (uint)uStack_40 + uVar2 +
           ((uVar20 >> 0x13 | uVar20 * 0x2000) ^ (uVar20 >> 0x11 | uVar20 * 0x8000) ^ uVar20 >> 10);
  iVar5 = uVar7 + 0x6ca6351 + uVar19 +
          ((uVar8 >> 0xb | uVar8 * 0x200000) ^ (uVar8 >> 6 | uVar8 * 0x4000000) ^
          (uVar8 >> 0x19 | uVar8 * 0x80)) + (~uVar8 & uVar21 | uVar8 & uVar22);
  uVar11 = iVar5 + uVar11;
  uVar9 = ((uVar16 >> 0xd | uVar16 * 0x80000) ^ (uVar16 >> 2 | uVar16 * 0x40000000) ^
          (uVar16 >> 0x16 | uVar16 * 0x400)) + ((uVar13 | uVar17) & uVar16 | uVar13 & uVar17) +
          iVar5;
  uVar7 = ((uVar3 >> 0x12 | uVar3 * 0x4000) ^ (uVar3 >> 7 | uVar3 * 0x2000000) ^ uVar3 >> 3) +
          uStack_40._4_4_ + uVar6 +
          ((uVar27 >> 0x13 | uVar27 * 0x2000) ^ (uVar27 >> 0x11 | uVar27 * 0x8000) ^ uVar27 >> 10);
  iVar5 = uVar21 + 0x14292967 + uVar7 +
          ((uVar11 >> 0xb | uVar11 * 0x200000) ^ (uVar11 >> 6 | uVar11 * 0x4000000) ^
          (uVar11 >> 0x19 | uVar11 * 0x80)) + (~uVar11 & uVar22 | uVar11 & uVar8);
  uVar17 = iVar5 + uVar17;
  uVar12 = ((uVar9 >> 0xd | uVar9 * 0x80000) ^ (uVar9 >> 2 | uVar9 * 0x40000000) ^
           (uVar9 >> 0x16 | uVar9 * 0x400)) + ((uVar16 | uVar13) & uVar9 | uVar16 & uVar13) + iVar5;
  uVar4 = ((uVar29 >> 0x12 | uVar29 * 0x4000) ^ (uVar29 >> 7 | uVar29 * 0x2000000) ^ uVar29 >> 3) +
          uVar3 + uVar30 +
          ((uVar19 >> 0x13 | uVar19 * 0x2000) ^ (uVar19 >> 0x11 | uVar19 * 0x8000) ^ uVar19 >> 10);
  iVar5 = uVar22 + 0x27b70a85 + uVar4 +
          ((uVar17 >> 0xb | uVar17 * 0x200000) ^ (uVar17 >> 6 | uVar17 * 0x4000000) ^
          (uVar17 >> 0x19 | uVar17 * 0x80)) + (~uVar17 & uVar8 | uVar17 & uVar11);
  uVar13 = iVar5 + uVar13;
  uVar22 = ((uVar12 >> 0xd | uVar12 * 0x80000) ^ (uVar12 >> 2 | uVar12 * 0x40000000) ^
           (uVar12 >> 0x16 | uVar12 * 0x400)) + ((uVar9 | uVar16) & uVar12 | uVar9 & uVar16) + iVar5
  ;
  uVar21 = ((uVar14 >> 0x12 | uVar14 * 0x4000) ^ (uVar14 >> 7 | uVar14 * 0x2000000) ^ uVar14 >> 3) +
           uVar29 + uVar28 +
           ((uVar7 >> 0x13 | uVar7 * 0x2000) ^ (uVar7 >> 0x11 | uVar7 * 0x8000) ^ uVar7 >> 10);
  iVar5 = uVar8 + 0x2e1b2138 + uVar21 +
          ((uVar13 >> 0xb | uVar13 * 0x200000) ^ (uVar13 >> 6 | uVar13 * 0x4000000) ^
          (uVar13 >> 0x19 | uVar13 * 0x80)) + (~uVar13 & uVar11 | uVar13 & uVar17);
  uVar16 = iVar5 + uVar16;
  uVar8 = ((uVar22 >> 0xd | uVar22 * 0x80000) ^ (uVar22 >> 2 | uVar22 * 0x40000000) ^
          (uVar22 >> 0x16 | uVar22 * 0x400)) + ((uVar12 | uVar9) & uVar22 | uVar12 & uVar9) + iVar5;
  uVar15 = ((uVar1 >> 0x12 | uVar1 * 0x4000) ^ (uVar1 >> 7 | uVar1 * 0x2000000) ^ uVar1 >> 3) +
           uVar14 + uVar26 +
           ((uVar4 >> 0x13 | uVar4 * 0x2000) ^ (uVar4 >> 0x11 | uVar4 * 0x8000) ^ uVar4 >> 10);
  iVar5 = uVar11 + 0x4d2c6dfc + uVar15 +
          ((uVar16 >> 0xb | uVar16 * 0x200000) ^ (uVar16 >> 6 | uVar16 * 0x4000000) ^
          (uVar16 >> 0x19 | uVar16 * 0x80)) + (~uVar16 & uVar17 | uVar16 & uVar13);
  uVar9 = iVar5 + uVar9;
  uVar11 = ((uVar8 >> 0xd | uVar8 * 0x80000) ^ (uVar8 >> 2 | uVar8 * 0x40000000) ^
           (uVar8 >> 0x16 | uVar8 * 0x400)) + ((uVar22 | uVar12) & uVar8 | uVar22 & uVar12) + iVar5;
  uVar29 = ((uVar25 >> 0x12 | uVar25 * 0x4000) ^ (uVar25 >> 7 | uVar25 * 0x2000000) ^ uVar25 >> 3) +
           uVar1 + uVar20 +
           ((uVar21 >> 0x13 | uVar21 * 0x2000) ^ (uVar21 >> 0x11 | uVar21 * 0x8000) ^ uVar21 >> 10);
  iVar5 = uVar17 + 0x53380d13 + uVar29 +
          ((uVar9 >> 0xb | uVar9 * 0x200000) ^ (uVar9 >> 6 | uVar9 * 0x4000000) ^
          (uVar9 >> 0x19 | uVar9 * 0x80)) + (~uVar9 & uVar13 | uVar9 & uVar16);
  uVar12 = iVar5 + uVar12;
  uVar17 = ((uVar11 >> 0xd | uVar11 * 0x80000) ^ (uVar11 >> 2 | uVar11 * 0x40000000) ^
           (uVar11 >> 0x16 | uVar11 * 0x400)) + ((uVar8 | uVar22) & uVar11 | uVar8 & uVar22) + iVar5
  ;
  uVar1 = ((uVar10 >> 0x12 | uVar10 * 0x4000) ^ (uVar10 >> 7 | uVar10 * 0x2000000) ^ uVar10 >> 3) +
          uVar25 + uVar27 +
          ((uVar15 >> 0x13 | uVar15 * 0x2000) ^ (uVar15 >> 0x11 | uVar15 * 0x8000) ^ uVar15 >> 10);
  iVar5 = uVar13 + 0x650a7354 + uVar1 +
          ((uVar12 >> 0xb | uVar12 * 0x200000) ^ (uVar12 >> 6 | uVar12 * 0x4000000) ^
          (uVar12 >> 0x19 | uVar12 * 0x80)) + (~uVar12 & uVar16 | uVar12 & uVar9);
  uVar22 = iVar5 + uVar22;
  uVar13 = ((uVar17 >> 0xd | uVar17 * 0x80000) ^ (uVar17 >> 2 | uVar17 * 0x40000000) ^
           (uVar17 >> 0x16 | uVar17 * 0x400)) + ((uVar11 | uVar8) & uVar17 | uVar11 & uVar8) + iVar5
  ;
  uVar25 = ((uVar23 >> 0x12 | uVar23 * 0x4000) ^ (uVar23 >> 7 | uVar23 * 0x2000000) ^ uVar23 >> 3) +
           uVar10 + uVar19 +
           ((uVar29 >> 0x13 | uVar29 * 0x2000) ^ (uVar29 >> 0x11 | uVar29 * 0x8000) ^ uVar29 >> 10);
  iVar5 = uVar16 + 0x766a0abb + uVar25 +
          ((uVar22 >> 0xb | uVar22 * 0x200000) ^ (uVar22 >> 6 | uVar22 * 0x4000000) ^
          (uVar22 >> 0x19 | uVar22 * 0x80)) + (~uVar22 & uVar9 | uVar22 & uVar12);
  uVar8 = iVar5 + uVar8;
  uVar16 = ((uVar13 >> 0xd | uVar13 * 0x80000) ^ (uVar13 >> 2 | uVar13 * 0x40000000) ^
           (uVar13 >> 0x16 | uVar13 * 0x400)) + ((uVar17 | uVar11) & uVar13 | uVar17 & uVar11) +
           iVar5;
  uVar10 = ((uVar2 >> 0x12 | uVar2 * 0x4000) ^ (uVar2 >> 7 | uVar2 * 0x2000000) ^ uVar2 >> 3) +
           uVar23 + uVar7 +
           ((uVar1 >> 0x13 | uVar1 * 0x2000) ^ (uVar1 >> 0x11 | uVar1 * 0x8000) ^ uVar1 >> 10);
  iVar5 = uVar9 + 0x81c2c92e + uVar10 +
          ((uVar8 >> 0xb | uVar8 * 0x200000) ^ (uVar8 >> 6 | uVar8 * 0x4000000) ^
          (uVar8 >> 0x19 | uVar8 * 0x80)) + (~uVar8 & uVar12 | uVar8 & uVar22);
  uVar11 = iVar5 + uVar11;
  uVar3 = ((uVar16 >> 0xd | uVar16 * 0x80000) ^ (uVar16 >> 2 | uVar16 * 0x40000000) ^
          (uVar16 >> 0x16 | uVar16 * 0x400)) + ((uVar13 | uVar17) & uVar16 | uVar13 & uVar17) +
          iVar5;
  uVar23 = ((uVar6 >> 0x12 | uVar6 * 0x4000) ^ (uVar6 >> 7 | uVar6 * 0x2000000) ^ uVar6 >> 3) +
           uVar2 + uVar4 +
           ((uVar25 >> 0x13 | uVar25 * 0x2000) ^ (uVar25 >> 0x11 | uVar25 * 0x8000) ^ uVar25 >> 10);
  iVar5 = uVar12 + 0x92722c85 + uVar23 +
          ((uVar11 >> 0xb | uVar11 * 0x200000) ^ (uVar11 >> 6 | uVar11 * 0x4000000) ^
          (uVar11 >> 0x19 | uVar11 * 0x80)) + (~uVar11 & uVar22 | uVar11 & uVar8);
  uVar17 = iVar5 + uVar17;
  uVar9 = ((uVar3 >> 0xd | uVar3 * 0x80000) ^ (uVar3 >> 2 | uVar3 * 0x40000000) ^
          (uVar3 >> 0x16 | uVar3 * 0x400)) + ((uVar16 | uVar13) & uVar3 | uVar16 & uVar13) + iVar5;
  uVar6 = ((uVar30 >> 0x12 | uVar30 * 0x4000) ^ (uVar30 >> 7 | uVar30 * 0x2000000) ^ uVar30 >> 3) +
          uVar6 + uVar21 +
          ((uVar10 >> 0x13 | uVar10 * 0x2000) ^ (uVar10 >> 0x11 | uVar10 * 0x8000) ^ uVar10 >> 10);
  iVar5 = uVar22 + 0xa2bfe8a1 + uVar6 +
          ((uVar17 >> 0xb | uVar17 * 0x200000) ^ (uVar17 >> 6 | uVar17 * 0x4000000) ^
          (uVar17 >> 0x19 | uVar17 * 0x80)) + (~uVar17 & uVar8 | uVar17 & uVar11);
  uVar13 = iVar5 + uVar13;
  uVar12 = ((uVar9 >> 0xd | uVar9 * 0x80000) ^ (uVar9 >> 2 | uVar9 * 0x40000000) ^
           (uVar9 >> 0x16 | uVar9 * 0x400)) + ((uVar3 | uVar16) & uVar9 | uVar3 & uVar16) + iVar5;
  uVar2 = ((uVar28 >> 0x12 | uVar28 * 0x4000) ^ (uVar28 >> 7 | uVar28 * 0x2000000) ^ uVar28 >> 3) +
          uVar30 + uVar15 +
          ((uVar23 >> 0x13 | uVar23 * 0x2000) ^ (uVar23 >> 0x11 | uVar23 * 0x8000) ^ uVar23 >> 10);
  iVar5 = uVar8 + 0xa81a664b + uVar2 +
          ((uVar13 >> 0xb | uVar13 * 0x200000) ^ (uVar13 >> 6 | uVar13 * 0x4000000) ^
          (uVar13 >> 0x19 | uVar13 * 0x80)) + (~uVar13 & uVar11 | uVar13 & uVar17);
  uVar16 = iVar5 + uVar16;
  uVar8 = ((uVar12 >> 0xd | uVar12 * 0x80000) ^ (uVar12 >> 2 | uVar12 * 0x40000000) ^
          (uVar12 >> 0x16 | uVar12 * 0x400)) + ((uVar9 | uVar3) & uVar12 | uVar9 & uVar3) + iVar5;
  uVar30 = ((uVar26 >> 0x12 | uVar26 * 0x4000) ^ (uVar26 >> 7 | uVar26 * 0x2000000) ^ uVar26 >> 3) +
           uVar28 + uVar29 +
           ((uVar6 >> 0x13 | uVar6 * 0x2000) ^ (uVar6 >> 0x11 | uVar6 * 0x8000) ^ uVar6 >> 10);
  iVar5 = uVar11 + 0xc24b8b70 + uVar30 +
          ((uVar16 >> 0xb | uVar16 * 0x200000) ^ (uVar16 >> 6 | uVar16 * 0x4000000) ^
          (uVar16 >> 0x19 | uVar16 * 0x80)) + (~uVar16 & uVar17 | uVar16 & uVar13);
  uVar3 = iVar5 + uVar3;
  uVar28 = ((uVar8 >> 0xd | uVar8 * 0x80000) ^ (uVar8 >> 2 | uVar8 * 0x40000000) ^
           (uVar8 >> 0x16 | uVar8 * 0x400)) + ((uVar12 | uVar9) & uVar8 | uVar12 & uVar9) + iVar5;
  uVar22 = ((uVar20 >> 0x12 | uVar20 * 0x4000) ^ (uVar20 >> 7 | uVar20 * 0x2000000) ^ uVar20 >> 3) +
           uVar26 + uVar1 +
           ((uVar2 >> 0x13 | uVar2 * 0x2000) ^ (uVar2 >> 0x11 | uVar2 * 0x8000) ^ uVar2 >> 10);
  iVar5 = uVar17 + 0xc76c51a3 + uVar22 +
          ((uVar3 >> 0xb | uVar3 * 0x200000) ^ (uVar3 >> 6 | uVar3 * 0x4000000) ^
          (uVar3 >> 0x19 | uVar3 * 0x80)) + (~uVar3 & uVar13 | uVar3 & uVar16);
  uVar9 = iVar5 + uVar9;
  uVar14 = ((uVar28 >> 0xd | uVar28 * 0x80000) ^ (uVar28 >> 2 | uVar28 * 0x40000000) ^
           (uVar28 >> 0x16 | uVar28 * 0x400)) + ((uVar8 | uVar12) & uVar28 | uVar8 & uVar12) + iVar5
  ;
  uVar24 = ((uVar27 >> 0x12 | uVar27 * 0x4000) ^ (uVar27 >> 7 | uVar27 * 0x2000000) ^ uVar27 >> 3) +
           uVar20 + uVar25 +
           ((uVar30 >> 0x13 | uVar30 * 0x2000) ^ (uVar30 >> 0x11 | uVar30 * 0x8000) ^ uVar30 >> 10);
  iVar5 = uVar13 + 0xd192e819 + uVar24 +
          ((uVar9 >> 0xb | uVar9 * 0x200000) ^ (uVar9 >> 6 | uVar9 * 0x4000000) ^
          (uVar9 >> 0x19 | uVar9 * 0x80)) + (~uVar9 & uVar16 | uVar9 & uVar3);
  uVar12 = iVar5 + uVar12;
  uVar26 = ((uVar14 >> 0xd | uVar14 * 0x80000) ^ (uVar14 >> 2 | uVar14 * 0x40000000) ^
           (uVar14 >> 0x16 | uVar14 * 0x400)) + ((uVar28 | uVar8) & uVar14 | uVar28 & uVar8) + iVar5
  ;
  uVar20 = ((uVar19 >> 0x12 | uVar19 * 0x4000) ^ (uVar19 >> 7 | uVar19 * 0x2000000) ^ uVar19 >> 3) +
           uVar27 + uVar10 +
           ((uVar22 >> 0x13 | uVar22 * 0x2000) ^ (uVar22 >> 0x11 | uVar22 * 0x8000) ^ uVar22 >> 10);
  iVar5 = uVar16 + 0xd6990624 + uVar20 +
          ((uVar12 >> 0xb | uVar12 * 0x200000) ^ (uVar12 >> 6 | uVar12 * 0x4000000) ^
          (uVar12 >> 0x19 | uVar12 * 0x80)) + (~uVar12 & uVar3 | uVar12 & uVar9);
  uVar8 = iVar5 + uVar8;
  uVar11 = ((uVar26 >> 0xd | uVar26 * 0x80000) ^ (uVar26 >> 2 | uVar26 * 0x40000000) ^
           (uVar26 >> 0x16 | uVar26 * 0x400)) + ((uVar14 | uVar28) & uVar26 | uVar14 & uVar28) +
           iVar5;
  uVar18 = ((uVar7 >> 0x12 | uVar7 * 0x4000) ^ (uVar7 >> 7 | uVar7 * 0x2000000) ^ uVar7 >> 3) +
           uVar19 + uVar23 +
           ((uVar24 >> 0x13 | uVar24 * 0x2000) ^ (uVar24 >> 0x11 | uVar24 * 0x8000) ^ uVar24 >> 10);
  iVar5 = uVar3 + 0xf40e3585 + uVar18 +
          ((uVar8 >> 0xb | uVar8 * 0x200000) ^ (uVar8 >> 6 | uVar8 * 0x4000000) ^
          (uVar8 >> 0x19 | uVar8 * 0x80)) + (~uVar8 & uVar9 | uVar8 & uVar12);
  uVar28 = iVar5 + uVar28;
  uVar19 = ((uVar11 >> 0xd | uVar11 * 0x80000) ^ (uVar11 >> 2 | uVar11 * 0x40000000) ^
           (uVar11 >> 0x16 | uVar11 * 0x400)) + ((uVar26 | uVar14) & uVar11 | uVar26 & uVar14) +
           iVar5;
  uVar27 = ((uVar4 >> 0x12 | uVar4 * 0x4000) ^ (uVar4 >> 7 | uVar4 * 0x2000000) ^ uVar4 >> 3) +
           uVar7 + uVar6 +
           ((uVar20 >> 0x13 | uVar20 * 0x2000) ^ (uVar20 >> 0x11 | uVar20 * 0x8000) ^ uVar20 >> 10);
  iVar5 = uVar9 + 0x106aa070 + uVar27 +
          ((uVar28 >> 0xb | uVar28 * 0x200000) ^ (uVar28 >> 6 | uVar28 * 0x4000000) ^
          (uVar28 >> 0x19 | uVar28 * 0x80)) + (~uVar28 & uVar12 | uVar28 & uVar8);
  uVar14 = iVar5 + uVar14;
  uVar7 = ((uVar19 >> 0xd | uVar19 * 0x80000) ^ (uVar19 >> 2 | uVar19 * 0x40000000) ^
          (uVar19 >> 0x16 | uVar19 * 0x400)) + ((uVar11 | uVar26) & uVar19 | uVar11 & uVar26) +
          iVar5;
  uVar16 = ((uVar21 >> 0x12 | uVar21 * 0x4000) ^ (uVar21 >> 7 | uVar21 * 0x2000000) ^ uVar21 >> 3) +
           uVar4 + uVar2 +
           ((uVar18 >> 0x13 | uVar18 * 0x2000) ^ (uVar18 >> 0x11 | uVar18 * 0x8000) ^ uVar18 >> 10);
  iVar5 = uVar12 + 0x19a4c116 + uVar16 +
          ((uVar14 >> 0xb | uVar14 * 0x200000) ^ (uVar14 >> 6 | uVar14 * 0x4000000) ^
          (uVar14 >> 0x19 | uVar14 * 0x80)) + (~uVar14 & uVar8 | uVar14 & uVar28);
  uVar26 = iVar5 + uVar26;
  uVar17 = ((uVar7 >> 0xd | uVar7 * 0x80000) ^ (uVar7 >> 2 | uVar7 * 0x40000000) ^
           (uVar7 >> 0x16 | uVar7 * 0x400)) + ((uVar19 | uVar11) & uVar7 | uVar19 & uVar11) + iVar5;
  uVar12 = ((uVar15 >> 0x12 | uVar15 * 0x4000) ^ (uVar15 >> 7 | uVar15 * 0x2000000) ^ uVar15 >> 3) +
           uVar21 + uVar30 +
           ((uVar27 >> 0x13 | uVar27 * 0x2000) ^ (uVar27 >> 0x11 | uVar27 * 0x8000) ^ uVar27 >> 10);
  iVar5 = uVar8 + 0x1e376c08 + uVar12 +
          ((uVar26 >> 0xb | uVar26 * 0x200000) ^ (uVar26 >> 6 | uVar26 * 0x4000000) ^
          (uVar26 >> 0x19 | uVar26 * 0x80)) + (~uVar26 & uVar28 | uVar26 & uVar14);
  uVar11 = iVar5 + uVar11;
  uVar21 = ((uVar17 >> 0xd | uVar17 * 0x80000) ^ (uVar17 >> 2 | uVar17 * 0x40000000) ^
           (uVar17 >> 0x16 | uVar17 * 0x400)) + ((uVar7 | uVar19) & uVar17 | uVar7 & uVar19) + iVar5
  ;
  uVar13 = ((uVar29 >> 0x12 | uVar29 * 0x4000) ^ (uVar29 >> 7 | uVar29 * 0x2000000) ^ uVar29 >> 3) +
           uVar15 + uVar22 +
           ((uVar16 >> 0x13 | uVar16 * 0x2000) ^ (uVar16 >> 0x11 | uVar16 * 0x8000) ^ uVar16 >> 10);
  iVar5 = uVar28 + 0x2748774c + uVar13 +
          ((uVar11 >> 0xb | uVar11 * 0x200000) ^ (uVar11 >> 6 | uVar11 * 0x4000000) ^
          (uVar11 >> 0x19 | uVar11 * 0x80)) + (~uVar11 & uVar14 | uVar11 & uVar26);
  uVar19 = iVar5 + uVar19;
  uVar8 = ((uVar21 >> 0xd | uVar21 * 0x80000) ^ (uVar21 >> 2 | uVar21 * 0x40000000) ^
          (uVar21 >> 0x16 | uVar21 * 0x400)) + ((uVar17 | uVar7) & uVar21 | uVar17 & uVar7) + iVar5;
  uVar9 = ((uVar1 >> 0x12 | uVar1 * 0x4000) ^ (uVar1 >> 7 | uVar1 * 0x2000000) ^ uVar1 >> 3) +
          uVar29 + uVar24 +
          ((uVar12 >> 0x13 | uVar12 * 0x2000) ^ (uVar12 >> 0x11 | uVar12 * 0x8000) ^ uVar12 >> 10);
  iVar5 = uVar14 + 0x34b0bcb5 + uVar9 +
          ((uVar19 >> 0xb | uVar19 * 0x200000) ^ (uVar19 >> 6 | uVar19 * 0x4000000) ^
          (uVar19 >> 0x19 | uVar19 * 0x80)) + (~uVar19 & uVar26 | uVar19 & uVar11);
  uVar7 = iVar5 + uVar7;
  uVar28 = ((uVar8 >> 0xd | uVar8 * 0x80000) ^ (uVar8 >> 2 | uVar8 * 0x40000000) ^
           (uVar8 >> 0x16 | uVar8 * 0x400)) + ((uVar21 | uVar17) & uVar8 | uVar21 & uVar17) + iVar5;
  uVar1 = ((uVar25 >> 0x12 | uVar25 * 0x4000) ^ (uVar25 >> 7 | uVar25 * 0x2000000) ^ uVar25 >> 3) +
          uVar1 + uVar20 +
          ((uVar13 >> 0x13 | uVar13 * 0x2000) ^ (uVar13 >> 0x11 | uVar13 * 0x8000) ^ uVar13 >> 10);
  iVar5 = uVar26 + 0x391c0cb3 + uVar1 +
          ((uVar7 >> 0xb | uVar7 * 0x200000) ^ (uVar7 >> 6 | uVar7 * 0x4000000) ^
          (uVar7 >> 0x19 | uVar7 * 0x80)) + (~uVar7 & uVar11 | uVar7 & uVar19);
  uVar17 = iVar5 + uVar17;
  uVar26 = ((uVar28 >> 0xd | uVar28 * 0x80000) ^ (uVar28 >> 2 | uVar28 * 0x40000000) ^
           (uVar28 >> 0x16 | uVar28 * 0x400)) + ((uVar8 | uVar21) & uVar28 | uVar8 & uVar21) + iVar5
  ;
  uVar25 = ((uVar10 >> 0x12 | uVar10 * 0x4000) ^ (uVar10 >> 7 | uVar10 * 0x2000000) ^ uVar10 >> 3) +
           uVar25 + uVar18 +
           ((uVar9 >> 0x13 | uVar9 * 0x2000) ^ (uVar9 >> 0x11 | uVar9 * 0x8000) ^ uVar9 >> 10);
  iVar5 = uVar11 + 0x4ed8aa4a + uVar25 +
          ((uVar17 >> 0xb | uVar17 * 0x200000) ^ (uVar17 >> 6 | uVar17 * 0x4000000) ^
          (uVar17 >> 0x19 | uVar17 * 0x80)) + (~uVar17 & uVar19 | uVar17 & uVar7);
  uVar21 = iVar5 + uVar21;
  uVar11 = ((uVar26 >> 0xd | uVar26 * 0x80000) ^ (uVar26 >> 2 | uVar26 * 0x40000000) ^
           (uVar26 >> 0x16 | uVar26 * 0x400)) + ((uVar28 | uVar8) & uVar26 | uVar28 & uVar8) + iVar5
  ;
  uVar10 = ((uVar23 >> 0x12 | uVar23 * 0x4000) ^ (uVar23 >> 7 | uVar23 * 0x2000000) ^ uVar23 >> 3) +
           uVar10 + uVar27 +
           ((uVar1 >> 0x13 | uVar1 * 0x2000) ^ (uVar1 >> 0x11 | uVar1 * 0x8000) ^ uVar1 >> 10);
  iVar5 = uVar19 + 0x5b9cca4f + uVar10 +
          ((uVar21 >> 0xb | uVar21 * 0x200000) ^ (uVar21 >> 6 | uVar21 * 0x4000000) ^
          (uVar21 >> 0x19 | uVar21 * 0x80)) + (~uVar21 & uVar7 | uVar21 & uVar17);
  uVar8 = iVar5 + uVar8;
  uVar19 = ((uVar11 >> 0xd | uVar11 * 0x80000) ^ (uVar11 >> 2 | uVar11 * 0x40000000) ^
           (uVar11 >> 0x16 | uVar11 * 0x400)) + ((uVar26 | uVar28) & uVar11 | uVar26 & uVar28) +
           iVar5;
  uVar23 = ((uVar6 >> 0x12 | uVar6 * 0x4000) ^ (uVar6 >> 7 | uVar6 * 0x2000000) ^ uVar6 >> 3) +
           uVar23 + uVar16 +
           ((uVar25 >> 0x13 | uVar25 * 0x2000) ^ (uVar25 >> 0x11 | uVar25 * 0x8000) ^ uVar25 >> 10);
  iVar5 = uVar7 + 0x682e6ff3 + uVar23 +
          ((uVar8 >> 0xb | uVar8 * 0x200000) ^ (uVar8 >> 6 | uVar8 * 0x4000000) ^
          (uVar8 >> 0x19 | uVar8 * 0x80)) + (~uVar8 & uVar17 | uVar8 & uVar21);
  uVar28 = iVar5 + uVar28;
  uVar7 = ((uVar19 >> 0xd | uVar19 * 0x80000) ^ (uVar19 >> 2 | uVar19 * 0x40000000) ^
          (uVar19 >> 0x16 | uVar19 * 0x400)) + ((uVar11 | uVar26) & uVar19 | uVar11 & uVar26) +
          iVar5;
  uVar12 = ((uVar2 >> 0x12 | uVar2 * 0x4000) ^ (uVar2 >> 7 | uVar2 * 0x2000000) ^ uVar2 >> 3) +
           uVar6 + uVar12 +
           ((uVar10 >> 0x13 | uVar10 * 0x2000) ^ (uVar10 >> 0x11 | uVar10 * 0x8000) ^ uVar10 >> 10);
  iVar5 = uVar17 + 0x748f82ee + uVar12 +
          ((uVar28 >> 0xb | uVar28 * 0x200000) ^ (uVar28 >> 6 | uVar28 * 0x4000000) ^
          (uVar28 >> 0x19 | uVar28 * 0x80)) + (~uVar28 & uVar21 | uVar28 & uVar8);
  uVar26 = iVar5 + uVar26;
  uVar6 = ((uVar7 >> 0xd | uVar7 * 0x80000) ^ (uVar7 >> 2 | uVar7 * 0x40000000) ^
          (uVar7 >> 0x16 | uVar7 * 0x400)) + ((uVar19 | uVar11) & uVar7 | uVar19 & uVar11) + iVar5;
  uVar17 = ((uVar30 >> 0x12 | uVar30 * 0x4000) ^ (uVar30 >> 7 | uVar30 * 0x2000000) ^ uVar30 >> 3) +
           uVar2 + uVar13 +
           ((uVar23 >> 0x13 | uVar23 * 0x2000) ^ (uVar23 >> 0x11 | uVar23 * 0x8000) ^ uVar23 >> 10);
  iVar5 = uVar21 + 0x78a5636f + uVar17 +
          ((uVar26 >> 0xb | uVar26 * 0x200000) ^ (uVar26 >> 6 | uVar26 * 0x4000000) ^
          (uVar26 >> 0x19 | uVar26 * 0x80)) + (~uVar26 & uVar8 | uVar26 & uVar28);
  uVar11 = iVar5 + uVar11;
  uVar2 = ((uVar6 >> 0xd | uVar6 * 0x80000) ^ (uVar6 >> 2 | uVar6 * 0x40000000) ^
          (uVar6 >> 0x16 | uVar6 * 0x400)) + ((uVar7 | uVar19) & uVar6 | uVar7 & uVar19) + iVar5;
  uVar13 = ((uVar22 >> 0x12 | uVar22 * 0x4000) ^ (uVar22 >> 7 | uVar22 * 0x2000000) ^ uVar22 >> 3) +
           uVar30 + uVar9 +
           ((uVar12 >> 0x13 | uVar12 * 0x2000) ^ (uVar12 >> 0x11 | uVar12 * 0x8000) ^ uVar12 >> 10);
  iVar5 = uVar8 + 0x84c87814 + uVar13 +
          ((uVar11 >> 0xb | uVar11 * 0x200000) ^ (uVar11 >> 6 | uVar11 * 0x4000000) ^
          (uVar11 >> 0x19 | uVar11 * 0x80)) + (~uVar11 & uVar28 | uVar11 & uVar26);
  uVar19 = iVar5 + uVar19;
  uVar30 = ((uVar2 >> 0xd | uVar2 * 0x80000) ^ (uVar2 >> 2 | uVar2 * 0x40000000) ^
           (uVar2 >> 0x16 | uVar2 * 0x400)) + ((uVar6 | uVar7) & uVar2 | uVar6 & uVar7) + iVar5;
  uVar21 = ((uVar24 >> 0x12 | uVar24 * 0x4000) ^ (uVar24 >> 7 | uVar24 * 0x2000000) ^ uVar24 >> 3) +
           uVar22 + uVar1 +
           ((uVar17 >> 0x13 | uVar17 * 0x2000) ^ (uVar17 >> 0x11 | uVar17 * 0x8000) ^ uVar17 >> 10);
  iVar5 = uVar28 + 0x8cc70208 + uVar21 +
          ((uVar19 >> 0xb | uVar19 * 0x200000) ^ (uVar19 >> 6 | uVar19 * 0x4000000) ^
          (uVar19 >> 0x19 | uVar19 * 0x80)) + (~uVar19 & uVar26 | uVar19 & uVar11);
  uVar7 = iVar5 + uVar7;
  uVar28 = ((uVar30 >> 0xd | uVar30 * 0x80000) ^ (uVar30 >> 2 | uVar30 * 0x40000000) ^
           (uVar30 >> 0x16 | uVar30 * 0x400)) + ((uVar2 | uVar6) & uVar30 | uVar2 & uVar6) + iVar5;
  uVar1 = ((uVar20 >> 0x12 | uVar20 * 0x4000) ^ (uVar20 >> 7 | uVar20 * 0x2000000) ^ uVar20 >> 3) +
          uVar24 + uVar25 +
          ((uVar13 >> 0x13 | uVar13 * 0x2000) ^ (uVar13 >> 0x11 | uVar13 * 0x8000) ^ uVar13 >> 10);
  iVar5 = uVar26 + 0x90befffa + uVar1 +
          ((uVar7 >> 0xb | uVar7 * 0x200000) ^ (uVar7 >> 6 | uVar7 * 0x4000000) ^
          (uVar7 >> 0x19 | uVar7 * 0x80)) + (~uVar7 & uVar11 | uVar7 & uVar19);
  uVar6 = iVar5 + uVar6;
  uVar26 = ((uVar28 >> 0xd | uVar28 * 0x80000) ^ (uVar28 >> 2 | uVar28 * 0x40000000) ^
           (uVar28 >> 0x16 | uVar28 * 0x400)) + ((uVar30 | uVar2) & uVar28 | uVar30 & uVar2) + iVar5
  ;
  uVar10 = ((uVar18 >> 0x12 | uVar18 * 0x4000) ^ (uVar18 >> 7 | uVar18 * 0x2000000) ^ uVar18 >> 3) +
           uVar20 + uVar10 +
           ((uVar21 >> 0x13 | uVar21 * 0x2000) ^ (uVar21 >> 0x11 | uVar21 * 0x8000) ^ uVar21 >> 10);
  iVar5 = uVar11 + 0xa4506ceb + uVar10 +
          ((uVar6 >> 0xb | uVar6 * 0x200000) ^ (uVar6 >> 6 | uVar6 * 0x4000000) ^
          (uVar6 >> 0x19 | uVar6 * 0x80)) + (~uVar6 & uVar19 | uVar6 & uVar7);
  uVar2 = iVar5 + uVar2;
  uVar25 = ((uVar26 >> 0xd | uVar26 * 0x80000) ^ (uVar26 >> 2 | uVar26 * 0x40000000) ^
           (uVar26 >> 0x16 | uVar26 * 0x400)) + ((uVar28 | uVar30) & uVar26 | uVar28 & uVar30) +
           iVar5;
  iVar5 = uVar18 + 0xbef9a3f7 +
          ((uVar27 >> 0x12 | uVar27 * 0x4000) ^ (uVar27 >> 7 | uVar27 * 0x2000000) ^ uVar27 >> 3) +
          uVar23 + ((uVar1 >> 0x13 | uVar1 * 0x2000) ^ (uVar1 >> 0x11 | uVar1 * 0x8000) ^
                   uVar1 >> 10) + uVar19 +
          ((uVar2 >> 0xb | uVar2 * 0x200000) ^ (uVar2 >> 6 | uVar2 * 0x4000000) ^
          (uVar2 >> 0x19 | uVar2 * 0x80)) + (~uVar2 & uVar7 | uVar2 & uVar6);
  uVar30 = iVar5 + uVar30;
  uVar1 = ((uVar25 >> 0xd | uVar25 * 0x80000) ^ (uVar25 >> 2 | uVar25 * 0x40000000) ^
          (uVar25 >> 0x16 | uVar25 * 0x400)) + ((uVar26 | uVar28) & uVar25 | uVar26 & uVar28) +
          iVar5;
  iVar5 = uVar27 + 0xc67178f2 +
          ((uVar16 >> 0x12 | uVar16 * 0x4000) ^ (uVar16 >> 7 | uVar16 * 0x2000000) ^ uVar16 >> 3) +
          uVar12 + ((uVar10 >> 0x13 | uVar10 * 0x2000) ^ (uVar10 >> 0x11 | uVar10 * 0x8000) ^
                   uVar10 >> 10) + uVar7 +
          ((uVar30 >> 0xb | uVar30 * 0x200000) ^ (uVar30 >> 6 | uVar30 * 0x4000000) ^
          (uVar30 >> 0x19 | uVar30 * 0x80)) + (~uVar30 & uVar6 | uVar30 & uVar2);
  param_1[0x12] =
       ((uVar25 | uVar26) & uVar1 | uVar25 & uVar26) + param_1[0x12] +
       ((uVar1 >> 0xd | uVar1 * 0x80000) ^ (uVar1 >> 2 | uVar1 * 0x40000000) ^
       (uVar1 >> 0x16 | uVar1 * 0x400)) + iVar5;
  param_1[0x1b] = 0;
  param_1[0x14] = uVar25 + param_1[0x14];
  param_1[0x18] = uVar2 + param_1[0x18];
  param_1[0x16] = uVar28 + param_1[0x16] + iVar5;
  param_1[0x13] = uVar1 + param_1[0x13];
  param_1[0x15] = uVar26 + param_1[0x15];
  param_1[0x17] = uVar30 + param_1[0x17];
  param_1[0x19] = uVar6 + param_1[0x19];
  return;
}

void isaac(void)

{
  uint uVar1;
  uint uVar2;
  long lVar3;
  uint uVar4;
  uint uVar5;
  
  lVar3 = 0;
  uVar2 = 0;
  rand_c = rand_c + 1;
  rand_b = rand_c + rand_b;
  do {
    uVar1 = *(uint *)(randmem + lVar3);
    uVar5 = uVar2 & 3;
    if (uVar5 == 2) {
      uVar5 = rand_a * 4 ^ rand_a;
    }
    else if (uVar5 == 3) {
      uVar5 = (uint)rand_a >> 0x10 ^ rand_a;
    }
    else if (uVar5 == 1) {
      uVar5 = (uint)rand_a >> 6 ^ rand_a;
    }
    else {
      uVar5 = rand_a << 0xd ^ rand_a;
    }
    uVar4 = uVar2 + 0x80;
    uVar2 = uVar2 + 1;
    rand_a = uVar5 + *(int *)(randmem + (ulong)(uVar4 & 0xff) * 4);
    uVar5 = rand_b + *(int *)(randmem + (ulong)(uVar1 >> 2 & 0xff) * 4) + rand_a;
    *(uint *)(randmem + lVar3) = uVar5;
    rand_b = *(int *)(randmem + (ulong)(uVar5 >> 10 & 0xff) * 4) + uVar1;
    *(int *)(randrsl + lVar3) = rand_b;
    lVar3 = lVar3 + 4;
  } while (uVar2 != 0x100);
  return;
}

void randinit(int param_1)

{
  int iVar1;
  uint uVar2;
  int iVar3;
  uint uVar4;
  uint uVar5;
  uint uVar6;
  uint uVar7;
  long lVar8;
  byte *pbVar9;
  uint uVar10;
  uint uVar11;
  int iVar12;
  uint uVar13;
  ulong uVar14;
  long lVar15;
  int iVar16;
  int iVar17;
  int local_4c;
  int local_48;
  int local_44;
  int local_40;
  
  rand_c = 0;
  rand_b = 0;
  rand_a = 0;
  iVar12 = 4;
  uVar10 = 0xc4efea1b;
  uVar4 = 0xa51a3c49;
  uVar5 = 0xd92a4a78;
  uVar2 = 0xf421ad8;
  uVar14 = 0xc3163e4b;
  uVar13 = 0x95d90059;
  lVar8 = 0;
  uVar7 = 0x1367df5a;
  uVar11 = 0x30609119;
  do {
    if (param_1 != 0) {
      uVar7 = uVar7 + *(int *)(randrsl + lVar8);
      uVar13 = uVar13 + *(int *)(randrsl + lVar8 + 4);
      uVar14 = (ulong)(uint)((int)uVar14 + *(int *)(randrsl + lVar8 + 8));
      uVar2 = uVar2 + *(int *)(randrsl + lVar8 + 0xc);
      uVar5 = uVar5 + *(int *)(randrsl + lVar8 + 0x10);
      uVar4 = uVar4 + *(int *)(randrsl + lVar8 + 0x14);
      uVar10 = uVar10 + *(int *)(randrsl + lVar8 + 0x18);
      uVar11 = uVar11 + *(int *)(randrsl + lVar8 + 0x1c);
    }
    local_40 = iVar12 + 3;
    local_44 = iVar12 + 1;
    local_48 = iVar12 + 2;
    local_4c = iVar12 + -1;
    iVar17 = iVar12 + -2;
    iVar16 = iVar12 + -3;
    uVar7 = uVar13 << 0xb ^ uVar7;
    iVar1 = uVar7 + uVar2;
    uVar13 = (uint)(uVar14 >> 2) ^ (int)uVar14 + uVar13;
    uVar5 = uVar13 + uVar5;
    uVar6 = iVar1 * 0x100 ^ iVar1 + (int)uVar14;
    iVar3 = uVar6 + uVar4;
    uVar2 = uVar5 >> 0x10 ^ uVar5 + iVar1;
    uVar10 = uVar2 + uVar10;
    uVar5 = iVar3 * 0x400 ^ iVar3 + uVar5;
    iVar1 = uVar5 + uVar11;
    uVar4 = uVar10 >> 4 ^ uVar10 + iVar3;
    uVar7 = uVar4 + uVar7;
    uVar10 = iVar1 * 0x100 ^ iVar1 + uVar10;
    uVar13 = uVar10 + uVar13;
    uVar11 = uVar7 >> 9 ^ uVar7 + iVar1;
    uVar6 = uVar11 + uVar6;
    uVar14 = (ulong)uVar6;
    uVar7 = uVar13 + uVar7;
    lVar15 = (long)iVar12;
    iVar12 = iVar12 + 8;
    *(uint *)(randmem + lVar8) = uVar7;
    *(uint *)(randmem + (long)iVar16 * 4) = uVar13;
    lVar8 = lVar8 + 0x20;
    *(uint *)(randmem + (long)iVar17 * 4) = uVar6;
    *(uint *)(randmem + (long)local_4c * 4) = uVar2;
    *(uint *)(randmem + lVar15 * 4) = uVar5;
    *(uint *)(randmem + (long)local_44 * 4) = uVar4;
    *(uint *)(randmem + (long)local_48 * 4) = uVar10;
    *(uint *)(randmem + (long)local_40 * 4) = uVar11;
  } while (iVar12 != 0x104);
  if (param_1 != 0) {
    pbVar9 = randmem;
    do {
      uVar6 = uVar6 + *(uint *)((long)pbVar9 + 8);
      uVar7 = (uVar13 + *(uint *)((long)pbVar9 + 4)) * 0x800 ^ *(uint *)pbVar9 + uVar7;
      iVar12 = uVar2 + *(uint *)((long)pbVar9 + 0xc) + uVar7;
      uVar13 = uVar6 >> 2 ^ uVar6 + uVar13 + *(uint *)((long)pbVar9 + 4);
      uVar5 = uVar5 + *(uint *)((long)pbVar9 + 0x10) + uVar13;
      uVar6 = iVar12 * 0x100 ^ iVar12 + uVar6;
      iVar1 = uVar4 + *(uint *)((long)pbVar9 + 0x14) + uVar6;
      uVar2 = uVar5 >> 0x10 ^ uVar5 + iVar12;
      uVar10 = uVar10 + *(uint *)((long)pbVar9 + 0x18) + uVar2;
      *(uint *)((long)pbVar9 + 0xc) = uVar2;
      uVar5 = iVar1 * 0x400 ^ iVar1 + uVar5;
      iVar12 = uVar11 + *(uint *)((long)pbVar9 + 0x1c) + uVar5;
      *(uint *)((long)pbVar9 + 0x10) = uVar5;
      uVar4 = uVar10 >> 4 ^ uVar10 + iVar1;
      uVar7 = uVar4 + uVar7;
      *(uint *)((long)pbVar9 + 0x14) = uVar4;
      uVar10 = iVar12 * 0x100 ^ iVar12 + uVar10;
      uVar13 = uVar10 + uVar13;
      *(uint *)((long)pbVar9 + 0x18) = uVar10;
      uVar11 = uVar7 >> 9 ^ uVar7 + iVar12;
      uVar7 = uVar13 + uVar7;
      *(uint *)((long)pbVar9 + 4) = uVar13;
      uVar6 = uVar11 + uVar6;
      *(uint *)((long)pbVar9 + 0x1c) = uVar11;
      *(uint *)pbVar9 = uVar7;
      *(uint *)((long)pbVar9 + 8) = uVar6;
      pbVar9 = (byte *)((long)pbVar9 + 0x20);
    } while (pbVar9 != (byte *)&blic);
  }
  isaac();
  return;
}


