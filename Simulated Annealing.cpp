// SA FOR PFSP.cpp : Using Simulated Anneling to solve permutation Flow Shop Scheduleing Problem
// Ste-Made
// Final project for Optimazation

#include "iostream"
#include "algorithm"
#include "cmath"
#include "cstring"
#include "ctime"
using namespace std;

#define ITERATION 20000//设置迭代次数为两万次
#define MAX 103

int machine;
int jobs;
int operation_time[MAX][MAX];//存储第i个工作在第j个机器上所需要的时间
int nonsense;//对于将输入存入数组时用到的一个无意义变量
int sequence[MAX];//存储工作序列
int Best_seq[MAX];
int New_seq[MAX];
int makespan[MAX][MAX];
int dp[MAX][MAX];//转储operation_time来进行计算
int optimal_for_all;
int optimal_now;
int optimal_old;
int cooling_constants[6] = { 400,300,200,100,50,10 };//Cooling Constants
double Aprob, Rprob;//接受概率以及随机概率

int GetMakespan(int* seq)//获取此次序列的MAKESPAN
{
    memset(dp, 0, sizeof(dp));
    for (int i = 1; i <= machine; i++)
    {
        for (int j = 1; j <= jobs; j++)
        {
            dp[i][j] = operation_time[seq[j]][i];
        }
    }
    for (int j = 2; j <= jobs; j++)//先对第一行以及第一列赋值
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

    return dp[machine][jobs];//表示最后一个工作在最后一台机器上完成时的时间

}

void Input(void)//处理输入
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

void ChangeSequence(void)//随机的改变序列中的两个值（交换二者顺序）
{
    int start = 0, end = 0;
    start = 1 + rand() % jobs;
    end = 1 + rand() % jobs;
    memcpy(New_seq, sequence, sizeof(sequence));
    Swap(New_seq + start, New_seq + end);
}

int main()
{
    srand((unsigned)time(NULL));//设置随机数种子
     Input();
     clock_t start, end;//用于计时
     start = clock();
     for (int i = 1; i <= jobs; i++)//初始化Best_seq(均为1，2，3，n)
     {
         Best_seq[i] = i;
     }
        for (int i = 0; i < 6; i++)
        {
            for (int i = 1; i <= jobs; i++)//初始化sequence
            {
                sequence[i] = i;
            }
            int cooling_temp = cooling_constants[i];
            for (int i = 1; i <= ITERATION; i++)
            {
                optimal_for_all = GetMakespan(Best_seq);//计算初始序列和最佳序列的Makespan
                optimal_old = GetMakespan(sequence);
                ChangeSequence();//获得新序列
                optimal_now = GetMakespan(New_seq);
                int mk1 = optimal_now - optimal_old;
                int mk2 = optimal_now - optimal_for_all;
                if (mk1 <= 0 && mk2 <= 0)//当前值相对上次的时间和最优时间都变小
                {
                    memcpy(sequence, New_seq, sizeof(New_seq));
                    memcpy(Best_seq, New_seq, sizeof(New_seq));
                    continue;
                }
                else if (mk1 <= 0 && mk2 > 0)//当前值只对上次时间变小而为对最优时间变小
                {
                    memcpy(sequence, New_seq, sizeof(New_seq));
                    continue;
                }
                else if (mk1 > 0)//当前时间相较于上一次变大
                {
                    Aprob = exp(-mk1 / cooling_temp);
                    Rprob = rand();
                    if (Rprob < Aprob)//产生的随机概率小于接受概率则接受此次“下山”
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
        cout << "运行时间" << (double)(end - start) / CLOCKS_PER_SEC << endl;
}


