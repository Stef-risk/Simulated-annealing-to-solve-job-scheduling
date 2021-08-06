// SA FOR PFSP.cpp : Using Simulated Anneling to solve permutation Flow Shop Scheduleing Problem
// Ste-Made
// Final project for Optimazation

#include "iostream"
#include "algorithm"
#include "cmath"
#include "cstring"
#include "ctime"
using namespace std;

#define ITERATION 20000//���õ�������Ϊ�����
#define MAX 103

int machine;
int jobs;
int operation_time[MAX][MAX];//�洢��i�������ڵ�j������������Ҫ��ʱ��
int nonsense;//���ڽ������������ʱ�õ���һ�����������
int sequence[MAX];//�洢��������
int Best_seq[MAX];
int New_seq[MAX];
int makespan[MAX][MAX];
int dp[MAX][MAX];//ת��operation_time�����м���
int optimal_for_all;
int optimal_now;
int optimal_old;
int cooling_constants[6] = { 400,300,200,100,50,10 };//Cooling Constants
double Aprob, Rprob;//���ܸ����Լ��������

int GetMakespan(int* seq)//��ȡ�˴����е�MAKESPAN
{
    memset(dp, 0, sizeof(dp));
    for (int i = 1; i <= machine; i++)
    {
        for (int j = 1; j <= jobs; j++)
        {
            dp[i][j] = operation_time[seq[j]][i];
        }
    }
    for (int j = 2; j <= jobs; j++)//�ȶԵ�һ���Լ���һ�и�ֵ
        dp[1][j] += dp[1][j - 1];
    for (int i = 2; i <= machine; i++)
        dp[i][1] += dp[i - 1][1];

    for (int i = 2; i <= machine; i++)
    {
        for (int j = 2; j <= jobs; j++)
        {
            dp[i][j] = dp[i][j] + max(dp[i][j - 1], dp[i - 1][j]);
        }
    }

    return dp[machine][jobs];//��ʾ���һ�����������һ̨���������ʱ��ʱ��

}

void Input(void)//��������
{
    cin >> jobs >> machine;
    for (int i = 1; i <= jobs; i++)
    {
        for (int j = 1; j <= machine; j++)
        {
            //cin >> nonsense;
            cin >> operation_time[i][j];
        }
    }
}

void Swap(int* a, int* b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

void ChangeSequence(void)//����ĸı������е�����ֵ����������˳��
{
    int start = 0, end = 0;
    start = 1 + rand() % jobs;
    end = 1 + rand() % jobs;
    memcpy(New_seq, sequence, sizeof(sequence));
    Swap(New_seq + start, New_seq + end);
}

int main()
{
    srand((unsigned)time(NULL));//�������������
     Input();
     clock_t start, end;//���ڼ�ʱ
     start = clock();
     for (int i = 1; i <= jobs; i++)//��ʼ��Best_seq(��Ϊ1��2��3��n)
     {
         Best_seq[i] = i;
     }
        for (int i = 0; i < 6; i++)
        {
            for (int i = 1; i <= jobs; i++)//��ʼ��sequence
            {
                sequence[i] = i;
            }
            int cooling_temp = cooling_constants[i];
            for (int i = 1; i <= ITERATION; i++)
            {
                optimal_for_all = GetMakespan(Best_seq);//�����ʼ���к�������е�Makespan
                optimal_old = GetMakespan(sequence);
                ChangeSequence();//���������
                optimal_now = GetMakespan(New_seq);
                int mk1 = optimal_now - optimal_old;
                int mk2 = optimal_now - optimal_for_all;
                if (mk1 <= 0 && mk2 <= 0)//��ǰֵ����ϴε�ʱ�������ʱ�䶼��С
                {
                    memcpy(sequence, New_seq, sizeof(New_seq));
                    memcpy(Best_seq, New_seq, sizeof(New_seq));
                    continue;
                }
                else if (mk1 <= 0 && mk2 > 0)//��ǰֵֻ���ϴ�ʱ���С��Ϊ������ʱ���С
                {
                    memcpy(sequence, New_seq, sizeof(New_seq));
                    continue;
                }
                else if (mk1 > 0)//��ǰʱ���������һ�α��
                {
                    Aprob = exp(-mk1 / cooling_temp);
                    Rprob = rand();
                    if (Rprob < Aprob)//�������������С�ڽ��ܸ�������ܴ˴Ρ���ɽ��
                    {
                        memcpy(sequence, New_seq, sizeof(New_seq));
                        continue;
                    }
                    continue;
                }
            }
    }
        end = clock();
        cout << "Optimal Result :" << GetMakespan(Best_seq) << endl;
        cout << "����ʱ��" << (double)(end - start) / CLOCKS_PER_SEC << endl;
}


