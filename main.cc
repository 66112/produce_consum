#include "cp.hpp"

pthread_mutex_t lock;
int num = 12;

void* produce(void* arg)
{
    BlockQueue* bqp = (BlockQueue*)arg;
    srand((unsigned int)time(NULL));   //随机种子
    for(;;)
    {
        usleep(500000);
        int data = rand()%100+1;
       // pthread_mutex_lock(&lock);
        bqp->Push(data);
        cout << "produce running... data: "<<data<<endl; 
       // pthread_mutex_unlock(&lock);
    }
}
void* consums(void* arg)
{
    BlockQueue* bqp = (BlockQueue*)arg;
    for(;;)
    {
        int data;
    //    sleep(1);
    //    pthread_mutex_lock(&lock);
        bqp->Pop(data);
        cout << "consums running... data: "<<data<<endl; 
    //    pthread_mutex_unlock(&lock);
    }
}
int main()
{
    BlockQueue* bqp = new BlockQueue(num);   //阻塞队列
    pthread_t r1,r2;
    pthread_mutex_init(&lock,NULL);
    pthread_create(&r1,NULL,produce,(void*)bqp);  //produce
    pthread_create(&r2,NULL,consums,(void*)bqp);  //consums

    pthread_join(r1,NULL);
    pthread_join(r2,NULL);
    
    pthread_mutex_destroy(&lock);
    delete bqp;
    return 0;
}
