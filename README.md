# soft2-20211209
## 1
struct_alignment.cとstruct_alignment.mdに、指示通りの内容を書いている。
## 2以降
### 共通部分について
falling.cとbouncing.cではmy_**系の関数を仕様に従って実装している。以後の課題ではbouncing.cをもとに作っている(そのため関数のプロトタイプ宣言等はphysics2.hにて行った)。
### 2
二次元への拡張にあたって、まずはphysics.2における構造体の定義にx方向に関する定義を加えた。その後、bouncing.cにおける各種関数にx方向の操作をyと同様に書き加え、main内で物体数を適当に増やしてmy_bouncing1.cとした。bouncing.cの段階で加速度の変更等をforループで行っていたため、書き換えはそう多くならなかった。
### 3
