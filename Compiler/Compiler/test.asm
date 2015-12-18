.data
_Str_0: .asciiz "After Abs :"
_Str_1: .asciiz "\n"
_Str_2: .asciiz "Now is the Swap\n"
_Str_3: .asciiz "Swap get the parameter 0 is "
_Str_4: .asciiz "\n"
_Str_5: .asciiz "Swap get the parameter 1 is "
_Str_6: .asciiz "\n"
_Str_7: .asciiz "Now is the swap\n"
_Str_8: .asciiz "swap get the parameter 0 is "
_Str_9: .asciiz "\n"
_Str_10: .asciiz "swap get the parameter 1 is "
_Str_11: .asciiz "\n"
_Str_12: .asciiz "The lever now is "
_Str_13: .asciiz "\n"
_Str_14: .asciiz "The value in array i[9-lever] is "
_Str_15: .asciiz "\n"
_Str_16: .asciiz "The value in array i[lever] is "
_Str_17: .asciiz "\n"
_Str_18: .asciiz "The array is reversed\n"
_Str_19: .asciiz "The array is assign\n"
_Str_20: .asciiz "The i[0] is "
_Str_21: .asciiz "\n"
_Str_22: .asciiz "The i[1] is "
_Str_23: .asciiz "\n"
_Str_24: .asciiz "The i[2] is "
_Str_25: .asciiz "\n"
_Str_26: .asciiz "The i[3] is "
_Str_27: .asciiz "\n"
_Str_28: .asciiz "The i[4] is "
_Str_29: .asciiz "\n"
_Str_30: .asciiz "The i[5] is "
_Str_31: .asciiz "\n"
_Str_32: .asciiz "The i[6] is "
_Str_33: .asciiz "\n"
_Str_34: .asciiz "The i[7] is "
_Str_35: .asciiz "\n"
_Str_36: .asciiz "The i[8] is "
_Str_37: .asciiz "\n"
_Str_38: .asciiz "The i[9] is "
_Str_39: .asciiz "\n"
_Str_40: .asciiz "Now the procedure named Reverse is called\n"
_Str_41: .asciiz "Please input two numbers now\n"
_Str_42: .asciiz "Adding the two numbers which you input before together and you get the result "
_Str_43: .asciiz "\n"
_Str_44: .asciiz "h is "
_Str_45: .asciiz "\n"
_Str_46: .asciiz "Now the f is "
_Str_47: .asciiz "\n"
_Str_48: .asciiz "Now the f is "
_Str_49: .asciiz "\n"
_Str_50: .asciiz "Now the g is "
_Str_51: .asciiz "\n"
_Str_52: .asciiz "Now the g is "
_Str_53: .asciiz "\n"
_Str_54: .asciiz "Now the g is "
_Str_55: .asciiz "\n"
_Str_56: .asciiz "Now the f is "
_Str_57: .asciiz "\n"
_Str_58: .asciiz "Now f is "
_Str_59: .asciiz ",and g is "
_Str_60: .asciiz "\n"
_Str_61: .asciiz "Now f is "
_Str_62: .asciiz ",and g is "
_Str_63: .asciiz "\n"
_Str_64: .asciiz "Now begin to Swap array i\n"
_Str_65: .asciiz "The i[0] is "
_Str_66: .asciiz "\n"
_Str_67: .asciiz "The i[1] is "
_Str_68: .asciiz "\n"
_Str_69: .asciiz "The i[2] is "
_Str_70: .asciiz "\n"
_Str_71: .asciiz "The i[3] is "
_Str_72: .asciiz "\n"
_Str_73: .asciiz "The i[4] is "
_Str_74: .asciiz "\n"
_Str_75: .asciiz "The i[5] is "
_Str_76: .asciiz "\n"
_Str_77: .asciiz "The i[6] is "
_Str_78: .asciiz "\n"
_Str_79: .asciiz "The i[7] is "
_Str_80: .asciiz "\n"
_Str_81: .asciiz "The i[8] is "
_Str_82: .asciiz "\n"
_Str_83: .asciiz "The i[9] is "
_Str_84: .asciiz "\n"
.text
jal MAIN_0
Abs_1:
addi $fp,$sp,500
sw $ra,-12($fp)
lw $t0,-20($fp)
lw $t0,0($t0)
bge $t0,0,_Label0
lw $t0,-20($fp)
lw $t0,0($t0)
sub $t2,$zero,$t0
lw $t1,-20($fp)
sw $t2,0($t1)
jal _Label1
_Label0:
_Label1:
lw $t0,-24($fp)
bge $t0,0,_Label2
lw $t0,-24($fp)
sub $t2,$zero,$t0
sw $t2,-24($fp)
jal _Label3
_Label2:
_Label3:
la $a0,_Str_0
li $v0,4
syscall
la $a0,_Str_1
li $v0,4
syscall
lw $sp,-4($fp)
lw $fp,-8($fp)
lw $ra,-12($sp)
jr $ra
Abs1_2:
addi $fp,$sp,500
sw $ra,-12($fp)
lw $t0,-20($fp)
lw $t0,0($t0)
bge $t0,0,_Label4
lw $t0,-20($fp)
lw $t0,0($t0)
sub $t2,$zero,$t0
sw $t2,-24($fp)
jal _Label5
_Label4:
lw $t0,-20($fp)
lw $t0,0($t0)
add $t2,$zero,$t0
sw $t2,-24($fp)
_Label5:
lw $t0,-24($fp)
add $t2,$zero,$t0
lw $t1,-20($fp)
sw $t2,0($t1)
lw $sp,-4($fp)
lw $fp,-8($fp)
lw $ra,-12($sp)
jr $ra
Abs2_3:
addi $fp,$sp,500
sw $ra,-12($fp)
lw $t0,-20($fp)
bge $t0,0,_Label6
lw $t0,-20($fp)
sub $t2,$zero,$t0
sw $t2,-24($fp)
jal _Label7
_Label6:
lw $t0,-20($fp)
add $t2,$zero,$t0
sw $t2,-24($fp)
_Label7:
lw $t0,-24($fp)
add $t2,$zero,$t0
sw $t2,-20($fp)
lw $sp,-4($fp)
lw $fp,-8($fp)
lw $ra,-12($sp)
jr $ra
Sub_4:
addi $fp,$sp,500
sw $ra,-12($fp)
lw $t0,-20($fp)
lw $t0,0($t0)
lw $t1,-24($fp)
lw $t1,0($t1)
sub $t2,$t0,$t1
sw $t2,0($fp)
lw $sp,-4($fp)
lw $fp,-8($fp)
lw $ra,-12($sp)
jr $ra
test_6:
addi $fp,$sp,500
sw $ra,-12($fp)
li $t0,2
add $t2,$zero,$t0
sw $t2,-32($fp)
lw $sp,-4($fp)
lw $fp,-8($fp)
lw $ra,-12($sp)
jr $ra
Swap_5:
addi $fp,$sp,500
sw $ra,-12($fp)
la $a0,_Str_2
li $v0,4
syscall
la $a0,_Str_3
li $v0,4
syscall
li $v0,1
lw $a0,-20($fp)
lw $a0,0($a0)
syscall
la $a0,_Str_4
li $v0,4
syscall
la $a0,_Str_5
li $v0,4
syscall
li $v0,1
lw $a0,-24($fp)
lw $a0,0($a0)
syscall
la $a0,_Str_6
li $v0,4
syscall
lw $t0,-20($fp)
lw $t0,0($t0)
add $t2,$zero,$t0
sw $t2,-28($fp)
lw $t0,-24($fp)
lw $t0,0($t0)
add $t2,$zero,$t0
lw $t1,-20($fp)
sw $t2,0($t1)
lw $t0,-28($fp)
add $t2,$zero,$t0
lw $t1,-24($fp)
sw $t2,0($t1)
lw $sp,-4($fp)
lw $fp,-8($fp)
lw $ra,-12($sp)
jr $ra
swap_7:
addi $fp,$sp,500
sw $ra,-12($fp)
la $a0,_Str_7
li $v0,4
syscall
la $a0,_Str_8
li $v0,4
syscall
li $v0,1
lw $a0,-20($fp)
syscall
la $a0,_Str_9
li $v0,4
syscall
la $a0,_Str_10
li $v0,4
syscall
li $v0,1
lw $a0,-24($fp)
syscall
la $a0,_Str_11
li $v0,4
syscall
lw $t0,-20($fp)
add $t2,$zero,$t0
sw $t2,-28($fp)
lw $t0,-24($fp)
add $t2,$zero,$t0
sw $t2,-20($fp)
lw $t0,-28($fp)
add $t2,$zero,$t0
sw $t2,-24($fp)
lw $sp,-4($fp)
lw $fp,-8($fp)
lw $ra,-12($sp)
jr $ra
Reverse_8:
addi $fp,$sp,500
sw $ra,-12($fp)
lw $t0,-20($fp)
bge $t0,5,_Label8
la $a0,_Str_12
li $v0,4
syscall
li $v0,1
lw $a0,-20($fp)
syscall
la $a0,_Str_13
li $v0,4
syscall
la $a0,_Str_14
li $v0,4
syscall
li $t0,9
lw $t1,-20($fp)
sub $t2,$t0,$t1
sw $t2,-24($fp)
lw $t0,-16($fp)
addi $t0,$t0,-36
lw $t1,-24($fp)
sll $t1,$t1,2
sub $t2,$t0,$t1
addi $t1,$fp,-28
lw $t0,0($t2)
sw $t0,0($t1)
li $v0,1
lw $a0,-28($fp)
syscall
la $a0,_Str_15
li $v0,4
syscall
la $a0,_Str_16
li $v0,4
syscall
lw $t0,-16($fp)
addi $t0,$t0,-36
lw $t1,-20($fp)
sll $t1,$t1,2
sub $t2,$t0,$t1
addi $t1,$fp,-32
lw $t0,0($t2)
sw $t0,0($t1)
li $v0,1
lw $a0,-32($fp)
syscall
la $a0,_Str_17
li $v0,4
syscall
li $t0,9
lw $t1,-20($fp)
sub $t2,$t0,$t1
sw $t2,-36($fp)
sw $sp,-4($sp)
sw $fp,-8($sp)
lw $t0,-16($fp)
sw $t0,-16($sp)
lw $t0,-16($fp)
addi $t0,$t0,-36
lw $t1,-20($fp)
sll $t1,$t1,2
sub $t2,$t0,$t1
sw $t2,-20($sp)
lw $t0,-16($fp)
addi $t0,$t0,-36
lw $t1,-36($fp)
sll $t1,$t1,2
sub $t2,$t0,$t1
sw $t2,-24($sp)
subi $sp,$sp,500
jal Swap_5
lw $t0,-20($fp)
addi $t2,$t0,1
sw $t2,-40($fp)
sw $sp,-4($sp)
sw $fp,-8($sp)
lw $t0,-16($fp)
sw $t0,-16($sp)
lw $t0,-40($fp)
sw $t0,-20($sp)
subi $sp,$sp,500
jal Reverse_8
jal _Label9
_Label8:
la $a0,_Str_18
li $v0,4
syscall
_Label9:
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
sw $t2,-24($fp)
lw $t0,-24($fp)
addi $t2,$t0,5
sw $t2,-28($fp)
_Label10:
addi $t0,$fp,-36
lw $t1,-24($fp)
sll $t1,$t1,2
sub $t2,$t0,$t1
addi $t1,$fp,-24
lw $t0,0($t1)
sw $t0,0($t2)
lw $t0,-24($fp)
addi $t2,$t0,1
sw $t2,-24($fp)
lw $t0,-24($fp)
lw $t1,-28($fp)
blt $t0,$t1,_Label10
lw $t0,-24($fp)
add $t2,$zero,$t0
sw $t2,-32($fp)
_Label11:
li $t0,1
lw $t1,-28($fp)
mulo $t2,$t0,$t1
sw $t2,-88($fp)
lw $t0,-28($fp)
lw $t1,-88($fp)
add $t2,$t0,$t1
sw $t2,-92($fp)
lw $t0,-32($fp)
lw $t1,-92($fp)
bgt $t0,$t1,_Label12
addi $t0,$fp,-36
lw $t1,-32($fp)
sll $t1,$t1,2
sub $t2,$t0,$t1
addi $t1,$fp,-32
lw $t0,0($t1)
sw $t0,0($t2)
lw $t0,-32($fp)
addi $t2,$t0,1
sw $t2,-32($fp)
jal _Label11
_Label12:
li $t0,5678
add $t2,$zero,$t0
sw $t2,-32($fp)
la $a0,_Str_19
li $v0,4
syscall
la $a0,_Str_20
li $v0,4
syscall
addi $t0,$fp,-36
li $t1,0
sll $t1,$t1,2
sub $t2,$t0,$t1
addi $t1,$fp,-96
lw $t0,0($t2)
sw $t0,0($t1)
li $v0,1
lw $a0,-96($fp)
syscall
la $a0,_Str_21
li $v0,4
syscall
la $a0,_Str_22
li $v0,4
syscall
addi $t0,$fp,-36
li $t1,1
sll $t1,$t1,2
sub $t2,$t0,$t1
addi $t1,$fp,-100
lw $t0,0($t2)
sw $t0,0($t1)
li $v0,1
lw $a0,-100($fp)
syscall
la $a0,_Str_23
li $v0,4
syscall
la $a0,_Str_24
li $v0,4
syscall
addi $t0,$fp,-36
li $t1,2
sll $t1,$t1,2
sub $t2,$t0,$t1
addi $t1,$fp,-104
lw $t0,0($t2)
sw $t0,0($t1)
li $v0,1
lw $a0,-104($fp)
syscall
la $a0,_Str_25
li $v0,4
syscall
la $a0,_Str_26
li $v0,4
syscall
addi $t0,$fp,-36
li $t1,3
sll $t1,$t1,2
sub $t2,$t0,$t1
addi $t1,$fp,-108
lw $t0,0($t2)
sw $t0,0($t1)
li $v0,1
lw $a0,-108($fp)
syscall
la $a0,_Str_27
li $v0,4
syscall
la $a0,_Str_28
li $v0,4
syscall
addi $t0,$fp,-36
li $t1,4
sll $t1,$t1,2
sub $t2,$t0,$t1
addi $t1,$fp,-112
lw $t0,0($t2)
sw $t0,0($t1)
li $v0,1
lw $a0,-112($fp)
syscall
la $a0,_Str_29
li $v0,4
syscall
la $a0,_Str_30
li $v0,4
syscall
addi $t0,$fp,-36
li $t1,5
sll $t1,$t1,2
sub $t2,$t0,$t1
addi $t1,$fp,-116
lw $t0,0($t2)
sw $t0,0($t1)
li $v0,1
lw $a0,-116($fp)
syscall
la $a0,_Str_31
li $v0,4
syscall
la $a0,_Str_32
li $v0,4
syscall
addi $t0,$fp,-36
li $t1,6
sll $t1,$t1,2
sub $t2,$t0,$t1
addi $t1,$fp,-120
lw $t0,0($t2)
sw $t0,0($t1)
li $v0,1
lw $a0,-120($fp)
syscall
la $a0,_Str_33
li $v0,4
syscall
la $a0,_Str_34
li $v0,4
syscall
addi $t0,$fp,-36
li $t1,7
sll $t1,$t1,2
sub $t2,$t0,$t1
addi $t1,$fp,-124
lw $t0,0($t2)
sw $t0,0($t1)
li $v0,1
lw $a0,-124($fp)
syscall
la $a0,_Str_35
li $v0,4
syscall
la $a0,_Str_36
li $v0,4
syscall
addi $t0,$fp,-36
li $t1,8
sll $t1,$t1,2
sub $t2,$t0,$t1
addi $t1,$fp,-128
lw $t0,0($t2)
sw $t0,0($t1)
li $v0,1
lw $a0,-128($fp)
syscall
la $a0,_Str_37
li $v0,4
syscall
la $a0,_Str_38
li $v0,4
syscall
addi $t0,$fp,-36
li $t1,9
sll $t1,$t1,2
sub $t2,$t0,$t1
addi $t1,$fp,-132
lw $t0,0($t2)
sw $t0,0($t1)
li $v0,1
lw $a0,-132($fp)
syscall
la $a0,_Str_39
li $v0,4
syscall
la $a0,_Str_40
li $v0,4
syscall
sw $sp,-4($sp)
sw $fp,-8($sp)
sw $fp,-16($sp)
li $t0,0
sw $t0,-20($sp)
subi $sp,$sp,500
jal Reverse_8
la $a0,_Str_41
li $v0,4
syscall
li $v0,5
syscall
addi $t0,$fp,-24
sw $v0,0($t0)
li $v0,5
syscall
addi $t0,$fp,-28
sw $v0,0($t0)
la $a0,_Str_42
li $v0,4
syscall
sw $sp,-4($sp)
sw $fp,-8($sp)
sw $fp,-16($sp)
addi $t0,$fp,-28
li $t1,0
sll $t1,$t1,2
sub $t2,$t0,$t1
sw $t2,-20($sp)
addi $t0,$fp,-24
li $t1,0
sll $t1,$t1,2
sub $t2,$t0,$t1
sw $t2,-24($sp)
subi $sp,$sp,500
jal Sub_4
lw $t0,0($sp)
add $t2,$zero,$t0
sw $t2,-136($fp)
li $v0,1
lw $a0,-136($fp)
syscall
la $a0,_Str_43
li $v0,4
syscall
sw $sp,-4($sp)
sw $fp,-8($sp)
sw $fp,-16($sp)
addi $t0,$fp,-24
li $t1,0
sll $t1,$t1,2
sub $t2,$t0,$t1
sw $t2,-20($sp)
addi $t0,$fp,-28
li $t1,0
sll $t1,$t1,2
sub $t2,$t0,$t1
sw $t2,-24($sp)
subi $sp,$sp,500
jal Sub_4
lw $t0,0($sp)
add $t2,$zero,$t0
sw $t2,-140($fp)
lw $t0,-140($fp)
add $t2,$zero,$t0
sw $t2,-32($fp)
la $a0,_Str_44
li $v0,4
syscall
li $v0,1
lw $a0,-32($fp)
syscall
la $a0,_Str_45
li $v0,4
syscall
la $a0,_Str_46
li $v0,4
syscall
li $v0,1
lw $a0,-24($fp)
syscall
sw $sp,-4($sp)
sw $fp,-8($sp)
sw $fp,-16($sp)
addi $t0,$fp,-24
li $t1,0
sll $t1,$t1,2
sub $t2,$t0,$t1
sw $t2,-20($sp)
subi $sp,$sp,500
jal Abs1_2
la $a0,_Str_47
li $v0,4
syscall
la $a0,_Str_48
li $v0,4
syscall
li $v0,1
lw $a0,-24($fp)
syscall
la $a0,_Str_49
li $v0,4
syscall
la $a0,_Str_50
li $v0,4
syscall
li $v0,1
lw $a0,-28($fp)
syscall
la $a0,_Str_51
li $v0,4
syscall
sw $sp,-4($sp)
sw $fp,-8($sp)
sw $fp,-16($sp)
lw $t0,-28($fp)
sw $t0,-20($sp)
subi $sp,$sp,500
jal Abs2_3
la $a0,_Str_52
li $v0,4
syscall
li $v0,1
lw $a0,-28($fp)
syscall
la $a0,_Str_53
li $v0,4
syscall
sw $sp,-4($sp)
sw $fp,-8($sp)
sw $fp,-16($sp)
addi $t0,$fp,-24
li $t1,0
sll $t1,$t1,2
sub $t2,$t0,$t1
sw $t2,-20($sp)
addi $t0,$fp,-28
li $t1,0
sll $t1,$t1,2
sub $t2,$t0,$t1
sw $t2,-24($sp)
subi $sp,$sp,500
jal Swap_5
la $a0,_Str_54
li $v0,4
syscall
li $v0,1
lw $a0,-28($fp)
syscall
la $a0,_Str_55
li $v0,4
syscall
la $a0,_Str_56
li $v0,4
syscall
li $v0,1
lw $a0,-24($fp)
syscall
la $a0,_Str_57
li $v0,4
syscall
li $t0,20
sub $t2,$zero,$t0
sw $t2,-24($fp)
li $t0,23
sub $t2,$zero,$t0
sw $t2,-28($fp)
la $a0,_Str_58
li $v0,4
syscall
li $v0,1
lw $a0,-24($fp)
syscall
la $a0,_Str_59
li $v0,4
syscall
li $v0,1
lw $a0,-28($fp)
syscall
la $a0,_Str_60
li $v0,4
syscall
sw $sp,-4($sp)
sw $fp,-8($sp)
sw $fp,-16($sp)
addi $t0,$fp,-24
li $t1,0
sll $t1,$t1,2
sub $t2,$t0,$t1
sw $t2,-20($sp)
lw $t0,-28($fp)
sw $t0,-24($sp)
subi $sp,$sp,500
jal Abs_1
la $a0,_Str_61
li $v0,4
syscall
li $v0,1
lw $a0,-24($fp)
syscall
la $a0,_Str_62
li $v0,4
syscall
li $v0,1
lw $a0,-28($fp)
syscall
la $a0,_Str_63
li $v0,4
syscall
la $a0,_Str_64
li $v0,4
syscall
li $t0,0
add $t2,$zero,$t0
sw $t2,-32($fp)
la $a0,_Str_65
li $v0,4
syscall
addi $t0,$fp,-36
li $t1,0
sll $t1,$t1,2
sub $t2,$t0,$t1
addi $t1,$fp,-144
lw $t0,0($t2)
sw $t0,0($t1)
li $v0,1
lw $a0,-144($fp)
syscall
la $a0,_Str_66
li $v0,4
syscall
la $a0,_Str_67
li $v0,4
syscall
addi $t0,$fp,-36
li $t1,1
sll $t1,$t1,2
sub $t2,$t0,$t1
addi $t1,$fp,-148
lw $t0,0($t2)
sw $t0,0($t1)
li $v0,1
lw $a0,-148($fp)
syscall
la $a0,_Str_68
li $v0,4
syscall
la $a0,_Str_69
li $v0,4
syscall
addi $t0,$fp,-36
li $t1,2
sll $t1,$t1,2
sub $t2,$t0,$t1
addi $t1,$fp,-152
lw $t0,0($t2)
sw $t0,0($t1)
li $v0,1
lw $a0,-152($fp)
syscall
la $a0,_Str_70
li $v0,4
syscall
la $a0,_Str_71
li $v0,4
syscall
addi $t0,$fp,-36
li $t1,3
sll $t1,$t1,2
sub $t2,$t0,$t1
addi $t1,$fp,-156
lw $t0,0($t2)
sw $t0,0($t1)
li $v0,1
lw $a0,-156($fp)
syscall
la $a0,_Str_72
li $v0,4
syscall
la $a0,_Str_73
li $v0,4
syscall
addi $t0,$fp,-36
li $t1,4
sll $t1,$t1,2
sub $t2,$t0,$t1
addi $t1,$fp,-160
lw $t0,0($t2)
sw $t0,0($t1)
li $v0,1
lw $a0,-160($fp)
syscall
la $a0,_Str_74
li $v0,4
syscall
la $a0,_Str_75
li $v0,4
syscall
addi $t0,$fp,-36
li $t1,5
sll $t1,$t1,2
sub $t2,$t0,$t1
addi $t1,$fp,-164
lw $t0,0($t2)
sw $t0,0($t1)
li $v0,1
lw $a0,-164($fp)
syscall
la $a0,_Str_76
li $v0,4
syscall
la $a0,_Str_77
li $v0,4
syscall
addi $t0,$fp,-36
li $t1,6
sll $t1,$t1,2
sub $t2,$t0,$t1
addi $t1,$fp,-168
lw $t0,0($t2)
sw $t0,0($t1)
li $v0,1
lw $a0,-168($fp)
syscall
la $a0,_Str_78
li $v0,4
syscall
la $a0,_Str_79
li $v0,4
syscall
addi $t0,$fp,-36
li $t1,7
sll $t1,$t1,2
sub $t2,$t0,$t1
addi $t1,$fp,-172
lw $t0,0($t2)
sw $t0,0($t1)
li $v0,1
lw $a0,-172($fp)
syscall
la $a0,_Str_80
li $v0,4
syscall
la $a0,_Str_81
li $v0,4
syscall
addi $t0,$fp,-36
li $t1,8
sll $t1,$t1,2
sub $t2,$t0,$t1
addi $t1,$fp,-176
lw $t0,0($t2)
sw $t0,0($t1)
li $v0,1
lw $a0,-176($fp)
syscall
la $a0,_Str_82
li $v0,4
syscall
la $a0,_Str_83
li $v0,4
syscall
addi $t0,$fp,-36
li $t1,9
sll $t1,$t1,2
sub $t2,$t0,$t1
addi $t1,$fp,-180
lw $t0,0($t2)
sw $t0,0($t1)
li $v0,1
lw $a0,-180($fp)
syscall
la $a0,_Str_84
li $v0,4
syscall
