#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include "physics2.h"

int main(int argc, char **argv){
	const Condition cond =
    {
    	.width  = 75,
    	.height = 40,
    	.G = 1.0,
     	.dt = 1.0,
     	.cor = 0.8
    };
  
  	size_t objnum = 2;
  	Object objects[objnum];

  	// objects[1] は巨大な物体を画面外に... 地球のようなものを想定
  	objects[0] = (Object){ .m = 60.0, .y = -19.9, .vy = 2.0};
  	objects[1] = (Object){ .m = 100000.0, .y =  1000.0, .vy = 0.0};

	// シミュレーション. ループは整数で回しつつ、実数時間も更新する
  	const double stop_time = 400;
  	double t = 0;
  	for (size_t i = 0 ; t <= stop_time ; i++){
    	t = i * cond.dt;
    	my_update_velocities(objects, objnum, cond);
    	my_update_positions(objects, objnum, cond);
    	my_bounce(objects, objnum, cond); // 壁があると仮定した場合に壁を跨いでいたら反射させる
    
    	// 表示の座標系は width/2, height/2 のピクセル位置が原点となるようにする
    	my_plot_objects(objects, objnum, t, cond);
    
    	usleep(200 * 1000); // 200 x 1000us = 200 ms ずつ停止
    	printf("\e[%dA", cond.height+3);// 壁とパラメータ表示分で3行
  	}
  	return EXIT_SUCCESS;
}

// 実習: 以下に my_ で始まる関数を実装する
// 最終的に phisics2.h 内の事前に用意された関数プロトタイプをコメントアウト

void my_plot_objects(Object objs[], const size_t numobj, const double t, const Condition cond){
	int y = 0;
	int x = 0;
	int check = 0;

	printf("+");
	for(int j=0 ; j<cond.width ; j++){
		printf("-");
	}

	printf("+\n");
	for(int i=0 ; i<cond.height ; i++){ //i行目
		printf("|");
		for(int j=0 ; j<cond.width ; j++){ //j列
			check = 0; //そのマスに既に何らかのobjectが描画されているかどうか
			for(int k=0 ; k<numobj ; k++){ //objectが存在するかどうか
				y = objs[k].y + cond.height/2;
				x = cond.width/2;
				if(y==i && x==j &&  check==0){
					printf("o");
					check = 1;
				}
			}
			if(check==0){
				printf(" ");
			}
		}
		printf("|\n");
	}
	
	printf("+");
	for(int j=0 ; j<cond.width ; j++){
		printf("-");
	}
	printf("+\n");
	printf("t = %6.2lf, obj[0].y = %8.2lf, obj[1].y = %8.2lf\n",t, objs[0].y, objs[1].y);
}

void my_update_velocities(Object objs[], const size_t numobj, const Condition cond){
	double v = 0;
	for(int i=0 ; i<numobj ; i++){ //i番目のobjectの更新
		double a = 0;
		for(int j=0 ; j<numobj ; j++){ //i番目のobjectが受けるj番目のobjectからの影響について
			if(i==j){
				continue;
			}
			double dy = objs[j].y-objs[i].y;
			double ady = fabs(dy);
			a += cond.G * objs[j].m / (ady*ady*ady) * dy;
		}
		v = objs[i].vy + a*cond.dt;
		objs[i].prev_vy = objs[i].vy;
		objs[i].vy = v;
	}
}

void my_update_positions(Object objs[], const size_t numobj, const Condition cond){
	for(int i=0 ; i<numobj ; i++){ //i番目のobjectの更新
		objs[i].prev_y = objs[i].y;
		objs[i].y += objs[i].prev_vy*cond.dt;
	}
}

void my_bounce(Object objs[], const size_t numobj, const Condition cond){
	for(int i=0 ; i<numobj ; i++){ //i番目のobjectについて
		if(objs[i].prev_y<cond.height/2 && objs[i].y>=cond.height/2){
			objs[i].y = cond.height-objs[i].y;
			objs[i].prev_vy = -cond.cor*objs[i].vy;
			objs[i].vy = -cond.cor*objs[i].vy;
		}else if(objs[i].prev_y>-cond.height/2 && objs[i].y<=-cond.height/2){
			objs[i].y = -cond.height-objs[i].y;
			objs[i].prev_vy = -cond.cor*objs[i].vy;
			objs[i].vy = -cond.cor*objs[i].vy;
		}
	}
}
