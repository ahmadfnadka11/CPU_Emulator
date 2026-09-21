Opcode	Mnemonic	Format	Meaning
0	HALT	—	stop execution
1	NOP	—	do nothing
2	ADD	A	rd = rs1 + rs2
3	SUB	A	rd = rs1 - rs2
4	AND	A	rd = rs1 & rs2
5	OR	A	rd = rs1 | rs2
6	XOR	A	rd = rs1 ^ rs2
7	NOT	A	rd = ~rs1
8	MOV	A	rd = rs1
9	LOADI	B	rd = immediate
10	SHL	B	rd = rs1 << imm
11	SHR	B	rd = rs1 >> imm
12	LOAD	B	rd = memory[rs1 + imm]
13	STORE	B	memory[rs1 + imm] = rd
14	JMP	B	pc = imm
15	BEQ	B	if rs1 == rd: pc = imm
16	BNE	B	if rs1 != rd: pc = imm
17	BLT	B	if rs1 < rd: pc = imm
18	BGT	B	if rs1 > rd: pc = imm
19	ADDI	B	rd = rs1 + imm
20	MUL	A	rd = rs1 * rs2
21	JAL	B	rd = pc + 1, then pc = imm
22	JR	B	pc = rs1 (uses only the rs1 field)