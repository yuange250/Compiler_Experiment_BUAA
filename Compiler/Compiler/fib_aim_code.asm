.data
.text
jal MAIN_0
fib_1:
addi $fp,$sp,500
sw $ra,-12($fp)
li $t0,1
add $t2,$zero,$t0
sw $t2,-24($fp)
li $t0,1
add $t2,$zero,$t0
sw $t2,-28($fp)
lw $t0,-20($fp)
bgt $t0,2,_Label0
li $t0,1
add $t2,$zero,$t0
sw $t2,0($fp)
jal _Label1
_Label0:
sw $sp,-4($sp)
sw $fp,-8($sp)
lw $t0,-16($fp)
sw $t0,-16($sp)
lw $t0,-20($fp)
subi $t2,$t0,1
sw $t2,-36($fp)
lw $t0,-36($fp)
sw $t0,-20($sp)
subi $sp,$sp,500
jal fib_1
lw $t0,0($sp)
add $t2,$zero,$t0
sw $t2,-40($fp)
sw $sp,-4($sp)
sw $fp,-8($sp)
lw $t0,-16($fp)
sw $t0,-16($sp)
lw $t0,-20($fp)
subi $t2,$t0,2
sw $t2,-44($fp)
lw $t0,-44($fp)
sw $t0,-20($sp)
subi $sp,$sp,500
jal fib_1
lw $t0,0($sp)
add $t2,$zero,$t0
sw $t2,-48($fp)
lw $t0,-40($fp)
lw $t1,-48($fp)
add $t2,$t0,$t1
sw $t2,0($fp)
_Label1:
lw $sp,-4($fp)
lw $fp,-8($fp)
lw $ra,-12($sp)
jr $ra
sort_2:
addi $fp,$sp,500
sw $ra,-12($fp)
li $t0,0
add $t2,$zero,$t0
sw $t2,-20($fp)
_Label2:
lw $t0,-20($fp)
bgt $t0,3,_Label7
lw $t0,-20($fp)
addi $t2,$t0,1
sw $t2,-24($fp)
_Label3:
lw $t0,-24($fp)
bgt $t0,3,_Label6
lw $t0,-16($fp)
addi $t0,$t0,-32
lw $t1,-20($fp)
sll $t1,$t1,2
sub $t2,$t0,$t1
addi $t1,$fp,-32
lw $t0,0($t2)
sw $t0,0($t1)
lw $t0,-16($fp)
addi $t0,$t0,-32
lw $t1,-24($fp)
sll $t1,$t1,2
sub $t2,$t0,$t1
addi $t1,$fp,-36
lw $t0,0($t2)
sw $t0,0($t1)
lw $t0,-32($fp)
lw $t1,-36($fp)
ble $t0,$t1,_Label4
lw $t0,-16($fp)
addi $t0,$t0,-32
lw $t1,-24($fp)
sll $t1,$t1,2
sub $t2,$t0,$t1
addi $t1,$fp,-28
lw $t0,0($t2)
sw $t0,0($t1)
lw $t0,-16($fp)
addi $t0,$t0,-32
lw $t1,-20($fp)
sll $t1,$t1,2
sub $t2,$t0,$t1
addi $t1,$fp,-40
lw $t0,0($t2)
sw $t0,0($t1)
lw $t0,-16($fp)
addi $t0,$t0,-32
lw $t1,-24($fp)
sll $t1,$t1,2
sub $t2,$t0,$t1
addi $t1,$fp,-40
lw $t0,0($t1)
sw $t0,0($t2)
lw $t0,-16($fp)
addi $t0,$t0,-32
lw $t1,-20($fp)
sll $t1,$t1,2
sub $t2,$t0,$t1
addi $t1,$fp,-28
lw $t0,0($t1)
sw $t0,0($t2)
jal _Label5
_Label4:
_Label5:
lw $t0,-24($fp)
addi $t2,$t0,1
sw $t2,-24($fp)
jal _Label3
_Label6:
lw $t0,-20($fp)
addi $t2,$t0,1
sw $t2,-20($fp)
jal _Label2
_Label7:
lw $sp,-4($fp)
lw $fp,-8($fp)
lw $ra,-12($sp)
jr $ra
MAIN_0:
subi $sp,$sp,500
addi $fp,$sp,500
sw $ra,-12($fp)
li $t0,110
add $t2,$zero,$t0
sw $t2,-28($fp)
li $t0,1
add $t2,$zero,$t0
sw $t2,-20($fp)
addi $t0,$fp,-32
li $t1,0
sll $t1,$t1,2
sub $t2,$t0,$t1
addi $t1,$fp,-20
lw $t0,0($t1)
sw $t0,0($t2)
lw $t0,-28($fp)
lw $t1,-20($fp)
add $t2,$t0,$t1
sw $t2,-48($fp)
lw $t0,-48($fp)
addi $t2,$t0,1
sw $t2,-52($fp)
addi $t0,$fp,-32
li $t1,1
sll $t1,$t1,2
sub $t2,$t0,$t1
addi $t1,$fp,-52
lw $t0,0($t1)
sw $t0,0($t2)
addi $t0,$fp,-32
li $t1,2
sll $t1,$t1,2
sub $t2,$t0,$t1
addi $t1,$fp,-28
lw $t0,0($t1)
sw $t0,0($t2)
addi $t0,$fp,-32
li $t1,1
sll $t1,$t1,2
sub $t2,$t0,$t1
addi $t1,$fp,-56
lw $t0,0($t2)
sw $t0,0($t1)
addi $t0,$fp,-32
li $t1,0
sll $t1,$t1,2
sub $t2,$t0,$t1
addi $t1,$fp,-60
lw $t0,0($t2)
sw $t0,0($t1)
lw $t0,-56($fp)
lw $t1,-60($fp)
add $t2,$t0,$t1
sw $t2,-64($fp)
addi $t0,$fp,-32
li $t1,3
sll $t1,$t1,2
sub $t2,$t0,$t1
addi $t1,$fp,-64
lw $t0,0($t1)
sw $t0,0($t2)
li $v0,5
syscall
addi $t0,$fp,-16
sw $v0,0($t0)
lw $t0,-16($fp)
bne $t0,0,_Label8
li $v0,5
syscall
addi $t0,$fp,-24
sw $v0,0($t0)
sw $sp,-4($sp)
sw $fp,-8($sp)
sw $fp,-16($sp)
lw $t0,-24($fp)
sw $t0,-20($sp)
subi $sp,$sp,500
jal fib_1
lw $t0,0($sp)
add $t2,$zero,$t0
sw $t2,-68($fp)
li $v0,1
lw $a0,-68($fp)
syscall
jal _Label13
_Label8:
lw $t0,-16($fp)
bne $t0,1,_Label11
sw $sp,-4($sp)
sw $fp,-8($sp)
sw $fp,-16($sp)
subi $sp,$sp,500
jal sort_2
li $t0,0
add $t2,$zero,$t0
sw $t2,-24($fp)
_Label9:
lw $t0,-24($fp)
bgt $t0,3,_Label10
addi $t0,$fp,-32
lw $t1,-24($fp)
sll $t1,$t1,2
sub $t2,$t0,$t1
addi $t1,$fp,-72
lw $t0,0($t2)
sw $t0,0($t1)
li $v0,1
lw $a0,-72($fp)
syscall
lw $t0,-24($fp)
addi $t2,$t0,1
sw $t2,-24($fp)
jal _Label9
_Label10:
jal _Label12
_Label11:
_Label12:
_Label13: