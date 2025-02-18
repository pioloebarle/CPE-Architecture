/*===========================================================================
FILENAME	: ALU.c
AUTHOR      : Piolo Pascual E. Besinga & Ivor Louisetyne Canque
DESCRIPTION : A C program that emulates the basic arithmetic and
			  and logical operations of an 8-bit ALU
COPYRIGHT   : created February 16, 2025
---------------------------------------------------------------------------
REVISION HISTORY
Date:               By:             Description:
02/04/2025			CABD			Finsihed Coding and Added Documentation
===========================================================================*/

#include <stdio.h>


void ALU(unsigned char operand1, unsigned char operand2, unsigned char control_signals);	//ALU function
unsigned char twosComp	(unsigned data);													//sets the 2's complement
void setFlags (unsigned int ACC);															//sets the flags of the ALU
void printBin (int data, unsigned char data_width);

unsigned char C = 0;
unsigned char Z = 0;
unsigned char OF = 0;
unsigned char SF = 0;
unsigned char ACC = 0;

void main(){
	
	//Subtraction
	ALU(0x03,0x05,0x02);
	
	//Addition
	ALU(0x88,0x85,0x01);
	
	//Multiplication 
	ALU(0xC0,0x0A,0x03); 
	
	//LOGIC OPERATIONS TEST CASES
	//AND
	ALU(0x03,0x05,0x04);
	
	//OR
	ALU(0x03,0x05,0x05);
	
	//NOT
	ALU(0x03,0x05,0x06);
	
	//XOR
	ALU(0x05,0x01,0x07);
	
	//SHR
	ALU(0x05,0x02,0x08);
	
	//SHL
	ALU(0x05,0x02,0x09);

	return;
}

ALU(unsigned char operand1, unsigned char operand2, unsigned char control_signals){
	unsigned int ACC;
	unsigned int Op1 = 0x00, Op2 = 0x00;
	
	int counter = 0;
	unsigned char A = 0x00, Q = 0x00, M = 0x00, tempQ = 0x00, tempA = 0x00;
	unsigned char Qn1 = 0x00, Q0 = 0x00;
	unsigned char A0 = 0x00, Q7 = 0x00, A7 = 0x00;
	
	ACC = 0x0000;
	C = 0;
	Z = 0;
	OF = 0;
	SF = 0;
	
	printf("************************************************\n");
	printf("Operand1: ");
	printBin(operand1, 8);
	printf("\nOperand2: ");
	printBin(operand2, 8);
	
	
	if(control_signals == 0x01 || control_signals == 0x02){
		Op1 = operand1;
		Op2 = operand2;	
		
		if((Op1 & 0x80) == 0x80){
			Op1 = twosComp(Op1);
		}
		
		if((Op2 & 0x80) == 0x80){
			Op2 = twosComp(Op2);
		}
		
		//ADDITION
		if(control_signals == 0x01){
			printf("\nAddition Operation: \n");
			if(Op1 == twosComp(operand1)){
				printf("2's Complement of operand1: ");
				printBin(Op1, 8);
			}
			
			if(Op2 == twosComp(operand2)){
				printf("\n2's Complement of operand2: ");
				printBin(Op2, 8);
			}
			
			printf("\nPerforming signed Addition\n");
			
		}
		else{
			//SUBTRACTION
			printf("\nSubtraction operation: \n");
			if(Op1 == twosComp(operand1)){
				printf("2's Complement of operand1: ");
				printBin(Op1, 8);
			}
			
			if(Op2 == twosComp(operand2)){
				printf("2's Complement of operand1: ");
				printBin(Op2, 8);
			}
			
			Op2 = twosComp(operand2);
			printf("Performing signed Subtraction\n");
		}
		ACC = Op1 + Op2;
	}
	//MULTIPLICATION USING BOOTH'S ALGORITHM
	if(control_signals == 0x03){
		printf("\nMultiplication Operation: \n");
		printf("\nProduct\t\tMultiplier\tQn1\tMultiplicand\t#");
		printf("\n----------------------------------------------------------");
		Q = operand2;
		
		while(counter != 9){
			printf("\n");
			printBin(A,8);
			printf("\t");
			printBin(Q,8);
			printf("\t");
			printBin(Qn1,1);
			printf("\t");
			printBin(operand1,8);
			printf("\t%d",counter);
			
			tempQ = Q;
			Q0 = Q & 0x01;
			Qn1 = Qn1 & 0x01;
			if(Q0 == 1 && Qn1 == 0){
				M = twosComp(operand1);
				A = A + M;
			}
			else if (Q0 == 0 && Qn1 == 1){	
				M = operand1;
				A = A + M;			
			}
				tempA = A;
				A7 = A  & 0x80;
				Qn1 = Q & 0x01;
				A0 = A & 0x01;
				A = (A >> 1) | A7;
				Q = (Q >> 1) | (A0 << 7);
				counter++;
			
		}
		printf("\n");
		ACC = (tempA << 8)| tempQ;
		
		
	}
	//AND LOGIC FUNCTION
	if(control_signals == 0x04){
		printf("\nAND Logic function: \n");
		ACC = operand1 & operand2; 
	}
	//OR LOGIC FUNCTION
	if(control_signals == 0x05){
		printf("\nOR Logic function: \n");
		ACC = operand1 | operand2; 
	}
	//NOT FUNCTION
	if(control_signals == 0x06){
		printf("\nNOT Logic function: \n");
		ACC = (~operand1) & 0xFF ; 
	}
	//XOR FUNCTION
	if(control_signals == 0x07){
		printf("\nXOR Logic function: \n");
		ACC = operand1 ^ operand2; 
	}
	//SHIFT RIGHT
	if(control_signals == 0x08){
		printf("\nShift Right Logic function: \n");
		ACC = operand1 >> 1; 
	}
	//SHIFT LEFT
	if(control_signals == 0x09){
		printf("\nShift Left Logic function: \n");
		ACC = operand1 << 1; 
	}
	
	
	printf("ACC = ");
	printBin(ACC, 16);
	setFlags(ACC);
	printf("\nCF = %d, ZF = %d, OF = %d, SF = %d\n\n", C, Z, OF, SF);
	
	return;
	
	
}

unsigned char twosComp(unsigned data){
	return (~(data) + 1) & 0xFF;
}

void setFlags(unsigned int ACC){
	
	if(ACC == 0x0000){
		Z = 1;
	}
	
	if((ACC & 0x0080) == 0x0080){
		SF = 1;
	}
	
	if(ACC > 0x7F){
		OF = 1;
		if(ACC > 0xFF){
			C = 1;
		}
	}
}
	

void printBin(int data, unsigned char data_width){
	int i, bit;
	for(i = (int)data_width - 1; i >= 0; i--){
		bit = data >> i;
		if(bit & 1){
			printf("1");
		}
		else{
			printf("0");
		}
	}
}
