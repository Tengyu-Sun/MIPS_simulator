  lb $0,$1,100
  lb $0,$2,101
L2:
  add $1,$2,$3
  bgez $3,L1
L1:  
  lb $0,$3,100
  sb $0,$3,102
  bgez $3,L2
  break
