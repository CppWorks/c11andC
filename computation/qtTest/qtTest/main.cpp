
#include <QtCore/QCoreApplication>
#include "consumer.h"
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Master master;
    Producer producer(master);
    Consumer consumer(master);
    producer.start();
    consumer.start();
    producer.wait();
    consumer.wait();
    return a.exec();
}
