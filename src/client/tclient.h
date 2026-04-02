#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTcpSocket>
#include <QVBoxLayout>
#include <QWidget>

class QDialogButtonBox;


class Worker : public QObject {
    Q_OBJECT

public:
    Worker(QString host, int port): host(host), port(port) {};
    ~Worker() = default;

public slots:
    void Process();

signals:
    void finished(QString fortune);
    void error(QString err);

private:
    QString host = QStringLiteral("127.0.0.1");
    int port;
    QTcpSocket socket;
};


class BlockingClient : public QDialog
{
    Q_OBJECT

public:
    BlockingClient(QWidget *parent = nullptr);
    QString getStatusLabel();
    void PrintFort();
private slots:
    void RequestNewFortune();
    void Fort();
    void ShowFortune(QString nextFortune);
    void DisplayError(const QString &message);
    void EnableGetFortuneButton();

private:
    QString host = QStringLiteral("127.0.0.1");
    int port = 4242;
    QTcpSocket socket;
    QString currentFortune;
    QLineEdit *hostLineEdit = nullptr;
    QLineEdit *portLineEdit = nullptr;
    QLabel *statusLabel;
    QPushButton *getFortuneButton = nullptr;
    QDialogButtonBox *buttonBox = nullptr;
    QVBoxLayout *boxLayout = nullptr;
};

