# allocate memory for the array
  addi $0,$1,10
  sb $0,$1,500
  addi $0,$1,3
  sb $0,$1,501
  addi $0,$1,7
  sb $0,$1,502
  addi $1,$0,2
  sb $0,$1,503
  addi $1,$0,8
  sb $0,$1,504
  addi $1,$0,9
  sb $0,$1,505
  addi $1,$0,4
  sb $0,$1,506
  addi $1,$0,5
  sb $0,$1,507
  addi $1,$0,11
  sb $0,$1,508
  addi $0,$1,1
  sb $0,$1,509
# store the length of the array
  addi $0,$1,10
  sb $0,$1,510
# $4 = 10
  lb $0,$4,510
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
  lb $1,$4,0
# load $5 = a[j]
  lb $2,$5,0
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
  sb $1,$4,0
# $5 = $3
  add $3,$0,$5
# a[j] = $5
  sb $2,$5,0
ENDIF:
#j = j + 1
  addi $2,$2,1
#jump to the beginning of inner loop
  beq $0,$0,BEGINFOR2
ENDFOR2:
#i = i + 1
  addi $1,$1,1
# jump to the beginning of outer loop
  beq $0,$0,BEGINFOR1
ENDFOR1:
  break
