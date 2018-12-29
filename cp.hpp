#ifndef __CP_HPP__
#define __CP_HPP__ 
#include <iostream>
#include <queue>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
using namespace std;

class BlockQueue   //阻塞队列
{
    public:
        BlockQueue(int num_):_num(num_),high_line(_num*2/3)
        {
            pthread_mutex_init(&qlock,NULL);
            pthread_cond_init(&cond_c,NULL);
            pthread_cond_init(&cond_p,NULL);
        }
        ~BlockQueue()
        {
            pthread_mutex_destroy(&qlock);
            pthread_cond_destroy(&cond_c);
            pthread_cond_destroy(&cond_p);
        }
        void Push(int data)
        {
            pthread_mutex_lock(&qlock);
            while(IsFull()){            //检查之前锁住,因为bq是临界资源
                pthread_cond_wait(&cond_p,&qlock);
            }
            bq.push(data);
            if(IsOverHighline())
                pthread_cond_signal(&cond_c);           //一旦队列满了，唤醒消费者线程
            pthread_mutex_unlock(&qlock);
        }
        void Pop(int &data)
        {
            pthread_mutex_lock(&qlock);
            while(IsEmpty()){
                pthread_cond_signal(&cond_p);       //一旦空了，唤醒生产者线程
                pthread_cond_wait(&cond_c,&qlock);
            }
            data = bq.front();
            bq.pop();
            pthread_mutex_unlock(&qlock); 
        }
    private:
        bool IsFull()
        {
            return (bq.size() == _num)?true:false;
        }
        bool IsEmpty()
        {
            return (bq.size() == 0)?true:false;
        }
        bool IsOverHighline()
        {
            return (bq.size() >= high_line) ? true:false;
        }
    private:
        queue<int> bq;           //队列
        int _num;                //消息上限
        int low_line;            //低于它唤醒生产者，阻塞消费者
        int high_line;           //高于它唤醒消费者，阻塞生产者

        pthread_mutex_t qlock;
        pthread_cond_t cond_c;   //consums的条件变量,判断是否为空
        pthread_cond_t cond_p;   //produce的条件变量，判断是否满
};
#endif 
