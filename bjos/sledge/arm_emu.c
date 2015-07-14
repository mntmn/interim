/*
 * Arm Processor Emulator
 *
 *  Created on: Nov 12, 2012
 *      Author: mdixon
 *   Edited By: omufeed
 */

// #include some headers you will be probably need
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

//Defines some types of our own. note: base these on platform independent types defined in <stdint.h>
typedef uint64_t REGISTER;		//registers are 32 bits
typedef uint64_t WORD;			//words are 32 bits
typedef int32_t SWORD;			//swords are 32 bits
typedef uint8_t BYTE;			//bytes are 8 bits
typedef uint8_t BIT;			//define bit data type

//Define the shift amount for specific bits
#define POS_CONDCODE (28)//Condition code
#define POS_INSTYPE (26)//Instruction type
#define POS_OPCODE (21)//Operation code
#define POS_I (25)//Immediate bit
#define POS_L (24)//Link bit
#define POS_S (20)//Status bit
#define POS_REGN (16)//regN
#define POS_REGDEST (12)//regDest
#define POS_ISHIFT (8)//Shift amount for immediate value
#define POS_SHIFT (4)//Shift amount for operand register
#define POS_IOPERAND (0)//Immediate value
#define POS_OPERAND (0)//Operand register
#define POS_N (31)//Negative bit

#define POS_BOFFSET (0)//Offset for branch

#define POS_P (24)//Pre/Post bit
#define POS_U (23)//Up/Down bit
#define POS_B (22)//Byte/Word bit
#define POS_W (21)//Writeback into base register bit
#define POS_LS (20)//Load/Store bit

#define POS_SWICODE (0)//Software interrupt code

//Define mask amount size
#define MASK_1BIT (0x1)
#define MASK_2BIT (0x3)
#define MASK_3BIT (0x7)
#define MASK_4BIT (0xf)
#define MASK_5BIT (0x1f)
#define MASK_6BIT (0x3f)
#define MASK_7BIT (0x7f)
#define MASK_8BIT (0xff)
#define MASK_24BIT (0xffffff)

//Define each condition code.
#define CC_EQ	(0x0)
#define CC_NE	(0x1)
#define CC_CS	(0x2)
#define CC_CC	(0x3)
#define CC_MI	(0x4)
#define CC_PL	(0x5)
#define CC_VS	(0x6)
#define CC_VC	(0x7)
#define CC_HI	(0x8)
#define CC_LS	(0x9)
#define CC_GE	(0xa)
#define CC_LT	(0xb)
#define CC_GT	(0xc)
#define CC_LE	(0xd)
#define CC_AL	(0xe)
#define CC_NV	(0xf)

// Define each instruction type code.
#define IT_DP	(0x0)
#define IT_DT	(0x1)
#define IT_BR	(0x2)
#define IT_SI	(0x3)

// Define each status flag
#define STAT_N	(1<<7)//Negative
#define STAT_Z	(1<<6)//Zero
#define STAT_C	(1<<5)//Carry
#define STAT_V	(1<<4)//Overflow
#define STAT_I	(1<<3)//Interrupt disabled
#define STAT_F	(1<<2)//Fast interrupt disabled
#define STAT_P	(3)//Processor operation mode, the last two bits. 00: User mode

// Define each op code.
#define OP_AND	(0x0)
#define OP_EOR	(0x1)
#define OP_SUB	(0x2)
#define OP_RSB	(0x3)
#define OP_ADD	(0x4)
#define OP_ADC	(0x5)
#define OP_SBC	(0x6)
#define OP_RSC	(0x7)
#define OP_TST	(0x8)
#define OP_TEQ	(0x9)
#define OP_CMP	(0xa)
#define OP_CMN	(0xb)
#define OP_ORR	(0xc)
#define OP_MOV	(0xd)
#define OP_BIC	(0xe)
#define OP_MVN	(0xf)

//custom SWI
#define SYS_ADD10		(0x01)//Add 10 to R0

#define SYS_POS_ADD10	(0x01)//Add 10 to R0

//Define instruction handling result type, include further error types
#define INST_VALID (1<<1)
#define INST_CONDITIONNOTMET (1<<2)
#define INST_INVALID (1<<3)
#define INST_MULTIPLYBYZERO (1<<4)
#define INST_RESTRICTEDMEMORYBLOCK (1<<5)

int maxMemorySize = 4096;//Temporarily set the memory size to 140 to better monitor the data in memory during the output, increase this value as required.
int userMemoryIndex = 120;//from 120 to maxMemorySize can be used by user to load/store data
int stkMemoryIndex = 100;//next 20 are reserved for stack
int instMemoryIndex = 52;//next 48 are reserved for user instructions. No access for user load/store data
int swiMemoryIndex = 12;//next 40 are reserved for interrupt instructions. No access for user load/store data
int swibMemoryIndex = 0;//First 12 rows are the interrupts branch instruction. No access for user load/store data

//storing registers within array makes execution code simpler
REGISTER registers[16];
//4 kilobytes of RAM in this example (since 1 word = 4 bytes)
WORD* memory = (WORD)0;
//the variable to fetch the instruction into
WORD inst;

BYTE regStatus = 0;//status register

int totalCycleCount = 0, instCycleCount = 0;

BYTE instResult = INST_VALID;//Instruction result handler

//Variables for data processing instructions
BIT iBit = 0, sBit = 0, lBit = 0, pBit = 0, uBit = 0, bBit = 0, wBit = 0, lsBit = 0;
BYTE opCode, regN, regDest, shiftAmount;
WORD operand, offset, swiCode;

//Turn on/off the debug mode; prints assisting data about the operation steps
int debugEnabled = 1;

//Turn on/off the instruction log; prints the type and input of the instruction
int instructionLogEnabled = 1;

//Listens to the keyboard input and is turned on if space is clicked. If it is on, an interrupt is fired
int feedback = 0;

//Get the negative value, use two's complement for negation
WORD getNegative(WORD value){
	return ~value + 1;
}

//Check if a specific value is negative, use two's complement for negation
int isNegative(WORD value){
	return (((value >> POS_N) & MASK_1BIT));
}

//Check if a specific value is positive
int isPositive(WORD value){
	return !(((value >> POS_N) & MASK_1BIT));
}

//Check if a specific 24 bit value is negative, use two's complement for negation. 24bit negative value is used for offset
int isNegative24bit(WORD value){
	return (((value >> (23)) & MASK_1BIT));
}

//Get the negative value, use two's complement for negation
WORD getNegative24bit(WORD value){
	return (~value & MASK_24BIT) + 1;
}

//The carry flag is set if the addition of two numbers causes a carry out of the most significant (leftmost) bits added.
int isCarryAdd(WORD op1Value, WORD op2Value, WORD result)
{
	return ((isNegative(op1Value) && isNegative(op2Value)) || (isNegative(op1Value) && isPositive(result)) || (isNegative(op2Value) && isPositive(result)));
}

//The carry (borrow) flag is also set if the subtraction of two numbers requires a borrow into the most significant (leftmost) bits subtracted.
int isCarrySub(WORD op1Value, WORD op2Value, WORD result){
	return ((isNegative(op1Value) && isPositive(op2Value)) || (isNegative(op1Value) && isPositive(result)) || (isPositive(op2Value) && isPositive(result)));
}

//If the sum of two numbers with the sign bits off yields a result number with the sign bit on, the "overflow" flag is turned on.
int isOverflowAdd(WORD op1Value, WORD op2Value, WORD result)
{
	return ((isNegative(op1Value) && isNegative(op2Value) && isPositive(result)) || (isPositive(op1Value) && isPositive(op2Value) && isNegative(result)));
}

//If the sum of two numbers with the sign bits on yields a result number with the sign bit off, the "overflow" flag is turned on.
int isOverflowSub(WORD op1Value, WORD op2Value, WORD result){
	return ((isNegative(op1Value) && isPositive(op2Value) && isPositive(result)) || (isPositive(op1Value) && isNegative(op2Value) && isNegative(result)));
}

//Provide some support functions to test condition of a specified SR flag
int isSet(int flag)
{
	return (regStatus & flag) == flag;
}

int isClear(int flag)
{
	return ((~regStatus) & flag) == flag;
}

void setFlag(int flag)
{
	regStatus = regStatus | flag;
}

void clearFlag(int flag)
{
	regStatus = regStatus & (~flag);
}

//set the SR flags by examining the result. The previous value of flags get initiated as well.
void setStatusReg(int isZ, int isN, int isC, int isV, int isI, int isF, int isP){
	//Set or clear the Zero, Negative, Carry, Overflow, Interrupt disabled and Fast interrupt disabled flags
	isN ? setFlag(STAT_N) : clearFlag(STAT_N);
	isZ ? setFlag(STAT_Z) : clearFlag(STAT_Z);
	isC ? setFlag(STAT_C) : clearFlag(STAT_C);
	isV ? setFlag(STAT_V) : clearFlag(STAT_V);
	isI ? setFlag(STAT_I) : clearFlag(STAT_I);
	isF ? setFlag(STAT_F) : clearFlag(STAT_F);

	//Check the processor operation mode and set the flags
}

// Write a function for each supported operation
void doAnd(int regNumber, WORD op1Value, WORD op2Value, int setSR)
{
	if (instructionLogEnabled) printf("Operation: AND %d %d\n", op1Value, op2Value);

	WORD result = op1Value & op2Value;

	registers[regNumber] = result;

	if (setSR) setStatusReg(result == 0, isNegative(result), 0, 0, 0, 0, 0);
}

void doOrr(int regNumber, WORD op1Value, WORD op2Value, int setSR)
{
	if (instructionLogEnabled) printf("Operation: ORR %d %d\n", op1Value, op2Value);

	WORD result = op1Value | op2Value;

	registers[regNumber] = result;

	if (setSR) setStatusReg(result == 0, isNegative(result), 0, 0, 0, 0, 0);
}

void doEor(int regNumber, WORD op1Value, WORD op2Value, int setSR)
{
	if (instructionLogEnabled) printf("Operation: EOR %d %d\n", op1Value, op2Value);

	WORD result = op1Value ^ op2Value;

	registers[regNumber] = result;

	if (setSR) setStatusReg(result == 0, isNegative(result), 0, 0, 0, 0, 0);
}

/*doAdd
 * Adds op1Value to op2Value and stores in the specified register
 *
 * enter: 	regNumber - the number of the destination register (0-15)
 * 			op1Value  - the first value to be added
 * 			op2Value  -  the second value to be added
 * 			setSR     - flag, if non-zero then the status register should be updated, else it shouldn't
 */
void doAdd(int regNumber, WORD op1Value, WORD op2Value, int setSR)
{
	if (instructionLogEnabled) printf("Operation: ADD %d %d\n", op1Value, op2Value);
	WORD result = op1Value + op2Value;

	registers[regNumber] = result;

	if (setSR) setStatusReg(result == 0, isNegative(result), isCarryAdd(op1Value, op2Value, result), isOverflowAdd(op1Value, op2Value, result), 0, 0, 0);
}

void doAdc(int regNumber, WORD op1Value, WORD op2Value, int setSR)
{
	if (instructionLogEnabled) printf("Operation: ADD %d %d\n", op1Value, op2Value);
	WORD result = op1Value + op2Value + isSet(STAT_C);

	registers[regNumber] = result;

	if (setSR) setStatusReg(result == 0, isNegative(result), isCarryAdd(op1Value, op2Value, result), isOverflowAdd(op1Value, op2Value, result), 0, 0, 0);
}

void doSub(int regNumber, WORD op1Value, WORD op2Value, int setSR)
{
	if (instructionLogEnabled) printf("Operation: SUB %d %d\n", op1Value, op2Value);

	WORD result = op1Value - op2Value;

	registers[regNumber] = result;

	if (setSR) setStatusReg(result == 0, isNegative(result), isCarrySub(op1Value, op2Value, result), isOverflowSub(op1Value, op2Value, result), 0, 0, 0);
}

void doSbc(int regNumber, WORD op1Value, WORD op2Value, int setSR)
{
	if (instructionLogEnabled) printf("Operation: SUB %d %d\n", op1Value, op2Value);

	WORD result = op1Value - op2Value - isClear(STAT_C);

	registers[regNumber] = result;

	if (setSR) setStatusReg(result == 0, isNegative(result), isCarrySub(op1Value, op2Value, result), isOverflowSub(op1Value, op2Value, result), 0, 0, 0);
}

void doRsb(int regNumber, WORD op1Value, WORD op2Value, int setSR)
{
	if (instructionLogEnabled) printf("Operation: RSB %d %d\n", op1Value, op2Value);

	WORD result = op2Value - op1Value;

	registers[regNumber] = result;

	if (setSR) setStatusReg(result == 0, isNegative(result), isCarrySub(op2Value, op1Value, result), isOverflowSub(op2Value, op1Value, result), 0, 0, 0);
}

void doRsc(int regNumber, WORD op1Value, WORD op2Value, int setSR)
{
	if (instructionLogEnabled) printf("Operation: RSB %d %d\n", op1Value, op2Value);

	WORD result = op2Value - op1Value - isClear(STAT_C);

	registers[regNumber] = result;

	if (setSR) setStatusReg(result == 0, isNegative(result), isCarrySub(op2Value, op1Value, result), isOverflowSub(op2Value, op1Value, result), 0, 0, 0);
}

void doMov(int regNumber, WORD opValue, int setSR)
{
	if (instructionLogEnabled) printf("Operation: MOV R%d %d\n", regNumber, opValue);

	registers[regNumber] = opValue;

	if (setSR) setStatusReg(opValue == 0, isNegative(opValue), 0, 0, 0, 0, 0);
}

void doMvn(int regNumber, WORD opValue, int setSR)
{
	if (instructionLogEnabled) printf("Operation: MVN R%d %d\n", regNumber, opValue);

	registers[regNumber] = ~opValue;

	if (setSR) setStatusReg(~opValue == 0, isNegative(~opValue), 0, 0, 0, 0, 0);
}

//The processor always assumes the status flag is set, either it is actually set or not
void doCmp(WORD op1Value, WORD op2Value)
{
	if (instructionLogEnabled) printf("Operation: CMP %d %d\n", op1Value, op2Value);

	WORD result = op1Value - op2Value;
	setStatusReg(result == 0, isNegative(result), isCarrySub(op1Value, op2Value, result), isOverflowSub(op1Value, op2Value, result), 0, 0, 0);
}

//The processor always assumes the status flag is set, either it is actually set or not
void doCmn(WORD op1Value, WORD op2Value)
{
	if (instructionLogEnabled) printf("Operation: CMN %d %d\n", op1Value, op2Value);

	WORD result = op1Value + op2Value;

	setStatusReg(result == 0, isNegative(result), isCarryAdd(op1Value, op2Value, result), isOverflowAdd(op1Value, op2Value, result), 0, 0, 0);
}

//The processor always assumes the status flag is set, either it is actually set or not
void doTst(WORD op1Value, WORD op2Value)
{
	if (instructionLogEnabled) printf("Operation: TST %d %d\n", op1Value, op2Value);

	setStatusReg((op1Value & op2Value) == 0, isNegative(op1Value & op2Value), 0, 0, 0, 0, 0);
}

//The processor always assumes the status flag is set, either it is actually set or not
void doTeq(WORD op1Value, WORD op2Value)
{
	if (instructionLogEnabled) printf("Operation: TEQ %d %d\n", op1Value, op2Value);

	setStatusReg((op1Value ^ op2Value) == 0, isNegative(op1Value ^ op2Value), 0, 0, 0, 0, 0);
}

void doBic(int regNumber, WORD op1Value, WORD op2Value, int setSR)
{
	if (instructionLogEnabled) printf("Operation: BIC %d %d\n", op1Value, op2Value);

	WORD result = op1Value & ~op2Value;

	registers[regNumber] = result;

	if (setSR) setStatusReg(result == 0, isNegative(result), 0, 0, 0, 0, 0);
}

//Store a value of register regNumber into stack and increase the stack pointer
void stkPush(int regNumber){
	memory[registers[13]++] = registers[regNumber];
}

//Load a latest from stack into the register regNumber and decrease the stack pointer
void stkPop(int regNumber){
	registers[regNumber] = memory[registers[13]--];
}

//Decide whether the instruction should be executed by examining the appropriate status register flags, depending on condition code.
int getConditionCode(WORD inst){
	int exec = 0;
	int conditionCode;
	// Start by extracting the condition code
	conditionCode = inst >> POS_CONDCODE;	// get the condition flags (top 4 bits of the instruction).

	switch ( conditionCode ) {
		case CC_EQ:
			// check if zero flag is set
			exec = isSet(STAT_Z);
			break;
		case CC_NE:
			// check if zero flag is clear
			exec = isClear(STAT_Z);
			break;
		case CC_CS:
			exec = isSet(STAT_C);
			break;
		case CC_CC:
			exec = isClear(STAT_C);
			break;
		case CC_MI:
			exec = isSet(STAT_N);
			break;
		case CC_PL:
			exec = isClear(STAT_N);
			break;
		case CC_VS:
			exec = isSet(STAT_V);
			break;
		case CC_VC:
			exec = isClear(STAT_V);
			break;
		case CC_HI:
			//If C is set and Z is clear, means > for unsigned
			exec = isSet(STAT_C) && isClear(STAT_Z);
			break;
		case CC_LS:
			//<= unsigned
			exec = isClear(STAT_C) && isSet(STAT_Z);
			break;
		case CC_GE:
			//>= signed
			exec = (isClear(STAT_N) && isClear(STAT_V)) || (isSet(STAT_N) && isSet(STAT_V));
			break;
		case CC_LT:
			// < signed
			exec = ( isSet( STAT_N ) && isClear( STAT_V ) ) || ( isClear(STAT_N) && isSet(STAT_V) ) ;
			break;
		case CC_GT:
			// > signed
			exec = isClear(STAT_Z) && ((isClear(STAT_N) && isClear(STAT_V)) || (isSet(STAT_N) && isSet(STAT_V)));
			break;
		case CC_LE:
			// <= signed
			exec = isSet(STAT_Z) || ( isSet( STAT_N ) && isClear( STAT_V ) ) || ( isClear(STAT_N) && isSet(STAT_V) ) ;
			break;
		case CC_AL:
			exec = 1;
			break;
		case CC_NV:
			exec = 0;
			break;
	}
	return exec;
}

//Do the data processing type operations
void doDataProcessing(WORD inst){
	iBit = (inst >> POS_I) & MASK_1BIT;//Get Immediate bit
	opCode = (inst >> POS_OPCODE) & MASK_4BIT;//Get Opcode, 4 bits
	sBit = (inst >> POS_S) & MASK_1BIT;//Get Status bit
	regN = (inst >> POS_REGN) & MASK_4BIT;//Get RegN, 4 bits
	regDest = (inst >> POS_REGDEST) & MASK_4BIT;//Get RegDest, 4 bits

	//Get the operand depending to the Immediate code
	if(iBit){
		shiftAmount = (inst >> POS_ISHIFT) & MASK_4BIT;//Immediate shift is 4 bits
		operand = (inst >> POS_IOPERAND) & MASK_8BIT;//Immediate value is 8 bits
	}
	else{
		shiftAmount = (inst >> POS_SHIFT) & MASK_8BIT;//Non-immediate shift is 8 bits
		operand = registers[(inst >> POS_OPERAND) & MASK_4BIT];//Non-immediate register number is 4 bits
	}

	//Apply the shift amount to the operand
	operand = operand << shiftAmount;

	//Print the decoded instruction if debug is enabled
	if (debugEnabled){
		printf("Immediate bit: %01X\n", iBit);
		printf("Op code: %01X\n", opCode);
		printf("Status bit: %01X\n", sBit);
		printf("RegN code: %01X\n", regN);
		printf("RegDest code: %01X\n", regDest);
		printf("operand : %d\n", operand);
	}

	switch(opCode)
	{
		case OP_AND:
			doAnd(regDest, registers[regN], operand, sBit);
			break;
		case OP_EOR:
			doEor(regDest, registers[regN], operand, sBit);
			break;
		case OP_SUB:
			doSub(regDest, registers[regN], operand, sBit);
			break;
		case OP_RSB:
			doRsb(regDest, registers[regN], operand, sBit);
			break;
		case OP_ADD:
			doAdd(regDest, registers[regN], operand, sBit);
			break;
		case OP_ADC:
			doAdc(regDest, registers[regN], operand, sBit);
			break;
		case OP_SBC:
			doSbc(regDest, registers[regN], operand, sBit);
			break;
		case OP_RSC:
			doRsc(regDest, registers[regN], operand, sBit);
			break;
		case OP_TST:
			doTst(registers[regN], operand);
			break;
		case OP_TEQ:
			doTeq(registers[regN], operand);
			break;
		case OP_CMP:
			doCmp(registers[regN], operand);
			break;
		case OP_CMN:
			doCmn(registers[regN], operand);
			break;
		case OP_ORR:
			doOrr(regDest, registers[regN], operand, sBit);
			break;
		case OP_MOV:
			doMov(regDest, operand, sBit);
			break;
		case OP_BIC:
			doBic(regDest, registers[regN], operand, sBit);
			break;
		case OP_MVN:
			doMvn(regDest, operand, sBit);
			break;
		default:
			instResult = INST_INVALID;
			break;
	}

	//Update the cycle count. If regDest is PC the +2, otherwise 1 cycle.
	instCycleCount = (regDest == 15) ? 3 : 1;
}

//Do the branch type operations
void doBranch(WORD inst){
	lBit = (inst >> POS_L) & MASK_1BIT;//Get Link bit
	offset = (inst >> POS_BOFFSET) & MASK_24BIT;//Get the offset, 24 bits

	if (instructionLogEnabled) printf("Operation: B %d\n", offset);

	if(lBit){
		//Check if there is already a link value in R14. Push to the stack if there is before overwriting it
		if (registers[14] > 0) stkPush(14);

		//Store the PC in link(R14)
		registers[14] = registers[15];
	}

	//Check if the 24bit offset value is negative? if yes then subtract the positive value of the offset, otherwise add the offset value
	if(isNegative24bit(offset)){
		registers[15] -= getNegative24bit(offset) / 4;
	}else{
		registers[15] += offset / 4;
	}

	registers[15] += 4;//Add 4 to the PC, because of the pipeline

	//Update the cycle count
	instCycleCount = 3;
}

//Do the data transfer operation
void doDataTransfer(WORD inst){

	iBit = (inst >> POS_I) & MASK_1BIT;//Get Immediate bit
	pBit = (inst >> POS_P) & MASK_1BIT;//Get Pre/Post bit
	uBit = (inst >> POS_U) & MASK_1BIT;//Get Up/Down bit
	bBit = (inst >> POS_B) & MASK_1BIT;//Get Byte/Word bit
	wBit = (inst >> POS_W) & MASK_1BIT;//Get Writeback to base register bit
	lsBit = (inst >> POS_LS) & MASK_1BIT;//Get Load/Store bit
	regN = (inst >> POS_REGN) & MASK_4BIT;//Get RegN, 4 bits
	regDest = (inst >> POS_REGDEST) & MASK_4BIT;//Get RegDest, 4 bits

	//Get the operand depending to the Immediate code
	if(iBit){
		shiftAmount = (inst >> POS_ISHIFT) & MASK_4BIT;//Immediate shift is 4 bits
		offset = (inst >> POS_IOPERAND) & MASK_8BIT;//Immediate value is 8 bits

		if (debugEnabled) printf("Shift amount code: %01X\n", shiftAmount);
	}
	else{
		shiftAmount = (inst >> POS_SHIFT) & MASK_8BIT;//Non-immediate shift is 8 bits
		offset = registers[(inst >> POS_OPERAND) & MASK_4BIT];//Non-immediate register number is 4 bits

		if (debugEnabled) printf("Shift amount code: %02X\n", shiftAmount);
	}

	if (instructionLogEnabled){
		printf("Operation: ");
		printf(lsBit ? "LDR" : "STR");
		printf(" R%d R%d, %d\n", regDest, regN, offset);
	}

	//Print the decoded instruction if debug is enabled
	if (debugEnabled){
		printf("Immediate bit: %01X\n", iBit);
		printf("Pre/Post bit: %01X\n", pBit);
		printf("Up/Down bit: %01X\n", uBit);
		printf("Byte/Word bit: %01X\n", bBit);
		printf("Writeback to base register bit: %01X\n", wBit);
		printf("Load/Store bit: %01X\n", lsBit);
		printf("RegN code: %01X\n", regN);
		printf("RegDest code: %01X\n", regDest);
		printf("offset : %d\n", offset);
	}

	//Set the initial memory index
	int memoryIndex = registers[regN];

	//If the pre bit is set, add/subtract offset to the base register before the transfer
	if(pBit) memoryIndex = (uBit) ? memoryIndex + offset :  memoryIndex - offset;

	//Check if the instruction is trying to access the restricted memory blocks
	if ((memoryIndex >= userMemoryIndex) && (memoryIndex <= maxMemorySize)){
		//If load/store bit is set load, if not store
		if(lsBit) registers[regDest] = (wBit) ? memory[memoryIndex] : memory[memoryIndex] & MASK_4BIT;//If the word/byte bit is set, load the least significant byte from the noted memory address
		else memory[memoryIndex] = (wBit) ? registers[regDest] : registers[regDest] & MASK_4BIT;//If the word/byte bit is set, store the least significant byte from regDest

		//If the pre bit is not set, add/subtract offset to the base register after the transfer
		if(!pBit) memoryIndex = (uBit) ? memoryIndex + offset :  memoryIndex - offset;

		//Set the base register value with the new index if the writeback is set
		if(wBit) registers[regN] = memoryIndex;

		//Update the cycle count. If it is LDR 3 cycles and +2 if regDest is PC. If it is STR then 2 cycles.
		instCycleCount = (lsBit) ? (regDest == 15) ? 5 : 3 : 2;
	}
	else{
		instResult = INST_RESTRICTEDMEMORYBLOCK;
	}
}

//Handle the software interrupt
void doSoftwareInterrupt(WORD inst){

	swiCode = (inst >> POS_SWICODE) & MASK_24BIT;//Interrupt code value is 24 bits

	if (instructionLogEnabled) printf("Interrup: SWI %d\n", swiCode);

	registers[14] = registers[15];//Backup the PC from return from interrupt

	registers[15] = swiCode + 4;//Jump to the branch address in vector table +4 for pipeline

	//Update the cycle count
	instCycleCount = 5;
}

// The main work will happen inside this function. It should decode then execute the passed instruction.
void decodeAndExecute(WORD inst){
	int insTypeCode;
	int exec;

	if (debugEnabled) printf("Decoding: %04X\n",inst);	// output the instruction being decoded

	exec = getConditionCode(inst);

	// only decode and execute the instruction if required to do so, i.e. condition satisfied.
	if ( exec ) {

		// next stage is to decode which type of instruction we have, so want to look at bits 27, 26
		insTypeCode = (inst >> POS_INSTYPE) & MASK_2BIT;

		if (debugEnabled) printf("Ins type code: %01X\n", insTypeCode);

		// once we know the instruction we can execute it!
		switch(insTypeCode){
			case IT_DP:
				doDataProcessing(inst);
				break;
			case IT_DT:
				doDataTransfer(inst);
				break;
			case IT_BR:
				doBranch(inst);
				break;
			case IT_SI:
				doSoftwareInterrupt(inst);
				break;
			default:
				instResult = INST_INVALID;
				break;
		}
	}
	else{
		instResult = INST_CONDITIONNOTMET;
	}
}

//Add delay and get feedback either to interrupt, exit or continue to next instruction
void getFeedback(){
	printf("Please enter (i)Interrupt, (x)Exit, (g)Continue: ");
	feedback = getchar();
	//Another getchar() to absorb the enter pressed after the value entered
	getchar();

	//if entered char = i(105) then set interrupt
	if ((feedback == 105)){
		//If hardware interrupt has happened, then execute the sample interrupt
		doSoftwareInterrupt(0xEF000001);
	}else if(feedback == 120){//if feedback is x(120)
		memory[(registers[15] - 4)] = 0;//Set next instruction as 0, so the program will eixt
	}
}

// Print the result of the registers, memory and the other required optputs
void printOutput(){
	if (instResult == INST_VALID){
		//Print register values
		printf("Registers\n");
		int i;
		for(i = 0; i<16; i++){
			printf("-R%d: %d ", i, registers[i]);
		}
		printf("\n");

		//Print only the user accessible memory values
		printf("Memory\n");
		for(i = userMemoryIndex; i < maxMemorySize; i++){
			printf("-M[%d]: %d ", i, memory[i]);
		}
		printf("\n");

		//Print the stack
		printf("Stack\n");
		for(i = stkMemoryIndex; i < userMemoryIndex; i++){
			printf("-S[%d]: %d ", i, memory[i]);
		}
		printf("\n");

		if (debugEnabled) printf("-Status Register: %X ", regStatus);//Print status register
		//Print status flags
		printf("Flags\n");
		printf("-Negative: %d ", isSet(STAT_N));
		printf("-Zero: %d ", isSet(STAT_Z));
		printf("-Carry: %d ", isSet(STAT_C));
		printf("-Overflow: %d ", isSet(STAT_V));
		printf("-Interrupt disabled: %d ", isSet(STAT_I));
		printf("-Fast interrupt disabled: %d ", isSet(STAT_F));
		printf("-Processor operation mode: %d ", isSet(STAT_P));
		printf("\n");

	} else{
		switch(instResult){
			case INST_CONDITIONNOTMET:
				printf("The condition is not met.\n");
				break;
			case INST_INVALID:
				printf("The instruction is invalid.\n");
				break;
			case INST_MULTIPLYBYZERO:
				printf("The instruction causes multiply by zero.\n");
				break;
			case INST_RESTRICTEDMEMORYBLOCK:
				printf("Restricted memory block. End users have access to memory indexes greater then %d\n", (userMemoryIndex - 1) );
				break;
			default:
				printf("Unexpected error happened.\n");
				break;
		}
	}
}

int arm_emulate(uint64_t* binary) {
  int isFinished = 0;
	uint64_t* memoryIndex = (WORD)binary;//Initialise the memory index for the instructions block
	int instCount = 0;
  
  int i = 0;
	for(i = 0; i < 16; i++) {
		registers[i] = 0;
	}
  registers[15] = binary+4;

  printf("r15: %p\n",registers[15]);
  printf("r15-4: %p\n",(WORD*)registers[15] - 4);

  while(!isFinished)
	{
		//After each instruction is completed, get user feedback
		getFeedback();

		// fetch the next instruction. Subtract 4 from the PC, because of the pipeline PC points to 4 instructions forward
    
		inst = *((WORD*)registers[15] - 4) & 0xffffffff;

		//increment the PC(register[15]) to point at the next instruction
		registers[15]++;

		//Initialise the instruction result values
		instResult = INST_VALID;

		//Initialise the instruction cycle count to 1, because it will take at least 1 cycle even if the condition is not met
		instCycleCount = 1;

		if ( inst != 0 ) {
			printf("---------- Instruction %d ----------\n", instCount);
			decodeAndExecute(inst);
			printOutput();

			//Update total cycle count
			totalCycleCount += instCycleCount;

			printf("---------- %d Cycles, end of instruction %d----------\n", instCycleCount, instCount);

			//Increase the amount of instructions executed
			instCount++;
		}
		else {
			// found an instruction with the value 0
			// finish once a 0 hit. Not correct behaviour but good for testing
			isFinished = 1;

			//Total instruction count doesn't include the finish instruction
			printf("========== %d Total cycles for %d instructions ==========\n", totalCycleCount, instCount);
			printf("Press enter to exit...\n");
			getchar();
		}
	}
	// all ok
	return 0;
}

int arm_emu_main(void)
{
	int isFinished = 0;
	int memoryIndex = instMemoryIndex;//Initialise the memory index for the instructions block
	int instCount = 0;

	//Technically the register values would be 0 anyway, yet this is a double check to make sure they are initialised
	int i = 0;
	for(i = 0; i < 16; i++){
		registers[i] = 0;
	}

	//initialise the stack pointer to the initial index of stack memory
	registers[13] = stkMemoryIndex;

	//initialise the PC to start from the instruction memory index. Add 4 for pipeline
	registers[15] = instMemoryIndex + 4;

	/*Interrupt branch instructions*/
	//B #28 		1110 1010 0000 0000 0000 0000 0001 1100 //branch to #13: Add 10 to R0. Jump to 11-4 next memory location, since PC is already showing the next instruction and another 4 because of pipeline
	memory[swibMemoryIndex + SYS_ADD10] = 0xEA00001C;
	//End Interrupt branch instructions

	/*Interrupt instructions*/
	//ADD R0,R0,#10 1110 0010 1000 0000 0000 0000 0000 1010 //Add 10 to R0
	memory[swiMemoryIndex + SYS_POS_ADD10] = 0xE280000A;
	//MOV R15,R14	1110 0001 1010 0000 1111 0000 0000 1110 //branch back to where interrupt was called
	memory[swiMemoryIndex + SYS_POS_ADD10 + 1] = 0xE1A0F00E;
	//End Interrupt instructions

	//for testing purposes put some data into the memory. This data represents the instructions to be executed.

	/*Math instructions
	//MOV r0,#9    		1110 0011 1010 0000 0000 0000 0000 1001
	memory[memoryIndex++] = 0xE3A00009;
	//MOV r1,#8			1110 0011 1010 0000 0001 0000 0000 1000
	memory[memoryIndex++] = 0xE3A01008;
	//ADD r2,r0,r1		1110 0000 1000 0000 0010 0000 0000 0001
	memory[memoryIndex++] = 0xE0802001;
	//ADD r3,r0,#5		1110 0010 1000 0000 0011 0000 0000 0101
	memory[memoryIndex++] = 0xE2803005;
	//SUB r4,r2,#5		1110 0010 0100 0010 0100 0000 0000 0101
	memory[memoryIndex++] = 0xE2424005;
	//RSBS r5,r0,#5		1110 0010 0111 0000 0101 0000 0000 0101
	memory[memoryIndex++] = 0xE2705005;
	//ADDS r6,r0,#205	1110 0010 1001 0000 0110 0000 1100 1101
	memory[memoryIndex++] = 0xE29060CD;
	//BICS r7,r0,#1		1110 0011 1101 0000 0111 0000 0000 0001
	memory[memoryIndex++] = 0xE3D07001;*/
	//End Math instructions

	/*Logic instructions
	//MOVS r0,#9    	1110 0011 1011 0000 0000 0000 0000 1001
	memory[memoryIndex++] = 0xE3B00009;
	//MOVS r1,#12		1110 0011 1011 0000 0001 0000 0000 1100
	memory[memoryIndex++] = 0xE3B0100C;
	//MVNS r5,#2			1110 0011 1111 0000 0101 0000 0000 0010
	//memory[memoryIndex++] = 0xE3F05002;
	//AND r2,r0,r1		1110 0000 0000 0000 0010 0000 0000 0001
	//memory[memoryIndex++] = 0xE0002001;
	//ORR r3,r0,r1		1110 0001 1000 0000 0011 0000 0000 0001
	//memory[memoryIndex++] = 0xE1803001;
	//EOR r4,r0,r1		1110 0000 0010 0000 0100 0000 0000 0001
	//memory[memoryIndex++] = 0xE0204001;
	//CMP r2,#9			1110 0011 0101 0010 0000 0000 0000 1001
	//memory[memoryIndex++] = 0xE3520009;
	//CMN r5,#1			1110 0011 0111 0101 0000 0000 0000 0001
	//memory[memoryIndex++] = 0xE3750001;
	//TST r1,#0			1110 0011 0001 0001 0000 0000 0000 0000
	memory[memoryIndex++] = 0xE3110000;
	//TEQ r1,#12			1110 0011 0011 0001 0000 0000 0000 1100
	memory[memoryIndex++] = 0xE331000C;
	//End Logic instructions*/

	/*Loop instructions*/
	//MOV R4,#100 		1110 0011 1010 0000 0100 0000 0110 0100
	memory[memoryIndex++] = 0xE3A04064;
	//MOV R3,#0    		1110 0011 1010 0000 0011 0000 0000 0000
	memory[memoryIndex++] = 0xE3A03000;
	//SUB R4,R4,R3		1110 0000 0100 0100 0100 0000 0000 0011 //.strR4100
	memory[memoryIndex++] = 0xE0444003;
	//ADD R3,R3,#1		1110 0010 1000 0011 0011 0000 0000 0001
	memory[memoryIndex++] = 0xE2833001;
	//CMP R3,#100		1110 0011 0101 0011 0000 0000 0110 0100
	memory[memoryIndex++] = 0xE3530064;
	//BNE #-32			0001 1010 1111 1111 1111 1111 1110 0000 //return to .strR4100 if loop not completed. 4+4 instructions back, because of pipeline
	memory[memoryIndex++] = 0x1AFFFFE0;
	//End Loop instructions

	/*Data transfer instructions
	//MOV R0,#10 		1110 0011 1010 0000 0000 0000 0000 1010 //Counter for loop = 10
	memory[memoryIndex++] = 0xE3A0000A;
	//MOV R4,#100 		1110 0011 1010 0000 0100 0000 0110 0100
	memory[memoryIndex++] = 0xE3A04064;
	//MOV R3,#120    		1110 0011 1010 0000 0011 0000 0111 1000
	memory[memoryIndex++] = 0xE3A03078;
	//STR R4,R3,#1!   1110 0110 1110 0011 0100 0000 0000 0001 //Post bit set .strR4100
	memory[memoryIndex++] = 0xE6E34001;
	//SUBS R0,R0,#1		1110 0010 0101 0000 0000 0000 0000 0001 //Start loop counter
	memory[memoryIndex++] = 0xE2500001;
	//BNE #-28			0001 1010 1111 1111 1111 1111 1110 0100 //return to .strR4100 if loop not completed. 3+4 instructions back, because of pipeline
	memory[memoryIndex++] = 0x1AFFFFE4;
	//End Data transfer instructions*/

	/*Software Interrupt instructions
	//SWI #1			1110 1111 0000 0000 0000 0000 0000 0001 //SWI to Add 10 to R0
	memory[memoryIndex++] = 0xEF000001;
	//SUBS r0,r0,#10	1110 0010 0101 0000 0000 0000 0000 1010
	memory[memoryIndex++] = 0xE250000A;
	//End Software Interrupt instructions*/

	/*Overflow and Carry instructions
	//MOV r0,#9    						1110 0011 1010 0000 0000 0000 0000 1001
	//memory[memoryIndex++] = 0xE3A00009;
	//MOV r1,#255,LSL #15				1110 0011 1010 0000 0001 1111 1111 1111//r1 = 8355840 		00000000011111111000000000000000
	memory[memoryIndex++] = 0xE3A01FFF;
	//MOV r1,r1,LSL #8					1110 0001 1010 0000 0001 0000 1000 0001//r1 = 2139095040	01111111100000000000000000000000
	memory[memoryIndex++] = 0xE1A01081;
	//ADDS r2,r1,r1						1110 0000 1001 0001 0010 0000 0000 0001//r2 = 4278190080	11111111000000000000000000000000 //causes overflow
	memory[memoryIndex++] = 0xE0912001;
	//MOV r3,r1,LSL #1					1110 0001 1010 0000 0011 0000 0001 0001//r3 = 4278190080	11111111000000000000000000000000
	memory[memoryIndex++] = 0xE1A03011;
	//ADDS r4,r3,r1						1110 0000 1001 0011 0100 0000 0000 0001//r4 = 6417285120 (1)01111110100000000000000000000000 //causes carry
	memory[memoryIndex++] = 0xE0934001;*/
	//End Overflow and Carry instructions

	//at the moment this will terminate the main loop (see comments below)
	memory[memoryIndex++] = 0;

	while(!isFinished)
	{
		//After each instruction is completed, get user feedback
		getFeedback();

		// fetch the next instruction. Subtract 4 from the PC, because of the pipeline PC points to 4 instructions forward
		inst = memory[(registers[15] - 4)];

		//increment the PC(register[15]) to point at the next instruction
		registers[15]++;

		//Initialise the instruction result values
		instResult = INST_VALID;

		//Initialise the instruction cycle count to 1, because it will take at least 1 cycle even if the condition is not met
		instCycleCount = 1;

		if ( inst != 0 ) {
			printf("---------- Instruction %d ----------\n", instCount);
			decodeAndExecute(inst);
			printOutput();

			//Update total cycle count
			totalCycleCount += instCycleCount;

			printf("---------- %d Cycles, end of instruction %d----------\n", instCycleCount, instCount);

			//Increase the amount of instructions executed
			instCount++;
		}
		else {
			// found an instruction with the value 0
			// finish once a 0 hit. Not correct behaviour but good for testing
			isFinished = 1;

			//Total instruction count doesn't include the finish instruction
			printf("========== %d Total cycles for %d instructions ==========\n", totalCycleCount, instCount);
			printf("Press enter to exit...\n");
			getchar();
		}
	}
	// all ok
	return 0;
}
