# allocate memory for the array
  addi $1,$0,10
  sw $0,$1,100
  addi $1,$0,3
  sw $0,$1,101
  addi $1,$0,7
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
  mem[110] = 10
# $4 = 10
  lw $0,$4,110
# $1 = i, $2 = j
  addi $1,$1,0
# start outer loop
BEGINFOR1:
  subi $7,$4,1 #$7 = length - 1
  subi $7,$7,$1 #$7 = $1 - $7
  bgez $7,ENDFOR1
# start inner loop
  addi $2,$1,1 #j = i + 1
BEGINFOR2: 
  subi $7,$2,$4 #$7 = $4(len) - $2(j)
  bgez $7,ENDFOR2 #if(j > len)
# $4 = a[i], $5 = a[j]
  lw $0,$4,$1 # load $4 = a[i]
  lw $0,$5,$2 # load $5 = a[j] 
# $6 = $4 - $5
  subi $6,$4,$5 
  bgez $6,ENDIF
# swap array elements
# $3 is the temp
# swap 
  addi $3,$0,$4 # temp = $4
  addi $4,$0,$5 # $4 = $5
  sw $0,$4,$1 # a[i] = $4
  addi $5,$0,$3 # $5 = $3
  sw $0,$5,$2 # a[j] = $5 
ENDIF:
  addi $2,$2,1 #j = j + 1
  j BEGINFOR2 #jump to the beginning of inner loop
ENDFOR2:
  addi $1,$1,1 #i = i + 1
  j BEGINFOR1 # jump to the beginning of outer loop
ENDFOR1:
  break

