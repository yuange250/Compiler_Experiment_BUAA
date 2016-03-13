.data
_Str_0: .asciiz " cocks: "
_Str_1: .asciiz " rabbits: "
_Str_2: .asciiz " cocks: "
_Str_3: .asciiz " rabbits: "
.text
jal MAIN_0
calculation_1:
addi $fp,$sp,500
sw $ra,-12($fp)
li $t0,0
add $t2,$zero,$t0
sw $t2,-28($fp)
li $t0,0
add $t2,$zero,$t0
sw $t2,-32($fp)
_Label0:
lw $t0,-32($fp)
lw $t1,-20($fp)
lw $t1,0($t1)
bgt $t0,$t1,_Label3
lw $t0,-20($fp)
lw $t0,0($t0)
lw $t1,-32($fp)
sub $t2,$t0,$t1
sw $t2,-36($fp)
lw $t0,-32($fp)
mul $t2,$t0,2
sw $t2,-40($fp)
lw $t0,-36($fp)
mul $t2,$t0,4
sw $t2,-44($fp)
lw $t0,-40($fp)
lw $t1,-44($fp)
add $t2,$t0,$t1
sw $t2,-48($fp)
lw $t0,-48($fp)
lw $t1,-24($fp)
lw $t1,0($t1)
bne $t0,$t1,_Label1
la $a0,_Str_0
li $v0,4
syscall
li $v0,1
lw $a0,-32($fp)
syscall
la $a0,_Str_1
li $v0,4
syscall
li $v0,1
lw $a0,-36($fp)
syscall
lw $t0,-28($fp)
addi $t2,$t0,1
sw $t2,-28($fp)
jal _Label2
_Label1:
_Label2:
lw $t0,-32($fp)
addi $t2,$t0,1
sw $t2,-32($fp)
jal _Label0
_Label3:
lw $t0,-28($fp)
bne $t0,0,_Label4
la $a0,_Str_2
li $v0,4
syscall
li $v0,1
lw $a0,-52($fp)
syscall
la $a0,_Str_3
li $v0,4
syscall
li $v0,1
lw $a0,-52($fp)
syscall
jal _Label5
_Label4:
_Label5:
lw $sp,-4($fp)
lw $fp,-8($fp)
lw $ra,-12($sp)
jr $ra
MAIN_0:
subi $sp,$sp,500
addi $fp,$sp,500
sw $ra,-12($fp)
li $t0,0
add $t2,$zero,$t0
sw $t2,-40($fp)
_Label6:
lw $t0,-40($fp)
bgt $t0,2,_Label13
lw $t0,-40($fp)
bne $t0,0,_Label7
li $t0,4
add $t2,$zero,$t0
sw $t2,-44($fp)
li $t0,8
add $t2,$zero,$t0
sw $t2,-48($fp)
jal _Label8
_Label7:
_Label8:
lw $t0,-40($fp)
bne $t0,1,_Label9
li $t0,8
add $t2,$zero,$t0
sw $t2,-44($fp)
li $t0,4
add $t2,$zero,$t0
sw $t2,-48($fp)
jal _Label10
_Label9:
_Label10:
lw $t0,-40($fp)
bne $t0,2,_Label11
li $t0,3
add $t2,$zero,$t0
sw $t2,-44($fp)
li $t0,5
add $t2,$zero,$t0
sw $t2,-48($fp)
jal _Label12
_Label11:
_Label12:
addi $t0,$fp,-16
lw $t1,-40($fp)
sll $t1,$t1,2
sub $t2,$t0,$t1
addi $t1,$fp,-44
lw $t0,0($t1)
sw $t0,0($t2)
addi $t0,$fp,-28
lw $t1,-40($fp)
sll $t1,$t1,2
sub $t2,$t0,$t1
addi $t1,$fp,-48
lw $t0,0($t1)
sw $t0,0($t2)
sw $sp,-4($sp)
sw $fp,-8($sp)
sw $fp,-16($sp)
addi $t0,$fp,-16
lw $t1,-40($fp)
sll $t1,$t1,2
sub $t2,$t0,$t1
sw $t2,-20($sp)
addi $t0,$fp,-28
lw $t1,-40($fp)
sll $t1,$t1,2
sub $t2,$t0,$t1
sw $t2,-24($sp)
subi $sp,$sp,500
jal calculation_1
lw $t0,-40($fp)
addi $t2,$t0,1
sw $t2,-40($fp)
jal _Label6
_Label13:
