#ifndef CONSUMER_H
#define CONSUMER_H
#include <QThread>
#include <QWaitCondition>

#include <stdio.h>
#include <stdlib.h>
#include <QTime>
#include <QMutex>
#include <def.h>
#include <QDebug>


class Master
{
public:
    Master()
    {
        numUsedBytes = 0;
        DataSize = 100;
        BufferSize = 30;

    }
    
    int DataSize;

    int BufferSize;
    char buffer[8192];

    QWaitCondition bufferNotEmpty;
    QWaitCondition bufferNotFull;
    QMutex mutex;
    int numUsedBytes;

};

class Producer : public QThread
{
public:
    Producer(Master& master, QObject *parent = NULL) : QThread(parent), pMaster(&master)
    {
    }

    void run() Q_DECL_OVERRIDE
    {
        qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));

        for (int i = 0; i < pMaster->DataSize; ++i) {
            pMaster->mutex.lock();
            if (pMaster->numUsedBytes == pMaster->BufferSize)
                pMaster->bufferNotFull.wait(&pMaster->mutex);
            pMaster->mutex.unlock();

            pMaster->buffer[i % pMaster->BufferSize] = 'a';

            pMaster->mutex.lock();
            ++(pMaster->numUsedBytes);
            pMaster->bufferNotEmpty.wakeAll();
            pMaster->mutex.unlock();
        }
    }
    Master* pMaster;
};

class Consumer : public QThread
{
    Q_OBJECT
public:
    Consumer(Master& master, QObject *parent = NULL) : QThread(parent), pMaster(&master)
    {
    }
    void run() Q_DECL_OVERRIDE
    {
        for (int i = 0; i < pMaster->DataSize; ++i) {
            pMaster->mutex.lock();
            if (pMaster->numUsedBytes == 0)
                pMaster->bufferNotEmpty.wait(&pMaster->mutex);
            pMaster->mutex.unlock();

            char f = pMaster->buffer[i % pMaster->BufferSize];
            qDebug() << "value is " << f;
            fprintf(stderr, "%c", pMaster->buffer[i % pMaster->BufferSize]);

            pMaster->mutex.lock();
            --(pMaster->numUsedBytes);
            pMaster->bufferNotFull.wakeAll();
            pMaster->mutex.unlock();
        }
        fprintf(stderr, "\n");
    }
    Master* pMaster;
    signals:
    void stringConsumed(const QString &text);
};
#endif // CONSUMER_H
