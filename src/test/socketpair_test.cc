/*
*Author:GeneralSandman
*Code:https://github.com/GeneralSandman/TinyWeb
*E-mail:generalsandman@163.com
*Web:www.generalsandman.cn
*/

/*---XXX---
*
****************************************
*
*/

#include <tiny_core/socketpair.h>
#include <tiny_core/eventloop.h>
#include <tiny_core/connection.h>
#include <tiny_base/buffer.h>
#include <tiny_core/time.h>
#include <tiny_core/timerid.h>
#include <tiny_core/callback.h>

#include <iostream>
#include <unistd.h>
#include <boost/bind.hpp>

using namespace std;

void getMessage(Connection *con, Buffer *buf, Time time)
{
    std::cout << "read form\n";
}

void print(){
    std::cout<<getpid()<<"--\n";
}

void parent()
{
}

void child()
{
}

int main()
{
    SocketPair pipe;
    pipe.createSocket();

    pid_t pid = fork();

    if (pid < 0)
    {
        cout << "error\n";
    }
    else if (pid == 0)
    {
        EventLoop *loop = new EventLoop();
        pipe.setChildSocket(loop);
        pipe.setReadCallback(boost::bind(&getMessage, _1, _2, _3));

        TimerId id1=loop->runEvery(1,boost::bind(print));
        TimerId id=loop->runAfter(5, boost::bind(&EventLoop::quit, loop));
        loop->loop();

        delete loop;
    }
    else
    {
        EventLoop *loop = new EventLoop();
        pipe.setParentSocket(loop);
        // pipe.writeToChild("aa");

        // TimerId id1=loop->runEvery(1,boost::bind(&SocketPair::writeToChild,&pipe,"aa"));
        TimerId id2=loop->runEvery(1,boost::bind(print));
        TimerId id=loop->runAfter(10, boost::bind(&EventLoop::quit, loop));
        loop->loop();

        delete loop;
    }

    return 0;
}
