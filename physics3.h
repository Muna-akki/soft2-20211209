// シミュレーション条件を格納する構造体
// 反発係数CORを追加
typedef struct condition
{
  int width; // 見えている範囲の幅
  int height; // 見えている範囲の高さ
  int depth; //奥行き
  double G; // 重力定数
  double dt; // シミュレーションの時間幅
  double cor; // 壁の反発係数
} Condition;

// 個々の物体を表す構造体
typedef struct object
{
  double m;
  double y;
  double x;
  double z;
  double prev_y; // 壁からの反発に使用
  double prev_x;
  double prev_z;
  double vy;
  double vx;
  double vz;
  double prev_vy;
  double prev_vx;
  double prev_vz;
} Object;

// 授業で用意した関数のプロトタイプ宣言

//void plot_objects(Object objs[], const size_t numobj, const double t, const Condition cond);
//void update_velocities(Object objs[], const size_t numobj, const Condition cond);
//void update_positions(Object objs[], const size_t numobj, const Condition cond);
//void bounce(Object objs[], const size_t numobj, const Condition cond);

// To do: 以下に上記と同じ引数で実行される my_* を作成
// 実装できたらmain関数で上記と入れ替えていく
// my_plot_objects(), my_update_velocities(), my_update_positions(), my_bounce の4つ 
void my_plot_objects(Object objs[], const size_t numobj, const double t, const Condition cond, int mode);
void my_update_velocities(Object objs[], const size_t numobj, const Condition cond);
void my_update_positions(Object objs[], const size_t numobj, const Condition cond);
void my_bounce(Object objs[], const size_t numobj, const Condition cond);
void read_file(FILE* fp, Object objs[], const size_t numobj, const Condition cond);
void collision(Object objs[], const size_t numobj, const Condition cond, double p);

