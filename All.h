#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <dos.h>
#include <io.h>
#include <time.h>
#include <sys\types.h>
#include <sys\timeb.h>

#include "Device.h"
#include "mmi_new.H"
#include "Queue.h"
#include "icp.h"
#include "Host.h"


#include"7188xa.h"

/*#if defined(I7188D)
 #if !defined(TIM_TT$)
   unsigned long far *TimeTicks=(unsigned long far *)0x40006c;
   #define TIM_TT$
 #endif
#endif*/