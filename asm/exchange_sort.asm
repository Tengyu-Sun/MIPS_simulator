// allocate memory for the array
  mem[100] = 10
  mem[101] = 3
  mem[102] = 7
  mem[103] = 2
  mem[104] = 8
  mem[105] = 9
  mem[106] = 4
  mem[107] = 5
  mem[108] = 11
  mem[109] = 1
// store the length of the array
  mem[110] = 10
  lw $5 110
// swap array elements
// $1 $2  to record  the addresses at i,j
// $3 is the temp, is temp in register?

//inner loop

  add $2 $1 1
L2: 
  sub $4,$2,$5 
  bgez $4,L1
  
  lw $1 
  
L1:  
