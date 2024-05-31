#include "instr.h"

#define P_BIT_NUM(num, bits)                        \
    do{std::bitset<bits> num_b(num);                \
    std::cout << std::setw(10) << #num << "_b = " << std::right << std::setw(32) << num_b << std::endl;}while(0)

const char* Instruction::fromTypeToStr(InsnClass type)
{
	return kInsnTypeNames[type];
}

void Instruction::printInsnType(InsnClass type)
{
	std::cout << fromTypeToStr(type) << std::endl;
}

Instruction::Instruction(EncodedInsn insn)
{

	insnType_ = insnERROR;
	switch (insn & 0x7F) /* 1st seven bits*/
	{
		// R-format insns
		case 0:
			break;
		case 0b0010111:
		{
		    rd_  = static_cast<RegId> (getBits<11, 7 >(insn));
			imm_ = getBits<31, 12>(insn) << 12;
			
			insnType_ = kInsnAuipc;
			//executor_ = &Instruction::executeAuipc;
            std::cout << "Instr: AUIPC, rd_ = " << rd_ << ", imm_ = " << imm_ << std::endl;

			//printInfo[PI_rd] = true;
			//printInfo[PI_imm] = true;

			break;
		}
		case 0b0110111:
		{
		    rd_  = static_cast<RegId> (getBits<11, 7 >(insn));
			imm_ = getBits<31, 12>(insn) << 12;

			insnType_ = kInsnLui;
			//executor_ = &Instruction::executeLui;
            std::cout << "Instr: Lui, rd_ = " << rd_ << ", imm_ = " << imm_ << std::endl;

			//printInfo[PI_rd] = true;
			//printInfo[PI_imm] = true;

			break;
		}
		case 0b0110011: /* DONE [only function realization needed] */
        {
		    rd_  = static_cast<RegId> (getBits<11, 7 >(insn));
			rs1_ = static_cast<RegId> (getBits<19, 15>(insn));
			rs2_ = static_cast<RegId> (getBits<24, 20>(insn));
			
			auto funct3 = getBits<14, 12>(insn);
			auto funct7 = getBits<31, 25>(insn);
			
			//printInfo[PI_rd] = true;
			//printInfo[PI_rs1] = true;
			//printInfo[PI_rs2] = true;

			/* TODO: switch-case ; executor = .. */

			if (funct3 == 0 && funct7 == 0)
			{
			    insnType_ = kInsnAdd;
				//executor_ = &Instruction::executeAdd;
                std::cout << "Instr: ADD, rs1_ = " << rs1_ << ", rs2_ = " << rs2_ << std::endl;
			}
			else if (funct3 == 0 && funct7 == 0b0100000)
			{
				insnType_ = kInsnSub;
				//executor_ = &Instruction::executeSub;
                std::cout << "Instr: SUB, rs1_ = " << rs1_ << ", rs2_ = " << rs2_ << std::endl;
			}
			else if (funct3 == 0b001 && funct7 == 0)
			{
			    insnType_ =  kInsnSll;
				//executor_ = &Instruction::executeSll;
                std::cout << "Instr: SLL, rs1_ = " << rs1_ << ", rs2_ = " << rs2_ << std::endl;
			}
			else if (funct3 == 0b010 && funct7 == 0)
			{
				insnType_ =  kInsnSlt;
				//executor_ = &Instruction::executeNOEXIST;
                std::cout << "Instr: SLT, rs1_ = " << rs1_ << ", rs2_ = " << rs2_ << std::endl;
			}		
			else if (funct3 == 0b011 && funct7 == 0)
			{
			    insnType_ =  kInsnSltu;
				//executor_ = &Instruction::executeNOEXIST;
                std::cout << "Instr: SLTU, rs1_ = " << rs1_ << ", rs2_ = " << rs2_ << std::endl;
			}
			else if (funct3 == 0b100 && funct7 == 0)
			{
			   	insnType_ =  kInsnXor;
				//executor_ = &Instruction::executeXor;
                std::cout << "Instr: XOR, rs1_ = " << rs1_ << ", rs2_ = " << rs2_ << std::endl;
			}	
			else if (funct3 == 0b101 && funct7 == 0)
			{
			    insnType_ =  kInsnSrl;
				//executor_ = &Instruction::executeSrl;
                std::cout << "Instr: SRL, rs1_ = " << rs1_ << ", rs2_ = " << rs2_ << std::endl;
			}
			else if (funct3 == 0b101 && funct7 == 0b0100000)
			{
			    insnType_ =  kInsnSra;
				//executor_ = &Instruction::executeSra;
                std::cout << "Instr: SRA, rs1_ = " << rs1_ << ", rs2_ = " << rs2_ << std::endl;
			}
			else if (funct3 == 0b110 && funct7 == 0)
			{
			    insnType_ =  kInsnOr;
				//executor_ = &Instruction::executeOr;
                std::cout << "Instr: OR, rs1_ = " << rs1_ << ", rs2_ = " << rs2_ << std::endl;
			}
			else if (funct3 == 0b111 && funct7 == 0)
			{
				insnType_ =  kInsnOr;
				//executor_ = &Instruction::executeOr;
                std::cout << "Instr: OR, rs1_ = " << rs1_ << ", rs2_ = " << rs2_ << std::endl;
			}
			else
				assert (0);
		    break;
        }
        case 0b1100011: /* BEQ BNE BLT BGE BLTU BGEU */
		{

			rs1_ = static_cast<RegId> (getBits<19, 15>(insn));
			rs2_ = static_cast<RegId> (getBits<24, 20>(insn));
			
			imm_ = (getBits<11, 8>(insn) << 1) + (getBits<7, 7>(insn) << 11) + 
					(getBits<30, 25>(insn) << 5) + (getBits<0, 0>( static_cast<SRegValue>(insn) >> 31 ) << 12);		

			auto funct3 = getBits<14, 12>(insn);

			//printInfo[PI_rs1] = true;
			//printInfo[PI_rs2] = true;
			//printInfo[PI_imm] = true;
			
			if (funct3 == 0b000)
			{
			    insnType_ = kInsnBeq;
				//executor_ = &Instruction::executeBeq;
                std::cout << "Instr: BEQ, rs1_ = " << rs1_ << ", rs2_ = " << rs2_ << ", imm_ = " << imm_ << std::endl;
			}
			else if (funct3 == 0b001)
			{
				insnType_ = kInsnBne;
				//executor_ = &Instruction::executeBne;
                std::cout << "Instr: BNE, rs1_ = " << rs1_ << ", rs2_ = " << rs2_ << ", imm_ = " << imm_ << std::endl;
			}
			else if (funct3 == 0b100)
			{
				insnType_ = kInsnBlt;
				//executor_ = &Instruction::executeBlt;
                std::cout << "Instr: BLT, rs1_ = " << rs1_ << ", rs2_ = " << rs2_ << ", imm_ = " << imm_ << std::endl;
			}
			else if (funct3 == 0b101)
			{
				insnType_ = kInsnBge;
				//executor_ = &Instruction::executeBge;
                std::cout << "Instr: BGE, rs1_ = " << rs1_ << ", rs2_ = " << rs2_ << ", imm_ = " << imm_ << std::endl;
			}
			else if (funct3 == 0b110)
			{
				insnType_ = kInsnBltu;
				imm_ = (getBits<11, 8>(insn) << 1) + (getBits<7, 7>(insn) << 11) + 
					(getBits<30, 25>(insn) << 5) + (getBits<0, 0>(insn) >> 31);	
				//executor_ = &Instruction::executeBltu;
                std::cout << "Instr: BLTU, rs1_ = " << rs1_ << ", rs2_ = " << rs2_ << ", imm_ = " << imm_ << std::endl;
			}
			else if (funct3 == 0b111)
			{
				insnType_ = kInsnBgeu;
				imm_ = (getBits<11, 8>(insn) << 1) + (getBits<7, 7>(insn) << 11) + 
					(getBits<30, 25>(insn) << 5) + (getBits<0, 0>(insn) >> 31);	
				//executor_ = &Instruction::executeBgeu;
                std::cout << "Instr: BGEU, rs1_ = " << rs1_ << ", rs2_ = " << rs2_ << ", imm_ = " << imm_ << std::endl;
			}
			else
				assert(0 && "Undefinied funct3 for PC-instruction");
		    break;
        }
		case 0b0000011: /* LB LH LW LBU LHU */ 
		{
		    rd_  = static_cast<RegId> (getBits<11, 7 >(insn));
			rs1_ = static_cast<RegId> (getBits<19, 15>(insn));

			imm_ = getBits<11, 0>( static_cast<SRegValue>(insn) >> 20 ); // static_cast<RegId>
			
			auto funct3 = getBits<14, 12>(insn);

			//printInfo[PI_rd] = true;
			//printInfo[PI_rs1] = true;
			//printInfo[PI_imm] = true;
						
			if (funct3 == 0b000)
			{
			    insnType_ = kInsnLb;
				//executor_ = &Instruction::executeLb;
                std::cout << "Instr: LB, rd_ = " << rd_ << ", rs1_ = " << rs1_ << ", imm_ = " << imm_ << std::endl;
			}
			else if (funct3 == 0b001)
			{
				insnType_ = kInsnLh;
				//executor_ = &Instruction::executeLh;
                std::cout << "Instr: LH, rd_ = " << rd_ << ", rs1_ = " << rs1_ << ", imm_ = " << imm_ << std::endl;
			}
			else if (funct3 == 0b010)
			{
				insnType_ = kInsnLw;
				//executor_ = &Instruction::executeLw;
                std::cout << "Instr: LW, rd_ = " << rd_ << ", rs1_ = " << rs1_ << ", imm_ = " << imm_ << std::endl;
			}
			else if (funct3 == 0b100)
			{
				insnType_ = kInsnLbu;
				//executor_ = &Instruction::executeLbu;
                std::cout << "Instr: LBU, rd_ = " << rd_ << ", rs1_ = " << rs1_ << ", imm_ = " << imm_ << std::endl;
			}
			else if (funct3 == 0b101)
			{
				insnType_ = kInsnLhu;
				//executor_ = &Instruction::executeLhu;
                std::cout << "Instr: LHU, rd_ = " << rd_ << ", rs1_ = " << rs1_ << ", imm_ = " << imm_ << std::endl;
			}
			else
				assert (0 && "Undefinied funct3 for instruction");
		    break;
		}
		case 0b1110011: /* ECALL - EBREAK */
		{
		    rd_  = static_cast<RegId> (getBits<11, 7 >(insn));
			rs1_ = static_cast<RegId> (getBits<19, 15>(insn));

			imm_ = getBits<31, 20>(insn);
			
			auto funct3 = getBits<14, 12>(insn);

			//printInfo[PI_rd] = true;
			//printInfo[PI_rs1] = true;
			//printInfo[PI_imm] = true;
			
			if (rd_ == 0 && funct3 == 0 && rs1_ == 0 && imm_ == 0)
			{
			    insnType_ = kInsnEcall;
				//executor_ = &Instruction::executeEcall;
                std::cout << "Instr: ECALL, rd_ = " << rd_ << ", rs1_ = " << rs1_ << ", imm_ = " << imm_ << std::endl;
			}
			else if (rd_ == 0 && funct3 == 0 && rs1_ == 0 && imm_ == 0b000000000001)
			{
				insnType_ = kInsnEbreak;
				//executor_ = &Instruction::executeEbreak;
                std::cout << "Instr: EBREAK, rd_ = " << rd_ << ", rs1_ = " << rs1_ << ", imm_ = " << imm_ << std::endl;
			}
			else
				assert (0 && "Undefinied instruction");
		    break;
		}
		case 0b0100011: /* SB SH SW */ 
		{
			rs1_ = static_cast<RegId> (getBits<19, 15>(insn));
			rs2_ = static_cast<RegId> (getBits<24, 20>(insn));
			
			imm_ = getBits<11, 7>(insn) + (getBits<6, 0>(static_cast<SRegValue>(insn) >> 25) << 5);

			auto funct3 = getBits<14, 12>(insn);

			//printInfo[PI_rs1] = true;
			//printInfo[PI_rs2] = true;
			//printInfo[PI_imm] = true;

			if (funct3 == 0b000)
			{
			    insnType_ = kInsnSb;
				//executor_ = &Instruction::executeSb;
                std::cout << "Instr: SB, rs1_ = " << rs1_ << ", rs2_ = " << rs2_ << ", imm_ = " << imm_ << std::endl;
			}
			else if (funct3 == 0b001)
			{
				insnType_ = kInsnSh;
				//executor_ = &Instruction::executeSh;
                std::cout << "Instr: SH, rs1_ = " << rs1_ << ", rs2_ = " << rs2_ << ", imm_ = " << imm_ << std::endl;
			}
			else if (funct3 == 0b010)
			{
				insnType_ = kInsnSw;
				//executor_ = &Instruction::executeSw;
                std::cout << "Instr: SW, rs1_ = " << rs1_ << ", rs2_ = " << rs2_ << ", imm_ = " << imm_ << std::endl;
			}
			else
				assert (0 && "Undefinied type");
		    break;
        }
		case 0b0010011: /* ADDI SLTI SLTIU XORI ORI ANDI SLLI SRLI SRAI */
		{
		    rd_  = static_cast<RegId> (getBits<11, 7 >(insn));
			rs1_ = static_cast<RegId> (getBits<19, 15>(insn));

			imm_ = getBits<11, 0>( static_cast<SRegValue>(insn) >> 20 ); // static_cast<RegId> 

			auto funct3 = getBits<14, 12>(insn);

			//printInfo[PI_rd] = true;
			//printInfo[PI_rs1] = true;
			//printInfo[PI_imm] = true;

			if (funct3 == 0b000)
			{
				insnType_ = kInsnAddi;
				//executor_ = &Instruction::executeAddi;
                std::cout << "Instr: ADDI, rd_ = " << rd_ << ", rs1_ = " << rs1_ << ", imm_ = " << imm_ << std::endl;
			}
			else if (funct3 == 0b010)
			{
				insnType_ = kInsnSlti;
				//executor_ = &Instruction::executeNOEXIST;
                std::cout << "Instr: SLTI, rd_ = " << rd_ << ", rs1_ = " << rs1_ << ", imm_ = " << imm_ << std::endl;
			}
			else if (funct3 == 0b011)
			{
				insnType_ = kInsnSltiu;
				//executor_ = &Instruction::executeNOEXIST;
                std::cout << "Instr: SLTIU, rd_ = " << rd_ << ", rs1_ = " << rs1_ << ", imm_ = " << imm_ << std::endl;
			}
			else if (funct3 == 0b100)
			{
				insnType_ = kInsnXori;
				//executor_ = &Instruction::executeXori;
                std::cout << "Instr: XORI, rd_ = " << rd_ << ", rs1_ = " << rs1_ << ", imm_ = " << imm_ << std::endl;
			}
			else if (funct3 == 0b110)
			{
				insnType_ = kInsnOri;
				//executor_ = &Instruction::executeOri;
                std::cout << "Instr: ORI, rd_ = " << rd_ << ", rs1_ = " << rs1_ << ", imm_ = " << imm_ << std::endl;
			}
			else if (funct3 == 0b111)
			{
				insnType_ = kInsnAndi;
				//executor_ = &Instruction::executeAndi;
                std::cout << "Instr: ANDI, rd_ = " << rd_ << ", rs1_ = " << rs1_ << ", imm_ = " << imm_ << std::endl;
			}
			else /* logical/arithmetic shifts */
			{
				auto funct7 = getBits<11, 5>(imm_);
				imm_ = getBits<4, 0>(imm_);

				if (funct7 == 0 && funct3 == 0b001)
				{
					insnType_ = kInsnSlli;
					//executor_ = &Instruction::executeSlli;
                    std::cout << "Instr: SLLI, rd_ = " << rd_ << ", rs1_ = " << rs1_ << ", imm_ = " << imm_ << std::endl;
				}
				else if (funct7 == 0b000000 && funct3 == 0b101)
				{
					insnType_ = kInsnSrli;
					//executor_ = &Instruction::executeSrli;
                    std::cout << "Instr: SRL, rd_ = " << rd_ << ", rs1_ = " << rs1_ << ", imm_ = " << imm_ << std::endl;
				}
				else if (funct7 == 0b100000 && funct3 == 0b101)
				{
					insnType_ = kInsnSrai;
					//executor_ = &Instruction::executeSrai;
                    std::cout << "Instr: SRAI, rd_ = " << rd_ << ", rs1_ = " << rs1_ << ", imm_ = " << imm_ << std::endl;
				}
				else
					assert (0 && "Undefinied type");
			}
			break;
		}
		case 0b1100111:
		{
			rd_  = static_cast<RegId> (getBits<11, 7 >(insn));
			rs1_ = static_cast<RegId> (getBits<19, 15>(insn));

			imm_ = getBits<11, 0>( static_cast<SRegValue>(insn) >> 20 ); // static_cast<RegId> 

			auto funct3 = getBits<14, 12>(insn);

			//printInfo[PI_rd] = true;
			//printInfo[PI_rs1] = true;
			//printInfo[PI_imm] = true;
			
			if (funct3 == 0b000)
			{
			    insnType_ = kInsnJalr;
				//executor_ = &Instruction::executeJalr;
                std::cout << "Instr: JALR, rd_ = " << rd_ << ", rs1_ = " << rs1_ << ", imm_ = " << imm_ << std::endl;
			}
			else 
				assert (0 && "Undefinied funct3 for instruction");
		    break;
		}
		case 0b1101111:
		{
			rd_  = static_cast<RegId> (getBits<11, 7 >(insn));
			
			SRegValue shiftImm = static_cast<SRegValue>(insn) >> 12;

			RegValue retImm = static_cast<RegValue>(shiftImm);

			imm_ = (getBits<19, 19>(static_cast<RegValue>(retImm)) << 20) + (getBits<18, 9>(retImm) << 1)
				 + (getBits<8 ,  8>(retImm) << 11) + (getBits<7 , 0>(retImm) << 12)+ (getBits<31, 20>(retImm) << 21);

			//P_BIT_NUM(imm_, 32);


			//printInfo[PI_rd] = true;
			//printInfo[PI_imm] = true;

			insnType_ = kInsnJal;
			//executor_ = &Instruction::executeJal;
            std::cout << "Instr: JAL, rd_ = " << rd_ << ", imm_ = " << imm_ << std::endl;
		    break;
		}
		default:
		{
			auto opcode = (insn & 0x7F);
			std::cout << "Undefinied opcode = " << opcode << std::endl;
			P_BIT_NUM(opcode, 7);
			break;
		}
	}	
}

// inst size in bits
constexpr int InstrSize = 32;

#include "test_examples.h"

int main() {

    // LIST OF INSTRUCTION TO DECODE
    int instrs[2] = {0x11223344, 0x11223344/*, ...*/};

    // RISC-V instructions to execute factorial
    FACT_INSNS

    for (int i = 0; i < 30; ++i) {
        Instruction insn(fact[i]);
    }
}