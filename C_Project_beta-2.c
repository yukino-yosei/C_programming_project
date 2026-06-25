#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int score[7][40][7];//score[i][j][k] : i반 j번 학생의 k과목 성적
int student[7],mode;//student[i] : i반 학급의 학생 수
int a,b,s;
char subject[7][30] = {"국어","영어","수학","한국사","사회탐구","과학탐구","전체"};

void update(int i,int j,int scr[7]){
    for(int k=0;k<7;++k){
        score[i][j][k] = scr[k];
    }
}

void mge_sort(int s,int e,int arr[]){
    if(s >= e) return;
    int m = (s + e) / 2;
    mge_sort(s,m,arr);
    mge_sort(m+1,e,arr);
    int *tmp = (int*)malloc((e - s + 1) * sizeof(int)),f = s,r = m + 1,t = 0;
    while(f <= m && r <= e){
        if(arr[f] > arr[r]) tmp[t++] = arr[f++];
        else tmp[t++] = arr[r++];
    }
    while(f <= m) tmp[t++] = arr[f++];
    while(r <= e) tmp[t++] = arr[r++];
    for(t = 0;t < e - s + 1;++t){
        arr[s + t] = tmp[t];
    }
    free(tmp);
}

int compute_rank(int i,int j,int subj){
    //반,번호는 1원점 인덱싱(-1할 필요 없음!)
    int scr = score[i][j][subj];
    int tmp[40];
    for(int k = 1;k <= student[i];++k){
        tmp[k] = score[i][k][subj];
    }
    mge_sort(1,student[i],tmp);
    for(int k = student[i];k > 0;--k){
        if(tmp[k] == scr){
            return k;
        }
    }
}

void print_index(int i,int subj){
    //Mean, grade cut, stdev
    double m = 0,sm = 0,stdev;
    int tmp[40];
    tmp[0] = tmp[student[i] + 1] = -1;
    for(int k = 1;k <= student[i];++k){
        m += score[i][k][subj];
        sm += score[i][k][subj] * score[i][k][subj];
        tmp[k] = score[i][k][subj];
    }
    //V(X) = E(X^2) - {E(X)}^2
    m /= student[i];
    stdev = sqrt((sm / student[i]) - m*m);
    mge_sort(1,student[i],tmp);
    int cut_idx = (int)((double)(0.1) * student[i]);
    for(;cut_idx > 0 && tmp[cut_idx] == tmp[cut_idx + 1];--cut_idx);
    printf("%d반 %s 과목 평균 점수 : %.2f\n",i,subject[subj],m);
    printf("%d반 %s 과목 점수 표준편차 : %.2f\n",i,subject[subj],stdev);
    printf("%d반 %s 과목 1등급 컷 : %d(-1이라고 쓰면 1등급 컷이 터진 것)\n",i,subject[subj],tmp[cut_idx]);
}

int compute_grade(int i,int j,int subj) {
    int rank = compute_rank(i,j,subj);
    int percentage = (double)rank / student[i] * 100;
    if (percentage <= 10) {
        return 1;
    }
    else if (percentage <= 34) {
        return 2;
    }
    else if (percentage <= 66) {
        return 3;
    }
    else if (percentage <= 90) {
        return 4;
    }
    else if (percentage <= 100) {
        return 5;
    }
}

int main(){
    printf("===========================================================\n");
    printf("성적분석 프로그램\n");

    for(int i = 1;i <= 6;++i){
        printf("%d반의 학생 수를 입력해 주세요.\n====>   ",i);
        scanf("%d",student + i);
        for(int j = 1;j <= student[i];++j){
            printf("%d반 %d번 학생의 성적을 입력해 주세요(국어-영어-수학-한국사-사회탐구-과학탐구 순서)\n====>   ",i,j);
            for(int k = 0;k < 6;++k){
                scanf("%d",&score[i][j][k]);
                score[i][j][6] += score[i][j][k];
            }
        }
    }
    while(1){
        printf("\n===========================================================\n");
        printf("원하는 작업을 선택하세요.\n");
        printf("1. 학생의 성적을 수정\n");
        printf("2. 학생의 석차를 계산\n");
        printf("3. 학생의 등급을 계산\n");
        printf("4. 학급 성적지표를 계산\n");
        printf("5. 프로그램을 종료\n");
        scanf("%d",&mode);
        switch (mode){
            case 1:
                printf("성적을 수정할 사람의 반, 번호를 순서대로 입력\n===>   ");
                scanf("%d %d",&a,&b);
                printf("%d반 %d번 학생의 성적 입력\n국어-영어-수학-한국사-사회탐구-과학탐구 순서\n===>   ",a,b);
                int scr[7];
                scr[6] = 0;
                for(int i = 0;i < 6;++i){
                    scanf("%d",scr + i);
                    scr[6] += scr[i];
                }
                update(a,b,scr);
                break;
            case 2:
                printf("석차를 조회할 사람의 반, 번호를 순서대로 입력\n===>   ");
                scanf("%d %d",&a,&b);
                printf("조회하고 싶은 과목을 입력\n1. 국어\n2. 영어\n3. 수학\n4. 한국사\n5. 사회탐구\n6. 과학탐구\n7. 전체성적\n===>   ");
                scanf("%d",&s);
                printf("%d반 %d번 학생의 학급내 %s 과목 석차 : %d\n",a,b,subject[s - 1],compute_rank(a,b,s - 1));
                break;
            case 3:
                printf("등급을 조회할 사람의 반, 번호를 순서대로 입력\n===>   ");
                scanf("%d %d",&a,&b);
                printf("조회하고 싶은 과목을 입력\n1. 국어\n2. 영어\n3. 수학\n4. 한국사\n5. 사회탐구\n6. 과학탐구\n7. 전체성적\n===>   ");
                scanf("%d",&s);
                printf("%d반 %d번 학생의 학급내 %s 과목 등급 : %d\n",a,b,subject[s - 1],compute_grade(a,b,s - 1));
                break;
            case 4:
                printf("성적 지표를 조회할 학급의 반을 입력\n===>   ");
                scanf("%d",&a);
                printf("조회하고 싶은 과목을 입력\n1. 국어\n2. 영어\n3. 수학\n4. 한국사\n5. 사회탐구\n6. 과학탐구\n7. 전체성적\n===>   ");
                scanf("%d",&s);
                print_index(a,s - 1);
                break;
            case 5:
                printf("프로그램 종료");
                goto EXIT;
                break;
            default:
                printf("명령 오류\n다시 입력\n===>   ");
        }
    }
    EXIT:
    return 0;
}
