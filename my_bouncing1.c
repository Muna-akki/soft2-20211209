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
  
  	size_t objnum = 4;
  	Object objects[objnum];

  	// objects[1] は巨大な物体を画面外に... 地球のようなものを想定
  	objects[0] = (Object){ .m = 60.0, .y = -19.9, .x=-35, .vy = 2.0, .vx = 1.5};
  	objects[1] = (Object){ .m = 100000.0, .y =  1000.0, .vy = 0.0, .vx = 0.0};
    objects[2] = (Object){ .m = 100, .y = 15, .x = 20, .vy = -5.0, .vx = -1.0};
    objects[3] = (Object){ .m = 15.0, .y = 5, .x = 0, .vy = 0.5, .vx = 0.2};
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
    	printf("\e[%dA", cond.height+3+(int)objnum);// 壁とパラメータ表示分で3行
  	}
  	return EXIT_SUCCESS;
}

// 実習: 以下に my_ で始まる関数を実装する
// 最終的に phisics2.h 内の事前に用意された関数プロトタイプをコメントアウト

void my_plot_objects(Object objs[], const size_t numobj, const double t, const Condition cond){
	double y = 0;
	double x = 0;
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
				y = objs[k].y + (double)cond.height/2.0;
				x = objs[k].x + (double)cond.width/2.0;
				if((int)y==i && (int)x==j &&  check==0){
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
	printf("t = %6.2lf\n", t);
    for(int i=0 ; i<numobj ; i++){
        printf("objs[%d].x = %8.2lf, objs[%d].y = %8.2lf\n", i, objs[i].x, i,  objs[i].y);
    }
}

void my_update_velocities(Object objs[], const size_t numobj, const Condition cond){
	double vx = 0;
    double vy = 0;
	for(int i=0 ; i<numobj ; i++){ //i番目のobjectの更新
		double ax = 0;
        double ay = 0;
		for(int j=0 ; j<numobj ; j++){ //i番目のobjectが受けるj番目のobjectからの影響について
			if(i==j){
				continue;
			}
			double dy = objs[j].y-objs[i].y;
            double dx = objs[j].x-objs[i].x;
            double dr = sqrt(dy*dy+dx*dx);
			ax += cond.G * objs[j].m / (dr*dr*dr) * dx;
            ay += cond.G * objs[j].m / (dr*dr*dr) * dy;
		}
		if(objs[i].m==0){
            ax = 0;
            ay = 0;
        }
        vx = objs[i].vx + ax*cond.dt;
		vy = objs[i].vy + ay*cond.dt;
        objs[i].prev_vx = objs[i].vx;
		objs[i].prev_vy = objs[i].vy;
        objs[i].vx = vx;
		objs[i].vy = vy;
	}
}

void my_update_positions(Object objs[], const size_t numobj, const Condition cond){
	for(int i=0 ; i<numobj ; i++){ //i番目のobjectの更新
		objs[i].prev_x = objs[i].x;
        objs[i].prev_y = objs[i].y;
        objs[i].x += objs[i].prev_vx*cond.dt;
		objs[i].y += objs[i].prev_vy*cond.dt;
	}
}

void my_bounce(Object objs[], const size_t numobj, const Condition cond){
	for(int i=0 ; i<numobj ; i++){ //i番目のobjectについて
		if(objs[i].prev_x<cond.width/2.0 && objs[i].x>cond.width/2.0){
            objs[i].x = cond.width-objs[i].x;
			objs[i].prev_vx = -cond.cor*objs[i].vx;
			objs[i].vx = -cond.cor*objs[i].vx;
        
		}else if(objs[i].prev_x>-cond.width/2.0 && objs[i].x<=-cond.width/2.0){
            objs[i].x = -cond.width-objs[i].x;
			objs[i].prev_vx = -cond.cor*objs[i].vx;
			objs[i].vx = -cond.cor*objs[i].vx;
        
		}else if(objs[i].prev_x>cond.width/2.0 && objs[i].x<cond.width/2.0){
			objs[i].x = cond.width-objs[i].x;
			objs[i].prev_vx = -cond.cor*objs[i].vx;
			objs[i].vx = -cond.cor*objs[i].vx;

		}else if(objs[i].prev_x<-cond.width/2.0 && objs[i].x>-cond.width/2.0){
			objs[i].x = -cond.width-objs[i].x;
			objs[i].prev_vx = -cond.cor*objs[i].vx;
			objs[i].vx = -cond.cor*objs[i].vx;
		}
        

		if(objs[i].prev_y<cond.height/2.0 && objs[i].y>cond.height/2.0){
			objs[i].y = cond.height-objs[i].y;
			objs[i].prev_vy = -cond.cor*objs[i].vy;
			objs[i].vy = -cond.cor*objs[i].vy;
		
		}else if(objs[i].prev_y>-cond.height/2.0 && objs[i].y<-cond.height/2.0){
			objs[i].y = -cond.height-objs[i].y;
			objs[i].prev_vy = -cond.cor*objs[i].vy;
			objs[i].vy = -cond.cor*objs[i].vy;
		
		}else if(objs[i].prev_y>cond.height/2.0 && objs[i].y<cond.height/2.0){
			objs[i].y = cond.height-objs[i].y;
			objs[i].prev_vy = -cond.cor*objs[i].vy;
			objs[i].vy = -cond.cor*objs[i].vy;
		
		}else if(objs[i].prev_y<=-cond.height/2.0 && objs[i].y>-cond.height/2.0){
			objs[i].y = -cond.height-objs[i].y;
			objs[i].prev_vy = -cond.cor*objs[i].vy;
			objs[i].vy = -cond.cor*objs[i].vy;
		}
	}
}
