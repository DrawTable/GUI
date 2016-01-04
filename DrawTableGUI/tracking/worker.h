#ifndef WORKER
#define WORKER

#include <QDebug>
#include <QObject>
#include <QTcpSocket>
#include <QHostAddress>
#include <QTextStream>
#include <QThread>


class Worker: public QObject
{
    Q_OBJECT
    QTcpSocket* socket;
    const int PORT = 5003;
    const QString HOST;

public:
    Worker();

public slots:
    void doWork();
    void connected();
    void showErrors(QAbstractSocket::SocketError err);
    void readyRead();
    void sendData(const char* data);
    void mouseMouve(int x, int y);
    void mousePressed();
    void mouseReleased();
    void quit();

signals:
    void finished();
    void connectionReady();
};

#endif // WORKER

