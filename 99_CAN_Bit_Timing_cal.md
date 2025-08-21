# CAN Bit-Timing 계산 방법

## 샘플링 포인트

Sampling point [%] = (seg1 +1) / (seg1 + seg2 + 1) * 100


## Bit rate (bit/s)

 = CAN CLK / div / (seg1 + seg2 + 1) 

60M Hz / 2 / x = 500000
x -1 

seg1 + seg2

seg 1 = 79
seg 2 = 20


