#include "tclient.h"

#include <QDataStream>
#include <QHBoxLayout>
#include <QString>
#include <QThread>
#include <iostream>

void Worker::Process() {
  socket.abort();
  socket.connectToHost(host, port); // Connect to the server
  if (!socket.waitForConnected(3000)) {
      std::cout << "Connection failed!" << std::endl;
      emit error("Connection failed!");
  }

  std::cout << "Connected to server!" << std::endl;
  QDataStream in(&socket);
  QString Fortune;

  if(!socket.waitForReadyRead(3000)){
      std::cout << "Reading failed!" << std::endl;
      emit error("Reading failed!");
  }
  in.startTransaction();
  in >> Fortune;
  socket.disconnectFromHost();
  emit finished(Fortune);
};


BlockingClient::BlockingClient(QWidget *parent) : QDialog(parent),
                                              boxLayout(new QVBoxLayout(parent)),
                                              hostLineEdit(new QLineEdit(parent)),
                                              portLineEdit(new QLineEdit(parent)),
                                              statusLabel(new QLabel(tr(" -Fortune-"), parent)),
                                              getFortuneButton(new QPushButton(tr("Get Fortune"), parent))
  {
    QHBoxLayout* ipHLayout = new QHBoxLayout(parent);
    ipHLayout->addWidget(new QLabel(tr("IP: "),parent));
    ipHLayout->addWidget(hostLineEdit);
    QHBoxLayout* portHLayout = new QHBoxLayout(parent);
    portHLayout->addWidget(new QLabel(tr("port: "),parent));
    portHLayout->addWidget(portLineEdit);

    boxLayout->addLayout(ipHLayout);
    boxLayout->addLayout(portHLayout);
    boxLayout->addWidget(statusLabel);
    boxLayout->addWidget(getFortuneButton);
    setLayout(boxLayout);
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    connect(getFortuneButton, &QPushButton::clicked, this, &BlockingClient::RequestNewFortune);
    hostLineEdit->setText(host);
    portLineEdit->setText(QString::number(port));
  };

  void BlockingClient::PrintFort(){
    std::cout<< currentFortune.toStdString() << std::endl;
  }

  QString BlockingClient::getStatusLabel(){
    return this->statusLabel->text();
  };

  void BlockingClient::RequestNewFortune() {
    getFortuneButton->setEnabled(false);
    host = hostLineEdit->text();
    port = portLineEdit->text().toInt();
    this->Fort();
  }

  void BlockingClient::Fort() {
    QThread* thread = new QThread;
    Worker* worker = new Worker(host, port);
    worker->moveToThread(thread);
    connect(worker, &Worker::error, this, &BlockingClient::DisplayError);
    connect(thread, &QThread::started, worker, &Worker::Process);
    connect(worker, &Worker::finished, thread, &QThread::quit);
    connect(worker, &Worker::finished, this, &BlockingClient::ShowFortune);
    connect(worker, &Worker::finished, worker, &QObject::deleteLater);
    connect(thread, &QThread::finished, thread, &QObject::deleteLater);
    thread->start();
  };
    void BlockingClient::ShowFortune(QString nextFortune){
          currentFortune = nextFortune;
          statusLabel->setText(currentFortune);
          getFortuneButton->setEnabled(true);
    };
    void BlockingClient::DisplayError(const QString &/*message*/) {}
    void BlockingClient::EnableGetFortuneButton() {  getFortuneButton->setEnabled(true); };



