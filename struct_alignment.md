# struct_alignment

## Student
```
128 0x7ffee0165458
4 0x7ffee0165458
100 0x7ffee016545c
4 0x7ffee01654c0
8 0x7ffee01654c8
8 0x7ffee01654d0
```
5つの変数のサイズを合わせても124バイトなのに対し、構造体のサイズは128バイト。アドレスを見ると、ageとheightの間に4バイト分パディングされている。

## TStudent1
```
136 0x7ffee01653d0
4 0x7ffee01653d0
100 0x7ffee01653d4
4 0x7ffee0165438
8 0x7ffee0165440
8 0x7ffee0165448
1 0x7ffee0165450
```
変数のサイズの和は125バイトで構造体は136バイト。アドレスを見るとageとheightの間に4バイト分パディングされている。残りは1バイトのtagの後にパディングされていると考えることができる。

## TStudent2
```
128 0x7ffee0165350
1 0x7ffee0165350
4 0x7ffee0165354
100 0x7ffee0165358
4 0x7ffee01653bc
8 0x7ffee01653c0
8 0x7ffee01653c8
```
先ほどと同じく変数のサイズの和は125バイトだが、構造体のサイズは128バイトと先ほどより小さくなっており、最初のtagの後に3バイト分パディングされているのみであるようだ。TStudent1ではtagの前に既にバイト数の調整が行われていた上にtagの後に大きな調整を行っていたようだが、TStudent2では大きな調整が必要なtagが前に出てきたことで細かい部分を後ろ側の変数で調整していると考えられる。