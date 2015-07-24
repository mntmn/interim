// Raspberry Pi VideoCoreIV
//////////////////////////////////////////////////////
// V3D Register Address Map
//////////////////////////////////////////////////////
#define V3D_BASE   0xC00000 // V3D Base Address ($20C00000 PHYSICAL, $7EC00000 BUS)
#define V3D_IDENT0  0x00000 // V3D Identification 0 (V3D Block Identity)
#define V3D_IDENT1  0x00004 // V3D Identification 1 (V3D Configuration A)
#define V3D_IDENT2  0x00008 // V3D Identification 2 (V3D Configuration B)
#define V3D_IDENT3  0x0000C // V3D Identification 3 (V3D Configuration C)
#define V3D_SCRATCH 0x00010 // V3D Scratch Register
#define V3D_L2CACTL 0x00020 // V3D L2 Cache Control
#define V3D_SLCACTL 0x00024 // V3D Slices Cache Control
#define V3D_INTCTL  0x00030 // V3D Interrupt Control
#define V3D_INTENA  0x00034 // V3D Interrupt Enables
#define V3D_INTDIS  0x00038 // V3D Interrupt Disables
#define V3D_CT0CS   0x00100 // V3D Control List Executor Thread 0 Control & Status
#define V3D_CT1CS   0x00104 // V3D Control List Executor Thread 1 Control & Status
#define V3D_CT0EA   0x00108 // V3D Control List Executor Thread 0 End Address
#define V3D_CT1EA   0x0010C // V3D Control List Executor Thread 1 End Address
#define V3D_CT0CA   0x00110 // V3D Control List Executor Thread 0 Current Address
#define V3D_CT1CA   0x00114 // V3D Control List Executor Thread 1 Current Address
#define V3D_CT0RA0  0x00118 // V3D Control List Executor Thread 0 Return Address
#define V3D_CT1RA0  0x0011C // V3D Control List Executor Thread 1 Return Address
#define V3D_CT0LC   0x00120 // V3D Control List Executor Thread 0 List Counter
#define V3D_CT1LC   0x00124 // V3D Control List Executor Thread 1 List Counter
#define V3D_CT0PC   0x00128 // V3D Control List Executor Thread 0 Primitive List Counter
#define V3D_CT1PC   0x0012C // V3D Control List Executor Thread 1 Primitive List Counter
#define V3D_PCS     0x00130 // V3D Pipeline Control & Status
#define V3D_BFC     0x00134 // V3D Binning Mode Flush Count
#define V3D_RFC     0x00138 // V3D Rendering Mode Frame Count
#define V3D_BPCA    0x00300 // V3D Current Address Of Binning Memory Pool
#define V3D_BPCS    0x00304 // V3D Remaining Size Of Binning Memory Pool
#define V3D_BPOA    0x00308 // V3D Address Of Overspill Binning Memory Block
#define V3D_BPOS    0x0030C // V3D Size Of Overspill Binning Memory Block
#define V3D_BXCF    0x00310 // V3D Binner Debug
#define V3D_SQRSV0  0x00410 // V3D Reserve QPUs 0-7
#define V3D_SQRSV1  0x00414 // V3D Reserve QPUs 8-15
#define V3D_SQCNTL  0x00418 // V3D QPU Scheduler Control
#define V3D_SQCSTAT 0x0041C // V3D QPU Scheduler State
#define V3D_SRQPC   0x00430 // V3D QPU User Program Request Program Address
#define V3D_SRQUA   0x00434 // V3D QPU User Program Request Uniforms Address
#define V3D_SRQUL   0x00438 // V3D QPU User Program Request Uniforms Length
#define V3D_SRQCS   0x0043C // V3D QPU User Program Request Control & Status
#define V3D_VPACNTL 0x00500 // V3D VPM Allocator Control
#define V3D_VPMBASE 0x00504 // V3D VPM Base (User) Memory Reservation
#define V3D_PCTRC   0x00670 // V3D Performance Counter Clear
#define V3D_PCTRE   0x00674 // V3D Performance Counter Enables
#define V3D_PCTR0   0x00680 // V3D Performance Counter Count 0
#define V3D_PCTRS0  0x00684 // V3D Performance Counter Mapping 0
#define V3D_PCTR1   0x00688 // V3D Performance Counter Count 1
#define V3D_PCTRS1  0x0068C // V3D Performance Counter Mapping 1
#define V3D_PCTR2   0x00690 // V3D Performance Counter Count 2
#define V3D_PCTRS2  0x00694 // V3D Performance Counter Mapping 2
#define V3D_PCTR3   0x00698 // V3D Performance Counter Count 3
#define V3D_PCTRS3  0x0069C // V3D Performance Counter Mapping 3
#define V3D_PCTR4   0x006A0 // V3D Performance Counter Count 4
#define V3D_PCTRS4  0x006A4 // V3D Performance Counter Mapping 4
#define V3D_PCTR5   0x006A8 // V3D Performance Counter Count 5
#define V3D_PCTRS5  0x006AC // V3D Performance Counter Mapping 5
#define V3D_PCTR6   0x006B0 // V3D Performance Counter Count 6
#define V3D_PCTRS6  0x006B4 // V3D Performance Counter Mapping 6
#define V3D_PCTR7   0x006B8 // V3D Performance Counter Count 7
#define V3D_PCTRS7  0x006BC // V3D Performance Counter Mapping 7
#define V3D_PCTR8   0x006C0 // V3D Performance Counter Count 8
#define V3D_PCTRS8  0x006C4 // V3D Performance Counter Mapping 8
#define V3D_PCTR9   0x006C8 // V3D Performance Counter Count 9
#define V3D_PCTRS9  0x006CC // V3D Performance Counter Mapping 9
#define V3D_PCTR10  0x006D0 // V3D Performance Counter Count 10
#define V3D_PCTRS10 0x006D4 // V3D Performance Counter Mapping 10
#define V3D_PCTR11  0x006D8 // V3D Performance Counter Count 11
#define V3D_PCTRS11 0x006DC // V3D Performance Counter Mapping 11
#define V3D_PCTR12  0x006E0 // V3D Performance Counter Count 12
#define V3D_PCTRS12 0x006E4 // V3D Performance Counter Mapping 12
#define V3D_PCTR13  0x006E8 // V3D Performance Counter Count 13
#define V3D_PCTRS13 0x006EC // V3D Performance Counter Mapping 13
#define V3D_PCTR14  0x006F0 // V3D Performance Counter Count 14
#define V3D_PCTRS14 0x006F4 // V3D Performance Counter Mapping 14
#define V3D_PCTR15  0x006F8 // V3D Performance Counter Count 15
#define V3D_PCTRS15 0x006FC // V3D Performance Counter Mapping 15
#define V3D_DBCFG   0x00E00 // V3D Configure
#define V3D_DBSCS   0x00E04 // V3D S Control & Status
#define V3D_DBSCFG  0x00E08 // V3D S Configure
#define V3D_DBSSR   0x00E0C // V3D S SR
#define V3D_DBSDR0  0x00E10 // V3D SD R0
#define V3D_DBSDR1  0x00E14 // V3D SD R1
#define V3D_DBSDR2  0x00E18 // V3D SD R2
#define V3D_DBSDR3  0x00E1C // V3D SD R3
#define V3D_DBQRUN  0x00E20 // V3D QPU Run
#define V3D_DBQHLT  0x00E24 // V3D QPU Halt
#define V3D_DBQSTP  0x00E28 // V3D QPU Step
#define V3D_DBQITE  0x00E2C // V3D QPU Interrupt Enables
#define V3D_DBQITC  0x00E30 // V3D QPU Interrupt Control
#define V3D_DBQGHC  0x00E34 // V3D QPU GHC
#define V3D_DBQGHG  0x00E38 // V3D QPU GHG
#define V3D_DBQGHH  0x00E3C // V3D QPU GHH
#define V3D_DBGE    0x00F00 // V3D PSE Error Signals
#define V3D_FDBGO   0x00F04 // V3D FEP Overrun Error Signals
#define V3D_FDBGB   0x00F08 // V3D FEP Interface Ready & Stall Signals, FEP Busy Signals
#define V3D_FDBGR   0x00F0C // V3D FEP Internal Ready Signals
#define V3D_FDBGS   0x00F10 // V3D FEP Internal Stall Input Signals
#define V3D_ERRSTAT 0x00F20 // V3D Miscellaneous Error Signals (VPM, VDW, VCD, VCM, L2C)

//////////////////////////////////////////////////
// V3D Identity Registers
//////////////////////////////////////////////////
// V3D_IDENT0: V3D Identification 0 (V3D Block Identity) Register Description
#define IDSTR 0x00FFFFFF // V3D_IDENT0: V3D ID String (Reads As "V3D") READ
#define TVER  0xFF000000 // V3D_IDENT0: V3D Technology Version (Reads Technology Version = 2) READ

// V3D_IDENT1: V3D Identification 1 (V3D Configuration A) Register Description
#define REVR  0x0000000F // V3D_IDENT1: V3D Revision READ
#define NSLC  0x000000F0 // V3D_IDENT1: Number Of Slices READ
#define QUPS  0x00000F00 // V3D_IDENT1: Number Of QPUs Per Slice READ
#define TUPS  0x0000F000 // V3D_IDENT1: Number Of TMUs Per Slice READ
#define NSEM  0x00FF0000 // V3D_IDENT1: Number Of Semaphores READ
#define HDRT  0x0F000000 // V3D_IDENT1: HDR Support (0 = Not Supported, 1 = Supported) READ
#define VPMSZ 0xF0000000 // V3D_IDENT1: VPM Memory Size (Multiples Of 1K, 0 => 16K) READ

// V3D_IDENT2: V3D Identification 2 (V3D Configuration B) Register Description
#define VRISZ 0x0000000F // V3D_IDENT2: VRI Memory Size (0 = Half Size, 1 = Full Size) READ
#define TLBSZ 0x000000F0 // V3D_IDENT2: Tile Buffer Size (0 = Quarter Size, 1 = Half Size, 2 = Full Size (32x32msm)) READ
#define TLBDB 0x00000F00 // V3D_IDENT2: Tile Buffer Double-Buffer Mode Support (0 = Not Supported, 1 = Supported) READ

////////////////////////////////////////////////////////////
// V3D Miscellaneous Registers
////////////////////////////////////////////////////////////
// V3D_SCRATCH: V3D Scratch Register Description
#define SCRATCH 0xFFFFFFFF // V3D_SCRATCH: Scratch Register (Read/Write Registers For General Purposes) READ/WRITE

////////////////////////////////////////////////////////////
// V3D Cache Control Registers
////////////////////////////////////////////////////////////
// V3D_L2CACTL: V3D L2 Cache Control Register Description
#define L2CENA 0x00000001 // V3D_L2CACTL: L2 Cache Enable (Reads State Of Cache Enable Bit, Write To Enable The L2 Cache) READ/WRITE
#define L2CDIS 0x00000002 // V3D_L2CACTL: L2 Cache Disable (Write To Disable The L2 Cache) WRITE
#define L2CCLR 0x00000004 // V3D_L2CACTL: L2 Cache Clear (Write To Clear The L2 Cache) WRITE

// V3D_SLCACTL: V3D Slices Cache Control Register Description
#define ICCS0_to_ICCS3   0x0000000F // V3D_SLCACTL: Instruction Cache Clear Bits (Write To Clear Instruction Cache) WRITE
#define UCCS0_to_UCCS3   0x00000F00 // V3D_SLCACTL: Uniforms Cache Clear Bits (Write To Clear Uniforms Cache) WRITE
#define T0CCS0_to_T0CCS3 0x000F0000 // V3D_SLCACTL: TMU0 Cache Clear Bits (Write To Clear TMU0 Cache) WRITE
#define T1CCS0_to_T1CCS3 0x0F000000 // V3D_SLCACTL: TMU1 Cache Clear Bits (Write To Clear TMU1 Cache) WRITE

//////////////////////////////////////////////////////////////////
// V3D Pipeline Interrupt Control
//////////////////////////////////////////////////////////////////
// V3D_INTCTL: V3D Interrupt Control Register Description
#define INT_FRDONE   0x00000001 // V3D_INTCTL: Render Mode Frame Done Interrupt Status (Set When All Tiles Of The Frame Have Been Written To Memory) READ/WRITE
#define INT_FLDONE   0x00000002 // V3D_INTCTL: Binning Mode Flush Done Interrupt Status (Set When Binning Is Complete With All Tile Lists Flushed To Memory) READ/WRITE
#define INT_OUTOMEM  0x00000004 // V3D_INTCTL: Binner Out Of Memory Interrupt Status (Set While The Binner Needs More Memory To Complete) READ/WRITE
#define INT_SPILLUSE 0x00000008 // V3D_INTCTL: Binner Used Overspill Memory Interrupt Status (Set When The Binner Starts Using The (Valid) Overspill Memory Buffer) READ/WRITE

// V3D_INTENA: V3D Interrupt Enables Register Description
#define EI_FRDONE   0x00000001 // V3D_INTENA: Render Mode Frame Done Interrupt Enable (Set When The INT_FRDONE Interrupt Is Set) READ/WRITE
#define EI_FLDONE   0x00000002 // V3D_INTENA: Binning Mode Flush Done Interrupt Enable (Set When The INT_FLDONE Interrupt Is Set) READ/WRITE
#define EI_OUTOMEM  0x00000004 // V3D_INTENA: Binner Out Of Memory Interrupt Enable (Set When The INT_OUTOMEM Interrupt Is Set) READ/WRITE
#define EI_SPILLUSE 0x00000008 // V3D_INTENA: Binner Used Overspill Memory Interrupt Enable (Set When The INT_SPILLUSE Interrupt Is Set) READ/WRITE

// V3D_INTDIS: V3D Interrupt Disables Register Description
#define DI_FRDONE   0x00000001 // V3D_INTDIS: Render Mode Frame Done Interrupt Disable (Set When The INT_FRDONE Interrupt Is Set) READ/WRITE
#define DI_FLDONE   0x00000002 // V3D_INTDIS: Binning Mode Flush Done Interrupt Disable (Set When The INT_FLDONE Interrupt Is Set) READ/WRITE
#define DI_OUTOMEM  0x00000004 // V3D_INTDIS: Binner Out Of Memory Interrupt Disable (Set When The INT_OUTOMEM Interrupt Is Set) READ/WRITE
#define DI_SPILLUSE 0x00000008 // V3D_INTDIS: Binner Used Overspill Memory Interrupt Disable (Set When The INT_SPILLUSE Interrupt Is Set) READ/WRITE
 
//////////////////////////////////////////////////////////////////////////////////////////////////////
// V3D Control List Executor Registers (Per Thread)
//////////////////////////////////////////////////////////////////////////////////////////////////////
// V3D_CTnCS: V3D Control List Executor Thread n Control & Status Register Description
#define CTMODE 0x00000001 // V3D_CTnCS: Control Thread Mode (Binning Mode Thread Only) READ
#define CTERR  0x00000008 // V3D_CTnCS: Control Thread Error (Set When Stopped With An Error, Cleared On Restarting) READ
#define CTSUBS 0x00000010 // V3D_CTnCS: Control Thread Sub-Mode READ/WRITE
#define CTRUN  0x00000020 // V3D_CTnCS: Control Thread Run READ/WRITE
#define CTRTSD 0x00000300 // V3D_CTnCS: Return Stack Depth (Number Of Levels Of List Nesting) READ
#define CTSEMA 0x00007000 // V3D_CTnCS: Counting Semaphore (Current State Of The Counting Semaphore For This Thread) READ
#define CTRSTA 0x00008000 // V3D_CTnCS: Reset Bit (Writing 1 Stops The Control Thread & Resets All Bits In This Register) WRITE
 
// V3D_CTnEA: V3D Control List Executor Thread n End Address Register Description
#define CTLEA 0xFFFFFFFF // V3D_CTnEA: Control List End Address (Set To The Byte Address After The Last Record In The Control List) READ/WRITE
 
// V3D_CTnCA: V3D Control List Executor Thread n Current Address Register Description
#define CTLCA 0xFFFFFFFF // V3D_CTnCA: Control List Current Address (Points To The Address Of The Current Record In The Control List) READ/WRITE
 
// V3D_CTnRA0: V3D Control List Executor Thread n Return Address Register Description
#define CTLRA 0xFFFFFFFF // V3D_CTnRA0: Control List Return Address 0 (Address On Return Address Stack) READ
 
// V3D_CTnLC: V3D Control List Executor Thread n List Counter Register Description
#define CTLSLCS 0x0000FFFF // V3D_CTnLC: Sub-list Counter (Count Of Return Commands Encountered) READ/WRITE
#define CTLLCM  0xFFFF0000 // V3D_CTnLC: Major List Counter (Count Of Flush Commands Encountered) READ/WRITE
 
// V3D_CTnPC: V3D Control List Executor Thread n Primitive List Counter Register Description
#define CTLPC 0xFFFFFFFF // V3D_CTnPC: Primitive List Counter (Count Of Primitives Remaining Whilst Processing A Primitive List) READ
 
//////////////////////////////////////////////////
// V3D Pipeline Registers
//////////////////////////////////////////////////
// V3D_PCS: V3D Pipeline Control & Status Register Description
#define BMACTIVE 0x00000001 // V3D_PCS: Binning Mode Active (Set While Binning Pipeline Is In Use) READ
#define BMBUSY   0x00000002 // V3D_PCS: Binning Mode Busy (Set While Any Binning Operations Are Actually In Progress) READ
#define RMACTIVE 0x00000004 // V3D_PCS: Rendering Mode Active (Set While Rendering Pipeline Is In Use) READ
#define RMBUSY   0x00000008 // V3D_PCS: Rendering Mode Busy (Set While Any Rendering Operations Are Actually In Progress) READ
#define BMOOM    0x00000100 // V3D_PCS: Binning Mode Out Of Memory (Set When PTB Runs Out Of Binning Memory While Binning) READ
 
// V3D_BFC: V3D Binning Mode Flush Count Register Description
#define BMFCT 0x000000FF // V3D_BFC: Flush Count (Count Increments In Binning Mode Once PTB Has Flushed All Tile Lists To Mem & PTB Has Finished With Tile State Data Array) READ/WRITE
 
// V3D_RFC: V3D Rendering Mode Frame Count Register Description
#define RMFCT 0x000000FF // V3D_RFC: Frame Count (Count Increments In Rendering Mode When Last Tile Store Operation Of Frame Completes, The Tile Has Fully Written Out To Mem) READ/WRITE
 
// V3D_BPCA: V3D Current Address Of Binning Memory Pool Register Description
#define BMPCA 0xFFFFFFFF // V3D_BPCA: Current Pool Address (The Address Of The Current Allocation Pointer In The Binning Memory Pool) READ
 
// V3D_BPCS: V3D Remaining Size Of Binning Memory Pool Register Description
#define BMPRS 0xFFFFFFFF // V3D_BPCS: Size Of Pool Remaining (The Number Of Bytes Remaining In The Binning Memory Pool) READ
 
// V3D_BPOA: V3D Address Of Overspill Binning Memory Block Register Description
#define BMPOA 0xFFFFFFFF // V3D_BPOA: Address Of Overspill Memory Block For Binning (Address Of Additional Mem That PTB Can Use For Binning Once Initial Pool Runs Out) READ/WRITE
 
// V3D_BPOS: V3D Size Of Overspill Binning Memory Block Register Description
#define BMPOS 0xFFFFFFFF // V3D_BPOS: Size Of Overspill Memory Block For Binning (Number Of Bytes Of Additional Mem That PTB Can Use For Binning Once Initial Pool Runs Out) READ/WRITE
 
////////////////////////////////////////////////////////////
// V3D QPU Scheduler Registers
////////////////////////////////////////////////////////////
// V3D_SQRSV0: V3D Reserve QPUs 0-7 Register Description
#define QPURSV0 0x0000000F // V3D_SQRSV0: Reservation Settings For QPU 0 READ/WRITE
#define QPURSV1 0x000000F0 // V3D_SQRSV0: Reservation Settings For QPU 1 READ/WRITE
#define QPURSV2 0x00000F00 // V3D_SQRSV0: Reservation Settings For QPU 2 READ/WRITE
#define QPURSV3 0x0000F000 // V3D_SQRSV0: Reservation Settings For QPU 3 READ/WRITE
#define QPURSV4 0x000F0000 // V3D_SQRSV0: Reservation Settings For QPU 4 READ/WRITE
#define QPURSV5 0x00F00000 // V3D_SQRSV0: Reservation Settings For QPU 5 READ/WRITE
#define QPURSV6 0x0F000000 // V3D_SQRSV0: Reservation Settings For QPU 6 READ/WRITE
#define QPURSV7 0xF0000000 // V3D_SQRSV0: Reservation Settings For QPU 7 READ/WRITE
 
// V3D_SQRSV1: V3D Reserve QPUs 8-15 Register Description
#define QPURSV8  0x0000000F // V3D_SQRSV1: Reservation Settings For QPU 8 READ/WRITE
#define QPURSV9  0x000000F0 // V3D_SQRSV1: Reservation Settings For QPU 9 READ/WRITE
#define QPURSV10 0x00000F00 // V3D_SQRSV1: Reservation Settings For QPU 10 READ/WRITE
#define QPURSV11 0x0000F000 // V3D_SQRSV1: Reservation Settings For QPU 11 READ/WRITE
#define QPURSV12 0x000F0000 // V3D_SQRSV1: Reservation Settings For QPU 12 READ/WRITE
#define QPURSV13 0x00F00000 // V3D_SQRSV1: Reservation Settings For QPU 13 READ/WRITE
#define QPURSV14 0x0F000000 // V3D_SQRSV1: Reservation Settings For QPU 14 READ/WRITE
#define QPURSV15 0xF0000000 // V3D_SQRSV1: Reservation Settings For QPU 15 READ/WRITE
 
// V3D_SQCNTL: V3D QPU Scheduler Control Register Description
#define VSRBL 0x00000003 // V3D_SQCNTL: Vertex Shader Scheduling Bypass Limit READ/WRITE
#define CSRBL 0x0000000C // V3D_SQCNTL: Coordinate Shader Scheduling Bypass Limit READ/WRITE
 
// V3D_SRQPC: V3D QPU User Program Request Program Address Register Description
#define QPURQPC 0xFFFFFFFF // V3D_SRQPC: Program Address (Writing This Register Queues A Request To Run A Program Starting At The Given Address) WRITE
 
// V3D_SRQUA: V3D QPU User Program Request Uniforms Address Register Description
#define QPURQUA 0xFFFFFFFF // V3D_SRQUA: Uniforms Address (Contains The Address Of The Uniforms Stream For The Next User Program To Be Queued Via A Write To V3DRQPC) READ/WRITE
 
// V3D_SRQUL: V3D QPU User Program Request Uniforms Length Register Description
#define QPURQUL 0x00000FFF // V3D_SRQUL: Uniforms Length (Contains The Max Length Of The Uniforms Stream For The Next User Program To Be Queued Via A Write To V3DRQPC) READ/WRITE
 
// V3D_SRQCS: V3D QPU User Program Request Control & Status Register Description
#define QPURQL   0x0000003F // V3D_SRQCS: Queue Length (Contains The Number Of Program Requests Currently Queued) READ/WRITE
#define QPURQERR 0x00000080 // V3D_SRQCS: Queue Error (Set When A Request Has Been Made When The Queue Is Full) READ/WRITE
#define QPURQCM  0x0000FF00 // V3D_SRQCS: Count Of User Program Requests Made (Contains The Total Number Of User Program Requests Made, Modulo 256) READ/WRITE
#define QPURQCC  0x00FF0000 // V3D_SRQCS: Count Of User Programs Completed (Contains The Total Number Of User Programs That Have Run & Completed, Modulo 256) READ/WRITE
 
////////////////////////////////////////
// V3D VPM Registers
////////////////////////////////////////
// V3D_VPACNTL: V3D VPM Allocator Control Register Description
#define VPARALIM 0x00000007 // V3D_VPACNTL: Rendering VPM Allocation Limit (Limits The Amount Of VPM Memory Allocated To Rendering Mode) READ/WRITE
#define VPABALIM 0x00000038 // V3D_VPACNTL: Binning VPM Allocation Limit (Limits The Amount Of VPM Memory Allocated To Binning Mode) READ/WRITE
#define VPARATO  0x000001C0 // V3D_VPACNTL: Rendering VPM Allocation Timeout (Sets A Timeout For Raising The Priority Of Rendering Mode Allocation Requests) READ/WRITE
#define VPABATO  0x00000E00 // V3D_VPACNTL: Binning VPM Allocation Timeout (Sets A Timeout For Raising The Priority Of Binning Mode Allocation Requests) READ/WRITE
#define VPALIMEN 0x00001000 // V3D_VPACNTL: Enable VPM Allocation Limits (Enables VPM Memory Allocation Limiting Using VPARALIM & VPABALIM) READ/WRITE
#define VPATOEN  0x00002000 // V3D_VPACNTL: Enable VPM Allocation Timeout (Enables VPM Memory Allocation Timeout Using VPARATO & VPABATO) READ/WRITE
 
// V3D_VPMBASE: V3D VPM Base (User) Memory Reservation Register Description
#define VPMURSV 0x0000001F // V3D_VPMBASE: VPM Memory Reserved For User Programs (Contains Amount Of VPM Mem Reserved For All User Programs, In Multiples Of 256 Bytes) READ/WRITE
 
////////////////////////////////////////////////////////
// V3D QPU Interrupt Control
////////////////////////////////////////////////////////
// V3D_DBQITE: V3D QPU Interrupt Enables Register Description
#define IE_QPU0_to_IE_QPU15 0x0000FFFF // V3D_DBQITE: QPU Interrupt Enable bits (Set Bit To Allow QPU To Generate An Interrupt) READ/WRITE
 
// V3D_DBQITC: V3D QPU Interrupt Control Register Description
#define IC_QPU0_to_IC_QPU15 0x0000FFFF // V3D_DBQITC: QPU Interrupt Control Bits (Reads When Interrupt Is Latched, Write To Clear Interrupt) READ/WRITE
 
//////////////////////////////////////////////////////
// V3D Performance Counters
//////////////////////////////////////////////////////
// V3D Sources For Performance Counters
#define COUNT_ID_0  =  0 // FEP Valid Primitives That Result In No Rendered Pixels, For All Rendered Tiles
#define COUNT_ID_1  =  1 // FEP Valid Primitives For All Rendered Tiles (Primitives May Be Counted In More Than One Tile)
#define COUNT_ID_2  =  2 // FEP Early-Z/Near/Far Clipped Quads
#define COUNT_ID_3  =  3 // FEP Valid Quads
#define COUNT_ID_4  =  4 // TLB Quads With No Pixels Passing The Stencil Test
#define COUNT_ID_5  =  5 // TLB Quads With No Pixels Passing The Z & Stencil Tests
#define COUNT_ID_6  =  6 // TLB Quads With Any Pixels Passing The Z & Stencil Tests
#define COUNT_ID_7  =  7 // TLB Quads With All Pixels Having Zero Coverage
#define COUNT_ID_8  =  8 // TLB Quads With Any Pixels Having Non-Zero Coverage
#define COUNT_ID_9  =  9 // TLB Quads With Valid Pixels Written To Color Buffer
#define COUNT_ID_10 = 10 // PTB Primitives Discarded By Being Outside The Viewport
#define COUNT_ID_11 = 11 // PTB Primitives That Need Clipping
#define COUNT_ID_12 = 12 // PSE Primitives That Are Discarded Because They Are Reversed
#define COUNT_ID_13 = 13 // QPU Total Idle Clock Cycles For All QPUs
#define COUNT_ID_14 = 14 // QPU Total Clock Cycles For All QPUs Doing Vertex/Coordinate Shading
#define COUNT_ID_15 = 15 // QPU Total Clock Cycles For All QPUs Doing Fragment Shading
#define COUNT_ID_16 = 16 // QPU Total Clock Cycles For All QPUs Executing Valid Instructions
#define COUNT_ID_17 = 17 // QPU Total Clock Cycles For All QPUs Stalled Waiting For TMUs
#define COUNT_ID_18 = 18 // QPU Total Clock Cycles For All QPUs Stalled Waiting For Scoreboard
#define COUNT_ID_19 = 19 // QPU Total Clock Cycles For All QPUs Stalled Waiting For Varyings
#define COUNT_ID_20 = 20 // QPU Total Instruction Cache Hits For All Slices
#define COUNT_ID_21 = 21 // QPU Total Instruction Cache Misses For All Slices
#define COUNT_ID_22 = 22 // QPU Total Uniforms Cache Hits For All Slices
#define COUNT_ID_23 = 23 // QPU Total Uniforms Cache Misses For All Slices
#define COUNT_ID_24 = 24 // TMU Total Texture Quads Processed
#define COUNT_ID_25 = 25 // TMU Total Texture Cache Misses (Number Of Fetches From Memory/L2 Cache)
#define COUNT_ID_26 = 26 // VPM Total Clock Cycles VDW Is Stalled Waiting For VPM Access
#define COUNT_ID_27 = 27 // VPM Total Clock Cycles VCD Is Stalled Waiting For VPM Access
#define COUNT_ID_28 = 28 // L2C Total Level 2 Cache Hits
#define COUNT_ID_29 = 29 // L2C Total Level 2 Cache Misses
 
// V3D_PCTRC: V3D Performance Counter Clear Register Description
#define CTCLR0_CTCLR15 0x0000FFFF // V3D_PCTRC: Performance Counter Clear Bits (Write To Clear The Performance Counter) WRITE
 
// V3D_PCTRE: V3D Performance Counter Enables Register Description
#define CTEN0_CTEN15 0x0000FFFF // V3D_PCTRE: Performance Counter Enable Bits (0 = Counter Disabled, 1 = Performance Counter Enabled To Count) READ/WRITE
 
// V3D_PCTRn: V3D Performance Counter Count n Register Description
#define PCTR 0xFFFFFFFF // V3D_PCTRn: Performance Count (Count Value) READ/WRITE
 
// V3D_PCTRSn: V3D Performance Counter Mapping n Register Description
#define PCTRS 0x0000001F // V3D_PCTRSn: Performance Counter Device ID READ/WRITE
 
//////////////////////////////////////////////////////////////////////
// V3D Error & Diagnostic Registers
//////////////////////////////////////////////////////////////////////
// V3D_BXCF: V3D Binner Debug Register Description
#define FWDDISA  0x00000001 // V3D_BXCF: Disable Forwarding In State Cache READ/WRITE
#define CLIPDISA 0x00000002 // V3D_BXCF: Disable Clipping READ/WRITE
 
// V3D_DBGE: V3D PSE Error Signals Register Description
#define VR1_A        0x00000002 // V3D_DBGE: Error A Reading VPM READ
#define VR1_B        0x00000004 // V3D_DBGE: Error B Reading VPM READ
#define MULIP0       0x00010000 // V3D_DBGE: Error Mulip 0 READ
#define MULIP1       0x00020000 // V3D_DBGE: Error Mulip 1 READ
#define MULIP2       0x00040000 // V3D_DBGE: Error Mulip 2 READ
#define IPD2_VALID   0x00080000 // V3D_DBGE: Error IPD2 Valid READ
#define IPD2_FPDUSED 0x00100000 // V3D_DBGE: Error IPD2 FPD Used READ
 
// V3D_FDBGO: V3D FEP Overrun Error Signals Register Description
#define WCOEFF_FIFO_FULL 0x00000002 // V3D_FDBGO: Not An Error READ
#define XYRELZ_FIFO_FULL 0x00000004 // V3D_FDBGO: Not An Error READ
#define QBFR_FIFO_ORUN   0x00000008 // V3D_FDBGO: Error READ
#define QBSZ_FIFO_ORUN   0x00000010 // V3D_FDBGO: Error READ
#define XYFO_FIFO_ORUN   0x00000020 // V3D_FDBGO: Error READ
#define FIXZ_ORUN        0x00000040 // V3D_FDBGO: Error READ
#define XYRELO_FIFO_ORUN 0x00000080 // V3D_FDBGO: Error READ
#define XYRELW_FIFO_ORUN 0x00000400 // V3D_FDBGO: Error READ
#define ZCOEFF_FIFO_FULL 0x00000800 // V3D_FDBGO: Not An Error
#define REFXY_FIFO_ORUN  0x00001000 // V3D_FDBGO: Error READ
#define DEPTHO_FIFO_ORUN 0x00002000 // V3D_FDBGO: Error READ
#define DEPTHO_ORUN      0x00004000 // V3D_FDBGO: Error READ
#define EZVAL_FIFO_ORUN  0x00008000 // V3D_FDBGO: Error READ
#define EZREQ_FIFO_ORUN  0x00020000 // V3D_FDBGO: Error READ
 
// V3D_FDBGB: V3D FEP Interface Ready & Stall Signals, FEP Busy Signals Register Description
#define EDGES_STALL        0x00000001 // V3D_FDBGB: Stall READ
#define EDGES_READY        0x00000002 // V3D_FDBGB: Ready READ
#define EDGES_ISCTRL       0x00000004 // V3D_FDBGB: READ
#define EDGES_CTRLID       0x00000038 // V3D_FDBGB: READ
#define ZRWPE_STALL        0x00000040 // V3D_FDBGB: Stall READ
#define ZRWPE_READY        0x00000080 // V3D_FDBGB: Ready READ
#define EZ_DATA_READY      0x00800000 // V3D_FDBGB: Ready READ
#define EZ_XY_READY        0x02000000 // V3D_FDBGB: Ready READ
#define RAST_BUSY          0x04000000 // V3D_FDBGB: Busy READ
#define QXYF_FIFO_OP_READY 0x08000000 // V3D_FDBGB: Ready READ
#define XYFO_FIFO_OP_READY 0x10000000 // V3D_FDBGB: Ready READ
 
// V3D_FDBGR: V3D FEP Internal Ready Signals Register Description
#define QXYF_FIFO_READY   0x00000001 // V3D_FDBGR: Ready READ
#define EZREQ_FIFO_READY  0x00000002 // V3D_FDBGR: Ready READ
#define EZVAL_FIFO_READY  0x00000004 // V3D_FDBGR: Ready READ
#define DEPTHO_FIFO_READY 0x00000008 // V3D_FDBGR: Ready READ
#define REFXY_FIFO_READY  0x00000010 // V3D_FDBGR: Ready READ
#define ZCOEFF_FIFO_READY 0x00000020 // V3D_FDBGR: Ready READ
#define XYRELW_FIFO_READY 0x00000040 // V3D_FDBGR: Ready READ
#define WCOEFF_FIFO_READY 0x00000080 // V3D_FDBGR: Ready READ
#define XYRELO_FIFO_READY 0x00000800 // V3D_FDBGR: Ready READ
#define ZO_FIFO_READY     0x00002000 // V3D_FDBGR: Ready READ
#define XYFO_FIFO_READY   0x00004000 // V3D_FDBGR: Ready READ
#define RAST_READY        0x00010000 // V3D_FDBGR: Ready READ
#define RAST_LAST         0x00020000 // V3D_FDBGR: Last READ
#define DEPTHO_READY      0x00040000 // V3D_FDBGR: Ready READ
#define EZLIM_READY       0x00080000 // V3D_FDBGR: Ready READ
#define XYNRM_READY       0x00100000 // V3D_FDBGR: Ready READ
#define XYNRM_LAST        0x00200000 // V3D_FDBGR: Last READ
#define XYRELZ_FIFO_READY 0x00400000 // V3D_FDBGR: Ready READ
#define XYRELZ_FIFO_LAST  0x00800000 // V3D_FDBGR: Last READ
#define INTERPZ_READY     0x01000000 // V3D_FDBGR: Ready READ
#define INTERPRW_READY    0x08000000 // V3D_FDBGR: Ready READ
#define RECIPW_READY      0x10000000 // V3D_FDBGR: Ready READ
#define FIXZ_READY        0x40000000 // V3D_FDBGR: Ready READ
 
// V3D_FDBGS: V3D FEP Internal Stall Input Signals Register Description
#define EZTEST_IP_QSTALL     0x00000001 // V3D_FDBGS: Stall READ
#define EZTEST_IP_PRSTALL    0x00000002 // V3D_FDBGS: Stall READ
#define EZTEST_IP_VLFSTALL   0x00000004 // V3D_FDBGS: Stall READ
#define EZTEST_STALL         0x00000008 // V3D_FDBGS: Stall READ
#define EZTEST_VLF_OKNOVALID 0x00000010 // V3D_FDBGS: Valid READ
#define EZTEST_QREADY        0x00000020 // V3D_FDBGS: Ready READ
#define EZTEST_ANYQF         0x00000040 // V3D_FDBGS: READ
#define EZTEST_ANYQVALID     0x00000080 // V3D_FDBGS: Valid READ
#define QXYF_FIFO_OP1_VALID  0x00000100 // V3D_FDBGS: Valid READ
#define QXYF_FIFO_OP1_LAST   0x00000200 // V3D_FDBGR: Last READ
#define QXYF_FIFO_OP1_DUMMY  0x00000400 // V3D_FDBGR: Dummy READ
#define QXYF_FIFO_OP_LAST    0x00000800 // V3D_FDBGR: Last READ
#define QXYF_FIFO_OP_VALID   0x00001000 // V3D_FDBGS: Valid READ
#define EZREQ_FIFO_OP_VALID  0x00002000 // V3D_FDBGS: Valid READ
#define XYNRM_IP_STALL       0x00004000 // V3D_FDBGS: Stall READ
#define EZLIM_IP_STALL       0x00008000 // V3D_FDBGS: Stall READ
#define DEPTHO_FIFO_IP_STALL 0x00010000 // V3D_FDBGS: Stall READ
#define INTERPZ_IP_STALL     0x00020000 // V3D_FDBGS: Stall READ
#define XYRELZ_FIFO_IP_STALL 0x00040000 // V3D_FDBGS: Stall READ
#define INTERPW_IP_STALL     0x00400000 // V3D_FDBGS: Stall READ
#define RECIPW_IP_STALL      0x02000000 // V3D_FDBGS: Stall READ
#define ZO_FIFO_IP_STALL     0x10000000 // V3D_FDBGS: Stall READ
 
// V3D_ERRSTAT: V3D Miscellaneous Error Signals (VPM, VDW, VCD, VCM, L2C) Register Description
#define VPAEABB  0x00000001 // V3D_ERRSTAT: VPM Allocator Error - Allocating Base While Busy READ
#define VPAERGS  0x00000002 // V3D_ERRSTAT: VPM Allocator Error - Request Too Big READ
#define VPAEBRGL 0x00000004 // V3D_ERRSTAT: VPM Allocator Error - Binner Request Greater Than Limit READ
#define VPAERRGL 0x00000008 // V3D_ERRSTAT: VPM Allocator Error - Renderer Request Greater Than Limit READ
#define VPMEWR   0x00000010 // V3D_ERRSTAT: VPM Error - Write Range READ
#define VPMERR   0x00000020 // V3D_ERRSTAT: VPM Error - Read Range READ
#define VPMERNA  0x00000040 // V3D_ERRSTAT: VPM Error - Read Non-Allocated READ
#define VPMEWNA  0x00000080 // V3D_ERRSTAT: VPM Error - Write Non-Allocated READ
#define VPMEFNA  0x00000100 // V3D_ERRSTAT: VPM Error - Free Non-Allocated READ
#define VPMEAS   0x00000200 // V3D_ERRSTAT: VPM Error - Allocated Size Error READ
#define VDWE     0x00000400 // V3D_ERRSTAT: VDW Error - Address Overflows READ
#define VCDE     0x00000800 // V3D_ERRSTAT: VCD Error - FIFO Pointers Out Of Sync READ
#define VCDI     0x00001000 // V3D_ERRSTAT: VCD Idle READ
#define VCMRE    0x00002000 // V3D_ERRSTAT: VCM Error (Renderer) READ
#define VCMBE    0x00004000 // V3D_ERRSTAT: VCM Error (Binner) READ
#define L2CARE   0x00008000 // V3D_ERRSTAT: L2C AXI Receive Fifo Overrun Error READ
 
