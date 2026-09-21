Opcode	Mnemonic	Format	Type	Operands
0	HALT	B	N	—
1	NOP	B	N	—
2	ADD	A	T	rd, rs1, rs2
3	SUB	A	T	rd, rs1, rs2
4	AND	A	T	rd, rs1, rs2
5	OR	A	T	rd, rs1, rs2
6	XOR	A	T	rd, rs1, rs2
7	NOT	A	W	rd, rs1
8	MOV	A	W	rd, rs1
9	LOADI	B	F	rd, imm
10	SHL	B	D	rd, rs1, imm
11	SHR	B	D	rd, rs1, imm
12	LOAD	B	D	rd, rs1, imm
13	STORE	B	D	rd, rs1, imm
14	JMP	B	I	imm
15	BEQ	B	D	rd, rs1, imm
16	BNE	B	D	rd, rs1, imm
17	BLT	B	D	rd, rs1, imm
18	BGT	B	D	rd, rs1, imm
19	ADDI	B	D	rd, rs1, imm
20	MUL	A	T	rd, rs1, rs2
21	JAL	B	F	rd, imm
22	JR	B	R	