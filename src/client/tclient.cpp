#include "tclient.h"

#include <QDataStream>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QString>
#include <QThread>
#include <QVBoxLayout>
#include <iostream>

void Worker::process() {
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
    connect(getFortuneButton, &QPushButton::clicked, this, &BlockingClient::requestNewFortune);
    hostLineEdit->setText(host);
    portLineEdit->setText(QString::number(port));
  };

  void BlockingClient::printFort(){
    std::cout<< currentFortune.toStdString() << std::endl;
  }

  QString BlockingClient::getStatusLabel(){
    return this->statusLabel->text();
  };

  void BlockingClient::requestNewFortune() {
    getFortuneButton->setEnabled(false);
    host = hostLineEdit->text();
    port = portLineEdit->text().toInt();
    this->fort();
  }

  void BlockingClient::fort() {
    QThread* thread = new QThread;
    Worker* worker = new Worker(host, port);
    worker->moveToThread(thread);
    connect(worker, &Worker::error, this, &BlockingClient::displayError);
    connect(thread, &QThread::started, worker, &Worker::process);
    connect(worker, &Worker::finished, thread, &QThread::quit);
    connect(worker, &Worker::finished, this, &BlockingClient::showFortune);
    connect(worker, &Worker::finished, worker, &QObject::deleteLater);
    connect(thread, &QThread::finished, thread, &QObject::deleteLater);
    thread->start();
  };
    void BlockingClient::showFortune(QString nextFortune){
          currentFortune = nextFortune;
          statusLabel->setText(currentFortune);
          getFortuneButton->setEnabled(true);
    };
    void BlockingClient::displayError(const QString &/*message*/) {}
    void BlockingClient::enableGetFortuneButton() {  getFortuneButton->setEnabled(true); };



