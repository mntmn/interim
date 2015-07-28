#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include "devices/rpi2/raspi.h"

#include <uspienv/timer.h>
#include <uspienv/logger.h>
#include <uspienv/interrupt.h>
#include <uspienv/exceptionhandler.h>
#include <uspienv/bcmpropertytags.h>

typedef void TInterruptHandler (void *pParam);

//#include "uspi/include/uspi/uspios.h"

void format_string(char *fmt, va_list argptr, char *formatted_string);

void LogWrite(const char *source,		// short name of module
	       unsigned	   severity,		// see above
	       const char *msg, ...) {
  //printf("[uspi-log] %s %d %s\r\n",source,severity,msg);

  va_list argptr;
  va_start(argptr,msg);
  vfprintf(stdout, msg, argptr);
  va_end(argptr);
  printf("\r\n");
}

TLogger *LoggerGet (void) {
  return NULL;
}

void LoggerWrite(TLogger *pThis, const char *pSource, TLogSeverity Severity, const char *pMessage, ...) {
  LogWrite(pSource, Severity, pMessage);
}

void uspi_assertion_failed() {
}

void InvalidateDataCache() {
  arm_invalidate_data_caches();
}

void CleanDataCache() {
  arm_clear_data_caches();
}

/*
void MsDelay(unsigned int ms) {
}

void usDelay(unsigned int us) {
}

void StartKernelTimer() {
}

void CancelKernelTimer() {
}

int SetPowerStateOn() {
  return 1;
}

void ConnectInterrupt() {
}*/

void DebugHexdump() {
  printf("[uspi] DebugHexdump not implemented.\r\n");
}

void debug_stacktrace (const uint32_t *pStackPtr, const char *pSource)
{	
	for (unsigned i = 0; i < 64; i++, pStackPtr++)
	{
		extern unsigned char _bss_end;

		if (*pStackPtr >= MEM_KERNEL_START && *pStackPtr < (uint32_t)_bss_end)
		{
			printf("[trace] [%u] 0x%X\r\n", i, (unsigned) *pStackPtr);
		}
	}
}

int GetMACAddress(uint8_t buf[6]) {
  buf[0]=0xde;
  buf[1]=0xad;
  buf[2]=0xbe;
  buf[3]=0xef;
  buf[4]=0x02;
  buf[5]=0x42;
  return 1;
}


#define	EnableInterrupts()	__asm volatile ("cpsie i")
#define	DisableInterrupts()	__asm volatile ("cpsid i")


static volatile unsigned s_nCriticalLevel = 0;
static volatile int s_bWereEnabled;

void EnterCritical (void)
{
	uint32_t nFlags;
	__asm volatile ("mrs %0, cpsr" : "=r" (nFlags));

	DisableInterrupts();

	if (s_nCriticalLevel++ == 0)
	{
		s_bWereEnabled = nFlags & 0x80 ? 0 : 1;
	}

	arm_dmb();
}

void LeaveCritical (void)
{
	arm_dmb();

	//assert (s_nCriticalLevel > 0);
	if (--s_nCriticalLevel == 0)
	{
		if (s_bWereEnabled)
		{
			EnableInterrupts();
		}
	}
}


void MsDelay (unsigned nMilliSeconds)
{
	TimerMsDelay (TimerGet (), nMilliSeconds);
}

void usDelay (unsigned nMicroSeconds)
{
	TimerusDelay (TimerGet (), nMicroSeconds);
}

unsigned StartKernelTimer (unsigned nDelay, TKernelTimerHandler *pHandler, void *pParam, void *pContext)
{
	return TimerStartKernelTimer (TimerGet (), nDelay, pHandler, pParam, pContext);
}

void CancelKernelTimer (unsigned hTimer)
{
	TimerCancelKernelTimer (TimerGet (), hTimer);
}

void ConnectInterrupt (unsigned nIRQ, TInterruptHandler *pHandler, void *pParam)
{
	InterruptSystemConnectIRQ (InterruptSystemGet (), nIRQ, pHandler, pParam);
}

int SetPowerStateOn (unsigned nDeviceId)
{
	TBcmPropertyTags Tags;
	BcmPropertyTags (&Tags);
	TPropertyTagPowerState PowerState;
	PowerState.nDeviceId = nDeviceId;
	PowerState.nState = POWER_STATE_ON | POWER_STATE_WAIT;
	if (   !BcmPropertyTagsGetTag (&Tags, PROPTAG_SET_POWER_STATE, &PowerState, sizeof PowerState, 8)
	    ||  (PowerState.nState & POWER_STATE_NO_DEVICE)
	    || !(PowerState.nState & POWER_STATE_ON))
	{
		_BcmPropertyTags (&Tags);

		return 0;
	}
	
	_BcmPropertyTags (&Tags);

	return 1;
}

static TExceptionHandler uspi_exception_handler;
static TInterruptSystem uspi_interrupts;
static TTimer uspi_timer;

void uspi_glue_init() {
  printf("uu ExceptionHandler2…\r\n");
  ExceptionHandler2(&uspi_exception_handler);
  printf("uu InterruptSystem…\r\n");
	InterruptSystem(&uspi_interrupts);
  printf("uu InterruptSystemInitialize…\r\n");
  InterruptSystemInitialize(&uspi_interrupts);
  printf("uu Timer…\r\n");
	Timer(&uspi_timer, &uspi_interrupts);
  printf("uu TimerInitialize…\r\n");
  TimerInitialize(&uspi_timer);
}
