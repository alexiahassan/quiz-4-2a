#include <iostream>
#include <chrono>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <semaphore.h>
// include additional necessary headers
using namespace std;

sem_t *response_sem;
sem_t *query_sem;

int globalCount;
bool queryDone = false;

void query(/*add necessary parameters*/)
{
    for (int k = 0; k < globalCount; k++)
    {
        sem_wait(response_sem);
        // query{
        //  loop
        //  sem_wait(response_sem)
        //  print ...
        //  sem_post(query_sem)
        //  0 .... 9

        cout << "[" << k << "]" << " SYN ... ";
        sem_post(query_sem);
    }
}

void response(/*add necessary parameters*/)
{
    // response {
    //  sem_wait(response_sem)
    //  print ...
    //  sem_post(query_sem)
    // count --
    for (int k = 0; k < globalCount; k++)
    {
        sem_wait(query_sem);
        cout << "ACK" << endl;
        sem_post(response_sem);
    }
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        printf("Usage: ./handshake <count>\n");
        exit(1);
    }

    //  * Steps to follow:
    //  * 1. Get the "count" from cmd args
    globalCount = atoi(argv[1]);

    // cout << "Count is: " << count << endl;
    //   * 2. Create necessary synchronization primitive(s)
    sem_t localResponseSem;
    sem_t localQuerySem;
    sem_init(&localQuerySem, 0, 0);
    sem_init(&localResponseSem, 0, 1);

    response_sem = &localResponseSem;
    query_sem = &localQuerySem;
    //  * 3. Create two threads, one for "SYN" and the other for "ACK"
    std::thread SYN(query);
    std::thread ACK(response);
   // cout << "calling syn" << count << endl;

    //cout << "calling respone" << count << endl;

    //  * 4. Provide the threads with necessary args
    //  * 5. Update the "query" and "response" functions to synchronize the output
    SYN.join();
    ACK.join();

    sem_destroy(response_sem);
    sem_destroy(query_sem);

    return 0;
}

/*
//query{
    loop
    sem_wait(response_sem)
    print ...
    sem_post(query_sem)
    0 .... 9
}

//response {
    sem_wait(response_sem)
    print ...
    sem_post(query_sem)

    count --
}*/