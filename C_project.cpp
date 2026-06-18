#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int score[7][40][7];//score[i][j][k] : i반 j번 학생의 k과목 성적
int student[7];//student[i] : i반 학급의 학생 수

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
    printf("Mean : %.2f\n",m);
    printf("Stdev : %.2f\n",stdev);
    printf("Cut : %d\n",tmp[cut_idx]);//1
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
    student[1] = 10;
    print_index(1,0);
    for(int i = 1;i <= 10;++i){
        printf("%d-th stud's grade : %d\n",i,compute_grade(1,i,0));
    }
    return 0;
}