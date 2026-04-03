#include <QDialog>
#include <QObject>
#include <QString>
#include <QTcpSocket>

class QDialogButtonBox;
class QLabel;
class QLineEdit;
class QPushButton;
class QVBoxLayout;
class QWidget;


class Worker : public QObject {
    Q_OBJECT

public:
    Worker(QString host, int port): host(host), port(port) {};
    ~Worker() = default;

public slots:
    void process();

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
    void printFort();
private slots:
    void requestNewFortune();
    void fort();
    void showFortune(QString nextFortune);
    void displayError(const QString &message);
    void enableGetFortuneButton();

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

