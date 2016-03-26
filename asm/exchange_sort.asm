# allocate memory for the array
  addi $0,$1,10
  sw $0,$1,100
  addi $0,$1,3
  sw $0,$1,101
  addi $0,$1,7
  sw $0,$1,102
  addi $1,$0,2
  sw $0,$1,103
  addi $1,$0,8
  sw $0,$1,104
  addi $1,$0,9
  sw $0,$1,105
  addi $1,$0,4
  sw $0,$1,106
  addi $1,$0,5
  sw $0,$1,107
  addi $1,$0,108
  sw $0,$1,11
  addi $0,$1,1
  sw $0,$1,1
# store the length of the array
  addi $0,$1,10
  sw $0,$1,110
# $4 = 10
  lw $0,$4,110
# $1 = i, i = 0
  addi $0,$1,0
# start outer loop
BEGINFOR1:
#$7 = $4 - 1
  subi $4,$7,1
#$7 = $1 - $7
  sub $1,$7,$7
# if(i >= len - 1) jump to the end of outer loop
  bgez $7,ENDFOR1
# start inner loop
#j = i + 1
  addi $1,$2,1
BEGINFOR2: 
#$7 = $2(j) - $4(len)
  sub $2,$4,$7 
#if(j >= len) jump to the end of inner loop
  bgez $7,ENDFOR2
# $4 = a[i], $5 = a[j]
# load $4 = a[i]
  lw $1,$4,0
# load $5 = a[j]
  lw $2,$5,0
# $6 = $4 - $5
  sub $4,$5,$6 
  bgez $6,ENDIF
# swap array elements
# $3 is the temp
# swap 
# temp = $4
  add $3,$0,$4
# $4 = $5
  add $4,$0,$5
# a[i] = $4
  sw $1,$4,0
# $5 = $3
  add $3,$0,$5
# a[j] = $5
  sw $2,$5,0
ENDIF:
#j = j + 1
  addi $2,$2,1
#jump to the beginning of inner loop
  j BEGINFOR2
ENDFOR2:
#i = i + 1
  addi $1,$1,1
# jump to the beginning of outer loop
  j BEGINFOR1
ENDFOR1:
  break
