#allocate memory for matrices
#left matrix
  addi $0,$1,1
  sb $0,$1,500
  sb $0,$1,501
  sb $0,$1,502
  sb $0,$1,503
  sb $0,$1,504
  sb $0,$1,505
  sb $0,$1,506
  sb $0,$1,507
  addi $0,$1,2
  sb $0,$1,508
  sb $0,$1,509
  sb $0,$1,510
  sb $0,$1,511
  sb $0,$1,512
  sb $0,$1,513
  sb $0,$1,514
  sb $0,$1,515
  addi $0,$1,3
  sb $0,$1,516
  sb $0,$1,517
  sb $0,$1,518
  sb $0,$1,519
  sb $0,$1,520
  sb $0,$1,521
  sb $0,$1,522
  sb $0,$1,523
  addi $0,$1,4
  sb $0,$1,524
  sb $0,$1,525
  sb $0,$1,526
  sb $0,$1,527
  sb $0,$1,528
  sb $0,$1,529
  sb $0,$1,530
  sb $0,$1,531
#right matrix
  addi $0,$1,1
  sb $0,$1,532
  sb $0,$1,536
  sb $0,$1,540
  sb $0,$1,544
  sb $0,$1,548
  sb $0,$1,552
  sb $0,$1,556
  sb $0,$1,560
  addi $0,$1,2
  sb $0,$1,533
  sb $0,$1,537
  sb $0,$1,541
  sb $0,$1,545
  sb $0,$1,549
  sb $0,$1,553
  sb $0,$1,557
  sb $0,$1,561
  addi $0,$1,3
  sb $0,$1,534
  sb $0,$1,538
  sb $0,$1,542
  sb $0,$1,546
  sb $0,$1,550
  sb $0,$1,554
  sb $0,$1,558
  sb $0,$1,562
  addi $0,$1,4
  sb $0,$1,535
  sb $0,$1,539
  sb $0,$1,543
  sb $0,$1,547
  sb $0,$1,551
  sb $0,$1,555
  sb $0,$1,559
  sb $0,$1,563
#result matrix
  addi $0,$1,0
  sb $0,$1,564
  sb $0,$1,565
  sb $0,$1,566
  sb $0,$1,567
  sb $0,$1,568
  sb $0,$1,569
  sb $0,$1,570
  sb $0,$1,571
  sb $0,$1,572
  sb $0,$1,573
  sb $0,$1,574
  sb $0,$1,575
  sb $0,$1,576
  sb $0,$1,577
  sb $0,$1,578
  sb $0,$1,579
# lM[a][b]*rM[b][c]=res[a][c]
# $1 = a
# $2 = b
# $3 = c
# $4 = i, $5 = k, $6 = j
# $7 = lM[i][k], $8 = rM[k][j], $9=res[i][j]
  addi $0,$1,4
  addi $0,$2,8
  addi $0,$3,4
# i = 0
  addi $0,$4,0
# start loop 1
BEGINFOR1:
# $10 = i - a
  sub $4,$1,$10
# if(i - a >= 0) break out of loop1
  bgez $10,ENDFOR1
  addi $0,$6,0
# start loop 2
BEGINFOR2:
  sub $6,$3,$10
  bgez $10,ENDFOR2
  addi $0,$5,0
# start loop 3
# $9 = res[i][j] = 0
  addi $0,$9,0
BEGINFOR3:
  sub $5,$2,$10
  bgez $10,ENDFOR3
# res[i][j] += lM[i][k]*rM[k][j]
# address of matrix[i][j] = i*row_length + j + start_address
# $10 = i*b + k
  mul $4,$2,$10
  add $5,$10,$10
# $7 = lM[i][k]
  lb $10,$7,500
# $10 = k*c + j
  mul $5,$3,$10
  add $6,$10,$10
# $8 = rM[k][j]
  lb $10,$8,532
# $10 = lM[i][k]*rM[k][j]
  mul $7,$8,$10
# $9 += lM[i][k]*rM[k][j]
  add $10,$9,$9
# k = k + 1
  addi $5,$5,1
  beq $0,$0,BEGINFOR3
ENDFOR3:
# write $9 to res[i][j]
  mul $4,$1,$10
  add $6,$10,$10
  sb $10,$9,564
# j = j + 1
  addi $6,$6,1
  beq $0,$0,BEGINFOR2
ENDFOR2:
# i = i + 1
  addi $4,$4,1
  beq $0,$0,BEGINFOR1
ENDFOR1:
  break
