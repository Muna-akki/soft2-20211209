#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include "physics3.h"

int main(int argc, char **argv){
	//３次元に拡張
    const Condition cond =
    {
    	.width  = 35,
    	.height = 40,
        .depth = 45,
    	.G = 1.0,
     	.dt = 1.0,
     	.cor = 0.8
    };

    if(argc>4){
        fprintf(stderr, "不正な引数\n");
        return EXIT_FAILURE;
    }
    
    //modeによって表示する断面を切り替える
    int mode = strtol(argv[1],NULL,10); //1:xy, 2:yz, 3:zx
    if(mode<=0 || mode>=4){
        printf("error in mode number\n");
        return EXIT_FAILURE;
    }
  	
    size_t objnum = strtol(argv[2],NULL,10);
  	Object objects[objnum];
    FILE* fp = fopen(argv[3], "r");
    if(fp==NULL){
        printf("cannot open file\n");
        return EXIT_FAILURE;
    }
    
    read_file(fp, objects, objnum, cond);

	// シミュレーション. ループは整数で回しつつ、実数時間も更新する
  	const double stop_time = 400;
  	double t = 0;
    
    struct timespec start;
    clock_gettime(CLOCK_REALTIME, &start);
    srand(start.tv_nsec);
  	
    for (size_t i = 0 ; t <= stop_time ; i++){
    	t = i * cond.dt;
        collision(objects, objnum, cond, (double)rand()/RAND_MAX);
    	my_update_velocities(objects, objnum, cond);
    	my_update_positions(objects, objnum, cond);
    	my_bounce(objects, objnum, cond); // 壁があると仮定した場合に壁を跨いでいたら反射させる

    	// 表示の座標系は width/2, height/2 のピクセル位置が原点となるようにする
    	my_plot_objects(objects, objnum, t, cond, mode);
    
    	usleep(200 * 1000); // 200 x 1000us = 200 ms ずつ停止
        
        if(mode==1){
            printf("\e[%dA", cond.height+3+(int)objnum);
        }else if(mode==2){
            printf("\e[%dA", cond.depth+3+(int)objnum);
        }else if(mode==3){
            printf("\e[%dA", cond.width+3+(int)objnum);
        }
    	
  	}
    fclose(fp);
  	return EXIT_SUCCESS;
}

// 実習: 以下に my_ で始まる関数を実装する
// 最終的に phisics2.h 内の事前に用意された関数プロトタイプをコメントアウト

void my_plot_objects(Object objs[], const size_t numobj, const double t, const Condition cond, int mode){
	double y = 0;
	double x = 0;
    double z = 0;
	int check = 0;
    double ave = 0;
    for(int i=0 ; i<numobj ; i++){
        ave += objs[i].m;
    }
    ave /= numobj;
    
    if(mode==1){ //xy平面で描画 (xが横、yが縦)
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
				    y = objs[k].y + cond.height/2.0;
				    x = objs[k].x + cond.width/2.0;
				    if((int)y==i && (int)x==j &&  check==0){
					    if(objs[k].m<ave/3){
                            printf("\x1b[31m");
                        }else if(objs[k].m<ave*2/3){
                            printf("\x1b[33m");
                        }else{
                            printf("\x1b[36m");
                        }
                        printf("o");
                        printf("\x1b[39m");
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

    }else if(mode==2){ //yz平面で描画 (yが横、zが縦)
        printf("+");
	    for(int j=0 ; j<cond.height ; j++){
		    printf("-");
	    }

	    printf("+\n");
	    for(int i=0 ; i<cond.depth ; i++){ //i行目
		    printf("|");
		    for(int j=0 ; j<cond.height ; j++){ //j列
			    check = 0; //そのマスに既に何らかのobjectが描画されているかどうか
			    for(int k=0 ; k<numobj ; k++){ //objectが存在するかどうか
				    z = objs[k].z + cond.depth/2.0;
				    y = objs[k].y + cond.height/2.0;
				    if((int)z==i && (int)y==j &&  check==0){
					    if(objs[k].m<ave/3){
                            printf("\x1b[31m");
                        }else if(objs[k].m<ave*2/3){
                            printf("\x1b[33m");
                        }else{
                            printf("\x1b[36m");
                        }
                        printf("o");
                        printf("\x1b[39m");
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
	    for(int j=0 ; j<cond.height ; j++){
		    printf("-");
	    }
	    printf("+\n");
    }else if(mode==3){ //zx平面で描画 (zが横、xが縦)
        printf("+");
	    for(int j=0 ; j<cond.depth ; j++){
		    printf("-");
	    }

	    printf("+\n");
	    for(int i=0 ; i<cond.width ; i++){ //i行目
		    printf("|");
		    for(int j=0 ; j<cond.depth ; j++){ //j列
			    check = 0; //そのマスに既に何らかのobjectが描画されているかどうか
			    for(int k=0 ; k<numobj ; k++){ //objectが存在するかどうか
				    x = objs[k].x + cond.width/2.0;
				    z = objs[k].z + cond.depth/2.0;
				    if((int)x==i && (int)z==j &&  check==0){
					    if(objs[k].m<ave/3){
                            printf("\x1b[31m");
                        }else if(objs[k].m<ave*2/3){
                            printf("\x1b[33m");
                        }else{
                            printf("\x1b[36m");
                        }
                        printf("o");
                        printf("\x1b[39m");
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
	    for(int j=0 ; j<cond.depth ; j++){
		    printf("-");
	    }
	    printf("+\n");
    }
	
    printf("t = %6.2lf\n", t);
    for(int i=0 ; i<numobj ; i++){
        printf("objs[%d].x = %8.2lf, objs[%d].y = %8.2lf, objs[%d].z = %8.2lf\n", i, objs[i].x, i,  objs[i].y, i, objs[i].z);
    }
}

void my_update_velocities(Object objs[], const size_t numobj, const Condition cond){
	double vx = 0;
    double vy = 0;
    double vz = 0;
	for(int i=0 ; i<numobj ; i++){ //i番目のobjectの更新
		double ax = 0;
        double ay = 0;
        double az = 0;
		for(int j=0 ; j<numobj ; j++){ //i番目のobjectが受けるj番目のobjectからの影響について
			if(i==j){
				continue;
			}
			double dy = objs[j].y-objs[i].y;
            double dx = objs[j].x-objs[i].x;
            double dz = objs[j].z-objs[i].z;
            double dr = sqrt(dy*dy+dx*dx+dz*dz);
			ax += cond.G * objs[j].m / (dr*dr*dr) * dx;
            ay += cond.G * objs[j].m / (dr*dr*dr) * dy;
            az += cond.G * objs[j].m / (dr*dr*dr) * dz;
		}
        if(objs[i].m==0){
            ax = 0;
            ay = 0;
            az = 0;
        }
        vx = objs[i].vx + ax*cond.dt;
		vy = objs[i].vy + ay*cond.dt;
        vz = objs[i].vz + az*cond.dt;
        objs[i].prev_vx = objs[i].vx;
		objs[i].prev_vy = objs[i].vy;
        objs[i].prev_vz = objs[i].vz;
        objs[i].vx = vx;
		objs[i].vy = vy;
        objs[i].vz = vz;
	}
}

void my_update_positions(Object objs[], const size_t numobj, const Condition cond){
	for(int i=0 ; i<numobj ; i++){ //i番目のobjectの更新
		objs[i].prev_x = objs[i].x;
        objs[i].prev_y = objs[i].y;
        objs[i].prev_z = objs[i].z;
        objs[i].x += objs[i].prev_vx*cond.dt;
		objs[i].y += objs[i].prev_vy*cond.dt;
        objs[i].z += objs[i].prev_vz*cond.dt;
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


        if(objs[i].prev_z<cond.depth/2.0 && objs[i].z>cond.depth/2.0){
			objs[i].z = cond.depth-objs[i].z;
			objs[i].prev_vz = -cond.cor*objs[i].vz;
			objs[i].vz = -cond.cor*objs[i].vz;
		
		}else if(objs[i].prev_z>-cond.depth/2.0 && objs[i].z<-cond.depth/2.0){
			objs[i].z = -cond.depth-objs[i].z;
			objs[i].prev_vz = -cond.cor*objs[i].vz;
			objs[i].vz = -cond.cor*objs[i].vz;
		
		}else if(objs[i].prev_z>cond.depth/2.0 && objs[i].z<cond.depth/2.0){
			objs[i].z = cond.depth-objs[i].z;
			objs[i].prev_vz = -cond.cor*objs[i].vz;
			objs[i].vz = -cond.cor*objs[i].vz;
		
		}else if(objs[i].prev_z<=-cond.depth/2.0 && objs[i].z>-cond.depth/2.0){
			objs[i].z = -cond.depth-objs[i].z;
			objs[i].prev_vz = -cond.cor*objs[i].vz;
			objs[i].vz = -cond.cor*objs[i].vz;
		}
	}
}

void read_file(FILE* fp, Object objs[], const size_t numobj, const Condition cond){
    int l = 200;
    char s[l];
    int count = 0;
    //どこまでがコメントかを調べる。
    while(1){
        fgets(s,l,fp);
        count++;
        if(s[0]=='#'){
            continue;
        }else{
            break;
        }
    }
    
    //ファイル先頭へ
    rewind(fp);

    for(int i=0 ; i<count-1 ; i++){
        fgets(s,l,fp);
    }
    int check = 0;
    for(int i=0 ; i<numobj ; i++){
        double m,x,y,z,vx,vy,vz;
        int n = fscanf(fp, "%lf %lf %lf %lf %lf %lf %lf",&m, &x, &y, &z, &vx, &vy, &vz);
        fscanf(fp,"%*[^\n]%*c");
        if(n!=7 || check == 1){
            check = 1;
            objs[i] = (Object){.m = 0.0, .x = cond.width, .y = cond.height, .z = cond.depth, .vx = 0, .vy = 0, .vz = 0};
        }else{
            objs[i] = (Object){ .m = m, .x = x, .y = y, .z = z, .vx = vx, .vy = vy, .vz = vz};
        }
    }

}

void collision(Object objs[], const size_t numobj, const Condition cond, double p){
    //iとjが一定距離min以下で衝突
    double dr = 0;
    double min = 3;
    double dsum = 0;
    for(int i=0 ; i<numobj ; i++){
        if(objs[i].m==0){
            continue;
        }
        for(int j=i+1 ; j<numobj ; j++){
            dsum = 0;
            dsum = (objs[i].x-objs[j].x)*(objs[i].x-objs[j].x);
            dsum += (objs[i].y-objs[j].y)*(objs[i].y-objs[j].y);
            dsum += (objs[i].z-objs[j].z)*(objs[i].z-objs[j].z);
            dr = sqrt(dsum);
            if(dr>min){
                continue;
            }
            double M = objs[i].m + objs[j].m;
            double vx = (objs[i].m*objs[i].vx + objs[j].m*objs[j].vx)/M;
            double vy = (objs[i].m*objs[i].vy + objs[j].m*objs[j].vy)/M;
            double vz = (objs[i].m*objs[i].vz + objs[j].m*objs[j].vz)/M;
            double prev_vx = (objs[i].m*objs[i].prev_vx + objs[j].m*objs[j].prev_vx)/M;
            double prev_vy = (objs[i].m*objs[i].prev_vy + objs[j].m*objs[j].prev_vy)/M;
            double prev_vz = (objs[i].m*objs[i].prev_vz + objs[j].m*objs[j].prev_vz)/M;
            
            //p:(1-p)に分かれる (pは乱数)
            objs[i].m = M*p;
            objs[i].prev_y = objs[i].y;
            objs[i].prev_x = objs[i].x;
            objs[i].prev_z = objs[i].z;
            objs[i].vy = vy*p;
            objs[i].vx = vx*p;
            objs[i].vz = vz*p;
            objs[i].prev_vx = prev_vx*p;
            objs[i].prev_vy = prev_vy*p;
            objs[i].prev_vz = prev_vz*p;
            
            
            objs[j].m = M*(1-p);
            objs[j].prev_y = objs[j].y;
            objs[j].prev_x = objs[j].x;
            objs[j].prev_z = objs[j].z;
            objs[j].vy = vy*(1-p);
            objs[j].vx = vx*(1-p);
            objs[j].vz = vz*(1-p);
            objs[j].prev_vx = prev_vx*(1-p);
            objs[j].prev_vy = prev_vy*(1-p);
            objs[j].prev_vz = prev_vz*(1-p);
        }     
    }
}


