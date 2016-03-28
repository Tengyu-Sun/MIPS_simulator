#allocate memory for matrices
#left matrix
  addi $0,$1,1
  sb $0,$1,500
  addi $0,$1,2
  sb $0,$1,501
  addi $0,$1,3
  sb $0,$1,502
  addi $0,$1,4
  sb $0,$1,503
#right matrix
  addi $0,$1,5
  sb $0,$1,504
  addi $0,$1,6
  sb $0,$1,505
  addi $0,$1,7
  sb $0,$1,506
  addi $0,$1,8
  sb $0,$1,507
#result matrix
  addi $0,$1,0
  sb $0,$1,508
  addi $0,$1,0
  sb $0,$1,509
  addi $0,$1,0
  sb $0,$1,510
  addi $0,$1,0
  sb $0,$1,511
# lM[a][b]*rM[b][c]=res[a][c]
# $1 = a
# $2 = b
# $3 = c
# $4 = i, $5 = j, $6 = k
# $7 = lM[i][k], $8 = rM[k][k], $9=res[i][j]
  addi $0,$1,2
  addi $0,$2,2
  addi $0,$3,2
# i = 0
  addi $0,$4,0
# start loop 1
BEGINFOR1:
# $10 = i - a
  sub $4,$1,$10
# if(i - a >= 0) break out of loop1
  bgez $10,ENDFOR1
  addi $0,$5,0
# start loop 2
BEGINFOR2:
  sub $5,$2,$10
  bgez $10,ENDFOR2
  addi $0,$6,0
# start loop 3
# $9 = res[i][j] = 0
  addi $0,$9,0
BEGINFOR3:
  sub $6,$3,$10
  bgez $10,ENDFOR3
# res[i][j] += lM[i][k]*rM[k][j]
# address of matrix[i][j] = i*row_length + j + start_address
# $10 = i*b + k
  mul $4,$2,$10
  add $6,$10,$10
# $7 = lM[i][k]
  lb $10,$7,500
# $10 = k*c + j
  mul $6,$3,$10
  add $5,$10,$10
# $8 = rM[k][j]
  lb $10,$8,504
# $10 = lM[i][k]*rM[k][j]
  mul $7,$8,$10
# $9 += lM[i][k]*rM[k][j]
  add $10,$9,$9
# k = k + 1
  addi $6,$6,1
  beq $0,$0,BEGINFOR3
ENDFOR3:
# write $9 to res[i][j]
  mul $4,$2,$10
  add $5,$10,$10
  sb $10,$9,508
# j = j + 1
  addi $5,$5,1
  beq $0,$0,BEGINFOR2
ENDFOR2:
# i = i + 1
  addi $4,$4,1
  beq $0,$0,BEGINFOR1
ENDFOR1:
  break
