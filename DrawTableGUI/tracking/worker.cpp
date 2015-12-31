#include "worker.h"


Worker::Worker() : HOST("localhost"){

}

void Worker::doWork(){
    socket = new QTcpSocket(this);
    socket->connectToHost(HOST, PORT);
    connect(socket, SIGNAL(connected()), this, SLOT(connected()));
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(showErrors(QAbstractSocket::SocketError)));
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
}


void Worker::connected(){
    qDebug() << "connected";
}


void Worker::showErrors(QAbstractSocket::SocketError err){
    qDebug() << err;
}


void Worker::readyRead(){
    QString line = socket->readLine();
    qDebug() << "Received: " << line;
}


void Worker::sendData(const char *data){
    socket->write(data);
    socket->flush();
}

void Worker::mouseMouve(int x, int y){
    sendData(QString("move:%1:%2\n").arg(x).arg(y).toUtf8());
}

void Worker::mousePressed(){
    sendData(QString("press\n").toUtf8());
}

void Worker::mouseReleased(){
    sendData(QString("release\n").toUtf8());
}

void Worker::quit(){
    sendData((QString("quit\n").toUtf8()));
    emit finished();
}
