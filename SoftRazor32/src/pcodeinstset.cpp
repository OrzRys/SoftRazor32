#include "mspcode.h"

/*
Reference From Semi VB Decompiler
Thank: vbgamer45
http://www.vbforums.com/member.php?51546-vbgamer45
*/

extern const VB_PCODE_INSTRUCTIONSET PCodeStdInst[251];
extern const VB_PCODE_INSTRUCTIONSET * PCodeLeadInst[5];

const LPCCH pcit_ive = "InvalidExcode";

/*

NU:*    Not used * Byte(s)  [未使用的参数]

%1      data 1 byte                       1
1字节数据

%2      data 2 bytes                      2
2字节数据

%4      data 4 bytes                      4
4字节数据

%8      data 8 bytes                      8
8字节数据

%a      Argument                          2
参数

%b      data byte                         1
数据Byte

%c      Call                              2
调用

%e      ??**Not used**??                  4 = 2 + 2

%j      Jump Over Word (signed short)     2
跳过2字节

%k      Jump Over Word (unsigned short)   2
跳过2字节

%l      Loacl Address                     2
局部地址

%p      Variable-length Params            Depending on the %j or %k
变长参数,长度取决于%j或%k

%s      String                            2
字符串

%t      ??                                2

%v      VTable Offset And GUID            4 = 2 + 2
虚表偏移和对象GUID

%w      data word                         2
数据word

%x      External Api                      2
外部API


Note:

%1 == %b
%1    movsx   reg, byte ptr [xxxxxxxx]    signed char
%b    movzx   reg, byte ptr [xxxxxxxx]    unsigned char

%2 == %w
%2    movsx   reg, word ptr [xxxxxxxx]    signed short
%w    movzx   reg, word ptr [xxxxxxxx]    unsigned short

%a
signed or unsigned short

%c
unsigned short

%j
signed short

%k
unsigned short

%l
unsigned short

%s
unsigned short

%t
unsigned short

%x
unsigned short


*/

#define PCPT_ARG                  L" %a"
#define PCPT_PTRARG               L" [%a]"
#define PCPT_CALL                 L" %c"
#define PCPT_PTRCALL              L" [%c]"
#define PCPT_I1                   L" %b"
#define PCPT_U1                   L" %1"
#define PCPT_I2                   L" %2"
#define PCPT_I4                   L" %4"
#define PCPT_I8                   L" %8"
#define PCPT_N1                   L" [NU:%1]"
#define PCPT_U2                   L" %w"
#define PCPT_U2_U2                L" %w, %w"
#define PCPT_U2_U2_U2             L" %w, %w, %w"
#define PCPT_I2_I2                L" %2, %2"
#define PCPT_I2_U2                L" %2, %w"
#define PCPT_I2_U2_U2             L" %2, %w, %w"
#define PCPT_I2_I2_U2             L" %2, %2, %w"
#define PCPT_I2_I2_I2             L" %2, %2, %2"
#define PCPT_I2_I2_I2_I2          L" %2, %2, %2, %2"
#define PCPT_I2_N2                L" %2, [NU:%2]"
#define PCPT_I2_I4                L" %2, %4"
#define PCPT_I2_I4_I2             L" %2, %4, %2"
#define PCPT_I2_I8                L" %2, %8"
#define PCPT_I4_I2                L" %4, %2"
#define PCPT_I4_U2                L" %4, %w"
#define PCPT_JO_I2_I2             L" %j%2, %2"
#define PCPT_STKREGARG            L" SR=%a"
#define PCPT_IMPCALL_ARG          L" ::call %c(%a)"
#define PCPT_VTABLE1              L" %v"
#define PCPT_CALLI4_I2            L" %x, %2"
#define PCPT_IMPCALLFPR4_I2       L" %x, [Stk+%2]"
#define PCPT_I2_CALL              L" %2, %c"
#define PCPT_I2_SR                L" [%2+SR]"
#define PCPT_STR                  L" %s"
#define PCPT_LOC                  L" %l"
#define PCPT_POPSTK               L" %t"
#define PCPT_ARG_I2               L"%a, %2"
#define PCPT_ARG_STR              L"%a, %s"
#define PCPT_LOC_ARG              L"%l, %a"
#define PCPT_PTR_PTRARG_I2        L"[[%a]+%2]"
#define PCPT_JO_VP                L"%j%p"
#define PCPT_JU_VP                L"%k%p"
#define PCPT_JU_U2_U2_VP          L"%k%w, %w, %p"
#define PCPT_JU_I2_U2_U2_VP       L"%k%2, %w, %w, %p"
#define PCPT_JU_I4_U2_VP          L"%k%4, %w, %p"
#define PCPT_JU_I2_I4_U2_VP       L"%k%2, %4, %w, %p"


//#define PCPM_MAKEVPLEN(fl, vl)    (0x80000000 | ((uint16_t)(fl) << 16) | (uint16_t)(vl))


/*P-Code 单字节指令集 (标准指令集)*/
const VB_PCODE_INSTRUCTIONSET PCodeStdInst[251] =
{
  /*00h*/{ "LargeBos", PCPT_N1, PCFLH_DBG, 2 },
  /*01h*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*02h*/{ "LargeBos_1", PCPT_N1, PCFLH_DBG, 2 },
  /*03h*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*04h*/{ "FLdRfVar", PCPT_ARG, PCT0_RETONEVAR, 3 },
  /*05h*/{ "ImpAdLdRfVar", PCPT_CALL, PCT0_STD, 3 },       //ON:ImpAdLdRf
  /*06h*/{ "MemLdRfVar", PCPT_ARG, 0, 3 },
  /*07h*/{ "FMemLdRfVar", PCPT_I4, 0, 5 },                //ON:FMemLdRf
  /*08h*/{ "FLdPr", PCPT_STKREGARG, PCT0_STD, 3 },
  /*09h*/{ "ImpAdCallHresult", PCPT_IMPCALL_ARG, PCT0_STD, 5 },
  /*0Ah*/{ "ImpAdCallFPR4", PCPT_IMPCALLFPR4_I2, PCT0_STD, 5 },
  /*0Bh*/{ "ImpAdCallI2", PCPT_IMPCALL_ARG, PCT0_STD, 5 },
  /*0Ch*/{ "ImpAdCallCy", PCPT_IMPCALL_ARG, PCT0_STD, 5 },
  /*0Dh*/{ "VCallHresult", PCPT_VTABLE1, PCT0_STD, 5 },
  /*0Eh*/{ "VCallFPR8", PCPT_I2, 0, 3 },
  /*0Fh*/{ "VCallI2", PCPT_I2, PCT0_STD, 3 },
  /*10h*/{ "ThisVCallHresult", PCPT_I2_CALL, PCT0_STD, 5 },
  /*11h*/{ "ThisVCallFPR8", PCPT_I2, 0, 3 },              //ON:ThisVCall
  /*12h*/{ "ThisVCallI2", PCPT_I2, 0, 3 },
  /*13h*/{ "ExitProcHresult", NULL, PCFH_ENDPROC | PCT0_STD, 1 },
  /*14h*/{ "ExitProcI4", NULL, PCFH_ENDPROC | PCT0_STD, 1 },
  /*15h*/{ "ExitProcI2", NULL, PCFH_ENDPROC | PCT0_NONE, 1 },
  /*16h*/{ "ExitProcR4", NULL, PCFH_ENDPROC | PCT0_NONE, 1 },
  /*17h*/{ "ExitProcR8", NULL, PCFH_ENDPROC | PCT0_NONE, 1 },
  /*18h*/{ "ExitProcCy", NULL, PCFH_ENDPROC | PCT0_NONE, 1 },
  /*19h*/{ "FStAdFunc", PCPT_I2, 0, 3 },
  /*1Ah*/{ "FFree1Ad", PCPT_I2, 0, 3 },
  /*1Bh*/{ "LitStr", PCPT_STR, PCT0_STD, 3 },
  /*1Ch*/{ "BranchF", PCPT_LOC, PCFH_CONDBRAN | PCT0_STD, 3 },
  /*1Dh*/{ "BranchT", PCPT_LOC, PCFH_CONDBRAN | PCT0_STD, 3 },
  /*1Eh*/{ "Branch", PCPT_LOC, PCFH_BRANCH | PCT0_STD, 3 },
  /*1Fh*/{ "CRec2Ansi", PCPT_I2, PCT0_STD, 3 },
  /*20h*/{ "CRec2Uni", PCPT_I2, 0, 3 },
  /*21h*/{ "FLdPrThis", NULL, PCT0_NONE, 1 },
  /*22h*/{ "ImpAdLdPr", PCPT_PTRCALL, PCT0_STD, 3 },
  /*23h*/{ "FStStrNoPop", PCPT_I2, 0, 3 },
  /*24h*/{ "NewIfNullPr", PCPT_POPSTK, PCT0_IDX, 3 },
  /*25h*/{ "PopAdLdVar", NULL, PCT0_NONE, 1 },
  /*26h*/{ "AryDescTemp", PCPT_I2, 0, 3 },
  /*27h*/{ "LitVar_Missing", PCPT_I2, PCT0_STD, 3 },
  /*28h*/{ "LitVarI2", PCPT_ARG_I2, PCT0_STD, 5 },
  /*29h*/{ "FFreeAd", PCPT_JO_VP, PCT0_CLEARMEMORY, -3 },
  /*2Ah*/{ "ConcatStr", NULL, PCT0_NONE, 1 },
  /*2Bh*/{ "PopTmpLdAd2", PCPT_I2, 0, 3 },
  /*2Ch*/{ "LateIdSt", PCPT_I4, 0, 5 },
  /*2Dh*/{ "AryUnlock", PCPT_I2, 0, 3 },
  /*2Eh*/{ "AryLock", PCPT_I2, 0, 3 },
  /*2Fh*/{ "FFree1Str", PCPT_I2, PCT0_ONESTRING, 3 },
  /*30h*/{ "PopTmpLdAd8", PCPT_I2, 0, 3 },
  /*31h*/{ "FStStr", PCPT_I2, 0, 3 },
  /*32h*/{ "FFreeStr", PCPT_JO_VP, PCT0_CLEARMEMORY, -3 },
  /*33h*/{ "LdFixedStr", PCPT_STR, PCT0_STD, 3 },
  /*34h*/{ "CStr2Ansi", NULL, PCT0_NONE, 1 },
  /*35h*/{ "FFree1Var", NULL, 0, 3 },
  /*36h*/{ "FFreeVar", PCPT_JO_VP, PCT0_CLEARMEMORY, -3 },
  /*37h*/{ "PopFPR4", NULL, PCT0_IDX, 1 },
  /*38h*/{ "CopyBytes", PCPT_I2, 0, 3 },
  /*39h*/{ "PopFPR8", NULL, PCT0_NONE, 1 },
  /*3Ah*/{ "LitVarStr", PCPT_ARG_STR, PCT0_STD, 5 },
  /*3Bh*/{ "Ary1StStrCopy", NULL, PCT0_NONE, 1 },
  /*3Ch*/{ "SetLastSystemError", NULL, PCT0_NONE, 1 },                     //ON:GetLastError
  /*3Dh*/{ "CastAd", PCPT_I2, 0, 3 },
  /*3Eh*/{ "FLdZeroAd", PCPT_I2, 0, 3 },
  /*3Fh*/{ "CVarCy", PCPT_I2, 0, 3 },
  /*40h*/{ "Ary1LdRf", NULL, PCT0_NONE, 1 },
  /*41h*/{ "Ary1LdPr", NULL, PCT0_NONE, 1 },
  /*42h*/{ "CR8Var", NULL, PCT0_NONE, 1 },                       //ON:CR8Var
  /*43h*/{ "FStStrCopy", PCPT_ARG, PCT0_STD, 3 },
  /*44h*/{ "CVarI2", PCPT_I2, 0, 3 },
  /*45h*/{ "Error", NULL, PCT0_NONE, 1 },
  /*46h*/{ "CVarStr", PCPT_I2, 0, 3 },
  /*47h*/{ "FnStFixedStrL", PCPT_LOC, PCT0_STD, 3 },
  /*48h*/{ "ILdPr", PCPT_I2, 0, 3 },
  /*49h*/{ "PopAdLd4", NULL, PCT0_NONE, 1 },
  /*4Ah*/{ "FnLenStr", NULL, PCT0_NONE, 1 },
  /*4Bh*/{ "OnErrorGoto", PCPT_LOC, PCT0_STD, 3 },
  /*4Ch*/{ "FnLBound", NULL, PCT0_NONE, 1 },
  /*4Dh*/{ "CVarRef", PCPT_I2_I2, 0, 5 },
  /*4Eh*/{ "FStVarCopyObj", PCPT_I2, 0, 3 },
  /*4Fh*/{ "MidStr", PCPT_I2, 0, 3 },
  /*50h*/{ "CI4Str", NULL, PCT0_NONE, 1 },
  /*51h*/{ "FLdZeroAd_1", PCPT_I2, 0, 3 },
  /*52h*/{ "Ary1StVar", NULL, PCT0_NONE, 1 },
  /*53h*/{ "CBoolCy", NULL, PCT0_NONE, 1 },
  /*54h*/{ "FMemStStrCopy", PCPT_I2_I2, 0, 5 },
  /*55h*/{ "CI2Var", NULL, PCT0_NONE, 1 },
  /*56h*/{ "NewIfNullAd", PCPT_I2, 0, 3 },
  /*57h*/{ "LateMemLdVar", PCPT_I2_I2, 0, 5 },
  /*58h*/{ "MemLdPr", PCPT_I2, 0, 3 },
  /*59h*/{ "PopTmpLdAd4", PCPT_I2, 0, 3 },                  //ON:PopTmpLdAdStr
  /*5Ah*/{ "Erase", NULL, PCT0_NONE, 1 },
  /*5Bh*/{ "FStAdFuncNoPop", PCPT_I2, 0, 3 },
  /*5Ch*/{ "BranchFVar", PCPT_LOC, PCFH_CONDBRAN, 3 },
  /*5Dh*/{ "HardType", NULL, PCT0_NONE, 1 },
  /*5Eh*/{ "ImpAdCallI4", PCPT_CALLI4_I2, PCT0_STD, 5 },
  /*5Fh*/{ "FMemLdPr", PCPT_I2_I2, 0, 5 },
  /*60h*/{ "CStrVarTmp", NULL, PCT0_NONE, 1 },
  /*61h*/{ "LateIdLdVar", PCPT_I2_I4, 0, 7 },
  /*62h*/{ "IStDarg", PCPT_I2, 0, 3 },
  /*63h*/{ "LitVar_TRUE", PCPT_I2, 0, 3 },
  /*64h*/{ "NextI2", PCPT_I2_I2, 0, 5 },
  /*65h*/{ "NextStepI2", PCPT_I2_I2, 0, 5 },
  /*66h*/{ "NextI4", PCPT_I2_I2, 0, 5 },
  /*67h*/{ "NextStepI4", PCPT_I2_I2, 0, 5 },
  /*68h*/{ "NextR4", PCPT_I2_I2, 0, 5 },
  /*69h*/{ "NextR8", PCPT_I2_I2, 0, 5 },
  /*6Ah*/{ "NextStepCy", PCPT_I2_I2, 0, 5 },
  /*6Bh*/{ "FLdI2", PCPT_PTRARG, PCT0_STD, 3 },
  /*6Ch*/{ "FLdI4", PCPT_PTRARG, PCT0_STD, 3 },
  /*6Dh*/{ "FLdCy", PCPT_PTRARG, 0, 3 },
  /*6Eh*/{ "FLdFPR4", PCPT_PTRARG, 0, 3 },
  /*6Fh*/{ "FLdFPR8", PCPT_PTRARG, 0, 3 },
  /*70h*/{ "FStI2", PCPT_PTRARG, PCT0_STD, 3 },
  /*71h*/{ "FStI4", PCPT_PTRARG, PCT0_STD, 3 },
  /*72h*/{ "FStCy", PCPT_PTRARG, 0, 3 },
  /*73h*/{ "FStFPR4", PCPT_PTRARG, 0, 3 },
  /*74h*/{ "FStFPR8", PCPT_PTRARG, 0, 3 },
  /*75h*/{ "ImpAdLdI2", PCPT_CALL, 0, 3 },
  /*76h*/{ "ImpAdLdI4", PCPT_CALL, PCT0_STD, 3 },
  /*77h*/{ "ImpAdLdCy", PCPT_CALL, PCT0_STD, 3 },
  /*78h*/{ "ImpAdLdFPR4", PCPT_CALL, PCT0_STD, 3 },
  /*79h*/{ "ImpAdLdFPR8", PCPT_CALL, PCT0_STD, 3 },
  /*7Ah*/{ "ImpAdStI2", PCPT_CALL, PCT0_STD, 3 },
  /*7Bh*/{ "ImpAdStI4", PCPT_CALL, PCT0_STD, 3 },
  /*7Ch*/{ "ImpAdStCy", PCPT_CALL, PCT0_STD, 3 },
  /*7Dh*/{ "ImpAdStFPR4", PCPT_CALL, PCT0_STD, 3 },
  /*7Eh*/{ "ImpAdStFPR8", PCPT_CALL, PCT0_STD, 3 },
  /*7Fh*/{ "ILdI2", PCPT_ARG, PCT0_STD, 3 },
  /*80h*/{ "ILdI4", PCPT_ARG, PCT0_STD, 3 },
  /*81h*/{ "ILdR8", PCPT_ARG, PCT0_STD, 3 },
  /*82h*/{ "ILdFPR4", PCPT_ARG, PCT0_STD, 3 },
  /*83h*/{ "ILdFPR8", PCPT_ARG, PCT0_STD, 3 },
  /*84h*/{ "IStI2", PCPT_CALL, PCT0_STD, 3 },
  /*85h*/{ "IStI4", PCPT_CALL, PCT0_STD, 3 },
  /*86h*/{ "IStCy", PCPT_I2, 0, 3 },
  /*87h*/{ "IStFPR4", PCPT_I2, 0, 3 },
  /*88h*/{ "IStFPR8", PCPT_I2, 0, 3 },
  /*89h*/{ "MemLdI2", PCPT_I2_SR, PCT0_IDX, 3 },
  /*8Ah*/{ "MemLdI4", PCPT_STR, 0, 3 },
  /*8Bh*/{ "MemLdCy", PCPT_I2, 0, 3 },
  /*8Ch*/{ "MemLdFPR4", PCPT_I2, 0, 3 },
  /*8Dh*/{ "MemLdFPR8", PCPT_I2, 0, 3 },
  /*8Eh*/{ "MemStI2", PCPT_I2, 0, 3 },
  /*8Fh*/{ "MemStI4", PCPT_I2, 0, 3 },
  /*90h*/{ "MemStCy", PCPT_I2, 0, 3 },
  /*91h*/{ "MemStFPR4", PCPT_I2, 0, 3 },
  /*92h*/{ "MemStFPR8", PCPT_I2, 0, 3 },
  /*93h*/{ "FMemLdI2", PCPT_I2_I2, 0, 5 },
  /*94h*/{ "FMemLdI4", PCPT_I2_I2, 0, 5 },
  /*95h*/{ "FMemLdCy", PCPT_I2_I2, 0, 5 },
  /*96h*/{ "FMemLdFPR4", PCPT_I2_I2, 0, 5 },
  /*97h*/{ "FMemLdFPR8", PCPT_I2_I2, 0, 5 },
  /*98h*/{ "FMemStI2", PCPT_PTR_PTRARG_I2, PCT0_STD, 5 },
  /*99h*/{ "FMemStI4", PCPT_PTR_PTRARG_I2, PCT0_STD, 5 },
  /*9Ah*/{ "FMemStCy", PCPT_PTR_PTRARG_I2, PCT0_STD, 5 },
  /*9Bh*/{ "FMemStFPR4", PCPT_PTR_PTRARG_I2, PCT0_STD, 5 },
  /*9Ch*/{ "FMemStFPR8", PCPT_PTR_PTRARG_I2, PCT0_STD, 5 },
  /*9Dh*/{ "Ary1LdI2", NULL, PCT0_NONE, 1 },
  /*9Eh*/{ "Ary1LdI4", NULL, PCT0_NONE, 1 },
  /*9Fh*/{ "Ary1LdCy", NULL, PCT0_NONE, 1 },
  /*A0h*/{ "Ary1LdFPR4", NULL, PCT0_NONE, 1 },
  /*A1h*/{ "Ary1LdFPR8", NULL, PCT0_NONE, 1 },
  /*A2h*/{ "Ary1StI2", NULL, PCT0_NONE, 1 },
  /*A3h*/{ "Ary1StI4", NULL, PCT0_NONE, 1 },
  /*A4h*/{ "Ary1StCy", NULL, PCT0_NONE, 1 },
  /*A5h*/{ "Ary1StFPR4", NULL, PCT0_NONE, 1 },
  /*A6h*/{ "Ary1StFPR8", NULL, PCT0_NONE, 1 },
  /*A7h*/{ "AryLdPr", PCPT_I2, 0, 3 },
  /*A8h*/{ "AryLdRf", PCPT_I2, 0, 3 },
  /*A9h*/{ "AddI2", NULL, PCT0_NONE, 1 },
  /*AAh*/{ "AddI4", NULL, PCT0_NONE, 1 },
  /*ABh*/{ "AddR8", NULL, PCT0_NONE, 1 },
  /*ACh*/{ "AddCy", NULL, PCT0_NONE, 1 },
  /*ADh*/{ "SubI2", NULL, PCT0_NONE, 1 },
  /*AEh*/{ "SubI4", NULL, PCT0_NONE, 1 },
  /*AFh*/{ "SubR8", NULL, PCT0_NONE, 1 },
  /*B0h*/{ "SubCy", NULL, PCT0_NONE, 1 },
  /*B1h*/{ "MulI2", NULL, PCT0_NONE, 1 },
  /*B2h*/{ "MulI4", NULL, PCT0_NONE, 1 },
  /*B3h*/{ "MulR8", NULL, PCT0_NONE, 1 },
  /*B4h*/{ "MulCy", NULL, PCT0_NONE, 1 },
  /*B5h*/{ "MulCyI2", NULL, PCT0_NONE, 1 },
  /*B6h*/{ "DivR4", NULL, PCT0_NONE, 1 },        //ON:DivR8
  /*B7h*/{ "UMiI2", NULL, PCT0_NONE, 1 },
  /*B8h*/{ "UMiI4", NULL, PCT0_NONE, 1 },
  /*B9h*/{ "UMiR8", NULL, PCT0_NONE, 1 },
  /*BAh*/{ "UMiCy", NULL, PCT0_NONE, 1 },
  /*BBh*/{ "AbsI2", NULL, PCT0_NONE, 1 },      //ON:FnAbsI2
  /*BCh*/{ "AbsI4", NULL, PCT0_NONE, 1 },      //ON:FnAbsI4
  /*BDh*/{ "AbsR8", NULL, PCT0_NONE, 1 },      //ON:FnAbsR4
  /*BEh*/{ "AbsCy", NULL, PCT0_NONE, 1 },      //ON:FnAbsCy
  /*BFh*/{ "IDvI2", NULL, PCT0_NONE, 1 },
  /*C0h*/{ "IDvI4", NULL, PCT0_NONE, 1 },
  /*C1h*/{ "ModI2", NULL, PCT0_NONE, 1 },
  /*C2h*/{ "ModI4", NULL, PCT0_NONE, 1 },
  /*C3h*/{ "NotI4", NULL, PCT0_NONE, 1 },
  /*C4h*/{ "AndUI1", NULL, PCT0_NONE, 1 },       //ON:AndI4
  /*C5h*/{ "OrI4", NULL, PCT0_NONE, 1 },
  /*C6h*/{ "EqI2", NULL, PCT0_NONE, 1 },
  /*C7h*/{ "EqI4", NULL, PCT0_NONE, 1 },
  /*C8h*/{ "EqR8", NULL, PCT0_NONE, 1 },
  /*C9h*/{ "EqCy", NULL, PCT0_NONE, 1 },
  /*CAh*/{ "EqCyR8", NULL, PCT0_NONE, 1 },
  /*CBh*/{ "NeI2", NULL, PCT0_NONE, 1 },
  /*CCh*/{ "NeI4", NULL, PCT0_NONE, 1 },
  /*CDh*/{ "NeR8", NULL, PCT0_NONE, 1 },         //ON:NeR4
  /*CEh*/{ "NeCy", NULL, PCT0_NONE, 1 },
  /*CFh*/{ "NeCyR8", NULL, PCT0_NONE, 1 },
  /*D0h*/{ "LtI2", NULL, PCT0_NONE, 1 },
  /*D1h*/{ "LtI4", NULL, PCT0_NONE, 1 },
  /*D2h*/{ "LtR8", NULL, PCT0_NONE, 1 },         //ON:LtR4
  /*D3h*/{ "LtCy", NULL, PCT0_NONE, 1 },
  /*D4h*/{ "LtCyR8", NULL, PCT0_NONE, 1 },
  /*D5h*/{ "LeI2", NULL, PCT0_NONE, 1 },
  /*D6h*/{ "LeI4", NULL, PCT0_NONE, 1 },
  /*D7h*/{ "LeR8", NULL, PCT0_NONE, 1 },
  /*D8h*/{ "LeCy", NULL, PCT0_NONE, 1 },
  /*D9h*/{ "LeCyR8", NULL, PCT0_NONE, 1 },
  /*DAh*/{ "GtI2", NULL, PCT0_NONE, 1 },
  /*DBh*/{ "GtI4", NULL, PCT0_NONE, 1 },
  /*DCh*/{ "GtR8", NULL, PCT0_NONE, 1 },         //ON:GtR4
  /*DDh*/{ "GtCy", NULL, PCT0_NONE, 1 },
  /*DEh*/{ "GtCyR8", NULL, PCT0_NONE, 1 },
  /*DFh*/{ "GeI2", NULL, PCT0_NONE, 1 },
  /*E0h*/{ "GeI4", NULL, PCT0_NONE, 1 },
  /*E1h*/{ "GeR8", NULL, PCT0_NONE, 1 },
  /*E2h*/{ "GeCy", NULL, PCT0_NONE, 1 },
  /*E3h*/{ "GeCyR8", NULL, PCT0_NONE, 1 },
  /*E4h*/{ "CI2I4", NULL, PCT0_NONE, 1 },
  /*E5h*/{ "CI2R8", NULL, PCT0_NONE, 1 },
  /*E6h*/{ "CI2Cy", NULL, PCT0_NONE, 1 },
  /*E7h*/{ "CI4I2", NULL, PCT0_NONE, 1 },
  /*E8h*/{ "CI4R8", NULL, PCT0_NONE, 1 },
  /*E9h*/{ "CI4Cy", NULL, PCT0_NONE, 1 },
  /*EAh*/{ "CR4R8", NULL, PCT0_NONE, 1 },        //ON:CR4R4
  /*EBh*/{ "CR8I2", NULL, PCT0_NONE, 1 },
  /*ECh*/{ "CR8I4", NULL, PCT0_NONE, 1 },
  /*EDh*/{ "CR8R4", NULL, PCT0_NONE, 1 },
  /*EEh*/{ "CR4Cy", NULL, PCT0_NONE, 1 },
  /*EFh*/{ "CCyI2", NULL, PCT0_NONE, 1 },
  /*F0h*/{ "CCyI4", NULL, PCT0_NONE, 1 },
  /*F1h*/{ "CCyR8", NULL, PCT0_NONE, 1 },
  /*F2h*/{ "CDateR8", NULL, PCT0_NONE, 1 },
  /*F3h*/{ "LitI2", PCPT_I2, PCT0_STD, 3 },
  /*F4h*/{ "LitI2_Byte", PCPT_U1, PCT0_STD, 2 },
  /*F5h*/{ "LitI4", PCPT_I4, PCT0_STD, 5 },
  /*F6h*/{ "LitR8", PCPT_I8, 0, 9 },
  /*F7h*/{ "LitCy4", PCPT_I4, 0, 5 },
  /*F8h*/{ "LitI2FP", PCPT_I2, 0, 3 },
  /*F9h*/{ "LitR4FP", PCPT_I4, 0, 5 },
  /*FAh*/{ "LitDate", PCPT_I8, 0, 9 }
};

/*P-Code 双字节指令集0 (扩展指令集) 0xFB前缀*/
const VB_PCODE_INSTRUCTIONSET PCodeLead0Inst[256] =
{
  /*00h*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*01h*/{ "ImpUI1", NULL, PCT0_NONE, 1 },
  /*02h*/{ "ImpI4", NULL, PCT0_NONE, 1 },
  /*03h*/{ "ImpI4_1", NULL, PCT0_NONE, 1 },
  /*04h*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*05h*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*06h*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*07h*/{ "ImpVar", PCPT_I2, 0, 3 },
  /*08h*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*09h*/{ "EqvUI1", NULL, PCT0_NONE, 1 },
  /*0Ah*/{ "EqvI4", NULL, PCT0_NONE, 1 },
  /*0Bh*/{ "EqvI4_1", NULL, PCT0_NONE, 1 },
  /*0Ch*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*0Dh*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*0Eh*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*0Fh*/{ "EqvVar", PCPT_I2, 0, 3 },
  /*10h*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*11h*/{ "XorI4", NULL, PCT0_NONE, 1 },
  /*12h*/{ "XorI4_1", NULL, PCT0_NONE, 1 },
  /*13h*/{ "XorI4_2", NULL, PCT0_NONE, 1 },
  /*14h*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*15h*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*16h*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*17h*/{ "XorVar", PCPT_I2, 0, 3 },
  /*18h*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*19h*/{ "OrI4", NULL, PCT0_NONE, 1 },
  /*1Ah*/{ "OrI4_1", NULL, PCT0_NONE, 1 },
  /*1Bh*/{ "OrI4_2", NULL, PCT0_NONE, 1 },
  /*1Ch*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*1Dh*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*1Eh*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*1Fh*/{ "OrVar", PCPT_I2, 0, 3 },
  /*20h*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*21h*/{ "AndI4", NULL, PCT0_NONE, 1 },          //ON:AndUI1
  /*22h*/{ "AndI4_1", NULL, PCT0_NONE, 1 },        //ON:AndUI1
  /*23h*/{ "AndI4_2", NULL, PCT0_NONE, 1 },        //ON:AndUI1
  /*24h*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*25h*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*26h*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*27h*/{ "AndVar", PCPT_I2, 0, 3 },
  /*28h*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*29h*/{ "EqI2", NULL, PCT0_NONE, 1 },
  /*2Ah*/{ "EqI2_1", NULL, PCT0_NONE, 1 },
  /*2Bh*/{ "EqI4", NULL, PCT0_NONE, 1 },
  /*2Ch*/{ "EqR8", NULL, PCT0_NONE, 1 },
  /*2Dh*/{ "EqR8_1", NULL, PCT0_NONE, 1 },
  /*2Eh*/{ "EqCy", NULL, PCT0_NONE, 1 },
  /*2Fh*/{ "EqVar", PCPT_I2, 0, 3 },
  /*30h*/{ "EqStr", NULL, PCT0_NONE, 1 },
  /*31h*/{ "EqTextVar", PCPT_I2, 0, 3 },
  /*32h*/{ "EqTextStr", NULL, PCT0_NONE, 1 },
  /*33h*/{ "EqVarBool", NULL, PCT0_NONE, 1 },
  /*34h*/{ "EqTextVarBool", NULL, PCT0_NONE, 1 },
  /*35h*/{ "EqCyR8", NULL, PCT0_NONE, 1 },
  /*36h*/{ "NeI2", NULL, PCT0_NONE, 1 },
  /*37h*/{ "NeI2_1", NULL, PCT0_NONE, 1 },
  /*38h*/{ "NeI4", NULL, PCT0_NONE, 1 },
  /*39h*/{ "NeR4", NULL, PCT0_NONE, 1 },
  /*3Ah*/{ "NeR4_1", NULL, PCT0_NONE, 1 },
  /*3Bh*/{ "NeCy", NULL, PCT0_NONE, 1 },
  /*3Ch*/{ "NeVar", PCPT_I2, 0, 3 },
  /*3Dh*/{ "NeStr", NULL, PCT0_NONE, 1 },
  /*3Eh*/{ "NeTextVar", PCPT_I2, 0, 3 },
  /*3Fh*/{ "NeTextStr", NULL, PCT0_NONE, 1 },
  /*40h*/{ "NeVarBool", NULL, PCT0_NONE, 1 },
  /*41h*/{ "NeTextVarBool", NULL, PCT0_NONE, 1 },
  /*42h*/{ "NeCyR8", NULL, PCT0_NONE, 1 },
  /*43h*/{ "LeUI1", NULL, PCT0_NONE, 1 },
  /*44h*/{ "LeI2", NULL, PCT0_NONE, 1 },
  /*45h*/{ "LeI4", NULL, PCT0_NONE, 1 },
  /*46h*/{ "LeR8", NULL, PCT0_NONE, 1 },           //ON:LeR4
  /*47h*/{ "LeR8_1", NULL, PCT0_NONE, 1 },         //ON:LeR4
  /*48h*/{ "LeCy", NULL, PCT0_NONE, 1 },
  /*49h*/{ "LeVar", PCPT_I2, 0, 3 },
  /*4Ah*/{ "LeStr", NULL, PCT0_NONE, 1 },
  /*4Bh*/{ "LeTextVar", PCPT_I2, 0, 3 },
  /*4Ch*/{ "LeTextStr", NULL, PCT0_NONE, 1 },
  /*4Dh*/{ "LeVarBool", NULL, PCT0_NONE, 1 },
  /*4Eh*/{ "LeTextVarBool", NULL, PCT0_NONE, 1 },
  /*4Fh*/{ "LeCyR8", NULL, PCT0_NONE, 1 },
  /*50h*/{ "GeUI1", NULL, PCT0_NONE, 1 },
  /*51h*/{ "GeI2", NULL, PCT0_NONE, 1 },
  /*52h*/{ "GeI4", NULL, PCT0_NONE, 1 },
  /*53h*/{ "GeR8", NULL, PCT0_NONE, 1 },           //ON:GeR4
  /*54h*/{ "GeR8_1", NULL, PCT0_NONE, 1 },         //ON:GeR4
  /*55h*/{ "GeCy", NULL, PCT0_NONE, 1 },
  /*56h*/{ "GeVar", PCPT_I2, 0, 3 },
  /*57h*/{ "GeStr", NULL, PCT0_NONE, 1 },
  /*58h*/{ "GeTextVar", PCPT_I2, 0, 3 },
  /*59h*/{ "GeTextStr", NULL, PCT0_NONE, 1 },
  /*5Ah*/{ "GeVarBool", NULL, PCT0_NONE, 1 },
  /*5Bh*/{ "GeTextVarBool", NULL, PCT0_NONE, 1 },
  /*5Ch*/{ "GeCyR8", NULL, PCT0_NONE, 1 },
  /*5Dh*/{ "LtUI1", NULL, PCT0_NONE, 1 },
  /*5Eh*/{ "LtI2", NULL, PCT0_NONE, 1 },
  /*5Fh*/{ "LtI4", NULL, PCT0_NONE, 1 },
  /*60h*/{ "LtR8", NULL, PCT0_NONE, 1 },             //ON:LtR4
  /*61h*/{ "LtR8_1", NULL, PCT0_NONE, 1 },           //ON:LtR4
  /*62h*/{ "LtCy", NULL, PCT0_NONE, 1 },
  /*63h*/{ "LtVar", PCPT_I2, 0, 3 },
  /*64h*/{ "LtStr", NULL, PCT0_NONE, 1 },
  /*65h*/{ "LtTextVar", PCPT_I2, 0, 3 },
  /*66h*/{ "LtTextStr", NULL, PCT0_NONE, 1 },
  /*67h*/{ "LtVarBool", NULL, PCT0_NONE, 1 },
  /*68h*/{ "LtTextVarBool", NULL, PCT0_NONE, 1 },
  /*69h*/{ "LtCyR8", NULL, PCT0_NONE, 1 },
  /*6Ah*/{ "GtUI1", NULL, PCT0_NONE, 1 },
  /*6Bh*/{ "GtI2", NULL, PCT0_NONE, 1 },
  /*6Ch*/{ "GtI4", NULL, PCT0_NONE, 1 },
  /*6Dh*/{ "GtR8", NULL, PCT0_NONE, 1 },             //ON:GtR4
  /*6Eh*/{ "GtR8_1", NULL, PCT0_NONE, 1 },           //ON:GtR4
  /*6Fh*/{ "GtCy", NULL, PCT0_NONE, 1 },
  /*70h*/{ "GtVar", PCPT_I2, 0, 3 },
  /*71h*/{ "GtStr", NULL, PCT0_NONE, 1 },
  /*72h*/{ "GtTextVar", PCPT_I2, 0, 3 },
  /*73h*/{ "GtTextStr", NULL, PCT0_NONE, 1 },
  /*74h*/{ "GtVarBool", NULL, PCT0_NONE, 1 },
  /*75h*/{ "GtTextVarBool", NULL, PCT0_NONE, 1 },
  /*76h*/{ "GtCyR8", NULL, PCT0_NONE, 1 },
  /*77h*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*78h*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*79h*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*7Ah*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*7Bh*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*7Ch*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*7Dh*/{ "LikeVar", PCPT_I2, 0, 3 },
  /*7Eh*/{ "LikeStr", NULL, PCT0_NONE, 1 },
  /*7Fh*/{ "LikeTextVar", PCPT_I2, 0, 3 },
  /*80h*/{ "LikeTextStr", NULL, PCT0_NONE, 1 },
  /*81h*/{ "LikeVarBool", NULL, PCT0_NONE, 1 },
  /*82h*/{ "LikeTextVarBool", NULL, PCT0_NONE, 1 },
  /*83h*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*84h*/{ "BetweenUI1", NULL, PCT0_NONE, 1 },
  /*85h*/{ "BetweenI2", NULL, PCT0_NONE, 1 },
  /*86h*/{ "BetweenI4", NULL, PCT0_NONE, 1 },
  /*87h*/{ "BetweenR8", NULL, PCT0_NONE, 1 },        //ON:BetweenR4
  /*88h*/{ "BetweenR8_1", NULL, PCT0_NONE, 1 },      //ON:BetweenR4
  /*89h*/{ "BetweenCy", NULL, PCT0_NONE, 1 },
  /*8Ah*/{ "BetweenVar", NULL, PCT0_NONE, 1 },
  /*8Bh*/{ "BetweenStr", NULL, PCT0_NONE, 1 },
  /*8Ch*/{ "BetweenTextVar", NULL, PCT0_NONE, 1 },
  /*8Dh*/{ "BetweenTextStr", NULL, PCT0_NONE, 1 },
  /*8Eh*/{ "AddUI1", NULL, PCT0_NONE, 1 },
  /*8Fh*/{ "AddI2", NULL, PCT0_NONE, 1 },
  /*90h*/{ "AddI4", NULL, PCT0_NONE, 1 },
  /*91h*/{ "AddR8", NULL, PCT0_NONE, 1 },          //AddR4
  /*92h*/{ "AddR8_1", NULL, PCT0_NONE, 1 },        //AddR4
  /*93h*/{ "AddCy", NULL, PCT0_NONE, 1 },
  /*94h*/{ "AddVar", PCPT_I2, 0, 3 },
  /*95h*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*96h*/{ "SubUI1", NULL, PCT0_NONE, 1 },
  /*97h*/{ "SubI2", NULL, PCT0_NONE, 1 },
  /*98h*/{ "SubI4", NULL, PCT0_NONE, 1 },
  /*99h*/{ "SubR8", NULL, PCT0_NONE, 1 },
  /*9Ah*/{ "SubR8_1", NULL, PCT0_NONE, 1 },
  /*9Bh*/{ "SubCy", NULL, PCT0_NONE, 1 },
  /*9Ch*/{ "SubVar", PCPT_I2, 0, 3 },
  /*9Dh*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*9Eh*/{ "ModUI1", NULL, PCT0_NONE, 1 },
  /*9Fh*/{ "ModI2", NULL, PCT0_NONE, 1 },
  /*A0h*/{ "ModI4", NULL, PCT0_NONE, 1 },
  /*A1h*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*A2h*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*A3h*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*A4h*/{ "ModVar", PCPT_I2, 0, 3 },
  /*A5h*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*A6h*/{ "IDvUI1", NULL, PCT0_NONE, 1 },
  /*A7h*/{ "IDvI2", NULL, PCT0_NONE, 1 },
  /*A8h*/{ "IDvI4", NULL, PCT0_NONE, 1 },
  /*A9h*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*AAh*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*ABh*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*ACh*/{ "IDvVar", PCPT_I2, 0, 3 },
  /*ADh*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*AEh*/{ "MulUI1", NULL, PCT0_NONE, 1 },
  /*AFh*/{ "MulI2", NULL, PCT0_NONE, 1 },
  /*B0h*/{ "MulI4", NULL, PCT0_NONE, 1 },
  /*B1h*/{ "MulR8", NULL, PCT0_NONE, 1 },          //ON:MulR4
  /*B2h*/{ "MulR8_1", NULL, PCT0_NONE, 1 },        //ON:MulR4
  /*B3h*/{ "MulCy", NULL, PCT0_NONE, 1 },
  /*B4h*/{ "MulVar", PCPT_I2, 0, 3 },
  /*B5h*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*B6h*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*B7h*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*B8h*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*B9h*/{ "DivR4", NULL, PCT0_NONE, 1 },          //AN:DivR8
  /*BAh*/{ "DivR4_1", NULL, PCT0_NONE, 1 },        //AN:DivR8
  /*BBh*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*BCh*/{ "DivVar", PCPT_I2, 0, 3 },
  /*BDh*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*BEh*/{ "NotUI1", NULL, PCT0_NONE, 1 },
  /*BFh*/{ "NotI4", NULL, PCT0_NONE, 1 },
  /*C0h*/{ "NotI4_1", NULL, PCT0_NONE, 1 },
  /*C1h*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*C2h*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*C3h*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*C4h*/{ "NotVar", PCPT_I2, 0, 3 },
  /*C5h*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*C6h*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*C7h*/{ "UMiI2", NULL, PCT0_NONE, 1 },
  /*C8h*/{ "UMiI4", NULL, PCT0_NONE, 1 },          //ON:UMiI2
  /*C9h*/{ "UMiR8", NULL, PCT0_NONE, 1 },          //ON:UMiR4
  /*CAh*/{ "UMiR8_1", NULL, PCT0_NONE, 1 },        //ON:UMiR4
  /*CBh*/{ "UMiCy", NULL, PCT0_NONE, 1 },
  /*CCh*/{ "UMiVar", PCPT_I2, 0, 3 },
  /*CDh*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*CEh*/{ "PwrVar", PCPT_I2, 0, 3 },
  /*CFh*/{ "PwrR8R8", NULL, PCT0_NONE, 1 },
  /*D0h*/{ "PwrR8I2", NULL, PCT0_NONE, 1 },
  /*D1h*/{ "MulCyI2", NULL, PCT0_NONE, 1 },
  /*D2h*/{ "FnObjIs", NULL, PCT0_NONE, 1 },         //!!!!!!!!!!
  /*D3h*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*D4h*/{ "AbsI2", NULL, PCT0_NONE, 1 },
  /*D5h*/{ "AbsI4", NULL, PCT0_NONE, 1 },
  /*D6h*/{ "AbsR8", NULL, PCT0_NONE, 1 },        //ON:FnAbsR4
  /*D7h*/{ "AbsR8_1", NULL, PCT0_NONE, 1 },      //ON:FnAbsR4
  /*D8h*/{ "AbsCy", NULL, PCT0_NONE, 1 },
  /*D9h*/{ "FnAbsVar", PCPT_I2, 0, 3 },
  /*DAh*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*DBh*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*DCh*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*DDh*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*DEh*/{ "FixR8", NULL, PCT0_NONE, 1 },        //ON:FnFixR4
  /*DFh*/{ "FixR8_1", NULL, PCT0_NONE, 1 },      //ON:FnFixR4
  /*E0h*/{ "FnFixCy", NULL, PCT0_NONE, 1 },
  /*E1h*/{ "FnFixVar", PCPT_I2, 0, 3 },
  /*E2h*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*E3h*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*E4h*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*E5h*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*E6h*/{ "IntR4", NULL, PCT0_NONE, 1 },          //ON:FnIntR8
  /*E7h*/{ "IntR4_1", NULL, PCT0_NONE, 1 },        //ON:FnIntR8
  /*E8h*/{ "FnIntCy", NULL, PCT0_NONE, 1 },
  /*E9h*/{ "FnIntVar", NULL, 0, 3 },
  /*EAh*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*EBh*/{ "FnLenVar", PCPT_I2, 0, 3 },
  /*ECh*/{ "FnLenStr", NULL, PCT0_NONE, 1 },
  /*EDh*/{ "FnLenBVar", PCPT_I2, 0, 3 },
  /*EEh*/{ "LenBStr", NULL, PCT0_NONE, 1 },
  /*EFh*/{ "FnConcatVar", PCPT_I2, 0, 3 },
  /*F0h*/{ "FnConcatStr", NULL, PCT0_NONE, 1 },
  /*F1h*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*F2h*/{ "SgnI2", NULL, PCT0_NONE, 1 },
  /*F3h*/{ "SgnI2_1", NULL, PCT0_NONE, 1 },
  /*F4h*/{ "SgnI4", NULL, PCT0_NONE, 1 },
  /*F5h*/{ "SgnR4", NULL, PCT0_NONE, 1 },        //ON:FnSgnR8
  /*F6h*/{ "SgnR4_1", NULL, PCT0_NONE, 1 },
  /*F7h*/{ "SgnCy", NULL, PCT0_NONE, 1 },
  /*F8h*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*F9h*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*FAh*/{ "FnSeekFile", NULL, PCT0_NONE, 1 },
  /*FBh*/{ "FnNameFile", NULL, PCT0_NONE, 1 },
  /*FCh*/{ "FnCStrI2", NULL, PCT0_NONE, 1 },
  /*FDh*/{ "FnCStrI2_1", NULL, PCT0_NONE, 1 },
  /*FEh*/{ "FnCStrI4", NULL, PCT0_NONE, 1 },
  /*FFh*/{ "FnCStrR4", NULL, PCT0_NONE, 1 }
};

/*P-Code 双字节指令集1 (扩展指令集) 0xFC前缀*/
const VB_PCODE_INSTRUCTIONSET PCodeLead1Inst[256] =
{
  /*00h*/{ "FnCStrR8", NULL, 0, 1 },
  /*01h*/{ "FnCStrCy", NULL, PCT0_NONE, 1 },
  /*02h*/{ "FnCStrVar", NULL, PCT0_NONE, 1 },
  /*03h*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*04h*/{ "CCyI2", NULL, 0, 1 },
  /*05h*/{ "CCyI2_1", NULL, 0, 1 },
  /*06h*/{ "CCyI4", NULL, 0, 1 },
  /*07h*/{ "CCyR8", NULL, 0, 1 },                     //ON:CCyR4
  /*08h*/{ "CCyR8_1", NULL, 0, 1 },                   //ON:CCyR4
  /*09h*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*0Ah*/{ "FnCCyVar", NULL, PCT0_NONE, 1 },
  /*0Bh*/{ "FnCCyStr", NULL, PCT0_NONE, 1 },
  /*0Ch*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*0Dh*/{ "CUI1I2", NULL, 0, 1 },
  /*0Eh*/{ "CUI1I4", NULL, 0, 1 },
  /*0Fh*/{ "CUI1R4", NULL, 0, 1 },
  /*10h*/{ "CUI1R4_1", NULL, 0, 1 },
  /*11h*/{ "CUI1Cy", NULL, PCT0_NONE, 1 },
  /*12h*/{ "FnCUI1Var", NULL, PCT0_NONE, 1 },
  /*13h*/{ "FnCUI1I2Str", NULL, PCT0_NONE, 1 },     //Convert to UI1 form vbaI2Str
  /*14h*/{ "CI2UI1", NULL, 0, 1 },
  /*15h*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*16h*/{ "CI2I4", NULL, 0, 1 },
  /*17h*/{ "CI2R8", NULL, 0, 1 },
  /*18h*/{ "CI2R8_1", NULL, 0, 1 },
  /*19h*/{ "CI2Cy", NULL, PCT0_NONE, 1 },
  /*1Ah*/{ "FnCI2Var", NULL, PCT0_NONE, 1 },
  /*1Bh*/{ "FnCI2Str", NULL, PCT0_NONE, 1 },
  /*1Ch*/{ "CI4I2", NULL, 0, 1 },                 //ON:CI4UI1
  /*1Dh*/{ "CI4I2_1", NULL, 0, 1 },               //ON:CI4UI1
  /*1Eh*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*1Fh*/{ "CI4R8", NULL, 0, 1 },
  /*20h*/{ "CI4R8_1", NULL, 0, 1 },
  /*21h*/{ "CI4Cy", NULL, PCT0_NONE, 1 },
  /*22h*/{ "FnCI4Var", NULL, PCT0_NONE, 1 },
  /*23h*/{ "FnCI4Str", NULL, PCT0_NONE, 1 },
  /*24h*/{ "CR8I2", NULL, 0, 1 },                 //ON:FnCSngI2
  /*25h*/{ "CR8I2_1", NULL, 0, 1 },               //ON:FnCSngI2
  /*26h*/{ "CR8I4", NULL, 0, 1 },                 //ON:CR4I4
  /*27h*/{ "CR4R8", NULL, 0, 1 },
  /*28h*/{ "CR4R8", NULL, 0, 1 },
  /*29h*/{ "CR4Cy", NULL, PCT0_NONE, 1 },          //ON:CR8Cy
  /*2Ah*/{ "FnCR8Var", NULL, PCT0_NONE, 1 },
  /*2Bh*/{ "FnCR4Str", NULL, PCT0_NONE, 1 },
  /*2Ch*/{ "CR8I2_2", NULL, PCT0_NONE, 1 },          //ON:FnCSngI2
  /*2Dh*/{ "CR8I2_3", NULL, PCT0_NONE, 1 },          //ON:FnCSngI2
  /*2Eh*/{ "CR8I4", NULL, PCT0_NONE, 1 },            //ON:CR4I4
  /*2Fh*/{ "CR8R8", NULL, 0, 1 },                   //ON:CR8R4
  /*30h*/{ "CR8R8_1", NULL, PCT0_NONE, 1 },          //ON:CR8R4
  /*31h*/{ "CR4Cy", NULL, 0, 1 },                   //ON:CR8Cy
  /*32h*/{ "FnCR8Var", NULL, PCT0_NONE, 1 },
  /*33h*/{ "FnCR8Str", NULL, PCT0_NONE, 1 },
  /*34h*/{ "CAdVar", NULL, PCT0_NONE, 1 },
  /*35h*/{ "CRefVarAry", NULL, PCT0_NONE, 1 },
  /*36h*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*37h*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*38h*/{ "CUI1Bool", NULL, PCT0_NONE, 1 },
  /*39h*/{ "CDblCy", NULL, PCT0_NONE, 1 },
  /*3Ah*/{ "CDblR8", NULL, PCT0_NONE, 1 },
  /*3Bh*/{ "CDblR8_1", NULL, PCT0_NONE, 1 },
  /*3Ch*/{ "CR8I2", NULL, 0, 1 },                     //ON:FnCSngI2,FnCSngUI1
  /*3Dh*/{ "CR8I2_1", NULL, PCT0_NONE, 1 },            //ON:FnCSngI2,FnCSngUI1
  /*3Eh*/{ "CSngI4", NULL, 0, 1 },
  /*3Fh*/{ "CSngR4", NULL, PCT0_NONE, 1 },             //ON:CSng
  /*40h*/{ "CSngR4_1", NULL, PCT0_NONE, 1 },           //ON:CSng
  /*41h*/{ "CSngCy", NULL, PCT0_NONE, 1 },
  /*42h*/{ "FnCSngVar", NULL, PCT0_NONE, 1 },
  /*43h*/{ "FnCR4Str_1", NULL, PCT0_NONE, 1 },           //AN:FnCSngStr
  /*44h*/{ "FnCByteVar", NULL, PCT0_NONE, 1 },
  /*45h*/{ "FnCIntVar", NULL, PCT0_NONE, 1 },
  /*46h*/{ "FnCLngVar", NULL, PCT0_NONE, 1 },
  /*47h*/{ "CDateR8", NULL, PCT0_NONE, 1 },
  /*48h*/{ "FnCDblVar", NULL, PCT0_NONE, 1 },
  /*49h*/{ "FnCCyVar", NULL, 0, 1 },                  //ON:FnCCurVar
  /*4Ah*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*4Bh*/{ "FnCStrVar", NULL, 0, 1 },
  /*4Ch*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*4Dh*/{ "FnCBoolVar", NULL, PCT0_NONE, 1 },
  /*4Eh*/{ "FnCDateVar", NULL, PCT0_NONE, 1 },
  /*4Fh*/{ "FnCDateVar_1", NULL, PCT0_NONE, 1 },
  /*50h*/{ "CBoolI2", NULL, PCT0_NONE, 1 },                 //ON:CBoolUI1
  /*51h*/{ "CBoolI2_1", NULL, PCT0_NONE, 1 },               //ON:CBoolUI1
  /*52h*/{ "CBoolI4", NULL, PCT0_NONE, 1 },
  /*53h*/{ "CBoolR8", NULL, PCT0_NONE, 1 },
  /*54h*/{ "CBoolR8_1", NULL, PCT0_NONE, 1 },
  /*55h*/{ "CBoolCy", NULL, PCT0_NONE, 1 },
  /*56h*/{ "FnCBoolVar", NULL, 0, 1 },
  /*57h*/{ "FnCBoolStr", NULL, PCT0_NONE, 1 },
  /*58h*/{ "FnCStr2Uni", NULL, 0, 1 },
  /*59h*/{ "FnCStrAry2Uni", NULL, PCT0_NONE, 1 },
  /*5Ah*/{ "FnCStr2Ansi", NULL, PCT0_NONE, 1 },
  /*5Bh*/{ "FnCStrAry2Ansi", NULL, PCT0_NONE, 1 },
  /*5Ch*/{ "PopAdLd4", NULL, PCT0_NONE, 1 },
  /*5Dh*/{ "FnCRecAnsi2Uni", PCPT_I2, 0, 3 },
  /*5Eh*/{ "FnCRecUni2Ansi", PCPT_I2, 0, 3 },
  /*5Fh*/{ "FnCStr2Vec", PCPT_I2, 0, 3 },
  /*60h*/{ "FnCVar2Vec", PCPT_I2, 0, 3 },
  /*61h*/{ "CVec2Var", PCPT_I2_I2, 0, 5 },
  /*62h*/{ "SetLastError", NULL, PCT0_NONE, 1 },
  /*63h*/{ "LitNothing", NULL, 0, 1 },
  /*64h*/{ "LitVar_Null", PCPT_I2, 0, 3 },
  /*65h*/{ "LitVar_TRUE", PCPT_I2, 0, 3 },
  /*66h*/{ "LitVar_FALSE", PCPT_I2, 0, 3 },
  /*67h*/{ "LitVar_Empty", PCPT_I2, 0, 3 },
  /*68h*/{ "LitVar_Missing", PCPT_I2, 0, 3 },
  /*69h*/{ "VCallHresult_1", PCPT_VTABLE1, 0, 5 },
  /*6Ah*/{ "ThisVCallHresult", PCPT_I2_N2, 0, 5 },
  /*6Bh*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*6Ch*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*6Dh*/{ "ExitProcHresult_1", NULL, PCFH_ENDPROC | PCT0_NONE, 1 },                /*******/
  /*6Eh*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*6Fh*/{ "FnCheckTypeVar", PCPT_I2, 0, 3 },
  /*70h*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*71h*/{ "CUnkVar", NULL, PCT0_NONE, 1 },
  /*72h*/{ "CVarUnk", PCPT_I2, 0, 3 },
  /*73h*/{ "LdPrUnkVar", NULL, PCT0_NONE, 1 },
  /*74h*/{ "FLdLateIdUnkVar", PCPT_I2_I4_I2, 0, 9 },
  /*75h*/{ "FnGetRec3", NULL, PCT0_NONE, 1 },
  /*76h*/{ "FnGetRec4", NULL, PCT0_NONE, 1 },
  /*77h*/{ "FnPutRec3", NULL, 0, 1 },
  /*78h*/{ "FnPutRec4", NULL, 0, 1 },
  /*79h*/{ "FnGetRecOwner3", PCPT_JU_VP, 0, -3 },
  /*7Ah*/{ "FnGetRecOwner4", PCPT_JU_VP, 0, -3 },
  /*7Bh*/{ "FnPutRecOwner3", PCPT_JU_VP, 0, -3 },
  /*7Ch*/{ "FnPutRecOwner4", PCPT_JU_VP, 0, -3 },
  /*7Dh*/{ "InputChanZero", NULL, 0, 1 },             //ON:Input
  /*7Eh*/{ "InputDoneZero", NULL, PCT0_NONE, 1 },      //ON:InputDone
  /*7Fh*/{ "InputItemUI1", NULL, 0, 1 },
  /*80h*/{ "InputItemI2", NULL, PCT0_NONE, 1 },
  /*81h*/{ "InputItemI4", NULL, PCT0_NONE, 1 },
  /*82h*/{ "InputItemR4", NULL, PCT0_NONE, 1 },
  /*83h*/{ "InputItemR8", NULL, 0, 1 },
  /*84h*/{ "InputItemCy", NULL, PCT0_NONE, 1 },
  /*85h*/{ "InputItemVar", NULL, PCT0_NONE, 1 },
  /*86h*/{ "InputItemStr", NULL, PCT0_NONE, 1 },
  /*87h*/{ "InputItemBool", NULL, PCT0_NONE, 1 },
  /*88h*/{ "InputItemDate", NULL, PCT0_NONE, 1 },
  /*89h*/{ "PopFPR4", NULL, PCT0_NONE, 1 },
  /*8Ah*/{ "PopFPR8", NULL, PCT0_NONE, 1 },
  /*8Bh*/{ "PopAd", NULL, PCT0_NONE, 1 },
  /*8Ch*/{ "PopAdLdVar", NULL, PCT0_NONE, 1 },
  /*8Dh*/{ "AryLdPr", PCPT_I2, 0, 3 },
  /*8Eh*/{ "AryLdRf", PCPT_I2, 0, 3 },
  /*8Fh*/{ "ParmAry1St", PCPT_I2_I2, 0, 5 },
  /*90h*/{ "Ary1LdUI1", NULL, PCT0_NONE, 1 },
  /*91h*/{ "Ary1LdI2", NULL, PCT0_NONE, 1 },
  /*92h*/{ "Ary1LdI4", NULL, PCT0_NONE, 1 },           //MON:Ary1LdAd
  /*93h*/{ "Ary1LdI4_1", NULL, PCT0_NONE, 1 },         //MON:Ary1LdAd
  /*94h*/{ "Ary1LdCy", NULL, 0, 1 },                  //ON:Ary1LdR8
  /*95h*/{ "Ary1LdCy_1", NULL, 0, 1 },                //ON:Ary1LdR8
  /*96h*/{ "Ary1LdRfVar", NULL, PCT0_NONE, 1 },
  /*97h*/{ "Ary1LdI4_2", NULL, PCT0_NONE, 1 },         //MON:Ary1LdAd
  /*98h*/{ "Ary1LdI4_3", NULL, PCT0_NONE, 1 },         //MON:Ary1LdAd
  /*99h*/{ "Ary1LdFPR4", NULL, PCT0_NONE, 1 },
  /*9Ah*/{ "Ary1LdFPR8", NULL, PCT0_NONE, 1 },
  /*9Bh*/{ "Ary1LdPrVar", NULL, PCT0_NONE, 1 },        //ON:Ary1LdPr
  /*9Ch*/{ "Ary1LdRfR8", NULL, 0, 1 },                //ON:Ary1LdRf
  /*9Dh*/{ "Ary1LdVar", NULL, 0, 1 },
  /*9Eh*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*9Fh*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*A0h*/{ "Ary1StUI1", NULL, PCT0_NONE, 1 },
  /*A1h*/{ "Ary1StI2", NULL, 0, 1 },
  /*A2h*/{ "Ary1StI4", NULL, 0, 1 },                  //ON:Ary1StR4
  /*A3h*/{ "Ary1StI4_1", NULL, 0, 1 },                //ON:Ary1StR4
  /*A4h*/{ "Ary1StR8", NULL, 0, 1 },                  //ON:Ary1StCy
  /*A5h*/{ "Ary1StR8_1", NULL, 0, 1 },                //ON:Ary1StCy
  /*A6h*/{ "Ary1StVar", NULL, PCT0_NONE, 1 },
  /*A7h*/{ "Ary1StStr", NULL, PCT0_NONE, 1 },
  /*A8h*/{ "Ary1StAdFunc", NULL, PCT0_NONE, 1 },       //ON:Ary1StAd           edx:0
  /*A9h*/{ "Ary1StFPR4", NULL, 0, 1 },
  /*AAh*/{ "Ary1StFPR8", NULL, 0, 1 },
  /*ABh*/{ "Ary1StVarAdFunc", NULL, 0, 1 },           //ON:Ary1StVarAd        edx:0
  /*ACh*/{ "Ary1StVarAdFuncM1", NULL, 0, 1 },         //ON:Ary1StVarAdFunc    edx:-1
  /*ADh*/{ "Ary1StVarUnkFunc", NULL, 0, 1 },          //ON:Ary1StVarUnk       edx:0
  /*AEh*/{ "Ary1StVarUnkFuncM1", NULL, PCT0_NONE, 1 }, //ON:Ary1StVarUnkFunc   edx:-1
  /*AFh*/{ "Ary1StAdFuncM1", NULL, PCT0_NONE, 1 },     //ON:Ary1StAdFunc       edx:-1
  /*B0h*/{ "Ary1StVarCopy", NULL, PCT0_NONE, 1 },
  /*B1h*/{ "Ary1StStrCopy", NULL, PCT0_NONE, 1 },
  /*B2h*/{ "Ary1LdRfVarg", PCPT_I2, 0, 3 },
  /*B3h*/{ "Ary1LdVarg", NULL, PCT0_NONE, 1 },
  /*B4h*/{ "Ary1LdRfVargParam", NULL, 0, 1 },
  /*B5h*/{ "Ary1StVargCopyM1", NULL, 0, 1 },            //ON:Ary1StVarg         edx:-1
  /*B6h*/{ "Ary1StVargCopy", NULL, 0, 1 },              //edx:0
  /*B7h*/{ "Ary1StVargAdFunc", NULL, 0, 1 },            //ON:Ary1StVargAd       edx:0
  /*B8h*/{ "Ary1StVargAdFuncM1", NULL, 0, 1 },          //ON:Ary1StVargAdFunc   edx:-1
  /*B9h*/{ "Ary1StVargUnkFunc", NULL, PCT0_NONE, 1 },    //ON:Ary1StVargUnk      edx:0
  /*BAh*/{ "Ary1StVargUnkFuncM1", NULL, PCT0_NONE, 1 },  //ON:Ary1StVargUnkFunc  edx:-1
  /*BBh*/{ "FnMidVar", NULL, 0, 1 },
  /*BCh*/{ "FnMidBStr", PCPT_U2, 0, 3 },
  /*BDh*/{ "FnMidVarB", NULL, 0, 1 },               //ON:MidBVar
  /*BEh*/{ "FnMidBStrB", PCPT_U2, 0, 3 },
  /*BFh*/{ "FnLineInputVar", NULL, PCT0_NONE, 1 },
  /*C0h*/{ "FnLineInputStr", NULL, PCT0_NONE, 1 },
  /*C1h*/{ "RaiseError", NULL, PCFH_ENDPROC, 1 },                   //ON:Error
  /*C2h*/{ "FnStopExe", NULL, PCFH_ENDPROC, 1 },                    //ON:Stop
  /*C3h*/{ "FnErase", NULL, 0, 1 },
  /*C4h*/{ "LargeBos_2", PCPT_N1, PCFLH_DBG, 2 },
  /*C5h*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*C6h*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*C7h*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*C8h*/{ "End", NULL, PCT0_NONE, 1 },
  /*C9h*/{ "Return", NULL, PCT0_NONE, 1 },
  /*CAh*/{ "FnLBound", NULL, PCT0_NONE, 1 },
  /*CBh*/{ "FnUBound", NULL, PCT0_NONE, 1 },
  /*CCh*/{ "ExitProcUI1", NULL, PCFH_ENDPROC, 1 },
  /*CDh*/{ "ExitProcI2_1", NULL, PCFH_ENDPROC | PCT0_NONE, 1 },
  /*CEh*/{ "ExitProcI4_1", NULL, PCFH_ENDPROC | PCT0_STD, 1 },        //ON:ExitProcAd
  /*CFh*/{ "ExitProcR4_1", NULL, PCFH_ENDPROC | PCT0_NONE, 1 },
  /*D0h*/{ "ExitProcR8_1", NULL, PCFH_ENDPROC | PCT0_NONE, 1 },
  /*D1h*/{ "ExitProcCy_1", NULL, PCFH_ENDPROC | PCT0_NONE, 1 },
  /*D2h*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*D3h*/{ "ExitProcI4_2", NULL, PCFH_ENDPROC, 1 },                  //ON:ExitProcAd
  /*D4h*/{ "ExitProcI4_3", NULL, PCFH_ENDPROC, 1 },                  //ON:ExitProcAd
  /*D5h*/{ "ExitProcI4_4", NULL, PCFH_ENDPROC, 1 },                  //ON:ExitProcAd
  /*D6h*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*D7h*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*D8h*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*D9h*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*DAh*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*DBh*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*DCh*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*DDh*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*DEh*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*DFh*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*E0h*/{ "FLdUI1", PCPT_I2, 0, 3 },
  /*E1h*/{ "FLdI2_1", PCPT_I2, 0, 3 },
  /*E2h*/{ "FLdI4_1", PCPT_I2, 0, 3 },
  /*E3h*/{ "FLdI4_2", PCPT_I2, 0, 3 },
  /*E4h*/{ "FLdCy_1", PCPT_I2, 0, 3 },
  /*E5h*/{ "FLdCy_2", PCPT_I2, 0, 3 },
  /*E6h*/{ "FLdRfVar_1", PCPT_I2, 0, 3 },
  /*E7h*/{ "FLdI4_3", PCPT_I2, 0, 3 },
  /*E8h*/{ "FLdI4_4", PCPT_I2, 0, 3 },
  /*E9h*/{ "FLdFPR4_1", PCPT_I2, 0, 3 },
  /*EAh*/{ "FLdFPR8_1", PCPT_I2, 0, 3 },
  /*EBh*/{ "FLdPr_1", PCPT_I2, 0, 3 },
  /*ECh*/{ "FLdRfVar_2", PCPT_I2, 0, 3 },
  /*EDh*/{ "FLdVar", PCPT_I2, 0, 3 },
  /*EEh*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*EFh*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*F0h*/{ "FStUI1", PCPT_PTRARG, 0, 3 },
  /*F1h*/{ "FStI2_1", PCPT_PTRARG, 0, 3 },
  /*F2h*/{ "FStI4_1", PCPT_PTRARG, 0, 3 },
  /*F3h*/{ "FStI4_2", PCPT_PTRARG, 0, 3 },
  /*F4h*/{ "FStCy_1", PCPT_PTRARG, 0, 3 },
  /*F5h*/{ "FStCy_2", PCPT_PTRARG, 0, 3 },
  /*F6h*/{ "FStVar", PCPT_PTRARG, 0, 3 },
  /*F7h*/{ "FStStr_1", PCPT_PTRARG, 0, 3 },
  /*F8h*/{ "FStAd", PCPT_PTRARG, 0, 3 },
  /*F9h*/{ "FStFPR4_1", PCPT_PTRARG, 0, 3 },
  /*FAh*/{ "FStFPR8_1", PCPT_PTRARG, 0, 3 },
  /*FBh*/{ "FStVarAd", PCPT_PTRARG, 0, 3 },
  /*FCh*/{ "FStVarAdFunc", PCPT_PTRARG, 0, 3 },
  /*FDh*/{ "FStVarUnk", PCPT_PTRARG, 0, 3 },
  /*FEh*/{ "FStVarUnkFunc", PCPT_PTRARG, 0, 3 },
  /*FFh*/{ "FStAdFunc", PCPT_PTRARG, 0, 3 }
};

/*P-Code 双字节指令集2 (扩展指令集) 0xFD前缀*/
const VB_PCODE_INSTRUCTIONSET PCodeLead2Inst[256] =
{
  /*00h*/{ "FStVarCopy", PCPT_PTRARG, 0, 3 },
  /*01h*/{ "FStStrCopy", PCPT_PTRARG, 0, 3 },
  /*02h*/{ "HardType", NULL, PCT0_NONE, 1 },
  /*03h*/{ "Branch_1", PCPT_LOC, PCFH_BRANCH, 3 },
  /*04h*/{ "BranchF_1", PCPT_LOC, PCFH_CONDBRAN, 3 },
  /*05h*/{ "BranchFVar_1", PCPT_LOC, PCFH_CONDBRAN, 3 },
  /*06h*/{ "BranchFVarFree", PCPT_LOC, PCFH_CONDBRAN, 3 },
  /*07h*/{ "BranchT_1", PCPT_LOC, PCFH_CONDBRAN, 3 },
  /*08h*/{ "BranchTVar", PCPT_LOC, PCFH_CONDBRAN, 3 },
  /*09h*/{ "BranchTVarFree", PCPT_LOC, PCFH_CONDBRAN, 3 },
  /*0Ah*/{ "Gosub", PCPT_LOC, PCFH_BRANCH, 3 },
  /*0Bh*/{ "OnErrorGoto", PCPT_LOC, 0, 3 },
  /*0Ch*/{ "Resume", PCPT_LOC, PCFH_BRANCH, 3 },
  /*0Dh*/{ "AryLock", PCPT_I2, 0, 3 },
  /*0Eh*/{ "AryUnlock", PCPT_I2, 0, 3 },
  /*0Fh*/{ "AryDescTemp", PCPT_I2, 0, 3 },
  /*10h*/{ "ILdUI1", PCPT_ARG, 0, 3 },
  /*11h*/{ "ILdI2_1", PCPT_ARG, 0, 3 },
  /*12h*/{ "ILdI4_1", PCPT_ARG, 0, 3 },
  /*13h*/{ "ILdI4_2", PCPT_ARG, 0, 3 },
  /*14h*/{ "ILdR8_1", PCPT_ARG, 0, 3 },
  /*15h*/{ "ILdR8_2", PCPT_ARG, 0, 3 },
  /*16h*/{ "ILdRfDarg", PCPT_I2_I2, 0, 5 },
  /*17h*/{ "ILdI4_3", PCPT_ARG, 0, 3 },
  /*18h*/{ "ILdI4_4", PCPT_ARG, 0, 3 },
  /*19h*/{ "ILdFPR4_1", PCPT_ARG, 0, 3 },
  /*1Ah*/{ "ILdFPR8_1", PCPT_ARG, 0, 3 },
  /*1Bh*/{ "ILdPr_1", PCPT_I2, 0, 3 },
  /*1Ch*/{ "FLdI4_5", PCPT_I2, 0, 3 },
  /*1Dh*/{ "ILdDarg", PCPT_I2, 0, 3 },
  /*1Eh*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*1Fh*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*20h*/{ "IStUI1", PCPT_I2, 0, 3 },
  /*21h*/{ "IStI2_1", PCPT_I2, 0, 3 },
  /*22h*/{ "IStI4_1", PCPT_I2, 0, 3 },
  /*23h*/{ "IStI4_2", PCPT_I2, 0, 3 },
  /*24h*/{ "IStCy_1", PCPT_I2, 0, 3 },
  /*25h*/{ "IStCy_2", PCPT_I2, 0, 3 },
  /*26h*/{ "IStDarg_1", PCPT_I2, 0, 3 },
  /*27h*/{ "IStStr", PCPT_I2, 0, 3 },
  /*28h*/{ "IStAd", PCPT_I2, 0, 3 },
  /*29h*/{ "IStFPR4_1", PCPT_I2, 0, 3 },
  /*2Ah*/{ "IStFPR8_1", PCPT_I2, 0, 3 },
  /*2Bh*/{ "IStDargAd", PCPT_I2, 0, 3 },
  /*2Ch*/{ "IStDargAdFunc", PCPT_I2, 0, 3 },
  /*2Dh*/{ "IStDargUnk", PCPT_I2, 0, 3 },
  /*2Eh*/{ "IStDargUnkFunc", PCPT_I2, 0, 3 },
  /*2Fh*/{ "IStAdFunc", PCPT_I2, 0, 3 },
  /*30h*/{ "IStDargCopy", PCPT_I2, 0, 3 },
  /*31h*/{ "IStStrCopy", PCPT_I2, 0, 3 },
  /*32h*/{ "PrintChan", NULL, PCT0_NONE, 1 },
  /*33h*/{ "WriteChan", NULL, PCT0_NONE, 1 },
  /*34h*/{ "PrintComma", NULL, PCT0_NONE, 1 },
  /*35h*/{ "PrintEos", NULL, PCT0_NONE, 1 },
  /*36h*/{ "PrintNL", NULL, PCT0_NONE, 1 },
  /*37h*/{ "PrintItemComma", NULL, PCT0_NONE, 1 },
  /*38h*/{ "PrintItemSemi", NULL, PCT0_NONE, 1 },
  /*39h*/{ "PrintItemNL", NULL, PCT0_NONE, 1 },
  /*3Ah*/{ "PrintObj", PCPT_I2, 0, 3 },
  /*3Bh*/{ "PrintSpc", NULL, PCT0_NONE, 1 },
  /*3Ch*/{ "PrintTab", NULL, 0, 1 },
  /*3Dh*/{ "Close", NULL, PCT0_NONE, 1 },
  /*3Eh*/{ "CloseAll", NULL, 0, 1 },
  /*3Fh*/{ "FLdZeroAd_2", PCPT_I2, 0, 3 },
  /*40h*/{ "IWMemLdUI1", PCPT_I2, 0, 3 },
  /*41h*/{ "IWMemLdI2", PCPT_I2, 0, 3 },
  /*42h*/{ "IWMemLdI4", PCPT_I2, 0, 3 },
  /*43h*/{ "IWMemLdI4_1", PCPT_I2, 0, 3 },
  /*44h*/{ "IWMemLdCy", PCPT_I2, 0, 3 },
  /*45h*/{ "IWMemLdCy_1", PCPT_I2, 0, 3 },
  /*46h*/{ "IWMemLdRfDarg", PCPT_I2_I2, 0, 5 },
  /*47h*/{ "IWMemLdI4_2", PCPT_I2, 0, 3 },
  /*48h*/{ "IWMemLdI4_3", PCPT_I2, 0, 3 },
  /*49h*/{ "IWMemLdFPR4", PCPT_I2, 0, 3 },
  /*4Ah*/{ "IWMemLdFPR8", PCPT_I2, 0, 3 },
  /*4Bh*/{ "WMemLdPr", PCPT_I2, 0, 3 },
  /*4Ch*/{ "IWMemLdRf", PCPT_I2, 0, 3 },
  /*4Dh*/{ "IWMemLdDarg", PCPT_I2, 0, 3 },
  /*4Eh*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*4Fh*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*50h*/{ "IWMemStUI1", PCPT_I2, 0, 3 },
  /*51h*/{ "IWMemStI2", PCPT_I2, 0, 3 },
  /*52h*/{ "IWMemStI4", PCPT_I2, 0, 3 },
  /*53h*/{ "IWMemStI4_1", PCPT_I2, 0, 3 },
  /*54h*/{ "IWMemStCy", PCPT_I2, 0, 3 },
  /*55h*/{ "IWMemStCy_1", PCPT_I2, 0, 3 },
  /*56h*/{ "IWMemStDarg", PCPT_I2, 0, 3 },
  /*57h*/{ "IWMemStStr", PCPT_I2, 0, 3 },
  /*58h*/{ "IWMemStAd", PCPT_I2, 0, 3 },
  /*59h*/{ "IWMemStFPR4", PCPT_I2, 0, 3 },
  /*5Ah*/{ "IWMemStFPR8", PCPT_I2, 0, 3 },
  /*5Bh*/{ "IWMemStDargAd", PCPT_I2, 0, 3 },
  /*5Ch*/{ "IWMemStDargAdFunc", PCPT_I2, 0, 3 },
  /*5Dh*/{ "IWMemStDargUnk", PCPT_I2, 0, 3 },
  /*5Eh*/{ "IWMemStDargUnkFunc", PCPT_I2, 0, 3 },
  /*5Fh*/{ "IWMemStAdFunc", PCPT_I2, 0, 3 },
  /*60h*/{ "IWMemStDargCopy", PCPT_I2, 0, 3 },
  /*61h*/{ "IWMemStStrCopy", PCPT_I2, 0, 3 },
  /*62h*/{ "FLdZeroAd_3", PCPT_I2, 0, 3 },
  /*63h*/{ "FnFStVarNoPop", PCPT_I2, 0, 3 },
  /*64h*/{ "FStStrNoPop_1", PCPT_I2, 0, 3 },
  /*65h*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*66h*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*67h*/{ "CVarUI1", PCPT_I2, 0, 3 },
  /*68h*/{ "CVarI2_1", PCPT_I2, 0, 3 },
  /*69h*/{ "CVarI4", PCPT_I2, 0, 3 },
  /*6Ah*/{ "CVarR4", PCPT_I2, 0, 3 },
  /*6Bh*/{ "CVarR8", PCPT_I2, 0, 3 },
  /*6Ch*/{ "CVarCy_1", PCPT_I2, 0, 3 },
  /*6Dh*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*6Eh*/{ "CVarStr_1", PCPT_I2, 0, 3 },
  /*6Fh*/{ "CVarAd", PCPT_I2, 0, 3 },
  /*70h*/{ "MemLdUI1", PCPT_I2, 0, 3 },
  /*71h*/{ "MemLdI2_1", PCPT_I2, 0, 3 },
  /*72h*/{ "MemLdI4_1", PCPT_I2, 0, 3 },
  /*73h*/{ "MemLdI4_2", PCPT_I2, 0, 3 },
  /*74h*/{ "MemLdCy_1", PCPT_I2, 0, 3 },
  /*75h*/{ "MemLdCy_2", PCPT_I2, 0, 3 },
  /*76h*/{ "MemLdRfVar_1", PCPT_I2, 0, 3 },
  /*77h*/{ "MemLdI4_3", PCPT_I2, 0, 3 },
  /*78h*/{ "MemLdI4_4", PCPT_I2, 0, 3 },
  /*79h*/{ "MemLdFPR4_1", PCPT_I2, 0, 3 },
  /*7Ah*/{ "MemLdFPR8_1", PCPT_I2, 0, 3 },
  /*7Bh*/{ "MemLdPr_1", PCPT_I2, 0, 3 },
  /*7Ch*/{ "MemLdRfVar_2", PCPT_I2, 0, 3 },
  /*7Dh*/{ "MemLdVar", PCPT_I2, 0, 3 },
  /*7Eh*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*7Fh*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*80h*/{ "MemStUI1", PCPT_I2, 0, 3 },
  /*81h*/{ "MemStI2_1", PCPT_I2, 0, 3 },
  /*82h*/{ "MemStI4_1", PCPT_I2, 0, 3 },
  /*83h*/{ "MemStI4_2", PCPT_I2, 0, 3 },
  /*84h*/{ "MemStCy_1", PCPT_I2, 0, 3 },
  /*85h*/{ "MemStCy_2", PCPT_I2, 0, 3 },
  /*86h*/{ "FnMemStVar", PCPT_I2, 0, 3 },
  /*87h*/{ "MemStStr", PCPT_I2, 0, 3 },
  /*88h*/{ "MemStAd", PCPT_I2, 0, 3 },
  /*89h*/{ "MemStFPR4_1", PCPT_I2, 0, 3 },
  /*8Ah*/{ "MemStFPR8_1", PCPT_I2, 0, 3 },
  /*8Bh*/{ "MemStVarAd", PCPT_I2, 0, 3 },
  /*8Ch*/{ "MemStVarAdFunc", PCPT_I2, 0, 3 },
  /*8Dh*/{ "MemStVarUnk", PCPT_I2, 0, 3 },
  /*8Eh*/{ "MemStVarUnkFunc", PCPT_I2, 0, 3 },
  /*8Fh*/{ "MemStAdFunc", PCPT_I2, 0, 3 },
  /*90h*/{ "MemStVarCopy", PCPT_I2, 0, 3 },
  /*91h*/{ "MemStStrCopy", PCPT_I2, 0, 3 },
  /*92h*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*93h*/{ "CDargRef", PCPT_I2, 0, 3 },
  /*94h*/{ "CVarRef", PCPT_I2_I2, 0, 5 },
  /*95h*/{ "ExitProcCb", PCPT_I2, PCFH_ENDPROC, 3 },
  /*96h*/{ "ExitProcCbStack", PCPT_I2, PCFH_ENDPROC, 3 },
  /*97h*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*98h*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*99h*/{ "FFree1Var_1", PCPT_I2, PCT0_CLEARMEMORY, 3 },
  /*9Ah*/{ "FFree1Str_1", PCPT_I2, PCT0_ONESTRING, 3 },
  /*9Bh*/{ "FFree1Ad_1", PCPT_I2, 0, 3 },
  /*9Ch*/{ "FStAdNoPop", PCPT_I2, 0, 3 },
  /*9Dh*/{ "FStAdFuncNoPop_1", PCPT_I2, 0, 3 },
  /*9Eh*/{ "FLdPrThis_1", NULL, PCT0_NONE, 1 },
  /*9Fh*/{ "LdPrVar", NULL, PCT0_NONE, 1 },
  /*A0h*/{ "ImpAdLdUI1", PCPT_CALL, 0, 3 },
  /*A1h*/{ "ImpAdLdI2_1", PCPT_CALL, 0, 3 },
  /*A2h*/{ "ImpAdLdI4_1", PCPT_CALL, 0, 3 },
  /*A3h*/{ "ImpAdLdI4_2", PCPT_CALL, 0, 3 },
  /*A4h*/{ "ImpAdLdCy_1", PCPT_CALL, 0, 3 },
  /*A5h*/{ "ImpAdLdCy_2", PCPT_CALL, 0, 3 },
  /*A6h*/{ "ImpAdLdRfVar_1", PCPT_CALL, 0, 3 },
  /*A7h*/{ "ImpAdLdI4_3", PCPT_CALL, 0, 3 },
  /*A8h*/{ "ImpAdLdI4_4", PCPT_CALL, 0, 3 },
  /*A9h*/{ "ImpAdLdFPR4_1", PCPT_CALL, 0, 3 },
  /*AAh*/{ "ImpAdLdFPR8_1", PCPT_CALL, 0, 3 },
  /*ABh*/{ "ImpAdLdPr_1", PCPT_CALL, 0, 3 },
  /*ACh*/{ "ImpAdLdRfVar_2", PCPT_CALL, 0, 3 },
  /*ADh*/{ "ImpAdLdVar", PCPT_CALL, 0, 3 },
  /*AEh*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*AFh*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*B0h*/{ "ImpAdStUI1", PCPT_I2, 0, 3 },
  /*B1h*/{ "ImpAdStI2_1", PCPT_I2, 0, 3 },
  /*B2h*/{ "ImpAdStI4_1", PCPT_I2, 0, 3 },
  /*B3h*/{ "ImpAdStI4_2", PCPT_I2, 0, 3 },
  /*B4h*/{ "ImpAdStCy_1", PCPT_I2, 0, 3 },
  /*B5h*/{ "ImpAdStCy_2", PCPT_I2, 0, 3 },
  /*B6h*/{ "ImpAdStVar", PCPT_I2, 0, 3 },
  /*B7h*/{ "ImpAdStStr", PCPT_I2, 0, 3 },
  /*B8h*/{ "ImpAdStAd", PCPT_I2, 0, 3 },
  /*B9h*/{ "ImpAdStFPR4_1", PCPT_I2, 0, 3 },
  /*BAh*/{ "ImpAdStFPR8_1", PCPT_I2, 0, 3 },
  /*BBh*/{ "ImpAdStVarAd", PCPT_I2, 0, 3 },
  /*BCh*/{ "ImpAdStVarAdFunc", PCPT_I2, 0, 3 },
  /*BDh*/{ "ImpAdStVarUnk", PCPT_I2, 0, 3 },
  /*BEh*/{ "ImpAdStVarUnkFunc", PCPT_I2, 0, 3 },
  /*BFh*/{ "ImpAdStAdFunc", PCPT_I2, 0, 3 },
  /*C0h*/{ "ImpAdStVarCopy", PCPT_I2, 0, 3 },
  /*C1h*/{ "ImpAdStStrCopy", PCPT_I2, 0, 3 },
  /*C2h*/{ "PopTmpLdAd1", PCPT_I2, 0, 3 },
  /*C3h*/{ "PopTmpLdAd2_1", PCPT_I2, 0, 3 },
  /*C4h*/{ "PopTmpLdAd4_1", PCPT_I2, 0, 3 },
  /*C5h*/{ "PopTmpLdAd8_1", PCPT_I2, 0, 3 },
  /*C6h*/{ "PopTmpLdAdVar", PCPT_I2, 0, 3 },
  /*C7h*/{ "PopTmpLdAd4_2", PCPT_I2, 0, 3 },
  /*C8h*/{ "PopTmpLdAdFPR4", PCPT_I2, 0, 3 },
  /*C9h*/{ "PopTmpLdAdFPR8", PCPT_I2, 0, 3 },
  /*CAh*/{ "CopyBytes", PCPT_I2, 0, 3 },
  /*CBh*/{ "ExitForCollObj", PCPT_I2_U2, PCT0_NONE, 5 },
  /*CCh*/{ "ExitForCollObj_1", PCPT_I2_U2, 0, 5 },
  /*CDh*/{ "ExitForCollObj_2", PCPT_I2_U2, 0, 5 },
  /*CEh*/{ "ExitForAryVar", PCPT_I2_U2, 0, 5 },
  /*CFh*/{ "ExitForVar", PCPT_I2_U2, PCT0_NONE, 5 },
  /*D0h*/{ "FMemLdUI1", PCPT_I2_I2, 0, 5 },
  /*D1h*/{ "FMemLdI2_1", PCPT_I2_I2, 0, 5 },
  /*D2h*/{ "FMemLdI4_1", PCPT_I2_I2, 0, 5 },
  /*D3h*/{ "FMemLdI4_2", PCPT_I2_I2, 0, 5 },
  /*D4h*/{ "FMemLdCy_1", PCPT_I2_I2, 0, 5 },
  /*D5h*/{ "FMemLdCy_2", PCPT_I2_I2, 0, 5 },
  /*D6h*/{ "FMemLdRf", PCPT_I2_I2, 0, 5 },
  /*D7h*/{ "FMemLdI4_3", PCPT_I2_I2, 0, 5 },
  /*D8h*/{ "FMemLdI4_4", PCPT_I2_I2, 0, 5 },
  /*D9h*/{ "FMemLdFPR4_1", PCPT_I2_I2, 0, 5 },
  /*DAh*/{ "FMemLdFPR8_2", PCPT_I2_I2, 0, 5 },
  /*DBh*/{ "FMemLdPr_1", PCPT_I2_I2, 0, 5 },
  /*DCh*/{ "FMemLdRfVar_1", PCPT_I2_I2, 0, 5 },
  /*DDh*/{ "FMemLdVar", PCPT_I2_I2, 0, 5 },
  /*DEh*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*DFh*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*E0h*/{ "FMemStUI1", PCPT_I2_I2, 0, 5 },
  /*E1h*/{ "FMemStI2_1", PCPT_I2_I2, 0, 5 },
  /*E2h*/{ "FMemStI4_1", PCPT_I2_I2, 0, 5 },
  /*E3h*/{ "FMemStI4_2", PCPT_I2_I2, 0, 5 },
  /*E4h*/{ "FMemStCy_1", PCPT_I2_I2, 0, 5 },
  /*E5h*/{ "FMemStCy_2", PCPT_I2_I2, 0, 5 },
  /*E6h*/{ "FMemStVar", PCPT_I2_I2, 0, 5 },
  /*E7h*/{ "FMemStStr", PCPT_I2_I2, 0, 5 },
  /*E8h*/{ "FMemStAd", PCPT_I2_I2, 0, 5 },
  /*E9h*/{ "FMemStFPR4_1", PCPT_I2_I2, 0, 5 },
  /*EAh*/{ "FMemStFPR8_1", PCPT_I2_I2, 0, 5 },
  /*EBh*/{ "FMemStVarAd", PCPT_I2_I2, 0, 5 },
  /*ECh*/{ "FMemStVarAdFunc", PCPT_I2_I2, 0, 5 },
  /*EDh*/{ "FMemStVarUnk", PCPT_I2_I2, 0, 5 },
  /*EEh*/{ "FMemStVarUnkFunc", PCPT_I2_I2, 0, 5 },
  /*EFh*/{ "FMemStAdFunc", PCPT_I2_I2, 0, 5 },
  /*F0h*/{ "FMemStVarCopy", PCPT_I2_I2, 0, 5 },
  /*F1h*/{ "FMemStStrCopy_1", PCPT_I2_I2, 0, 5 },
  /*F2h*/{ "FnCastAd_1", PCPT_I2, 0, 3 },
  /*F3h*/{ "FnCastAdVar", PCPT_I2, 0, 3 },
  /*F4h*/{ "New", PCPT_I2, 0, 3 },
  /*F5h*/{ "NewIfPCPT_I2Rf", PCPT_I2, 0, 3 },
  /*F6h*/{ "NewIfPCPT_I2Ad", PCPT_I2, 0, 3 },
  /*F7h*/{ "NewIfPCPT_I2Pr", PCPT_I2, 0, 3 },
  /*F8h*/{ "CVarBoolI2", PCPT_I2, 0, 3 },
  /*F9h*/{ "FnCVarDateVar", PCPT_I2, 0, 3 },
  /*FAh*/{ "CVarErrI4", PCPT_I2, 0, 3 },
  /*FBh*/{ "CVarDate", PCPT_I2, 0, 3 },
  /*FCh*/{ "CVarAryVarg", PCPT_I2, 0, 3 },
  /*FDh*/{ "CStrVarTmp_1", NULL, PCT0_NONE, 1 },
  /*FEh*/{ "FnCStrVarVal", PCPT_I2, 0, 3 },
  /*FFh*/{ "FnDestructOFrame", PCPT_I2_I2, 0, 5 }
};

/*P-Code 双字节指令集3 (扩展指令集) 0xFE前缀*/
const VB_PCODE_INSTRUCTIONSET PCodeLead3Inst[256] =
{
  /*00h*/{ "ThisVCallUI1", PCPT_I2, 0, 3 },
  /*01h*/{ "ThisVCallI2_1", PCPT_I2, 0, 3 },
  /*02h*/{ "ThisVCallI2_2", PCPT_I2, 0, 3 },
  /*03h*/{ "ThisVCallR4", PCPT_I2, 0, 3 },
  /*04h*/{ "ThisVCallR8", PCPT_I2, 0, 3 },
  /*05h*/{ "ThisVCallCy", PCPT_I2, 0, 3 },
  /*06h*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*07h*/{ "ThisVCallI2_3", PCPT_I2, 0, 3 },
  /*08h*/{ "ThisVCallI2_4", PCPT_I2, 0, 3 },
  /*09h*/{ "ThisVCallFPR8_1", PCPT_I2, 0, 3 },
  /*0Ah*/{ "ThisVCallFPR8_2", PCPT_I2, 0, 3 },
  /*0Bh*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*0Ch*/{ "ThisVCallFPR8_3", PCPT_I2, 0, 3 },
  /*0Dh*/{ "ThisVCallCbFrame", PCPT_I2_I2, 0, 5 },
  /*0Eh*/{ "FnStFixedStrL_1", PCPT_LOC, 0, 3 },
  /*0Fh*/{ "FnStFixedStrLFree", PCPT_I2, 0, 3 },
  /*10h*/{ "VCallUI1", PCPT_I2, 0, 3 },
  /*11h*/{ "VCallI2_1", PCPT_I2, 0, 3 },
  /*12h*/{ "VCallI2_2", PCPT_I2, 0, 3 },
  /*13h*/{ "VCallR4", PCPT_I2, 0, 3 },
  /*14h*/{ "VCallR8", PCPT_I2, 0, 3 },
  /*15h*/{ "VCallCy", PCPT_I2, 0, 3 },
  /*16h*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*17h*/{ "VCallI2_3", PCPT_I2, 0, 3 },
  /*18h*/{ "VCallI2_4", PCPT_I2, 0, 3 },
  /*19h*/{ "VCallFPR8_1", PCPT_I2, 0, 3 },
  /*1Ah*/{ "VCallFPR8_2", PCPT_I2, 0, 3 },
  /*1Bh*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*1Ch*/{ "VCallFPR8_3", PCPT_I2, 0, 3 },
  /*1Dh*/{ "VCallCbFrame", PCPT_I2_I2, 0, 5 },
  /*1Eh*/{ "FnStFixedStrR", PCPT_I2, 0, 3 },
  /*1Fh*/{ "FnStFixedStrRFree", PCPT_I2, 0, 3 },
  /*20h*/{ "ImpAdCallUI1", PCPT_I2_I2, 0, 5 },
  /*21h*/{ "ImpAdCallI4_1", PCPT_I2_I2, 0, 5 },
  /*22h*/{ "ImpAdCallI4_2", PCPT_I2_I2, 0, 5 },
  /*23h*/{ "ImpAdCallR4", PCPT_I2_I2, 0, 5 },
  /*24h*/{ "ImpAdCallR8", PCPT_I2_I2, 0, 5 },
  /*25h*/{ "ImpAdCallCy_1", PCPT_I2_I2, 0, 5 },
  /*26h*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*27h*/{ "ImpAdCallI4_3", PCPT_I2_I2, 0, 5 },
  /*28h*/{ "ImpAdCallI4_4", PCPT_I2_I2, 0, 5 },
  /*29h*/{ "ImpAdCallFPR4_1", PCPT_I2_I2, 0, 5 },
  /*2Ah*/{ "ImpAdCallFPR4_2", PCPT_I2_I2, 0, 5 },
  /*2Bh*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*2Ch*/{ "ImpAdCallFPR4_3", PCPT_I2_I2, 0, 5 },
  /*2Dh*/{ "ImpAdCallCbFrame", PCPT_I2_I2_I2_I2, 0, 9 },
  /*2Eh*/{ "LdStkRf", PCPT_I2, 0, 3 },
  /*2Fh*/{ "LdFrameRf", PCPT_I2, 0, 3 },
  /*30h*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*31h*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*32h*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*33h*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*34h*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*35h*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*36h*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*37h*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*38h*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*39h*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*3Ah*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*3Bh*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*3Ch*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*3Dh*/{ "LitVarUI1", PCPT_I2_I2, 0, 5 },
  /*3Eh*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*3Fh*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*40h*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*41h*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*42h*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*43h*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*44h*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*45h*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*46h*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*47h*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*48h*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*49h*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*4Ah*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*4Bh*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*4Ch*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*4Dh*/{ "SetVarVar", NULL, PCT0_NONE, 1 },
  /*4Eh*/{ "SetVarVarFunc", NULL, PCT0_NONE, 1 },
  /*4Fh*/{ "ImpAdCallHresult", PCPT_I2_I2, 0, 5 },
  /*50h*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*51h*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*52h*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*53h*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*54h*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*55h*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*56h*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*57h*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*58h*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*59h*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*5Ah*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*5Bh*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*5Ch*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*5Dh*/{ "FnOpenFile", PCPT_I2, 0, 3 },
  /*5Eh*/{ "FnLockFile", PCPT_I2, 0, 3 },
  /*5Fh*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*60h*/{ "FnEraseDestruct", PCPT_I2, 0, 3 },
  /*61h*/{ "LdFixedStr", PCPT_I2, 0, 3 },
  /*62h*/{ "ForUI1", PCPT_I2_I2, 0, 5 },
  /*63h*/{ "ForI2", PCPT_I2_I2, 0, 5 },
  /*64h*/{ "ForI4", PCPT_I2_I2, 0, 5 },
  /*65h*/{ "ForR4", PCPT_I2_I2, 0, 5 },
  /*66h*/{ "ForR8", PCPT_I2_I2, 0, 5 },
  /*67h*/{ "ForCy", PCPT_I2_I2, 0, 5 },
  /*68h*/{ "ForVar", PCPT_I2_I2, 0, 5 },
  /*69h*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*6Ah*/{ "ForStepUI1", NULL, 0, 5 },
  /*6Bh*/{ "ForStepI2", PCPT_I2_I2, 0, 5 },
  /*6Ch*/{ "ForStepI4", PCPT_I2_I2, 0, 5 },
  /*6Dh*/{ "ForStepR4", PCPT_I2_I2, 0, 5 },
  /*6Eh*/{ "ForStepR8", PCPT_I2_I2, 0, 5 },
  /*6Fh*/{ "ForStepCy", PCPT_I2_I2, 0, 5 },
  /*70h*/{ "ForStepVar", PCPT_I2_I2, 0, 5 },
  /*71h*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*72h*/{ "ForEachCollVar", PCPT_I2_I2, 0, 5 },
  /*73h*/{ "NextEachCollVar", PCPT_I2_I2, 0, 5 },
  /*74h*/{ "ForEachCollAd", PCPT_I2_I2, 0, 5 },
  /*75h*/{ "NextEachCollAd", PCPT_I2_I2, 0, 5 },
  /*76h*/{ "ForEachAryVar", PCPT_I2_I2_I2, 0, 7 },
  /*77h*/{ "NextEachAryVar", PCPT_I2_I2_I2, 0, 7 },
  /*78h*/{ "NextStepUI1", PCPT_I2_I2, 0, 5 },
  /*79h*/{ "NextI2_1", PCPT_I2_I2, 0, 5 },
  /*7Ah*/{ "NextI4_1", PCPT_I2_I2, 0, 5 },
  /*7Bh*/{ "NextR4_1", PCPT_I2_I2, 0, 5 },
  /*7Ch*/{ "NextR8_1", PCPT_I2_I2, 0, 5 },
  /*7Dh*/{ "NextStepCy_1", PCPT_I2_I2, 0, 5 },
  /*7Eh*/{ "NextVar", PCPT_I2_I2, 0, 5 },
  /*7Fh*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*80h*/{ "NextStepUI1", PCPT_I2_I2, 0, 5 },
  /*81h*/{ "NextStepI2_1", PCPT_I2_I2, 0, 5 },
  /*82h*/{ "NextStepI4_1", PCPT_I2_I2, 0, 5 },
  /*83h*/{ "NextR4_2", PCPT_I2_I2, 0, 5 },
  /*84h*/{ "NextR8_2", PCPT_I2_I2, 0, 5 },
  /*85h*/{ "NextStepCy_2", PCPT_I2_I2, 0, 5 },
  /*86h*/{ "NextVar_1", PCPT_I2_I2, 0, 5 },
  /*87h*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*88h*/{ "FnForEachCollObj", PCPT_I2_I2_I2, 0, 7 },
  /*89h*/{ "ForEachVar", PCPT_I2_I2, 0, 5 },
  /*8Ah*/{ "ForEachVarFree", PCPT_I2_I2, 0, 5 },
  /*8Bh*/{ "FnNextEachCollObj", PCPT_I2_I2_I2, PCT0_NONE, 7 },
  /*8Ch*/{ "NextEachVar", PCPT_I2_I2, 0, 5 },             //OLen 9 Fixed
  /*8Dh*/{ "FnCheckType", PCPT_I2, 0, 3 },
  /*8Eh*/{ "Redim", PCPT_I2_I2_I2_I2, 0, 9 },
  /*8Fh*/{ "RedimPreserve", PCPT_I2_I2_I2_I2, 0, 9 },
  /*90h*/{ "RedimVar", PCPT_I2_I2, 0, 5 },
  /*91h*/{ "RedimPreserveVar", PCPT_I2_I2, 0, 5 },
  /*92h*/{ "FnFDupVar", PCPT_I2_I2, 0, 5 },
  /*93h*/{ "FDupStr", PCPT_I2_I2, 0, 5 },
  /*94h*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*95h*/{ "OnGosub", PCPT_I2_I2_I2, 0, 7 },
  /*96h*/{ "OnGoto", PCPT_I2_I2_I2, 0, 7 },
  /*97h*/{ "AddRef", NULL, PCT0_NONE, 1 },
  /*98h*/{ "LateMemCall", PCPT_I2_I2, 0, 5 },
  /*99h*/{ "LateMemLdVar", PCPT_I2_I2, 0, 5 },
  /*9Ah*/{ "LateMemCallLdVar", PCPT_I2_I2_I2, 0, 7 },
  /*9Bh*/{ "LateMemSt", PCPT_I2, 0, 3 },
  /*9Ch*/{ "LateMemCallSt", PCPT_I2_I2, 0, 5 },
  /*9Dh*/{ "LateMemStAd", PCPT_I2_I2, 0, 5 },
  /*9Eh*/{ "ExitProcFrameCb", PCPT_I2_I2, PCFH_ENDPROC, 5 },             //OLen 4 Fixed
  /*9Fh*/{ "ExitProcFrameCbStack", PCPT_I2_I2, PCFH_ENDPROC, 5 },        //OLen 4 Fixed
  /*A0h*/{ "LateIdCall", PCPT_I4_I2, 0, 7 },
  /*A1h*/{ "LateIdLdVar", PCPT_I2_I4, 0, 7 },
  /*A2h*/{ "LateIdCallLdVar", PCPT_I2_I4_I2, 0, 9 },
  /*A3h*/{ "LateIdSt", PCPT_I4, 0, 5 },
  /*A4h*/{ "LateIdCallSt", PCPT_I4_I2, 0, 7 },
  /*A5h*/{ "LateIdStAd", PCPT_I4_I2, 0, 7 },
  /*A6h*/{ "LateMemNamedCall", PCPT_JU_U2_U2_VP, 0, -7 },               // IG I2 I2
  /*A7h*/{ "LateMemNamedCallLdVar", PCPT_JU_I2_U2_U2_VP, 0, -9 },
  /*A8h*/{ "LateMemNamedCallStVar", PCPT_JU_U2_U2_VP, 0, -7 },
  /*A9h*/{ "LateMemNamedStAd", PCPT_JU_U2_U2_VP, 0, -7 },
  /*AAh*/{ "LateIdNamedCall", PCPT_JU_I4_U2_VP, 0, -9 },
  /*ABh*/{ "LateIdNamedCallLdVar", PCPT_JU_I2_I4_U2_VP, 0, -11 },
  /*ACh*/{ "LateIdNamedCallStVar", PCPT_JU_I4_U2_VP, 0, -9 },
  /*ADh*/{ "LateIdNamedStAd", PCPT_JU_I4_U2_VP, 0, -9 },
  /*AEh*/{ "VarIndexLdVar", PCPT_I2_U2, 0, 5 },
  /*AFh*/{ "VarIndexLdRfVar", PCPT_I2_U2, 0, 5 },
  /*B0h*/{ "VarIndexSt", PCPT_U2, 0, 3 },
  /*B1h*/{ "VarIndexStAd", PCPT_U2, 0, 3 },
  /*B2h*/{ "FFreeVar_1", PCPT_JO_VP, PCT0_CLEARMEMORY, -3 },
  /*B3h*/{ "FFreeStr_1", PCPT_JO_VP, PCT0_CLEARMEMORY, -3 },
  /*B4h*/{ "FFreeAd_1", PCPT_JO_VP, PCT0_CLEARMEMORY, -3 },
  /*B5h*/{ "LitI2_1", PCPT_I2, 0, 3 },
  /*B6h*/{ "LitI2FP_1", PCPT_I2, 0, 3 },
  /*B7h*/{ "LitCy4_1", PCPT_I4, 0, 5 },
  /*B8h*/{ "LitI4_1", PCPT_I4, 0, 5 },
  /*B9h*/{ "LitI4_2", PCPT_I4, 0, 5 },
  /*BAh*/{ "LitR4FP_1", PCPT_I4, 0, 5 },
  /*BBh*/{ "LitR8_1", PCPT_I8, 0, 9 },
  /*BCh*/{ "LitR8_2", PCPT_I8, 0, 9 },
  /*BDh*/{ "LitDate_1", PCPT_I8, 0, 9 },
  /*BEh*/{ "LitDate_2", PCPT_I8, 0, 9 },
  /*BFh*/{ "LitStr_1", PCPT_U2, 0, 3 },
  /*C0h*/{ "LitVarI2_1", PCPT_I2_I2, 0, 5 },
  /*C1h*/{ "LitVarI4", PCPT_I2_I4, 0, 7 },
  /*C2h*/{ "LitVarR4", PCPT_I2_I4, 0, 7 },
  /*C3h*/{ "LitVarCy", PCPT_I2_I8, 0, 11 },             //OLEN: 1
  /*C4h*/{ "LitVarR8", PCPT_I2_I8, 0, 11 },
  /*C5h*/{ "LitVarDate", PCPT_I2_I8, 0, 11 },           //OLEN: 1
  /*C6h*/{ "LitVarStr_1", PCPT_I2_U2, 0, 5 },
  /*C7h*/{ "CStrBool", NULL, PCT0_NONE, 1 },
  /*C8h*/{ "CStrDate", NULL, PCT0_NONE, 1 },
  /*C9h*/{ "CDateStr", NULL, PCT0_NONE, 1 },
  /*CAh*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*CBh*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*CCh*/{ "FreeStrNoPop", NULL, 0, 1 },
  /*CDh*/{ "FreeVarNoPop", NULL, 0, 1 },
  /*CEh*/{ "FreeAdNoPop", NULL, 0, 1 },
  /*CFh*/{ "EraseNoPop", NULL, PCT0_NONE, 1 },
  /*D0h*/{ "WMemLdUI1", PCPT_I2, 0, 3 },
  /*D1h*/{ "WMemLdI2", PCPT_I2, 0, 3 },
  /*D2h*/{ "WMemLdI4", PCPT_I2, 0, 3 },
  /*D3h*/{ "WMemLdI4_1", PCPT_I2, 0, 3 },
  /*D4h*/{ "WMemLdCy", PCPT_I2, 0, 3 },
  /*D5h*/{ "WMemLdCy_1", PCPT_I2, 0, 3 },
  /*D6h*/{ "WMemLdRfVar", PCPT_I2, 0, 3 },
  /*D7h*/{ "WMemLdI4_2", PCPT_I2, 0, 3 },
  /*D8h*/{ "WMemLdI4_3", PCPT_I2, 0, 3 },
  /*D9h*/{ "WMemLdFPR4", PCPT_I2, 0, 3 },
  /*DAh*/{ "WMemLdFPR8", PCPT_I2, 0, 3 },
  /*DBh*/{ "WMemLdPr_1", PCPT_I2, 0, 3 },
  /*DCh*/{ "WMemLdRfVar_1", PCPT_I2, 0, 3 },
  /*DDh*/{ "WMemLdVar", PCPT_I2, 0, 3 },
  /*DEh*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*DFh*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*E0h*/{ "WMemStUI1", PCPT_I2, 0, 3 },
  /*E1h*/{ "WMemStI2", PCPT_I2, 0, 3 },
  /*E2h*/{ "WMemStI4", PCPT_I2, 0, 3 },
  /*E3h*/{ "WMemStI4_1", PCPT_I2, 0, 3 },
  /*E4h*/{ "WMemStCy", PCPT_I2, 0, 3 },
  /*E5h*/{ "WMemStCy_1", PCPT_I2, 0, 3 },
  /*E6h*/{ "WMemStVar", PCPT_I2, 0, 3 },
  /*E7h*/{ "WMemStStr", PCPT_I2, 0, 3 },
  /*E8h*/{ "WMemStAd", PCPT_I2, 0, 3 },
  /*E9h*/{ "WMemStFPR4", PCPT_I2, 0, 3 },
  /*EAh*/{ "WMemStFPR8", PCPT_I2, 0, 3 },
  /*EBh*/{ "WMemStVarAd", PCPT_I2, 0, 3 },
  /*ECh*/{ "WMemStVarAdFunc", PCPT_I2, 0, 3 },
  /*EDh*/{ "WMemStVarUnk", PCPT_I2, 0, 3 },
  /*EEh*/{ "WMemStVarUnkFunc", PCPT_I2, 0, 3 },
  /*EFh*/{ "WMemStAdFunc", PCPT_I2, 0, 3 },
  /*F0h*/{ "WMemStVarCopy", PCPT_I2, 0, 3 },
  /*F1h*/{ "WMemStStrCopy", PCPT_I2, 0, 3 },
  /*F2h*/{ "VarIndexLdRfVarLock", PCPT_I2_I2_U2, 0, 7 },
  /*F3h*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*F4h*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*F5h*/{ "FnAssignRecord", PCPT_U2, 0, 3 },
  /*F6h*/{ "FnDestructAnsi", PCPT_I2_U2, 0, 5 },      //ON:DestructAnsiOFrame
  /*F7h*/{ "FStVarZero", PCPT_I2, 0, 3 },
  /*F8h*/{ "FStVarCopyObj_1", PCPT_I2, 0, 3 },
  /*F9h*/{ "VerifyVarObj", NULL, 0, 1 },
  /*FAh*/{ "VerifyPVarObj", NULL, PCT0_NONE, 1 },
  /*FBh*/{ "FnInStrB4", NULL, PCT0_NONE, 1 },
  /*FCh*/{ "FnInStrB4Var", PCPT_I2, 0, 3 },
  /*FDh*/{ "FnInStr4", NULL, PCT0_NONE, 1 },
  /*FEh*/{ "FnInStr4Var", PCPT_I2, 0, 3 },
  /*FFh*/{ "FnStrComp3", NULL, PCT0_NONE, 1 }
};

/*P-Code 双字节指令集4 (扩展指令集) 0xFF前缀*/
const VB_PCODE_INSTRUCTIONSET PCodeLead4Inst[71] =
{
  /*00h*/{ "FnStrComp3Var", PCPT_I2, 0, 3 },
  /*01h*/{ "FnStAryMove", NULL, PCT0_NONE, 1 },
  /*02h*/{ "FnStAryCopy", NULL, PCT0_NONE, 1 },
  /*03h*/{ "FnStAryRecMove", PCPT_U2, 0, 3 },
  /*04h*/{ "FnStAryRecCopy", PCPT_U2, 0, 3 },
  /*05h*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*06h*/{ "AryInRecLdPr", PCPT_U2_U2, 0, 5 },      //****************************
  /*07h*/{ "AryInRecLdRf", PCPT_U2_U2, 0, 5 },
  /*08h*/{ "CExtInstUnk", NULL, 0, 1 },             // Nop
  /*09h*/{ "FnIStVarCopyObj", PCPT_I2, 0, 3 },
  /*0Ah*/{ "FnArrayRebase1Var", NULL, PCT0_NONE, 1 },
  /*0Bh*/{ "Assert", NULL, PCT0_NONE, 1 },
  /*0Ch*/{ "RaiseEvent", PCPT_I4_U2, 0, 7 },
  /*0Dh*/{ "FnPrintObject", PCPT_U2_U2, 0, 5 },
  /*0Eh*/{ "FnPrintFile", PCPT_U2_U2, 0, 5 },
  /*0Fh*/{ "FnWriteFile", PCPT_U2_U2, 0, 5 },
  /*10h*/{ "FnInputFile", PCPT_U2_U2, 0, 5 },
  /*11h*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*12h*/{ "FnGetRecFxStr3", NULL, PCT0_NONE, 1 },
  /*13h*/{ "FnGetRecFxStr4", NULL, PCT0_NONE, 1 },
  /*14h*/{ "FnPutRecFxStr3", NULL, 0, 1 },
  /*15h*/{ "FnPutRecFxStr4", NULL, 0, 1 },
  /*16h*/{ "FnGetRecOwn3", PCPT_U2, 0, 3 },
  /*17h*/{ "FnGetRecOwn4", PCPT_U2, 0, 3 },
  /*18h*/{ "FnPutRecOwn3", PCPT_U2, 0, 3 },
  /*19h*/{ "FnPutRecOwn4", PCPT_U2, 0, 3 },
  /*1Ah*/{ "LitI2_Byte_1", PCPT_I1, 0, 2 },
  /*1Bh*/{ "FnCBoolVarNull", NULL, PCT0_NONE, 1 },
  /*1Ch*/{ "LargeBos_3", PCPT_N1, 0, 2 },
  /*1Dh*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*1Eh*/{ "ImpAdCallNonVirt", PCPT_U2_U2, 0, 5 },
  /*1Fh*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*20h*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*21h*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*22h*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*23h*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*24h*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*25h*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*26h*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*27h*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*28h*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*29h*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*2Ah*/{ "FnDestructRecord", PCPT_U2, 0, 3 },
  /*2Bh*/{ "VCallFPR8_4", PCPT_U2, 0, 3 },
  /*2Ch*/{ "ThisVCallFPR8_4", PCPT_U2, 0, 3 },
  /*2Dh*/{ "ZeroRetVal", NULL, PCT0_NONE, 1 },
  /*2Eh*/{ "ZeroRetValVar", NULL, PCT0_NONE, 1 },
  /*2Fh*/{ "ExitProcCbHresult", PCPT_I2_I2, PCFH_ENDPROC, 5 },
  /*30h*/{ "ExitProcFrameCbHresult", PCPT_I2_I2_I2, PCFH_ENDPROC, 7 },
  /*31h*/{ "FnEraseDestrKeepData", PCPT_U2, 0, 3 },
  /*32h*/{ "CDargRefUdt", PCPT_U2_U2, 0, 5 },
  /*33h*/{ "CVarRefUdt", PCPT_I2_U2, 0, 5 },
  /*34h*/{ "CVarUdt", PCPT_I2_U2, 0, 5 },
  /*35h*/{ "FnCUdtVar", PCPT_U2, 0, 3 },       //ON:StUdtVar
  /*36h*/{ "FnAryVar", PCPT_U2, 0, 3 },        //ON:StAryVar
  /*37h*/{ "CopyBytesZero", PCPT_U2, 0, 3 },
  /*38h*/{ "FnFLdZeroAry", PCPT_I2_I2, 0, 5 },
  /*39h*/{ "FStVarZero_1", PCPT_I2, 0, 3 },
  /*3Ah*/{ "FnCVarAryUdt", PCPT_I2_U2_U2, 0, 7 },
  /*3Bh*/{ "RedimVarUdt", PCPT_U2_U2_U2, 0, 7 },
  /*3Ch*/{ "RedimPreserveVarUdt", PCPT_U2_U2_U2, 0, 7 },
  /*3Dh*/{ "VarLateMemLdRfVar", PCPT_I2_U2, 0, 5 },
  /*3Eh*/{ "VarLateMemCallLdRfVar", PCPT_I2_U2_U2, 0, 7 },         //OLen 6 Fixed
  /*3Fh*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*40h*/{ pcit_ive, NULL, PCFH_INVALID, 1 },
  /*41h*/{ "VarLateMemLdVar", PCPT_I2_U2, 0, 5 },
  /*42h*/{ "VarLateMemCallLdVar", PCPT_I2_U2_U2, 0, 7 },
  /*43h*/{ "VarLateMemSt", PCPT_U2, 0, 3 },
  /*44h*/{ "VarLateMemCallSt", PCPT_U2_U2, 0, 5 },
  /*45h*/{ "VarLateMemStAd", PCPT_U2_U2, 0, 5 },
  /*46h*/{ pcit_ive, NULL, PCFH_INVALID, 1 }
};

const VB_PCODE_INSTRUCTIONSET * PCodeLeadInst[5] =
{
  PCodeLead0Inst,
  PCodeLead1Inst,
  PCodeLead2Inst,
  PCodeLead3Inst,
  PCodeLead4Inst
};