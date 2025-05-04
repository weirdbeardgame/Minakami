#include "main.h"
#include "common.h"

/* sdata 3f1988 */ static int softreset_step_timer;
/* sbss 3f4d78 */ static int *SubTitleAddr;
/* sbss 3f4d7c */ static int soft_reset_disable;

#define PRINT_ASSERT(...)                                                      \
  SetAssertPreMessage("\n\tFILE_NAME : %s \n\tLINE :%d \n\tFUNC_NAME: %s\n",   \
                      __FILE__, __LINE__, __FUNCTION__);                       \
  PrintAssertReal(__VA_ARGS__);

int main()
{
  InitGPhaseSys();
  do
  {
    GPhaseSysMain();
  }
  while (true);
}

void DebugMemoryCheck(void)
{
  printf("\n");
  printf("\n");
  printf("\n");
  printf("<<<<<<<<<<<<<<<MEMORY CHECK>>>>>>>>>>>>>>>>\n");
  printf("DEBUG_PROGRAM_ADDR = 0x%x\n", 0x4D6C00);
  printf("HEAP_AREA_ADDR = 0x%x\n", 0x4D6C00);
  printf("PRE_PACKET3D_ADDR = 0x%x\n", 0x01A91B00);
  printf("PACKET3D_ADDR = 0x%x\n", 0x01AA1500);
  printf("PACKET2D_ADDR = 0x%x\n", 0x01E79B00);
  printf("CAMERA_VCI_ADDR = 0x%x\n", 0x59EC00);
  printf("MODEL_HEAP_ADDR = 0x%x\n", 0x5A6C00);
  printf("EVENT_DATA_ADDR = 0x%x\n", 0xD4EC00);
  printf("MSG_DATA_ADDR = 0x%x\n", 0xD9EC00);
  printf("ROOM1_DATA_ADDR = 0x%x\n", 0xDDBC00);
  printf("ROOM2_DATA_ADDR = 0x%x\n", 0x011EBC00);
  printf("MEM_UTIL_HEAP_ADDR = 0x%x\n", 0x015FBC00);
  printf("TEX_2D_ADDR = 0x%x\n", 0x018AD000);
  printf("PHOTO_DATA_ADDR = 0x%x\n", 0x019A9B00);
  printf("EFFECT_WRK1_ADDR = 0x%x\n", 0x01E79B00);
  printf("EFFECT_WRK2_ADDR = 0x%x\n", 0x01F05B00);
  printf("RELEASE_END_ADDR = 0x%x\n", 0x01F91B00);

  if (0x50000 < GetFileSize(0xd35))
  {
#line 176
    PRINT_ASSERT("EventObj Size Is Over");
  }

  if (0x50000 < GetFileSize(0xd36))
  {
    PRINT_ASSERT("Event 50 Obj Size Is Over");
  }
  if (0x3d000 < GetFileSize(0xd38))
  {
    PRINT_ASSERT("MsgObj Size Is Over");
  }
  if (0x8000 < GetFileSize(0xe27))
  {
    PRINT_ASSERT("VCI Size Is Over");
  }

  DebugMsgDataCheck();

  printf("STACK_ADDR = 0x%x\n", 0x01FA0000);
  printf("\n");
  printf("\n");
  printf("MENU_END_ADDR = 0x%x\n", 0x019A91C0);
  printf("\n");
  printf("\n");
}

void newAssert(/* a1 5 */ char *pStr)
{
  g3ddbgAssert(false, pStr);
  return;
}

void init_super()
{
  InitCostume();
  soft_reset_disable = 0;
  InitSystemON();
  DebugMemoryCheck();
  InitGraph2dBoot();
  playPssInit();
  InitLogo();
  LoadingInit();
  TitleInit();
  InitOptionSetup(&opt_wrk);
  ClearFlgCtrlInit();
  MemoryCardInit();
  MemoryCardDebugReqSizeDisp();
  dmaVif1Init((void *) 0x0, 0, (void *) 0x31aa1500, 0x1ec30);
  InitDrawEnv(dmaVif1GetPacketFLUSH_DIRECT, dmaVif1GetPacketFLUSH_DIRECT,
              dmaVif1SetPacketFLUSH_DIRECT, dmaVif1SetPacketFLUSH_DIRECT);
  SetPrintWarning(Zero2PrintWarningFunc);
  SetPrintAssert(newAssert);
  gra3dInit((void *) 0x1a91b00, 64000);
  IngameWrkInit(0, 1);
  sceSifAllocSysMemory(1, 0x600000, 0x0);
  FinderBankSetup();
  SceneEffectInit();
}

void end_super()
{
  return;
}

static void CallSoftReset()
{
  SetNextGPhase(GID_SOFTRESETMAIN);
  printf("SOFT RESET!!\n");
  return;
}

static int CheckSoftReset()
{
  if (sys_wrk.sreset_ng != 0)
  {
    sys_wrk.sreset_count = 0;
    return 0;
  }
  else if ((soft_reset_disable == 0) && (*key_now[8] != 0) && (*key_now[9] != 0)
           && (*key_now[10] != 0) && (*key_now[11] != 0) && (*key_now[12] != 0)
           && (*key_now[13] != 0))
  {
    printf("SOFT RESET COUNT DOWN = %d\n", 0x3C - sys_wrk.sreset_count);
    sys_wrk.sreset_count += 1;
  }
  else
  {
    sys_wrk.sreset_count = 0;
    return 0;
  }

  if (sys_wrk.sreset_count >= 0x3C)
  {
    sys_wrk.sreset_count = 0;
    CallSoftReset();
    return 1;
  }
  else
  {
    return 0;
  }
}

GPHASE_ENUM pre_super(GPHASE_ENUM super)
{
  ClearDrawEnv();
  SetScissorRegister(0, *(long *) (&GetDrawEnv(0)->scissor1));
  SetScissorRegister(1, *(long *) (&GetDrawEnv(1)->scissor1));
  GET_SCISSOR_REGISTER(0);
  PadSyncCallback();
  PadAnalogMain();
  return GPHASE_CONTINUE;
}

GPHASE_ENUM after_super()
{
  EachDebugMain();
  ee_iopMain();
  SendDMAMain();
  return (GPHASE_ENUM) (CheckSoftReset() != 0);
}

void init_Boot_Init()
{
  SetLanguage(GetSystemLanguage());
  LoadReq(GetLanguage() + 4, 0x01E79B00);
  LoadReq(GetLanguage() + 0xD38, 0xD9EC00);
  SubTitleAddr =
      (int *) SAFE_MALLOC(GetSystemHeapWrkP(), 0, GetFileSize(0x1184));
  LoadReq__FiUi(0x1184, (u_int) SubTitleAddr);
}

void end_Boot_Init()
{
  return;
}

GPHASE_ENUM one_Boot_Init(/* a0 4 */ GPHASE_ENUM dummy)
{
  if (IsLoadEndAll() != 0)
  {
    PK2SendVram(0x1e79b00, -1, -1, 0);
    SetNextGPhase(GID_BOOT_PADCHECK);
  }
  return GPHASE_CONTINUE;
}

void SoftResetLock()
{
  sys_wrk.sreset_count = 0;
  soft_reset_disable += 1;
}

void SoftResetUnlock()
{
  soft_reset_disable--;
  if (soft_reset_disable < 0)
  {
#line 460
    PRINT_ASSERT("SoftResetUnlock Cnt under 0");
    soft_reset_disable = 0;
  }
}

int *GetSubTitleAddr(void)
{
  return SubTitleAddr;
}

void init_SoftResetMain()
{
  FileLoadCancelAll();
  softreset_step_timer = 0;
  return;
}

GPHASE_ENUM one_SoftResetMain(GPHASE_ENUM dummy)
{
  softreset_step_timer += 1;
  if (2 < softreset_step_timer)
  {
    SetNextGPhase(GID_UBI_MODE);
    ResetOutReqFlg();
    MissionReleaseSaveData();
  }
  return GPHASE_CONTINUE;
}

void end_SoftResetMain()
{
  return;
}
