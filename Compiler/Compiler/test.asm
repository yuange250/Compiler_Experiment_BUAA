.data
_Str_0: .asciiz " "
_Str_1: .asciiz "  fib"
_Str_2: .asciiz "="
_Str_3: .asciiz "  gcd("
_Str_4: .asciiz ","
_Str_5: .asciiz ")="
.text
jal MAIN_0
input_1:
addi $fp,$sp,500
sw $ra,-12($fp)
lw $t1,-16($fp)
lw $s0,-28($t1)
lw $t1,-16($fp)
lw $s1,-20($t1)
li $s2,97
lw $t1,-16($fp)
lw $s3,-16($t1)
add $s3,$s0,$s0
li $t0,2
mulo $t4,$t0,$s1
add $s3,$t4,$t4
lw $t0,-16($fp)
addi $t0,$t0,-32
li $t1,0
sll $t1,$t1,2
sub $t2,$t0,$t1
sw $s2,0($t2)
li $t0,1
add $s3,$zero,$t0
_Label0:
bgt $s3,6,_Label1
li $v0,5
syscall
add $s1,$v0,$zero
lw $t0,-16($fp)
addi $t0,$t0,-32
sll $t1,$s3,2
sub $t2,$t0,$t1
sw $s1,0($t2)
addi $s3,$s3,1
jal _Label0
_Label1:
lw $t1,-16($fp)
sw $s1,-20($t1)
lw $t1,-16($fp)
sw $s3,-16($t1)
lw $sp,-4($fp)
lw $fp,-8($fp)
lw $ra,-12($sp)
jr $ra
fib_2:
addi $fp,$sp,500
sw $ra,-12($fp)
lw $s0,-20($fp)
li $t0,1
add $t2,$zero,$t0
sw $t2,-24($fp)
li $t0,1
add $t2,$zero,$t0
sw $t2,-28($fp)
bgt $s0,2,_Label2
li $t0,1
add $t2,$zero,$t0
sw $t2,0($fp)
jal _Label3
_Label2:
subi $t4,$s0,1
sw $sp,-4($sp)
sw $fp,-8($sp)
lw $t0,-16($fp)
sw $t0,-16($sp)
sw $t4,-20($sp)
subi $sp,$sp,500
jal fib_2
lw $s0,-20($fp)
lw $t0,0($sp)
add $t5,$zero,$t0
subi $t6,$s0,2
sw $sp,-4($sp)
sw $fp,-8($sp)
lw $t0,-16($fp)
sw $t0,-16($sp)
sw $t6,-20($sp)
sw $t5,-40($fp)
subi $sp,$sp,500
jal fib_2
lw $t0,0($sp)
add $t7,$zero,$t0
lw $t0,-40($fp)
add $t2,$t0,$t7
sw $t2,0($fp)
_Label3:
lw $sp,-4($fp)
lw $fp,-8($fp)
lw $ra,-12($sp)
jr $ra
qsort_3:
addi $fp,$sp,500
sw $ra,-12($fp)
lw $s0,-24($fp)
lw $s1,-20($fp)
lw $s2,-40($fp)
lw $s3,-28($fp)
lw $s4,-32($fp)
lw $s5,-36($fp)
add $s3,$zero,$s1
add $s4,$zero,$s0
add $t4,$s1,$s0
div $t5,$t4,2
lw $t0,-16($fp)
addi $t0,$t0,-32
sll $t1,$t5,2
sub $t2,$t0,$t1
lw $s2,0($t2)
_Label4:
subi $s3,$s3,1
_Label5:
addi $s3,$s3,1
ble $s3,$s0,_Label6
add $s5,$zero,$s2
jal _Label7
_Label6:
lw $t0,-16($fp)
addi $t0,$t0,-32
sll $t1,$s3,2
sub $t2,$t0,$t1
lw $s5,0($t2)
_Label7:
lw $t0,-16($fp)
addi $t0,$t0,-32
sll $t1,$s3,2
sub $t2,$t0,$t1
lw $t6,0($t2)
blt $t6,$s2,_Label5
addi $s4,$s4,1
_Label8:
subi $s4,$s4,1
blt $s4,$s1,_Label9
lw $t0,-16($fp)
addi $t0,$t0,-32
sll $t1,$s4,2
sub $t2,$t0,$t1
lw $s5,0($t2)
jal _Label10
_Label9:
add $s5,$zero,$s2
_Label10:
blt $s2,$s5,_Label8
bgt $s3,$s4,_Label11
lw $t0,-16($fp)
addi $t0,$t0,-32
sll $t1,$s3,2
sub $t2,$t0,$t1
lw $s5,0($t2)
lw $t0,-16($fp)
addi $t0,$t0,-32
sll $t1,$s4,2
sub $t2,$t0,$t1
lw $t7,0($t2)
lw $t0,-16($fp)
addi $t0,$t0,-32
sll $t1,$s3,2
sub $t2,$t0,$t1
sw $t7,0($t2)
lw $t0,-16($fp)
addi $t0,$t0,-32
sll $t1,$s4,2
sub $t2,$t0,$t1
sw $s5,0($t2)
addi $s3,$s3,1
subi $s4,$s4,1
jal _Label12
_Label11:
_Label12:
ble $s3,$s4,_Label4
bge $s3,$s0,_Label13
sw $sp,-4($sp)
sw $fp,-8($sp)
lw $t0,-16($fp)
sw $t0,-16($sp)
sw $s3,-20($sp)
sw $s0,-24($sp)
sw $s2,-40($fp)
sw $s3,-28($fp)
sw $s4,-32($fp)
sw $s5,-36($fp)
subi $sp,$sp,500
jal qsort_3
lw $s1,-20($fp)
lw $s4,-32($fp)
lw $s5,-36($fp)
lw $t0,0($sp)
add $t8,$zero,$t0
add $s5,$zero,$t8
jal _Label14
_Label13:
_Label14:
ble $s4,$s1,_Label15
sw $sp,-4($sp)
sw $fp,-8($sp)
lw $t0,-16($fp)
sw $t0,-16($sp)
sw $s1,-20($sp)
sw $s4,-24($sp)
sw $s2,-40($fp)
sw $s3,-28($fp)
sw $s4,-32($fp)
sw $s5,-36($fp)
subi $sp,$sp,500
jal qsort_3
lw $s5,-36($fp)
lw $t0,0($sp)
add $t9,$zero,$t0
add $s5,$zero,$t9
jal _Label16
_Label15:
_Label16:
li $t0,1
add $t2,$zero,$t0
sw $t2,0($fp)
sw $s2,-40($fp)
sw $s3,-28($fp)
sw $s4,-32($fp)
sw $s5,-36($fp)
lw $sp,-4($fp)
lw $fp,-8($fp)
lw $ra,-12($sp)
jr $ra
swap_4:
addi $fp,$sp,500
sw $ra,-12($fp)
lw $t1,-20($fp)
lw $s0,0($t1)
lw $t1,-24($fp)
lw $s1,0($t1)
add $t2,$zero,$s0
sw $t2,-28($fp)
add $s0,$zero,$s1
lw $t0,-28($fp)
add $s1,$zero,$t0
lw $t1,-20($fp)
sw $s0,0($t1)
lw $t1,-24($fp)
sw $s1,0($t1)
lw $sp,-4($fp)
lw $fp,-8($fp)
lw $ra,-12($sp)
jr $ra
mod_5:
addi $fp,$sp,500
sw $ra,-12($fp)
lw $s0,-20($fp)
lw $s1,-24($fp)
div $t4,$s0,$s1
mulo $t5,$t4,$s1
sub $t2,$s0,$t5
sw $t2,0($fp)
lw $sp,-4($fp)
lw $fp,-8($fp)
lw $ra,-12($sp)
jr $ra
gcd_6:
addi $fp,$sp,500
sw $ra,-12($fp)
lw $s0,-20($fp)
lw $s1,-24($fp)
bge $s0,$s1,_Label17
sw $sp,-4($sp)
sw $fp,-8($sp)
lw $t0,-16($fp)
sw $t0,-16($sp)
addi $t0,$fp,-20
sw $t0,-20($sp)
addi $t0,$fp,-24
sw $t0,-24($sp)
subi $sp,$sp,500
jal swap_4
lw $s0,-20($fp)
lw $s1,-24($fp)
jal _Label18
_Label17:
_Label18:
bne $s1,0,_Label19
add $t2,$zero,$s0
sw $t2,0($fp)
jal _Label20
_Label19:
sw $sp,-4($sp)
sw $fp,-8($sp)
lw $t0,-16($fp)
sw $t0,-16($sp)
sw $s0,-20($sp)
sw $s1,-24($sp)
subi $sp,$sp,500
jal mod_5
lw $s1,-24($fp)
lw $t0,0($sp)
add $t4,$zero,$t0
sw $sp,-4($sp)
sw $fp,-8($sp)
lw $t0,-16($fp)
sw $t0,-16($sp)
sw $s1,-20($sp)
lw $t0,0($sp)
sw $t0,-24($sp)
subi $sp,$sp,500
jal gcd_6
lw $t0,0($sp)
add $t5,$zero,$t0
add $t2,$zero,$t5
sw $t2,0($fp)
_Label20:
lw $sp,-4($fp)
lw $fp,-8($fp)
lw $ra,-12($sp)
jr $ra
MAIN_0:
subi $sp,$sp,4048
addi $fp,$sp,4048
sw $ra,-12($fp)
lw $s0,-28($fp)
sw $sp,-4($sp)
sw $fp,-8($sp)
sw $fp,-16($sp)
subi $sp,$sp,500
jal input_1
lw $s0,-28($fp)
sw $sp,-4($sp)
sw $fp,-8($sp)
sw $fp,-16($sp)
li $t0,0
sw $t0,-20($sp)
li $t0,6
sw $t0,-24($sp)
subi $sp,$sp,500
jal qsort_3
lw $s0,-28($fp)
lw $t0,0($sp)
subi $sp,$sp,4
add $t4,$zero,$t0
mul $t2,$t4,5
sw $t2,-24($fp)
li $t0,0
add $s0,$zero,$t0
_Label21:
bgt $s0,6,_Label22
la $a0,_Str_0
li $v0,4
syscall
addi $t0,$fp,-32
sll $t1,$s0,2
sub $t2,$t0,$t1
subi $sp,$sp,4
lw $t5,0($t2)
li $v0,1
add $a0,$t5,$zero
syscall
addi $s0,$s0,1
jal _Label21
_Label22:
la $a0,_Str_1
li $v0,4
syscall
addi $t0,$fp,-32
li $t1,0
sll $t1,$t1,2
sub $t2,$t0,$t1
subi $sp,$sp,4
lw $t6,0($t2)
li $v0,1
add $a0,$t6,$zero
syscall
la $a0,_Str_2
li $v0,4
syscall
addi $t0,$fp,-32
li $t1,0
sll $t1,$t1,2
sub $t2,$t0,$t1
subi $sp,$sp,4
lw $t7,0($t2)
sw $sp,-4($sp)
sw $fp,-8($sp)
sw $fp,-16($sp)
sw $t7,-20($sp)
sw $s0,-28($fp)
subi $sp,$sp,500
jal fib_2
lw $t0,0($sp)
subi $sp,$sp,4
add $t8,$zero,$t0
li $v0,1
add $a0,$t8,$zero
syscall
la $a0,_Str_3
li $v0,4
syscall
addi $t0,$fp,-32
li $t1,1
sll $t1,$t1,2
sub $t2,$t0,$t1
subi $sp,$sp,4
lw $t9,0($t2)
li $v0,1
add $a0,$t9,$zero
syscall
la $a0,_Str_4
li $v0,4
syscall
addi $t0,$fp,-32
li $t1,2
sll $t1,$t1,2
sub $t2,$t0,$t1
subi $sp,$sp,4
lw $t3,0($t2)
li $v0,1
add $a0,$t3,$zero
syscall
la $a0,_Str_5
li $v0,4
syscall
addi $t0,$fp,-32
li $t1,1
sll $t1,$t1,2
sub $t2,$t0,$t1
subi $sp,$sp,4
lw $t4,0($t2)
addi $t0,$fp,-32
li $t1,2
sll $t1,$t1,2
sub $t2,$t0,$t1
subi $sp,$sp,4
lw $t5,0($t2)
sw $sp,-4($sp)
sw $fp,-8($sp)
sw $fp,-16($sp)
sw $t4,-20($sp)
sw $t5,-24($sp)
sw $s0,-28($fp)
subi $sp,$sp,500
jal gcd_6
lw $t0,0($sp)
subi $sp,$sp,4
add $t6,$zero,$t0
li $v0,1
add $a0,$t6,$zero
syscall
