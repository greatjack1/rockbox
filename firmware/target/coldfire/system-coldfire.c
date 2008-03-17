/***************************************************************************
 *             __________               __   ___.
 *   Open      \______   \ ____   ____ |  | _\_ |__   _______  ___
 *   Source     |       _//  _ \_/ ___\|  |/ /| __ \ /  _ \  \/  /
 *   Jukebox    |    |   (  <_> )  \___|    < | \_\ (  <_> > <  <
 *   Firmware   |____|_  /\____/ \___  >__|_ \|___  /\____/__/\_ \
 *                     \/            \/     \/    \/            \/
 * $Id$
 *
 * Copyright (C) 2002 by Alan Korr
 *
 * All files in this archive are subject to the GNU General Public License.
 * See the file COPYING in the source tree root for full license agreement.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 ****************************************************************************/
#include <stdio.h>
#include "config.h"
#include "system.h"
#include "lcd.h"
#include "font.h"

#define default_interrupt(name) \
  extern __attribute__((weak,alias("UIE"))) void name (void)

static const char* const irqname[] = {
    "", "", "AccessErr","AddrErr","IllInstr", "DivX0", "","",
    "PrivVio","Trace","Line-A", "Line-F","Debug","","FormErr","Uninit",
    "","","","","","","","",
    "Spurious","Level1","Level2","Level3","Level4","Level5","Level6","Level7",
    "Trap0","Trap1","Trap2","Trap3","Trap4","Trap5","Trap6","Trap7",
    "Trap8","Trap9","Trap10","Trap11","Trap12","Trap13","Trap14","Trap15",
    "SWT","Timer0","Timer1","I2C","UART1","UART2","DMA0","DMA1",
    "DMA2","DMA3","QSPI","","","","","",
    "PDIR1FULL","PDIR2FULL","EBUTXEMPTY","IIS2TXEMPTY",
    "IIS1TXEMPTY","PDIR3FULL","PDIR3RESYN","UQ2CHANERR",
    "AUDIOTICK","PDIR2RESYN","PDIR2UNOV","PDIR1RESYN",
    "PDIR1UNOV","UQ1CHANERR","IEC2BUFATTEN","IEC2PARERR",
    "IEC2VALNOGOOD","IEC2CNEW","IEC1BUFATTEN","UCHANTXNF",
    "UCHANTXUNDER","UCHANTXEMPTY","PDIR3UNOV","IEC1PARERR",
    "IEC1VALNOGOOD","IEC1CNEW","EBUTXRESYN","EBUTXUNOV",
    "IIS2TXRESYN","IIS2TXUNOV","IIS1TXRESYN","IIS1TXUNOV",
    "GPI0","GPI1","GPI2","GPI3","GPI4","GPI5","GPI6","GPI7",
    "","","","","","","","SOFTINT0",
    "SOFTINT1","SOFTINT2","SOFTINT3","",
    "","CDROMCRCERR","CDROMNOSYNC","CDROMILSYNC",
    "CDROMNEWBLK","","","","","","IIC2","ADC",
    "","","","","","","","",
    "","","","","","","","",
    "","","","","","","","",
    "","","","","","","","",
    "","","","","","","","",
    "","","","","","","","",
    "","","","","","","","",
    "","","","","","","",""
};

default_interrupt (TRAP0); /* Trap #0 */
default_interrupt (TRAP1); /* Trap #1 */
default_interrupt (TRAP2); /* Trap #2 */
default_interrupt (TRAP3); /* Trap #3 */
default_interrupt (TRAP4); /* Trap #4 */
default_interrupt (TRAP5); /* Trap #5 */
default_interrupt (TRAP6); /* Trap #6 */
default_interrupt (TRAP7); /* Trap #7 */
default_interrupt (TRAP8); /* Trap #8 */
default_interrupt (TRAP9); /* Trap #9 */
default_interrupt (TRAP10); /* Trap #10 */
default_interrupt (TRAP11); /* Trap #11 */
default_interrupt (TRAP12); /* Trap #12 */
default_interrupt (TRAP13); /* Trap #13 */
default_interrupt (TRAP14); /* Trap #14 */
default_interrupt (TRAP15); /* Trap #15 */
default_interrupt (SWT); /* Software Watchdog Timer */
default_interrupt (TIMER0); /* Timer 0 */
default_interrupt (TIMER1); /* Timer 1 */
default_interrupt (I2C); /* I2C */
default_interrupt (UART1); /* UART 1 */
default_interrupt (UART2); /* UART 2 */
default_interrupt (DMA0); /* DMA 0 */
default_interrupt (DMA1); /* DMA 1 */
default_interrupt (DMA2); /* DMA 2 */
default_interrupt (DMA3); /* DMA 3 */
default_interrupt (QSPI); /* QSPI */

default_interrupt (PDIR1FULL); /* Processor data in 1 full */
default_interrupt (PDIR2FULL); /* Processor data in 2 full */
default_interrupt (EBUTXEMPTY); /* EBU transmit FIFO empty */
default_interrupt (IIS2TXEMPTY); /* IIS2 transmit FIFO empty */
default_interrupt (IIS1TXEMPTY); /* IIS1 transmit FIFO empty */
default_interrupt (PDIR3FULL); /* Processor data in 3 full */
default_interrupt (PDIR3RESYN); /* Processor data in 3 resync */
default_interrupt (UQ2CHANERR); /* IEC958-2 Rx U/Q channel error */
default_interrupt (AUDIOTICK); /* "tick" interrupt */
default_interrupt (PDIR2RESYN); /* Processor data in 2 resync */
default_interrupt (PDIR2UNOV); /* Processor data in 2 under/overrun */
default_interrupt (PDIR1RESYN); /* Processor data in 1 resync */
default_interrupt (PDIR1UNOV); /* Processor data in 1 under/overrun */
default_interrupt (UQ1CHANERR); /* IEC958-1 Rx U/Q channel error */
default_interrupt (IEC2BUFATTEN);/* IEC958-2 channel buffer full */
default_interrupt (IEC2PARERR); /* IEC958-2 Rx parity or symbol error */
default_interrupt (IEC2VALNOGOOD);/* IEC958-2 flag not good */
default_interrupt (IEC2CNEW); /* IEC958-2 New C-channel received */
default_interrupt (IEC1BUFATTEN);/* IEC958-1 channel buffer full */
default_interrupt (UCHANTXNF); /* U channel Tx reg next byte is first */
default_interrupt (UCHANTXUNDER);/* U channel Tx reg underrun */
default_interrupt (UCHANTXEMPTY);/* U channel Tx reg is empty */
default_interrupt (PDIR3UNOV); /* Processor data in 3 under/overrun */
default_interrupt (IEC1PARERR); /* IEC958-1 Rx parity or symbol error */
default_interrupt (IEC1VALNOGOOD);/* IEC958-1 flag not good */
default_interrupt (IEC1CNEW); /* IEC958-1 New C-channel received */
default_interrupt (EBUTXRESYN); /* EBU Tx FIFO resync */
default_interrupt (EBUTXUNOV); /* EBU Tx FIFO under/overrun */
default_interrupt (IIS2TXRESYN); /* IIS2 Tx FIFO resync */
default_interrupt (IIS2TXUNOV); /* IIS2 Tx FIFO under/overrun */
default_interrupt (IIS1TXRESYN); /* IIS1 Tx FIFO resync */
default_interrupt (IIS1TXUNOV); /* IIS1 Tx FIFO under/overrun */
default_interrupt (GPI0); /* GPIO interrupt 0 */
default_interrupt (GPI1); /* GPIO interrupt 1 */
default_interrupt (GPI2); /* GPIO interrupt 2 */
default_interrupt (GPI3); /* GPIO interrupt 3 */
default_interrupt (GPI4); /* GPIO interrupt 4 */
default_interrupt (GPI5); /* GPIO interrupt 5 */
default_interrupt (GPI6); /* GPIO interrupt 6 */
default_interrupt (GPI7); /* GPIO interrupt 7 */

default_interrupt (SOFTINT0); /* Software interrupt 0 */
default_interrupt (SOFTINT1); /* Software interrupt 1 */
default_interrupt (SOFTINT2); /* Software interrupt 2 */
default_interrupt (SOFTINT3); /* Software interrupt 3 */

default_interrupt (CDROMCRCERR); /* CD-ROM CRC error */
default_interrupt (CDROMNOSYNC); /* CD-ROM No sync */
default_interrupt (CDROMILSYNC); /* CD-ROM Illegal sync */
default_interrupt (CDROMNEWBLK); /* CD-ROM New block */

default_interrupt (IIC2); /* I2C 2 */
default_interrupt (ADC);  /* A/D converter */

#if defined(IAUDIO_X5) || defined(IAUDIO_M5)
#define EXCP_BUTTON_GPIO_READ   GPIO_READ
#define EXCP_BUTTON_MASK        0x0c000000
#define EXCP_BUTTON_VALUE       0x08000000 /* On button and !hold */
#define EXCP_PLLCR              0x10400000
#elif defined(IAUDIO_M3)
#define EXCP_BUTTON_GPIO_READ   GPIO1_READ
#define EXCP_BUTTON_MASK        0x00000202
#define EXCP_BUTTON_VALUE       0x00000200 /* On button and !hold */
#define EXCP_PLLCR              0x10800000
#else
#define EXCP_BUTTON_GPIO_READ   GPIO1_READ
#define EXCP_BUTTON_MASK        0x00000022
#define EXCP_BUTTON_VALUE       0x00000000
#define EXCP_PLLCR              0x10800000
#endif

static void system_display_exception_info(unsigned long format,
                                          unsigned long pc) __attribute__ ((noreturn, used));
static void system_display_exception_info(unsigned long format,
                                          unsigned long pc)
{
    int vector = (format >> 18) & 0xff;
    char str[32];

    /* clear screen */
    lcd_clear_display ();
    lcd_setfont(FONT_SYSFIXED);

    snprintf(str, sizeof(str), "I%02x:%s", vector, irqname[vector]);
    lcd_puts(0, 0, str);
    snprintf(str, sizeof(str), "at %08x", pc);
    lcd_puts(0, 1, str);
    lcd_update();

    /* set cpu frequency to 11mhz (to prevent overheating) */
    DCR   = (DCR & ~0x01ff) | 1;
    PLLCR = EXCP_PLLCR;

    while (1)
    {
        if ((EXCP_BUTTON_GPIO_READ & EXCP_BUTTON_MASK) == EXCP_BUTTON_VALUE)
            SYPCR = 0xc0;
           /* Start watchdog timer with 512 cycles timeout. Don't service it. */
    }

    /* We need a reset method that works in all cases. Calling system_reboot()
       doesn't work when we're called from the debug interrupt, because then
       the CPU is in emulator mode and the only ways leaving it are exexcuting
       an rte instruction or performing a reset. Even disabling the breakpoint
       logic and performing special rte magic doesn't make system_reboot()
       reliable. The system restarts, but boot often fails with ata error -42. */
}

static void UIE(void) __attribute__ ((noreturn));
static void UIE(void)
{
    asm volatile("subq.l #4,%sp"); /* phony return address - never used */
    asm volatile("jmp    system_display_exception_info");
    while (1); /* loop to silence 'noreturn' function does return */
}

/* reset vectors are handled in crt0.S */
void (* const vbr[]) (void) __attribute__ ((section (".vectors"))) =
{
    UIE,UIE,UIE,UIE,UIE,UIE,
    UIE,UIE,UIE,UIE,UIE,UIE,UIE,UIE,
    UIE,UIE,UIE,UIE,UIE,UIE,UIE,UIE,
    UIE,UIE,UIE,TIMER0,TIMER1,UIE,UIE,UIE,
    /*          lvl 3  lvl 4             */

    TRAP0,TRAP1,TRAP2,TRAP3,TRAP4,TRAP5,TRAP6,TRAP7,
    TRAP8,TRAP9,TRAP10,TRAP11,TRAP12,TRAP13,TRAP14,TRAP15,

    SWT,UIE,UIE,I2C,UART1,UART2,DMA0,DMA1,
    DMA2,DMA3,QSPI,UIE,UIE,UIE,UIE,UIE,
    PDIR1FULL,PDIR2FULL,EBUTXEMPTY,IIS2TXEMPTY,
    IIS1TXEMPTY,PDIR3FULL,PDIR3RESYN,UQ2CHANERR,
    AUDIOTICK,PDIR2RESYN,PDIR2UNOV,PDIR1RESYN,
    PDIR1UNOV,UQ1CHANERR,IEC2BUFATTEN,IEC2PARERR,
    IEC2VALNOGOOD,IEC2CNEW,IEC1BUFATTEN,UCHANTXNF,
    UCHANTXUNDER,UCHANTXEMPTY,PDIR3UNOV,IEC1PARERR,
    IEC1VALNOGOOD,IEC1CNEW,EBUTXRESYN,EBUTXUNOV,
    IIS2TXRESYN,IIS2TXUNOV,IIS1TXRESYN,IIS1TXUNOV,
    GPI0,GPI1,GPI2,GPI3,GPI4,GPI5,GPI6,GPI7,
    UIE,UIE,UIE,UIE,UIE,UIE,UIE,SOFTINT0,
    SOFTINT1,SOFTINT2,SOFTINT3,UIE,
    UIE,CDROMCRCERR,CDROMNOSYNC,CDROMILSYNC,
    CDROMNEWBLK,UIE,UIE,UIE,UIE,UIE,IIC2,ADC,

    UIE,UIE,UIE,UIE,UIE,UIE,UIE,UIE,UIE,UIE,UIE,UIE,UIE,UIE,UIE,UIE,
    UIE,UIE,UIE,UIE,UIE,UIE,UIE,UIE,UIE,UIE,UIE,UIE,UIE,UIE,UIE,UIE,
    UIE,UIE,UIE,UIE,UIE,UIE,UIE,UIE,UIE,UIE,UIE,UIE,UIE,UIE,UIE,UIE,
    UIE,UIE,UIE,UIE,UIE,UIE,UIE,UIE,UIE,UIE,UIE,UIE,UIE,UIE,UIE,UIE,
    UIE,UIE,UIE,UIE,UIE,UIE,UIE,UIE,UIE,UIE,UIE,UIE,UIE,UIE,UIE,UIE,
    UIE,UIE,UIE,UIE,UIE,UIE,UIE,UIE,UIE,UIE,UIE,UIE,UIE,UIE,UIE,UIE,
    UIE,UIE,UIE,UIE,UIE,UIE,UIE,UIE,UIE,UIE,UIE,UIE,UIE,UIE,UIE,UIE,
    UIE,UIE,UIE,UIE,UIE,UIE,UIE,UIE,UIE,UIE,UIE,UIE,UIE,UIE,UIE,UIE
};

void system_init(void)
{
    /* Clear the accumulators. From here on it's the responsibility of
       whoever uses them to clear them after use and before giving control
       to "foreign" code (use movclr instruction). */
    asm volatile ("movclr.l %%acc0, %%d0\n\t"
                  "movclr.l %%acc1, %%d0\n\t"
                  "movclr.l %%acc2, %%d0\n\t"
                  "movclr.l %%acc3, %%d0\n\t"
                  : : : "d0");

    /* Set EMAC unit to fractional mode with saturation, since that's
       what'll be the most useful for most things which the main thread
       will do. */
    coldfire_set_macsr(EMAC_FRACTIONAL | EMAC_SATURATE);

    /* Set INTBASE and SPURVEC */
    INTBASE = 64;
    SPURVEC = 24;

    MPARK     = 0x81;   /* PARK[1,0]=10 + BCR24BIT                   */
    
#ifndef HAVE_ADJUSTABLE_CPU_FREQ
    cf_set_cpu_frequency(CPUFREQ_DEFAULT);
#endif
}

void system_reboot (void)
{
    set_cpu_frequency(0);

    asm(" move.w #0x2700,%sr");
    /* Reset the cookie for the crt0 crash check */
    asm(" move.l #0,%d0");
    asm(" move.l %d0,0x10017ffc");
    asm(" movec.l %d0,%vbr");
    asm(" move.l 0,%sp");
    asm(" move.l 4,%a0");
    asm(" jmp (%a0)");
}

/* Utilise the breakpoint hardware to catch invalid memory accesses. */
int system_memory_guard(int newmode)
{
    static const unsigned long modes[MAXMEMGUARD][8] = {
        {   /* catch nothing */
            0x2C870000, 0x00000000, /* TDR  = 0x00000000 */
            0x2C8D0000, 0x00000000, /* ABLR = 0x00000000 */
            0x2C8C0000, 0x00000000, /* ABHR = 0x00000000 */
            0x2C860000, 0x00050000, /* AATR = 0x0005 */
        },
        {   /* catch flash ROM writes */
            0x2C8D0000, 0x00000000, /* ABLR = 0x00000000 */
            0x2C8C0FFF, 0xFFFF0000, /* ABHR = 0x0FFFFFFF */
            0x2C860000, 0x6F050000, /* AATR = 0x6F05 */
            0x2C878000, 0x20080000, /* TDR  = 0x80002008 */
        },
        {   /* catch all accesses to zero area */
            0x2C8D0000, 0x00000000, /* ABLR = 0x00000000 */
            0x2C8C0FFF, 0xFFFF0000, /* ABHR = 0x0FFFFFFF */
            0x2C860000, 0xEF050000, /* AATR = 0xEF05 */
            0x2C878000, 0x20080000, /* TDR  = 0x80002008 */
        }
        /* Note: CPU space accesses (movec instruction), interrupt acknowledges
           and emulator mode accesses are never caught. */
    };
    static int cur_mode = MEMGUARD_NONE;

    int oldmode = cur_mode;
    const unsigned long *ptr;
    int i;

    if (newmode == MEMGUARD_KEEP)
        newmode = oldmode;

    /* Always set the new mode, we don't know the old settings
       as we cannot read back */
    ptr = modes[newmode];
    for (i = 0; i < 4; i++)
    {
        asm ( "wdebug (%0) \n" : : "a"(ptr));
        ptr += 2;
    }
    cur_mode = newmode;

    return oldmode;
}

/* allow setting of audio clock related bits */
void coldfire_set_pllcr_audio_bits(long bits)
{
    PLLCR = (PLLCR & ~0x70400000) | (bits & 0x70400000);
}

/* Set DATAINCONTROL without disturbing FIFO reset state */
void coldfire_set_dataincontrol(unsigned long value)
{
    /* Have to be atomic against recording stop initiated by DMA1 */
    int level = set_irq_level(DMA_IRQ_LEVEL);
    DATAINCONTROL = (DATAINCONTROL & (1 << 9)) | value;
    set_irq_level(level);
}
