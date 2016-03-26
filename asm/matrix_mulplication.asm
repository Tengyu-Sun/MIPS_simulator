#allocate memory for matrices
#left matrix
  addi $0,$1,1
  sw $0,$1,100
  addi $0,$1,2
  sw $0,$1,101
  addi $0,$1,3
  sw $0,$1,102
  addi $0,$1,4
  sw $0,$1,103
#right matrix
  addi $0,$1,5
  sw $0,$1,104
  addi $0,$1,6
  sw $0,$1,105
  addi $0,$1,7
  sw $0,$1,106
  addi $0,$1,8
  sw $0,$1,107
#result matrix
  addi $0,$1,0
  sw $0,$1,108
  addi $0,$1,0
  sw $0,$1,109
  addi $0,$1,0
  sw $0,$1,110
  addi $0,$1,0
  sw $0,$1,111
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
# $16 = res[i][j] = 0
  addi $0,$16,0
BEGINFOR3:
  sub $6,$3,$10
  bgez $10,ENDFOR3
# res[i][j] += lM[i][k]*rM[k][j]
# address of matrix[i][j] = i*row_length + j + start_address
# $11 = i*b + k 
  mul $4,$2,$11
  add $6,$11,$11
# $12 = lM[i][k]  
  lw $11,$12,100
# $13 = k*c + j
  mul $6,$3,$13
  add $5,$13,$13
# $14 = rM[k][j]
  lw $13,$14,104
# $15 = lM[i][k]*rM[k][j]
  mul $12,$14,$15
# $16 += lM[i][k]*rM[k][j]
  add $15,$16,$16
# k = k + 1
  addi $6,$6,1
  j BEGINFOR3
ENDFOR3:
# write $16 to res[i][j]
  mul $4,$2,$10
  add $5,$10,$10
  lw $10,$16,108
# j = j + 1
  addi $5,$5,1
  j BEGINFOR2
ENDFOR2:
# i = i + 1
  addi $4,$4,1
  j BEGINFOR1
ENDFOR1:
  break
