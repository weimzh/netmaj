/*
 * NETMAJ - network mahjongg -  Copyright Notice
 *     Copyright (C) 1994, 1995, 1996 Koji Suzuki (suz@kt.rim.or.jp)
 *
 *  You can redistribute it and/or modify this software under the terms 
 * of the GNU General Public License version 2 as published by
 * the Free Software Foundation.
 *
 *  This software is provided 'as-is', without any express or implied
 * warranty.  In no event will the author be held liable for any damages
 * arising from the use of this software.
 *
 *  Koji Suzuki   : suz@kt.rim.or.jp
 */

#define USAGE "\
usage:\n\
    planalize file \n"

#include "pai.h"
#include "ahand.h"
#include "global.h"
#include <signal.h>

extern char *pai_strings[];
river_t mrv[4][RIVER_MAX];
FILE *fp;
FILE *df = 0;
void term(){
        exit(1);
}

hand_t last_hand[4];

char *name;

main(argc,argv) char *argv[]; {
        global_t g,*gp;
        int i,c;
        int cnt=-1;

        while((c = getopt(argc,argv,"x:n:"))!=EOF) {
                switch(c) {
                case 'x':
                        sscanf(optarg,"%d",&cnt);
                        break;
                case 'n':
                        name = optarg;
                        break;
                default:
                        fprintf(stderr,USAGE);
                        exit(1);
                        break;
                }
        }
        if (argc > optind) fp = fopen(argv[optind],"r");
        if (!fp) exit(1);

        signal(SIGTERM,term);
        signal(SIGINT,term);

        bzero(&g,sizeof(g));
        for (i=0; i<4; i++) g._pplayer[i] = i;
        mt_next_new(&g);

        gp = &g;
        while (rlog(&g,fp)==0 && cnt != 0) {
                for (i=0; i<4; i++)
                        if (!name || !strcmp(player[i].name,name))
                                yomi_reach(i);

                mt_next_new(&g);
                cnt --;
        }
        yomi_kekka();
}

str2pai(p) char *p; {
        int i;
        for (i=0; i<(K_MAX*4); i++) {
                if (!(strncmp(pai_strings[i],p,3))) return i;
        }
        return 0;
}

str2attr(p) char *p; {
        int i,ret;

        ret = 0;
        for (i=0; i<8; i++) 
                if (F_CHARS[i] == p[0]) {
                        F_SET(ret,(i<<F_SHFT));
                        break;
                }
        for (i=0; i<8; i++) 
                if (T_CHARS[i] == p[1]) {
                        T_SET(ret,(i<<T_SHFT));
                        break;
                }
        for (i=0; i<8; i++) 
                if (R_CHARS[i] == p[2]) {
                        R_SET(ret,(i<<R_SHFT));
                        break;
                }
        return ret;
}

rlog(gp,fp) global_t *gp; FILE *fp; {
    int c,i,j,n,x;
    int d1[1],d2[1],d3[1],d4[1],d5[1],d6[1],d7[1],d8[1],d9[1];
    char *p;
    char buf[80],s1[80],s2[80],s3[80],s4[80],s5[80];
    int rule;

    for (i=0; i<4; i++) pplayer[i] = i;
    while (1) {
        if (fgets(buf,sizeof(buf),fp)==0) return (-1);
        if (sscanf(buf,"rule: %d",&rule) == 1) {
                expand_rule(&rule);
        }
        else if (((c=sscanf(buf,"%s %d %s %d %s %s %d"
                ,s1,d1,s2,d2,s3,s4,d3))==7) &&
            !strcmp("局",s2) && !strcmp("供託",s4)) {
                if (!strcmp("東",s1)) big_turn = 0; 
                else if (!strcmp("南",s1)) big_turn = 1; 
                else if (!strcmp("西",s1)) big_turn = 2; 
                else if (!strcmp("北",s1)) big_turn = 3; 
                small_turn = *d1 - 1;
                small_turn_cnt = *d2;
                rest_point = *d3;
        break;
        }
    } 
    while (1) {
        if (fgets(buf,sizeof(buf),fp)==0) return (-1);
        if (((c=sscanf(buf,"\t%s %s",s1,s2))==2) &&
            !strcmp("ドラ",s1)) {
                p = buf+6;
                for (i=0; p && (c=str2pai(p))>0 ; i++,p+=3) {
                }
        break;
        }
    }
    while (1) {
        if (fgets(buf,sizeof(buf),fp)==0) return (-1);
        if (((c=sscanf(buf,"\t%s %s",s1,s2))==2) &&
            !strcmp("ウラドラ",s1)) {
                p = buf+10;
                for (i=0; p && (c=str2pai(p))>0 ; i++,p+=3) {
                }
        break;
        }
    } 

    for (i=0; i<4; i++) {
        while (1) {
            if (fgets(buf,sizeof(buf),fp)==0) return (-1);
            if (((c=sscanf(buf,"%s %s %d(%d)",s1,s2,d1,d2)==4) &&
                !strcmp("家",s1+2))) {
                strcpy(player[i].name,s2);
                player[i].gpoint = *d1 - *d2;
                player[i].ppoint = 0;
            break;
            }
        } 
        if (fgets(s1,sizeof(buf),fp)==0) return (-1);
        if (fgets(s2,sizeof(buf),fp)==0) return (-1);
        if (fgets(s3,sizeof(buf),fp)==0) return (-1);
        if (fgets(s4,sizeof(buf),fp)==0) return (-1);
        if (fgets(s5,sizeof(buf),fp)==0) return (-1);
        for (j=0; j<13; j++) {
                init_hand[i].closed[j] = c = str2pai(s1+1+(j*3));
                if (c == 0) break;
        }
        init_hand[i].closed_num = j;
        hand[i] = init_hand[i];
        for (j=0; j<RIVER_MAX; j++) {
                mrv[i][j].out = c = str2pai(s3+1+(j*3));
                if (c == 0) break;
        }
        n = j;
        for (j=0; j<n; j++) {
                mrv[i][j].in = c = str2pai(s2+1+(j*3));
                if (c == 3) mrv[i][j].in = mrv[i][j].out;
        }
        for (j=0; j<n; j++) {
                mrv[i][j].attr = c = str2attr(s4+1+(j*3));
        }
        for (j=0; j<14; j++) {
                last_hand[i].closed[j] = c = str2pai(s5+1+(j*3));
                if (c == 0) break;
        }
        last_hand[i].closed_num = j;
        p = s5+1+j*3;
        x = 0;
        j = 0;
        while (*p) {
                if (c = str2pai(p)) {
                        while (c = str2pai(p)) {
                                last_hand[i].opened[x][j] = c;
                                j++;
                                p+=3;
                        }
                        x++;
                        j=0;
                }
                else p++;
        }
    }
    return 0;
}

int total_reach;
int total_reach_agari;
int total_waiting[K_MAX];
int total_waiting_agari[K_MAX];
int total_wait_kind[K_MAX];
int total_wait_kind_agari[K_MAX];
int total_reach_pos[RIVER_MAX];
int total_reach_pos_agari[RIVER_MAX];
int total_ryanmen;
int total_ryanmen_agari;
int total_syabo;
int total_syabo_agari;
int total_kanchan;
int total_kanchan_agari;
int total_penchan;
int total_penchan_agari;
int total_tanki;
int total_tanki_agari;
int total_nobetan;
int total_nobetan_agari;
int total_tamen;
int total_tamen_agari;
int total_ryanmen_soba[4];
int total_kanchan_soba[6];
int total_penchan_soba[4];
int total_kanchan_3ken;
int total_penchan_3ken;

int total_zone9[6][5];
int total_zone9_cnt[6];
int total_zone8[6][5];
int total_zone8_cnt[6];
int total_zone7[6][5];
int total_zone7_cnt[6];
int total_zone6[6][5];
int total_zone6_cnt[6];
int total_zone5[6][4];
int total_zone5_cnt[4];

int total_av_zone9[5];
int total_av_zone9_cnt;
int total_av_zone8[5];
int total_av_zone8_cnt;
int total_av_zone7[5];
int total_av_zone7_cnt;
int total_av_zone6[5];
int total_av_zone6_cnt;
int total_av_zone5[4];
int total_av_zone5_cnt;

int total_t_zone9[6][5];
int total_t_zone9_cnt[6];
int total_t_zone8[6][5];
int total_t_zone8_cnt[6];
int total_t_zone7[6][5];
int total_t_zone7_cnt[6];
int total_t_zone6[6][5];
int total_t_zone6_cnt[6];
int total_t_zone5[6][4];
int total_t_zone5_cnt[4];

int total_av_t_zone9[5];
int total_av_t_zone9_cnt;
int total_av_t_zone8[5];
int total_av_t_zone8_cnt;
int total_av_t_zone7[5];
int total_av_t_zone7_cnt;
int total_av_t_zone6[5];
int total_av_t_zone6_cnt;
int total_av_t_zone5[4];
int total_av_t_zone5_cnt;

int yomi_pai[K_MAX];
int yomi_rank[K_MAX];
int total_yomi_point[5];
int total_yomi_atari[5];
int total_yomi_count;

int total_0ken[2];
int total_0ken_cnt;
int total_1ken[1];
int total_1ken_cnt;
int total_2ken[2];
int total_2ken_cnt;
int total_3ken[3];
int total_3ken_cnt;
int total_4ken[4];
int total_4ken_cnt;
int total_sute_wait[6];
int total_sute_wait_cnt[6];
int total_1ken_rev[3];
int total_1ken_rev_cnt;

yomi_reach(p) {
        int reach = 0;
        int i,j,n,rp,waiting;
        ahand_t a,*ap;
        hand_t h;
        int agari=0;
        int v2num=0;
        int kanchan=0;
        int penchan=0;
        int ryanmen=0;
        int syabo=0;
        int tanki=0;
        
        int wait_kind[K_MAX];

        for (i=0; i<RIVER_MAX; i++) {
                if (T_GET(mrv[p][i].attr) ==  T_RV_RCH) {
                        reach = 1;
                        rp = i;
                        break;
                }
        }
        if (!reach) {
                return;
        }

        n = last_hand[p].closed_num;
        if (n % 3 == 2) {
                agari = 1;
                last_hand[p].closed_num --;
                n --;
        }
        waiting = 0;
        ah_new(&a,&last_hand[p],-1);
        ah_analize(&a,0);
        if (result_rest != 1) return;
        for (i=0; i<result_cnt; i++) {
                ap = &result_ahand[i];
                if (ap->rest_num) tanki++;
                else {
                        for (j=0; j<ap->part_num; j++) {
                                switch (ap->part_kind[j]) {
                                case AH_V2:
                                        v2num++; break;
                                case AH_H2a:
                                        ryanmen++; break;
                                case AH_H2b:
                                        kanchan++; break;
                                case AH_H2c:
                                        penchan++; break;
                                }
                        }
                        if (v2num == 2) syabo++; break;
                }
        }
	wait_kind[0] = 0;
        for (i=1; i<K_MAX;i++) {
                wait_kind[i] = 0;
                h = last_hand[p];
                h.closed[n] = i<<2;
                h.closed_num++;
                ah_new(&a,&h,-1);
                ah_analize(&a,0);
                if (result_rest == 0) {
                        wait_kind[i] = 1;
                        waiting++;
                }
        }
        if (waiting == 0) {
                return;
        }

        total_reach++;
        if (agari) total_reach_agari++;
        total_waiting[waiting]++;
        if (agari) total_waiting_agari[waiting]++;
        total_reach_pos[rp]++;
        if (agari) total_reach_pos_agari[rp]++;
        if (waiting == 2) {
                if (ryanmen) {
                        total_ryanmen++;
                        if (agari) total_ryanmen_agari++;
                }
                else if (syabo) {
                        total_syabo++;
                        if (agari) total_syabo_agari++;
                }
                else {
                        total_nobetan++;
                        if (agari) total_nobetan_agari++;
                }
        }
        else if (waiting == 1) {
                if (kanchan) {
                        total_kanchan++;
                        if (agari) total_kanchan_agari++;
                }
                else if (penchan) {
                        total_penchan++;
                        if (agari) total_penchan_agari++;
                }
                else {
                        total_tanki++;
                        if (agari) total_tanki_agari++;
                }
        }
        else {
                total_tamen++;
                if (agari) total_tamen_agari++;
        }
        for (i=1; i< K_MAX; i++) {
                if (wait_kind[i]) {
                        total_wait_kind[i]++;
                        if (agari) total_wait_kind_agari[i]++;
                }
        }
        if (waiting == 2 && ryanmen) {
		int machi,pk;
		int yonken_a,yonken_b;
		int waiting_4ken_a,waiting_4ken_b;
		int waiting_4ken_as,waiting_4ken_bs;

		waiting_4ken_a = waiting_4ken_b = 0;
		waiting_4ken_as = waiting_4ken_bs = 0;
        	for (i=1; i< K_MAX; i++) {
                	if (wait_kind[i]) {
				machi = i;
				break;
			}
		}
		yonken_a = K_PREV2(machi);
		yonken_b = K_NEXT2(machi+3);
        	for (i=0; i<=rp; i++) {
			pk = P_KIND(mrv[p][i].out);
			if (pk == yonken_a) waiting_4ken_a = 1;
			if (pk == yonken_b) waiting_4ken_b = 1;
			if (pk == machi+1) waiting_4ken_bs = 1;
			if (pk == machi+2) waiting_4ken_as = 1;
                }
		if (waiting_4ken_a)  total_ryanmen_soba[0]++;
		if (waiting_4ken_bs) total_ryanmen_soba[1]++;
		if (waiting_4ken_as) total_ryanmen_soba[2]++;
		if (waiting_4ken_b)  total_ryanmen_soba[3]++;

        }
        if (waiting == 1 && kanchan) {
		int machi,pk;
		int soba[6];
		for (i=0; i<6; i++) soba[i] = 0;
        	for (i=1; i< K_MAX; i++) {
                	if (wait_kind[i]) {
				machi = i;
				break;
			}
		}
        	for (i=0; i<=rp; i++) {
			pk = P_KIND(mrv[p][i].out);
			if (pk == K_PREV2(K_PREV2(machi - 1))) soba[0]=1;
			if (pk == K_PREV2(machi - 1)) soba[1]=1;
			if (pk == machi - 1) soba[2]=1;
			if (pk == machi + 1) soba[3]=1;
			if (pk == K_NEXT2(machi + 1)) soba[4]=1;
			if (pk == K_NEXT2(K_NEXT2(machi + 1))) soba[5]=1;
		}
		for (i=0; i<6; i++)
			if (soba[i]) total_kanchan_soba[i]++;
		if (soba[1] && soba[4])
			total_kanchan_3ken++;
	}
	if (waiting == 1 && penchan) {
		int machi,pk;
		int soba[4];
		for (i=0; i<4; i++) soba[i] = 0;
        	for (i=1; i< K_MAX; i++) {
                	if (wait_kind[i]) {
				machi = i;
				break;
			}
		}
        	for (i=0; i<=rp; i++) {
			pk = P_KIND(mrv[p][i].out);
			if (pk == machi - 1) soba[1]=1;
			if (pk == K_PREV2(machi - 1)) soba[0]=1;
			if (pk == machi + 1) soba[2]=1;
			if (pk == K_NEXT2(machi + 1)) soba[3]=1;
		}
		for (i=0; i<4; i++)
			if (soba[i]) total_penchan_soba[i]++;
		if (soba[0] && soba[3])
			total_penchan_3ken++;
	}
    {
	int cnt = 0,j;
	int zone[6][5];
	int pk;

	for (i=0; i<6; i++) for (j=0; j<5; j++) zone[i][j] = 0;
	for (i=rp; i>=0; i--) {
		pk=P_KIND(mrv[p][i].out);
		if (pk == K_MAN_1 || pk == K_PIN_1 || pk == K_SOU_1) {
			if (wait_kind[pk+1]) zone[cnt][0] = 1;
			if (wait_kind[pk+2]) zone[cnt][1] = 1;
			if (wait_kind[pk+3]) zone[cnt][2] = 1;
			if (wait_kind[pk+4]) zone[cnt][3] = 1;
			if (wait_kind[pk+5]) zone[cnt][4] = 1;
			cnt++;
		}
		else if (pk == K_MAN_9 || pk == K_PIN_9 || pk == K_SOU_9) {
			if (wait_kind[pk-1]) zone[cnt][0] = 1;
			if (wait_kind[pk-2]) zone[cnt][1] = 1;
			if (wait_kind[pk-3]) zone[cnt][2] = 1;
			if (wait_kind[pk-4]) zone[cnt][3] = 1;
			if (wait_kind[pk-5]) zone[cnt][4] = 1;
			cnt++;
		}
		if (cnt == 6) break;
	}
	for (i=0; i<cnt; i++) {
		total_av_zone9_cnt++;
		total_zone9_cnt[i]++;
		for (j=0; j<5; j++)
			if (zone[i][j]) {
				total_zone9[i][j]++;
				total_av_zone9[j]++;
			}
	}
    }
    {
	int cnt = 0,j;
	int zone[6][5];
	int pk;

	for (i=0; i<6; i++) for (j=0; j<5; j++) zone[i][j] = 0;
	for (i=rp; i>=0; i--) {
		pk=P_KIND(mrv[p][i].out);
		if (pk == K_MAN_2 || pk == K_PIN_2 || pk == K_SOU_2) {
			if (wait_kind[pk-1]) zone[cnt][0] = 1;
			if (wait_kind[pk+1]) zone[cnt][1] = 1;
			if (wait_kind[pk+2]) zone[cnt][2] = 1;
			if (wait_kind[pk+3]) zone[cnt][3] = 1;
			if (wait_kind[pk+4]) zone[cnt][4] = 1;
			cnt++;
		}
		if (pk == K_MAN_8 || pk == K_PIN_8 || pk == K_SOU_8) {
			if (wait_kind[pk+1]) zone[cnt][0] = 1;
			if (wait_kind[pk-1]) zone[cnt][1] = 1;
			if (wait_kind[pk-2]) zone[cnt][2] = 1;
			if (wait_kind[pk-3]) zone[cnt][3] = 1;
			if (wait_kind[pk-4]) zone[cnt][4] = 1;
			cnt++;
		}
		if (cnt == 6) break;
	}
	for (i=0; i<cnt; i++) {
		total_zone8_cnt[i]++;
		total_av_zone8_cnt++;
		for (j=0; j<5; j++) 
			if (zone[i][j]) {
				total_zone8[i][j]++;
				total_av_zone8[j]++;
			}
	}
    }
    {
	int cnt = 0,j;
	int zone[6][5];
	int pk;

	for (i=0; i<6; i++) for (j=0; j<5; j++) zone[i][j] = 0;
	for (i=rp; i>=0; i--) {
		pk=P_KIND(mrv[p][i].out);
		if (pk == K_MAN_3 || pk == K_PIN_3 || pk == K_SOU_3) {
			if (wait_kind[pk-2]) zone[cnt][0] = 1;
			if (wait_kind[pk-1]) zone[cnt][1] = 1;
			if (wait_kind[pk+1]) zone[cnt][2] = 1;
			if (wait_kind[pk+2]) zone[cnt][3] = 1;
			if (wait_kind[pk+3]) zone[cnt][4] = 1;
			cnt++;
		}
		if (pk == K_MAN_7 || pk == K_PIN_7 || pk == K_SOU_7) {
			if (wait_kind[pk+2]) zone[cnt][0] = 1;
			if (wait_kind[pk+1]) zone[cnt][1] = 1;
			if (wait_kind[pk-1]) zone[cnt][2] = 1;
			if (wait_kind[pk-2]) zone[cnt][3] = 1;
			if (wait_kind[pk-3]) zone[cnt][4] = 1;
			cnt++;
		}
		if (cnt == 6) break;
	}
	for (i=0; i<cnt; i++) {
		total_zone7_cnt[i]++;
		total_av_zone7_cnt++;
		for (j=0; j<5; j++) 
			if (zone[i][j]) {
				total_zone7[i][j]++;
				total_av_zone7[j]++;
			}
	}
    }
    {
	int cnt = 0,j;
	int zone[6][5];
	int pk;

	for (i=0; i<6; i++) for (j=0; j<5; j++) zone[i][j] = 0;
	for (i=rp; i>=0; i--) {
		pk=P_KIND(mrv[p][i].out);
		if (pk == K_MAN_4 || pk == K_PIN_4 || pk == K_SOU_4) {
			if (wait_kind[pk-2]) zone[cnt][0] = 1;
			if (wait_kind[pk-1]) zone[cnt][1] = 1;
			if (wait_kind[pk+1]) zone[cnt][2] = 1;
			if (wait_kind[pk+2]) zone[cnt][3] = 1;
			if (wait_kind[pk+3]) zone[cnt][4] = 1;
			cnt++;
		}
		if (pk == K_MAN_6 || pk == K_PIN_6 || pk == K_SOU_6) {
			if (wait_kind[pk+2]) zone[cnt][0] = 1;
			if (wait_kind[pk+1]) zone[cnt][1] = 1;
			if (wait_kind[pk-1]) zone[cnt][2] = 1;
			if (wait_kind[pk-2]) zone[cnt][3] = 1;
			if (wait_kind[pk-3]) zone[cnt][4] = 1;
			cnt++;
		}
		if (cnt == 6) break;
	}
	for (i=0; i<cnt; i++) {
		total_zone6_cnt[i]++;
		total_av_zone6_cnt++;
		for (j=0; j<5; j++) 
			if (zone[i][j]) {
				total_zone6[i][j]++;
				total_av_zone6[j]++;
			}
	}
    }
    {
	int cnt = 0,j;
	int zone[6][8];
	int pk;

	for (i=0; i<6; i++) for (j=0; j<8; j++) zone[i][j] = 0;
	for (i=rp; i>=0; i--) {
		pk=P_KIND(mrv[p][i].out);
		if (pk == K_MAN_5 || pk == K_PIN_5 || pk == K_SOU_5) {
			if (wait_kind[pk-4]) zone[cnt][0] = 1;
			if (wait_kind[pk-3]) zone[cnt][1] = 1;
			if (wait_kind[pk-2]) zone[cnt][2] = 1;
			if (wait_kind[pk-1]) zone[cnt][3] = 1;
			if (wait_kind[pk+1]) zone[cnt][4] = 1;
			if (wait_kind[pk+2]) zone[cnt][5] = 1;
			if (wait_kind[pk+3]) zone[cnt][6] = 1;
			if (wait_kind[pk+4]) zone[cnt][7] = 1;
			cnt++;
		}
		if (cnt == 6) break;
	}
	for (i=0; i<cnt; i++) {
		total_zone5_cnt[i]+=2;
		total_av_zone5_cnt+=2;
		for (j=0; j<4; j++) {
			if (zone[i][j]) {
				total_zone5[i][j]++;
				total_av_zone5[j]++;
			}
			if (zone[i][7-j]) {
				total_zone5[i][j]++;
				total_av_zone5[j]++;
			}
		}
	}
    }
    {
	int cnt = 0,j;
	int zone[6][5];
	int pk;

	for (i=0; i<6; i++) for (j=0; j<5; j++) zone[i][j] = 0;
	for (i=rp; i>=0; i--) {
		if (mrv[p][i].out == mrv[p][i].in) continue;
		pk=P_KIND(mrv[p][i].out);
		if (pk == K_MAN_1 || pk == K_PIN_1 || pk == K_SOU_1) {
			if (wait_kind[pk+1]) zone[cnt][0] = 1;
			if (wait_kind[pk+2]) zone[cnt][1] = 1;
			if (wait_kind[pk+3]) zone[cnt][2] = 1;
			if (wait_kind[pk+4]) zone[cnt][3] = 1;
			if (wait_kind[pk+5]) zone[cnt][4] = 1;
			cnt++;
		}
		if (pk == K_MAN_9 || pk == K_PIN_9 || pk == K_SOU_9) {
			if (wait_kind[pk-1]) zone[cnt][0] = 1;
			if (wait_kind[pk-2]) zone[cnt][1] = 1;
			if (wait_kind[pk-3]) zone[cnt][2] = 1;
			if (wait_kind[pk-4]) zone[cnt][3] = 1;
			if (wait_kind[pk-5]) zone[cnt][4] = 1;
			cnt++;
		}
		if (cnt == 6) break;
	}
	for (i=0; i<cnt; i++) {
		total_t_zone9_cnt[i]++;
		total_av_t_zone9_cnt++;
		for (j=0; j<5; j++) 
			if (zone[i][j]) {
				total_t_zone9[i][j]++;
				total_av_t_zone9[j]++;
			}
	}
    }
    {
	int cnt = 0,j;
	int zone[6][5];
	int pk;

	for (i=0; i<6; i++) for (j=0; j<5; j++) zone[i][j] = 0;
	for (i=rp; i>=0; i--) {
		if (mrv[p][i].out == mrv[p][i].in) continue;
		pk=P_KIND(mrv[p][i].out);
		if (pk == K_MAN_2 || pk == K_PIN_2 || pk == K_SOU_2) {
			if (wait_kind[pk-1]) zone[cnt][0] = 1;
			if (wait_kind[pk+1]) zone[cnt][1] = 1;
			if (wait_kind[pk+2]) zone[cnt][2] = 1;
			if (wait_kind[pk+3]) zone[cnt][3] = 1;
			if (wait_kind[pk+4]) zone[cnt][4] = 1;
			cnt++;
		}
		if (pk == K_MAN_8 || pk == K_PIN_8 || pk == K_SOU_8) {
			if (wait_kind[pk+1]) zone[cnt][0] = 1;
			if (wait_kind[pk-1]) zone[cnt][1] = 1;
			if (wait_kind[pk-2]) zone[cnt][2] = 1;
			if (wait_kind[pk-3]) zone[cnt][3] = 1;
			if (wait_kind[pk-4]) zone[cnt][4] = 1;
			cnt++;
		}
		if (cnt == 6) break;
	}
	for (i=0; i<cnt; i++) {
		total_t_zone8_cnt[i]++;
		total_av_t_zone8_cnt++;
		for (j=0; j<5; j++) 
			if (zone[i][j]) {
				total_t_zone8[i][j]++;
				total_av_t_zone8[j]++;
			}
	}
    }
    {
	int cnt = 0,j;
	int zone[6][5];
	int pk;

	for (i=0; i<6; i++) for (j=0; j<5; j++) zone[i][j] = 0;
	for (i=rp; i>=0; i--) {
		if (mrv[p][i].out == mrv[p][i].in) continue;
		pk=P_KIND(mrv[p][i].out);
		if (pk == K_MAN_3 || pk == K_PIN_3 || pk == K_SOU_3) {
			if (wait_kind[pk-2]) zone[cnt][0] = 1;
			if (wait_kind[pk-1]) zone[cnt][1] = 1;
			if (wait_kind[pk+1]) zone[cnt][2] = 1;
			if (wait_kind[pk+2]) zone[cnt][3] = 1;
			if (wait_kind[pk+3]) zone[cnt][4] = 1;
			cnt++;
		}
		if (pk == K_MAN_7 || pk == K_PIN_7 || pk == K_SOU_7) {
			if (wait_kind[pk+2]) zone[cnt][0] = 1;
			if (wait_kind[pk+1]) zone[cnt][1] = 1;
			if (wait_kind[pk-1]) zone[cnt][2] = 1;
			if (wait_kind[pk-2]) zone[cnt][3] = 1;
			if (wait_kind[pk-3]) zone[cnt][4] = 1;
			cnt++;
		}
		if (cnt == 6) break;
	}
	for (i=0; i<cnt; i++) {
		total_t_zone7_cnt[i]++;
		total_av_t_zone7_cnt++;
		for (j=0; j<5; j++) 
			if (zone[i][j]) {
				total_t_zone7[i][j]++;
				total_av_t_zone7[j]++;
			}
	}
    }
    {
	int cnt = 0,j;
	int zone[6][5];
	int pk;

	for (i=0; i<6; i++) for (j=0; j<5; j++) zone[i][j] = 0;
	for (i=rp; i>=0; i--) {
		if (mrv[p][i].out == mrv[p][i].in) continue;
		pk=P_KIND(mrv[p][i].out);
		if (pk == K_MAN_4 || pk == K_PIN_4 || pk == K_SOU_4) {
			if (wait_kind[pk-2]) zone[cnt][0] = 1;
			if (wait_kind[pk-1]) zone[cnt][1] = 1;
			if (wait_kind[pk+1]) zone[cnt][2] = 1;
			if (wait_kind[pk+2]) zone[cnt][3] = 1;
			if (wait_kind[pk+3]) zone[cnt][4] = 1;
			cnt++;
		}
		if (pk == K_MAN_6 || pk == K_PIN_6 || pk == K_SOU_6) {
			if (wait_kind[pk+2]) zone[cnt][0] = 1;
			if (wait_kind[pk+1]) zone[cnt][1] = 1;
			if (wait_kind[pk-1]) zone[cnt][2] = 1;
			if (wait_kind[pk-2]) zone[cnt][3] = 1;
			if (wait_kind[pk-3]) zone[cnt][4] = 1;
			cnt++;
		}
		if (cnt == 6) break;
	}
	for (i=0; i<cnt; i++) {
		total_t_zone6_cnt[i]++;
		total_av_t_zone6_cnt++;
		for (j=0; j<5; j++) 
			if (zone[i][j]) {
				total_t_zone6[i][j]++;
				total_av_t_zone6[j]++;
			}
	}
    }
    {
	int cnt = 0,j;
	int zone[6][8];
	int pk;

	for (i=0; i<6; i++) for (j=0; j<8; j++) zone[i][j] = 0;
	for (i=rp; i>=0; i--) {
		if (mrv[p][i].out == mrv[p][i].in) continue;
		pk=P_KIND(mrv[p][i].out);
		if (pk == K_MAN_5 || pk == K_PIN_5 || pk == K_SOU_5) {
			if (wait_kind[pk-4]) zone[cnt][0] = 1;
			if (wait_kind[pk-3]) zone[cnt][1] = 1;
			if (wait_kind[pk-2]) zone[cnt][2] = 1;
			if (wait_kind[pk-1]) zone[cnt][3] = 1;
			if (wait_kind[pk+1]) zone[cnt][4] = 1;
			if (wait_kind[pk+2]) zone[cnt][5] = 1;
			if (wait_kind[pk+3]) zone[cnt][6] = 1;
			if (wait_kind[pk+4]) zone[cnt][7] = 1;
			cnt++;
		}
		if (cnt == 6) break;
	}
	for (i=0; i<cnt; i++) {
		total_t_zone5_cnt[i]+=2;
		total_av_t_zone5_cnt+=2;
		for (j=0; j<4; j++) {
			if (zone[i][j]) {
				total_t_zone5[i][j]++;
				total_av_t_zone5[j]++;
			}
			if (zone[i][7-j]) {
				total_t_zone5[i][j]++;
				total_av_t_zone5[j]++;
			}
		}
	}
    }
    {
	int j,off,pk,wc,x,order;
	int sute[K_MAX];
	int sute_color[3];

	sute_color[0] = sute_color[1] = sute_color[2] = 0;
	order = 1;
	for (i=0; i<K_MAX; i++) sute[i] = 0;
	for (i=rp; i>=0; i--) {
		if (mrv[p][i].out == mrv[p][i].in) continue;
		pk=P_KIND(mrv[p][i].out);
		if (!sute[pk]) sute[pk] = order++;
		if (IS_MAN(pk)) sute_color[0]++;
		if (IS_PIN(pk)) sute_color[1]++;
		if (IS_SOU(pk)) sute_color[2]++;
	}

	for (i=0; i<3; i++) {
		off = i*9;
		wc = 0;
		x = sute_color[i]<5?sute_color[i]:5;
		for (j=0; j<9; j++) if (wait_kind[K_MAN_1+off+j]) wc=1;
		total_sute_wait_cnt[x]++;
		total_sute_wait[x] += wc;
	}

	for (i=0; i<3; i++) {
		off = i*9;
	    for (j=0; j<3; j++) {
		if (sute[K_MAN_1+j+off] && sute[K_MAN_3+j+off]
		 && sute[K_MAN_1+j+off] < sute[K_MAN_3+j+off]) {
				total_1ken_rev_cnt++;
				if (wait_kind[K_MAN_2+j+off])
					total_1ken_rev[0]++;
				if (wait_kind[K_MAN_4+j+off])
					total_1ken_rev[1]++;
				if (wait_kind[K_MAN_5+j+off])
					total_1ken_rev[2]++;
		}
		if (sute[K_MAN_9-j+off] && sute[K_MAN_7-j+off]
		 && sute[K_MAN_9-j+off] < sute[K_MAN_7-j+off]) {
				total_1ken_rev_cnt++;
				if (wait_kind[K_MAN_8-j+off])
					total_1ken_rev[0]++;
				if (wait_kind[K_MAN_6-j+off])
					total_1ken_rev[1]++;
				if (wait_kind[K_MAN_5-j+off])
					total_1ken_rev[2]++;
		}
	    }
	}

	for (i=0; i<3; i++) {
		off = i*9;
		/* mentu */
		for (j=0; j<6; j++) {
			if (sute[K_MAN_2+j+off] && sute[K_MAN_3+j+off]) {
				total_0ken_cnt++;
				if (wait_kind[K_MAN_1+j+off])
					total_0ken[0]++;
				if (wait_kind[K_MAN_4+j+off])
					total_0ken[1]++;
			}
		}
		/* aida 1 ken */
		for (j=0; j<7; j++) {
			if (sute[K_MAN_1+j+off] && sute[K_MAN_3+j+off]) {
				total_1ken_cnt++;
				if (wait_kind[K_MAN_2+j+off])
					total_1ken[0]++;
			}
		}
		/* aida 2 ken */
		for (j=0; j<6; j++) {
			if (sute[K_MAN_1+j+off] && sute[K_MAN_4+j+off]) {
				total_2ken_cnt++;
				if (wait_kind[K_MAN_2+j+off])
					total_2ken[0]++;
				if (wait_kind[K_MAN_3+j+off])
					total_2ken[1]++;
			}
		}
		/* aida 3 ken */
		for (j=0; j<5; j++) {
			if (sute[K_MAN_1+j+off] && sute[K_MAN_5+j+off]) {
				total_3ken_cnt++;
				if (wait_kind[K_MAN_2+j+off])
					total_3ken[0]++;
				if (wait_kind[K_MAN_3+j+off])
					total_3ken[1]++;
				if (wait_kind[K_MAN_4+j+off])
					total_3ken[2]++;
			}
		}
		/* aida 4 ken */
		for (j=0; j<4; j++) {
			if (sute[K_MAN_1+j+off] && sute[K_MAN_6+j+off]) {
				total_4ken_cnt++;
				if (wait_kind[K_MAN_2+j+off])
					total_4ken[0]++;
				if (wait_kind[K_MAN_3+j+off])
					total_4ken[1]++;
				if (wait_kind[K_MAN_4+j+off])
					total_4ken[2]++;
				if (wait_kind[K_MAN_5+j+off])
					total_4ken[3]++;
			}
		}
	}
    }
    {
	int j,k;
	yomi(p);
	total_yomi_count++;
	for (i=0; i<5; i++) {
		if (wait_kind[yomi_rank[i+1]]) 
			total_yomi_atari[i] ++;
		total_yomi_point[i] += yomi_pai[yomi_rank[i+1]];
	}
    }
}

#define PERCENTR(x)   ((total_reach==0)?0:100.0*x/total_reach)
#define PERCENT(x)    ((x==0)?0:100.0*x##_agari/x)
#define PERCENTA(x,i) ((x[i]==0)?0:100.0*x##_agari[i]/x[i])

yomi_kekka() {
        int i;
        printf("リーチ回数 %d 平均和了率 %5.1f\n\n"
                ,total_reach,PERCENT(total_reach));

        printf("待ち数:   1     2     3     4     5     6     7\n");
        printf("待ち率:");
        for (i=1; i< 7; i++) {
                printf("%5.1f ",PERCENTR(total_waiting[i]));
        }
        printf("\n和了率:");
        for (i=1; i< 7; i++) {
                printf("%5.1f ",PERCENTA(total_waiting,i));
        }
        printf("\n\n");

        printf("待ち: 間チャン/辺チャン/単騎/両面/シャボ/延単/多面\n");
        printf("待ち率:%5.1f %5.1f %5.1f %5.1f %5.1f %5.1f %5.1f\n"
                ,PERCENTR(total_kanchan)
                ,PERCENTR(total_penchan)
                ,PERCENTR(total_tanki)
                ,PERCENTR(total_ryanmen)
                ,PERCENTR(total_syabo)
                ,PERCENTR(total_nobetan)
                ,PERCENTR(total_tamen));
        printf("和了率:%5.1f %5.1f %5.1f %5.1f %5.1f %5.1f %5.1f\n\n"
                ,PERCENT(total_kanchan)
                ,PERCENT(total_penchan)
                ,PERCENT(total_tanki)
                ,PERCENT(total_ryanmen)
                ,PERCENT(total_syabo)
                ,PERCENT(total_nobetan)
                ,PERCENT(total_tamen));

        printf("\n順目:     1     2     3     4     5     6\n");
        printf("待ち率:");
        for (i=0; i< 6; i++) {
                printf("%5.1f " ,PERCENTR(total_reach_pos[i]));
        }
        printf("\n和了率:");
        for (i=0; i< 6; i++) {
                printf("%5.1f ",PERCENTA(total_reach_pos,i));
        }
        printf("\n\n順目:     7     8     9    10    11    12\n");
        printf("待ち率:");
        for (i=6; i< 12; i++) {
                printf("%5.1f " ,PERCENTR(total_reach_pos[i]));
        }
        printf("\n和了率:");
        for (i=6; i< 12; i++) {
                printf("%5.1f ",PERCENTA(total_reach_pos,i));
        }
        printf("\n\n順目:    13    14    15    16    17    18\n");
        printf("待ち率:");
        for (i=12; i< 18; i++) {
                printf("%5.1f " ,PERCENTR(total_reach_pos[i]));
        }
        printf("\n和了率:");
        for (i=12; i< 18; i++) {
                printf("%5.1f ",PERCENTA(total_reach_pos,i));
        }
        printf("\n");

        printf("\n待ち牌:");
        for (i=K_MAN_1; i<= K_MAN_9; i++) {
                printf("  %s ",pai_strings[(i<<2)+1]);
        }
        printf("\n待ち率:");
        for (i=K_MAN_1; i<= K_MAN_9; i++) {
                printf("%5.1f ",PERCENTR(total_wait_kind[i]));
        }
        printf("\n和了率:");
        for (i=K_MAN_1; i<= K_MAN_9; i++) {
                printf("%5.1f " ,PERCENTA(total_wait_kind,i));
        }
        printf("\n\n待ち牌:");
        for (i=K_PIN_1; i<= K_PIN_9; i++) {
                printf("  %s ",pai_strings[(i<<2)+1]);
        }
        printf("\n待ち率:");
        for (i=K_PIN_1; i<= K_PIN_9; i++) {
                printf("%5.1f ",PERCENTR(total_wait_kind[i]));
        }
        printf("\n和了率:");
        for (i=K_PIN_1; i<= K_PIN_9; i++) {
                printf("%5.1f " ,PERCENTA(total_wait_kind,i));
        }
        printf("\n\n待ち牌:");
        for (i=K_SOU_1; i<= K_SOU_9; i++) {
                printf("  %s ",pai_strings[(i<<2)+1]);
        }
        printf("\n待ち率:");
        for (i=K_SOU_1; i<= K_SOU_9; i++) {
                printf("%5.1f ",PERCENTR(total_wait_kind[i]));
        }
        printf("\n和了率:");
        for (i=K_SOU_1; i<= K_SOU_9; i++) {
                printf("%5.1f " ,PERCENTA(total_wait_kind,i));
        }
        printf("\n\n待ち牌:");
        for (i=K_TON; i<= K_CHUN; i++) {
                printf("  %s ",pai_strings[(i<<2)+1]);
        }
        printf("\n待ち率:");
        for (i=K_TON; i<= K_CHUN; i++) {
                printf("%5.1f ",PERCENTR(total_wait_kind[i]));
        }
        printf("\n和了率:");
        for (i=K_TON; i<= K_CHUN; i++) {
                printf("%5.1f " ,PERCENTA(total_wait_kind,i));
        }
	printf("\n\n");

        printf("待ちから見た統計\n\n");

	printf("    両面待ちの場合\n");
	if (total_ryanmen) {
		printf("\tすて牌状況\n");
		printf("\t%5.1f   *   %5.1f %5.1f   *   %5.1f\n"
			,100.0 * total_ryanmen_soba[0]/total_ryanmen
			,100.0 * total_ryanmen_soba[1]/total_ryanmen
			,100.0 * total_ryanmen_soba[2]/total_ryanmen
			,100.0 * total_ryanmen_soba[3]/total_ryanmen);
	}
	printf("    カンチャンの場合\n");
	if (total_kanchan) {
		printf("\tすて牌状況\n");
		printf("\t%5.1f %5.1f %5.1f   *   %5.1f %5.1f %5.1f\n"
			,100.0 * total_kanchan_soba[0]/total_kanchan
			,100.0 * total_kanchan_soba[1]/total_kanchan
			,100.0 * total_kanchan_soba[2]/total_kanchan
			,100.0 * total_kanchan_soba[3]/total_kanchan
			,100.0 * total_kanchan_soba[4]/total_kanchan
			,100.0 * total_kanchan_soba[5]/total_kanchan);
	}
	printf("    ペンチャンの場合\n");
	if (total_penchan) {
		printf("\tすて牌状況\n");
		printf("\t%5.1f %5.1f    *   %5.1f %5.1f\n"
			,100.0 * total_penchan_soba[0]/total_penchan
			,100.0 * total_penchan_soba[1]/total_penchan
			,100.0 * total_penchan_soba[2]/total_penchan
			,100.0 * total_penchan_soba[3]/total_penchan);
	}

    printf("捨て牌から見た統計\n");
    {
	int n,w,pk;
	printf("待ち牌率\n");
	printf("\t  1/9   2/8   3/7   4/6    5     字\n\t");
	w = 0;
	for (i=0; i<3; i++) {
		w += total_wait_kind[K_MAN_1+i*9];
		w += total_wait_kind[K_MAN_9+i*9];
	}
	printf("%5.1f ",100.0 * w / (total_reach * 6 ) );
	w = 0;
	for (i=0; i<3; i++) {
		w += total_wait_kind[K_MAN_2+i*9];
		w += total_wait_kind[K_MAN_8+i*9];
	}
	printf("%5.1f ",100.0 * w / (total_reach * 6 ) );
	w = 0;
	for (i=0; i<3; i++) {
		w += total_wait_kind[K_MAN_3+i*9];
		w += total_wait_kind[K_MAN_7+i*9];
	}
	printf("%5.1f ",100.0 * w / (total_reach * 6 ) );
	w = 0;
	for (i=0; i<3; i++) {
		w += total_wait_kind[K_MAN_4+i*9];
		w += total_wait_kind[K_MAN_6+i*9];
	}
	printf("%5.1f ",100.0 * w / (total_reach * 6 ) );
	w = 0;
	for (i=0; i<3; i++) {
		w += total_wait_kind[K_MAN_5+i*9];
	}
	printf("%5.1f ",100.0 * w / (total_reach * 3 ) );
	w = 0;
	for (i=0; i<7; i++) {
		w += total_wait_kind[K_TON+i];
	}
	printf("%5.1f ",100.0 * w / (total_reach * 7 ) );
	printf("\n\n");
    }
	
	printf("\n1/9牌を捨てたときの待ち牌率\n");
	printf("\t     1/9  2/8   3/7   4/6    5    6/4\n");
	for (i=0; i<6; i++) {
		if (total_zone9_cnt[i])
			printf("\t%d:    * %5.1f %5.1f %5.1f %5.1f %5.1f \n",i
			  ,100.0 * total_zone9[i][0]/total_zone9_cnt[i]
			  ,100.0 * total_zone9[i][1]/total_zone9_cnt[i]
			  ,100.0 * total_zone9[i][2]/total_zone9_cnt[i]
			  ,100.0 * total_zone9[i][3]/total_zone9_cnt[i]
			  ,100.0 * total_zone9[i][4]/total_zone9_cnt[i]);
	}
	if (total_av_zone9_cnt)
		printf("\tav    * %5.1f %5.1f %5.1f %5.1f %5.1f \n"
		  ,100.0 * total_av_zone9[0]/total_av_zone9_cnt
		  ,100.0 * total_av_zone9[1]/total_av_zone9_cnt
		  ,100.0 * total_av_zone9[2]/total_av_zone9_cnt
		  ,100.0 * total_av_zone9[3]/total_av_zone9_cnt
		  ,100.0 * total_av_zone9[4]/total_av_zone9_cnt);
	printf("\n2/8牌を捨てたときの待ち牌率\n");
	printf("\t    1/9    2/8  3/7   4/6    5    6/4\n");
	for (i=0; i<6; i++) {
		if (total_zone8_cnt[i])
			printf("\t%d: %5.1f    * %5.1f %5.1f %5.1f %5.1f \n",i
			  ,100.0 * total_zone8[i][0]/total_zone8_cnt[i]
			  ,100.0 * total_zone8[i][1]/total_zone8_cnt[i]
			  ,100.0 * total_zone8[i][2]/total_zone8_cnt[i]
			  ,100.0 * total_zone8[i][3]/total_zone8_cnt[i]
			  ,100.0 * total_zone8[i][4]/total_zone8_cnt[i]);
	}
	if (total_av_zone8_cnt)
		printf("\tav %5.1f    * %5.1f %5.1f %5.1f %5.1f \n"
		  ,100.0 * total_av_zone8[0]/total_av_zone8_cnt
		  ,100.0 * total_av_zone8[1]/total_av_zone8_cnt
		  ,100.0 * total_av_zone8[2]/total_av_zone8_cnt
		  ,100.0 * total_av_zone8[3]/total_av_zone8_cnt
		  ,100.0 * total_av_zone8[4]/total_av_zone8_cnt);
	printf("\n3/7牌を捨てたときの待ち牌率\n");
	printf("\t     1/9   2/8   3/7  4/6    5    6/4\n");
	for (i=0; i<6; i++) {
		if (total_zone7_cnt[i])
			printf("\t%d: %5.1f %5.1f    * %5.1f %5.1f %5.1f \n",i
			  ,100.0 * total_zone7[i][0]/total_zone7_cnt[i]
			  ,100.0 * total_zone7[i][1]/total_zone7_cnt[i]
			  ,100.0 * total_zone7[i][2]/total_zone7_cnt[i]
			  ,100.0 * total_zone7[i][3]/total_zone7_cnt[i]
			  ,100.0 * total_zone7[i][4]/total_zone7_cnt[i]);
	}
	if (total_av_zone7_cnt)
		printf("\tav %5.1f %5.1f    * %5.1f %5.1f %5.1f \n"
		  ,100.0 * total_av_zone7[0]/total_av_zone7_cnt
		  ,100.0 * total_av_zone7[1]/total_av_zone7_cnt
		  ,100.0 * total_av_zone7[2]/total_av_zone7_cnt
		  ,100.0 * total_av_zone7[3]/total_av_zone7_cnt
		  ,100.0 * total_av_zone7[4]/total_av_zone7_cnt);
	printf("\n4/6牌を捨てたときの待ち牌率\n");
	printf("\t     2/8   3/7   4/6   5    6/4   7/3\n");
	for (i=0; i<6; i++) {
		if (total_zone6_cnt[i])
			printf("\t%d: %5.1f %5.1f    * %5.1f %5.1f %5.1f \n",i
			  ,100.0 * total_zone6[i][0]/total_zone6_cnt[i]
			  ,100.0 * total_zone6[i][1]/total_zone6_cnt[i]
			  ,100.0 * total_zone6[i][2]/total_zone6_cnt[i]
			  ,100.0 * total_zone6[i][3]/total_zone6_cnt[i]
			  ,100.0 * total_zone6[i][4]/total_zone6_cnt[i]);
	}
	if (total_av_zone6_cnt)
		printf("\tav %5.1f %5.1f    * %5.1f %5.1f %5.1f \n"
		  ,100.0 * total_av_zone6[0]/total_av_zone6_cnt
		  ,100.0 * total_av_zone6[1]/total_av_zone6_cnt
		  ,100.0 * total_av_zone6[2]/total_av_zone6_cnt
		  ,100.0 * total_av_zone6[3]/total_av_zone6_cnt
		  ,100.0 * total_av_zone6[4]/total_av_zone6_cnt);
	printf("\n5牌を捨てたときの待ち牌率\n");
	printf("\t     1/9   2/8   3/7   4/6  5\n");
	for (i=0; i<6; i++) {
	 if (total_zone5_cnt[i])
		  printf("\t%d: %5.1f %5.1f %5.1f %5.1f  *\n",i
			  ,100.0 * total_zone5[i][0]/total_zone5_cnt[i]
			  ,100.0 * total_zone5[i][1]/total_zone5_cnt[i]
			  ,100.0 * total_zone5[i][2]/total_zone5_cnt[i]
			  ,100.0 * total_zone5[i][3]/total_zone5_cnt[i]);
	}
	if (total_av_zone5_cnt)
	  printf("\tav %5.1f %5.1f %5.1f %5.1f  *\n"
		  ,100.0 * total_av_zone5[0]/total_av_zone5_cnt
		  ,100.0 * total_av_zone5[1]/total_av_zone5_cnt
		  ,100.0 * total_av_zone5[2]/total_av_zone5_cnt
		  ,100.0 * total_av_zone5[3]/total_av_zone5_cnt);

	printf("\n手出しで1/9牌を捨てたときの待ち牌率\n");
	printf("\t     1/9  2/8   3/7   4/6    5    6/4\n");
	for (i=0; i<6; i++) {
		if (total_t_zone9_cnt[i])
			printf("\t%d:    * %5.1f %5.1f %5.1f %5.1f %5.1f \n",i
			  ,100.0 * total_t_zone9[i][0]/total_t_zone9_cnt[i]
			  ,100.0 * total_t_zone9[i][1]/total_t_zone9_cnt[i]
			  ,100.0 * total_t_zone9[i][2]/total_t_zone9_cnt[i]
			  ,100.0 * total_t_zone9[i][3]/total_t_zone9_cnt[i]
			  ,100.0 * total_t_zone9[i][4]/total_t_zone9_cnt[i]);
	}
	if (total_av_t_zone9_cnt)
		printf("\tav    * %5.1f %5.1f %5.1f %5.1f %5.1f \n"
		  ,100.0 * total_av_t_zone9[0]/total_av_t_zone9_cnt
		  ,100.0 * total_av_t_zone9[1]/total_av_t_zone9_cnt
		  ,100.0 * total_av_t_zone9[2]/total_av_t_zone9_cnt
		  ,100.0 * total_av_t_zone9[3]/total_av_t_zone9_cnt
		  ,100.0 * total_av_t_zone9[4]/total_av_t_zone9_cnt);
	printf("\n手出しで2/8牌を捨てたときの待ち牌率\n");
	printf("\t    1/9    2/8  3/7   4/6    5    6/4\n");
	for (i=0; i<6; i++) {
		if (total_t_zone8_cnt[i])
			printf("\t%d: %5.1f    * %5.1f %5.1f %5.1f %5.1f \n",i
			  ,100.0 * total_t_zone8[i][0]/total_t_zone8_cnt[i]
			  ,100.0 * total_t_zone8[i][1]/total_t_zone8_cnt[i]
			  ,100.0 * total_t_zone8[i][2]/total_t_zone8_cnt[i]
			  ,100.0 * total_t_zone8[i][3]/total_t_zone8_cnt[i]
			  ,100.0 * total_t_zone8[i][4]/total_t_zone8_cnt[i]);
	}
	if (total_av_t_zone8_cnt)
		printf("\tav %5.1f    * %5.1f %5.1f %5.1f %5.1f \n"
		  ,100.0 * total_av_t_zone8[0]/total_av_t_zone8_cnt
		  ,100.0 * total_av_t_zone8[1]/total_av_t_zone8_cnt
		  ,100.0 * total_av_t_zone8[2]/total_av_t_zone8_cnt
		  ,100.0 * total_av_t_zone8[3]/total_av_t_zone8_cnt
		  ,100.0 * total_av_t_zone8[4]/total_av_t_zone8_cnt);
	printf("\n手出しで3/7牌を捨てたときの待ち牌率\n");
	printf("\t     1/9   2/8   3/7  4/6    5    6/4\n");
	for (i=0; i<6; i++) {
		if (total_t_zone7_cnt[i])
			printf("\t%d: %5.1f %5.1f    * %5.1f %5.1f %5.1f \n",i
			  ,100.0 * total_t_zone7[i][0]/total_t_zone7_cnt[i]
			  ,100.0 * total_t_zone7[i][1]/total_t_zone7_cnt[i]
			  ,100.0 * total_t_zone7[i][2]/total_t_zone7_cnt[i]
			  ,100.0 * total_t_zone7[i][3]/total_t_zone7_cnt[i]
			  ,100.0 * total_t_zone7[i][4]/total_t_zone7_cnt[i]);
	}
	if (total_av_t_zone7_cnt)
		printf("\tav %5.1f %5.1f    * %5.1f %5.1f %5.1f \n"
		  ,100.0 * total_av_t_zone7[0]/total_av_t_zone7_cnt
		  ,100.0 * total_av_t_zone7[1]/total_av_t_zone7_cnt
		  ,100.0 * total_av_t_zone7[2]/total_av_t_zone7_cnt
		  ,100.0 * total_av_t_zone7[3]/total_av_t_zone7_cnt
		  ,100.0 * total_av_t_zone7[4]/total_av_t_zone7_cnt);
	printf("\n手出しで4/6牌を捨てたときの待ち牌率\n");
	printf("\t     2/8   3/7   4/6   5    6/4   7/3\n");
	for (i=0; i<6; i++) {
		if (total_t_zone6_cnt[i])
			printf("\t%d: %5.1f %5.1f    * %5.1f %5.1f %5.1f \n",i
			  ,100.0 * total_t_zone6[i][0]/total_t_zone6_cnt[i]
			  ,100.0 * total_t_zone6[i][1]/total_t_zone6_cnt[i]
			  ,100.0 * total_t_zone6[i][2]/total_t_zone6_cnt[i]
			  ,100.0 * total_t_zone6[i][3]/total_t_zone6_cnt[i]
			  ,100.0 * total_t_zone6[i][4]/total_t_zone6_cnt[i]);
	}
	if (total_av_t_zone6_cnt)
		printf("\tav %5.1f %5.1f    * %5.1f %5.1f %5.1f \n"
		  ,100.0 * total_av_t_zone6[0]/total_av_t_zone6_cnt
		  ,100.0 * total_av_t_zone6[1]/total_av_t_zone6_cnt
		  ,100.0 * total_av_t_zone6[2]/total_av_t_zone6_cnt
		  ,100.0 * total_av_t_zone6[3]/total_av_t_zone6_cnt
		  ,100.0 * total_av_t_zone6[4]/total_av_t_zone6_cnt);

	printf("\n手出しで5牌を捨てたときの待ち牌率\n");
	printf("\t     1/9   2/8   3/7   4/6  5\n");
	for (i=0; i<6; i++) {
	 if (total_t_zone5_cnt[i])
		  printf("\t%d: %5.1f %5.1f %5.1f %5.1f  *\n",i
			  ,100.0 * total_t_zone5[i][0]/total_t_zone5_cnt[i]
			  ,100.0 * total_t_zone5[i][1]/total_t_zone5_cnt[i]
			  ,100.0 * total_t_zone5[i][2]/total_t_zone5_cnt[i]
			  ,100.0 * total_t_zone5[i][3]/total_t_zone5_cnt[i]);
	}
	if (total_av_t_zone5_cnt)
	  printf("\tav %5.1f %5.1f %5.1f %5.1f  *\n"
		  ,100.0 * total_av_t_zone5[0]/total_av_t_zone5_cnt
		  ,100.0 * total_av_t_zone5[1]/total_av_t_zone5_cnt
		  ,100.0 * total_av_t_zone5[2]/total_av_t_zone5_cnt
		  ,100.0 * total_av_t_zone5[3]/total_av_t_zone5_cnt);

	if (total_yomi_count) {
		int p,a;
		printf("\n読みの確からしさ\n");
		p = a = 0;
		for (i=0;i<5; i++) {
			p += total_yomi_point[i];
			a += total_yomi_atari[i];
		    printf("予想%d位までのあたり確率 予想 %5.1f 実際 %5.1f\n",i+1
			,100.0 * p / 10000 / total_yomi_count
			,100.0 * a / total_yomi_count);
		}
	}
	if (total_0ken_cnt) {
		printf("順子落し   %5.1f *  * %5.1f\n"
			,100.0 * total_0ken[0] / total_0ken_cnt
			,100.0 * total_0ken[1] / total_0ken_cnt);
	}
	if (total_1ken_cnt) {
		printf("間 1ケン   * %5.1f   *\n"
			,100.0 * total_1ken[0] / total_1ken_cnt);
	}
	if (total_2ken_cnt) {
		printf("間 2ケン   * %5.1f %5.1f   *\n"
			,100.0 * total_2ken[0] / total_2ken_cnt
			,100.0 * total_2ken[1] / total_2ken_cnt);
	}
	if (total_3ken_cnt) {
		printf("間 3ケン   * %5.1f %5.1f %5.1f  *\n"
			,100.0 * total_3ken[0] / total_3ken_cnt
			,100.0 * total_3ken[1] / total_3ken_cnt
			,100.0 * total_3ken[2] / total_3ken_cnt);
	}
	if (total_4ken_cnt) {
		printf("間 4ケン   * %5.1f %5.1f %5.1f %5.1f  *\n"
			,100.0 * total_4ken[0] / total_4ken_cnt
			,100.0 * total_4ken[1] / total_4ken_cnt
			,100.0 * total_4ken[2] / total_4ken_cnt
			,100.0 * total_4ken[3] / total_4ken_cnt);
	}
	if (total_1ken_rev_cnt) {
		printf("間 1ケン逆切り   * %5.1f   * %5.1f %5.1f\n"
			,100.0 * total_1ken_rev[0] / total_1ken_rev_cnt
			,100.0 * total_1ken_rev[1] / total_1ken_rev_cnt
			,100.0 * total_1ken_rev[2] / total_1ken_rev_cnt);
	}
	printf("\n捨て牌枚数と待ち確率\n");
	for (i=0; i<6; i++) {
		if (total_sute_wait_cnt[i])
		printf("%d: %5.1f\n",i
			,100.0 * total_sute_wait[i]/total_sute_wait_cnt[i]);
	}
}

yomi(pp) int pp; {
	int i,j,x,n,m,paik,off,tot;
	int yp[K_MAX];
	int sute[K_MAX];
	int sute2[K_MAX];
	int sute3[K_MAX];
	int sute3_cnt = 0;
	int reachf;
	int color_sute[3];

	for (i=0; i<3; i++) color_sute[i] = 0;
	for (i=0; i<K_MAX; i++)
		yomi_pai[i] = sute[i] = sute2[i] = sute3[i] = 0;

	{
		for (i=0; i<3; i++) {
			yp[K_MAN_1+i*9] = 250;
			yp[K_MAN_2+i*9] = 440;
			yp[K_MAN_3+i*9] = 700;
			yp[K_MAN_4+i*9] = 800;
			yp[K_MAN_5+i*9] = 870;
			yp[K_MAN_6+i*9] = 800;
			yp[K_MAN_7+i*9] = 700;
			yp[K_MAN_8+i*9] = 440;
			yp[K_MAN_9+i*9] = 250;
		}
		for (i=0; i<7; i++) {
			yp[K_TON+i] = 50;
		}
		reachf = 0;
		for (i=0; i<=RIVER_MAX; i++) {
			paik = P_KIND(mrv[pp][i].out);
			if (T_GET(mrv[pp][i].attr) == T_HN_TSM) break;
			if (!reachf)
				sute[paik] = 1;
			if (T_GET(mrv[pp][i].attr) == T_RV_RCH)
				reachf = 1;
			yp[paik] = 0;
			sute2[paik] = 1;
			if (mrv[pp][i].out != mrv[pp][i].in) {
				if (IS_MAN(paik)) color_sute[0]++;
				if (IS_PIN(paik)) color_sute[1]++;
				if (IS_SOU(paik)) color_sute[2]++;
				sute3[paik] = ++sute3_cnt;
			}
		}
		for (i=0; i<3; i++) {
			off = i*9;
			if (sute[K_MAN_1+off]) {
				yp[K_MAN_2+off] *= 4.2/4.4;
				yp[K_MAN_3+off] *= 6.8/7.0;
				yp[K_MAN_4+off] *= 6.9/8.0;
				yp[K_MAN_5+off] *= 9.0/8.7;
				yp[K_MAN_6+off] *= 8.5/8.0;
			}
			if (sute[K_MAN_9+off]) {
				yp[K_MAN_8+off] *= 4.2/4.4;
				yp[K_MAN_7+off] *= 6.8/7.0;
				yp[K_MAN_6+off] *= 6.9/8.0;
				yp[K_MAN_5+off] *= 9.0/8.7;
				yp[K_MAN_4+off] *= 8.5/8.0;
			}
			if (sute[K_MAN_2+off]) {
				yp[K_MAN_1+off] *= 1.4/2.5;
				yp[K_MAN_3+off] *= 5.9/7.0;
				yp[K_MAN_4+off] *= 8.2/8.0;
				yp[K_MAN_5+off] *= 7.5/8.7;
				yp[K_MAN_6+off] *= 9.3/8.0;
			}
			if (sute[K_MAN_8+off]) {
				yp[K_MAN_9+off] *= 1.4/2.5;
				yp[K_MAN_7+off] *= 5.9/7.0;
				yp[K_MAN_6+off] *= 8.2/8.0;
				yp[K_MAN_5+off] *= 7.5/8.7;
				yp[K_MAN_4+off] *= 9.3/8.0;
			}
			if (sute[K_MAN_3+off]) {
				yp[K_MAN_1+off] *= 0.9/2.5;
				yp[K_MAN_2+off] *= 1.9/4.4;
				yp[K_MAN_4+off] *= 6.6/8.0;
				yp[K_MAN_5+off] *= 8.6/8.7;
				yp[K_MAN_6+off] *= 6.5/8.0;
			}
			if (sute[K_MAN_7+off]) {
				yp[K_MAN_9+off] *= 0.9/2.5;
				yp[K_MAN_8+off] *= 1.9/4.4;
				yp[K_MAN_6+off] *= 6.6/8.0;
				yp[K_MAN_5+off] *= 8.6/8.7;
				yp[K_MAN_4+off] *= 6.5/8.0;
			}
			if (sute[K_MAN_4+off]) {
				yp[K_MAN_2+off] *= 2.4/4.4;
				yp[K_MAN_3+off] *= 3.5/7.0;
				yp[K_MAN_5+off] *= 7.6/8.0;
				yp[K_MAN_6+off] *= 8.0/8.7;
				yp[K_MAN_7+off] *= 5.3/7.0;
			}
			if (sute[K_MAN_6+off]) {
				yp[K_MAN_8+off] *= 2.4/4.4;
				yp[K_MAN_7+off] *= 3.5/7.0;
				yp[K_MAN_5+off] *= 7.6/8.0;
				yp[K_MAN_4+off] *= 8.0/8.7;
				yp[K_MAN_3+off] *= 5.3/7.0;
			}
			if (sute[K_MAN_5+off]) {
				yp[K_MAN_1+off] *= 2.5/2.5;
				yp[K_MAN_2+off] *= 1.9/4.4;
				yp[K_MAN_3+off] *= 5.7/7.0;
				yp[K_MAN_4+off] *= 5.4/8.0;
				yp[K_MAN_6+off] *= 5.4/8.0;
				yp[K_MAN_7+off] *= 5.7/7.0;
				yp[K_MAN_8+off] *= 1.9/4.4;
				yp[K_MAN_9+off] *= 2.5/2.5;
			}
		}
		for (i=0; i<3; i++) {
			off = i*9;
			for (j=0; j<5; j++) {
			   if (!sute2[K_MAN_1+j+off]
			       && !sute2[K_MAN_2+j+off]
			       && !sute2[K_MAN_3+j+off]) {
					yp[K_MAN_2+j+off] *= 1.2;
			    }
			}
			for (j=0; j<4; j++) {
			   if (!sute2[K_MAN_1+j+off]
			       && !sute2[K_MAN_2+j+off]
			       && !sute2[K_MAN_3+j+off]
			       && !sute2[K_MAN_4+j+off]) {
					yp[K_MAN_1+j+off] *= 1.3;
					yp[K_MAN_4+j+off] *= 1.3;
			    }
			}
		}
		for (i=1; i<K_MAX; i++) {
			yomi_pai[i] += yp[i];
		}
	}
	tot = 0;
	for (i=1; i<K_MAX; i++) {
		tot += yomi_pai[i];
	}
	for (i=1; i<K_MAX; i++) {
		if (tot)
			yomi_pai[i] *= 16120 /tot;
		else
			yomi_pai[i] = 0;
	}

	for (i=1; i< K_MAX; i++) {
		yomi_rank[i] = i;
	}
	for (i=1; i< K_MAX-1; i++) for(j=1; j<K_MAX-i; j++) {
		if (yomi_pai[yomi_rank[j]] < yomi_pai[yomi_rank[j+1]]) {
			int tmp;
			tmp = yomi_rank[j];
			yomi_rank[j] = yomi_rank[j+1];
			yomi_rank[j+1] = tmp;
		}
	}
}
