#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <iostream>
#include <iomanip>
#include <assert.h>

#include <bitset>

#include <sys/types.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/stat.h>

#include <limits.h>
#include <errno.h>

#include <fcntl.h>
#include <unistd.h>

constexpr size_t kPhysMemSize = 8192 * 32;
constexpr size_t kInsnSize = 4;
constexpr size_t regBitLength = 32;

typedef uint32_t PhysAddr;
typedef uint32_t VirtAddr;

typedef uint32_t RegValue;
typedef int32_t SRegValue;
typedef uint32_t EncodedInsn;

class Instruction;

enum RegId
{
    kX0,
	kX1,
	kX2,
    kX3,
    kX4,
    kX5,
    kX6,
    kX7,
    kX8,
    kX9,
    kX10,
    kX11,
    kX12,
    kX13,
    kX14,
    kX15,
    kX16,
    kX17,
    kX18,
    kX19,
    kX20,
    kX21,
    kX22,
    kX23,
    kX24,
    kX25,
    kX26,
    kX27,
    kX28,
    kX29,
    kX30,
	kX31,
	kRegIdZero = kX0,
    kRegStackP = kX2,
    kRegFileGprSize = 32,
    kErrVal = -1,
};

enum InsnClass : uint8_t //256 instruction are possible
{
/* + */ 	kInsnAdd,		/* 0b0110011 */
/* + */		kInsnSub,
/* + */		kInsnSll,
/*   */		kInsnSlt,
/*   */		kInsnSltu,
/* + */		kInsnXor,
/* + */		kInsnSrl,
/* + */		kInsnSra,
/* + */		kInsnOr,
/* + */		kInsnAnd,
/* + */		kInsnBeq, 		/*/ 0b1100011 */
/* + */		kInsnBne,
/* + */		kInsnBlt,
/* + */		kInsnBge,
/*   */		kInsnBltu,
/*   */		kInsnBgeu,
/* + */		kInsnLb,		/* 0b0000011 */
/* + */		kInsnLh,
/* + */		kInsnLw,
/* + */		kInsnLbu,
/* + */		kInsnLhu,
/* + */		kInsnEcall,		/* 0b1110011 */
/* + */		kInsnEbreak,
/* + */		kInsnSb,		/* 0b0100011 */
/* + */		kInsnSh,
/* + */		kInsnSw,
/* + */		kInsnAddi,		/* 0b0010011 */
/*   */		kInsnSlti,
/*   */		kInsnSltiu,
/* + */		kInsnXori,
/* + */		kInsnOri,
/* + */		kInsnAndi,
/* + */		kInsnSlli,
/* + */		kInsnSrli,
/* + */		kInsnSrai,
/* + */		kInsnJal,		/* 0b1101111 */
/* + */		kInsnJalr,		/* 0b1100111 */
/* + */		kInsnAuipc,		/* 0b0010111 */
/* + */		kInsnLui,		/* 0b0110111*/
			insnERROR,
};


const char kInsnTypeNames[][20] =
{
	"ADD", "SUB", "SLL", "SLT", "SLTU", "XOR", "SRL", "SRA", "OR", "AND", 
	"BEQ", "BNE", "BLT", "BGE", "BLTU", "BGEU",
	"LB", "LH", "LW", "LBU", "LHU",
	"ECALL", "EBREAK",
	"SB", "SH", "SW",
	"ADDI", "SLTI", "SLTIU", "XORI", "ORI", "ANDI", "SLLI", "SRLI", "SRAI",
	"JAL",
	"JALR",
	"AUIPC",
	"LUI",
	"ERROR"
};



template<int hi,int lo, class T = RegValue>
T getBits(T value)
{
    return ( ( value << (sizeof(T) * 8 - hi - 1) ) >> (sizeof(T) * 8 - hi - 1) ) >> lo ; 
}

enum PrintInfo { PI_rd, PI_rs1, PI_rs2, PI_imm, PI_size };
const char INFO_NAMES[][20] = { "rd", "rs1", "rs2", "imm" };

class Instruction final
{
private:
	
    InsnClass insnType_;
	RegId rd_, rs1_, rs2_;
	RegValue imm_;

public:

    Instruction(EncodedInsn insn);

// GETTERS

    RegId rd()  const {return rd_;} 
    RegId rs1() const {return rs1_;}
    RegId rs2() const {return rs2_;}

    RegValue imm() const {return imm_;}

	InsnClass insn() const { return insnType_;}

// OTHER

	const char* fromTypeToStr(InsnClass type);
	void printInsnType(InsnClass type);
};