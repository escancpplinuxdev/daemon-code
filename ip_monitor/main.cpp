// main.cpp
#include <QCoreApplication>
#include <QTimer>
#include <QProcess>
#include <QDebug>
#include <QDateTime>
#include <QMap>
#include <QStringList>

// ── Config ────────────────────────────────────────────────────────────────────
#define PING_INTERVAL_MS   5000     // ping every 5 seconds
#define PING_TIMEOUT_SEC   2        // ping -W timeout
#define PING_COUNT         1        // ping -c count
#define NOTIFY_SCRIPT      "./notify.sh"

// ── IP list — edit here ───────────────────────────────────────────────────────
static const QStringList IP_LIST =
{
    "192.168.1.231",
    "192.168.1.234",
    "192.168.1.220",
    "192.168.1.219",
    "192.168.2.138",
    "192.168.2.58",
    "192.168.2.54",
    "8.8.8.8",
    "1.1.1.1"
};

// ─────────────────────────────────────────────────────────────────────────────

class IPMonitor : public QObject
{
    Q_OBJECT

public:
    explicit IPMonitor(QObject* parent = nullptr)
        : QObject(parent)
    {
        // Initialise all IPs as UP (unknown → assume up, first failure notifies)
        for (const QString& ip : IP_LIST)
        {
            m_status[ip]  = true;   // true = reachable
            m_process[ip] = nullptr;
        }
    }

    void start()
    {
        m_timer = new QTimer(this);
        connect(m_timer, &QTimer::timeout, this, &IPMonitor::pingAll);
        m_timer->start(PING_INTERVAL_MS);

        qInfo() << "[" << timestamp() << "] Monitor started —"
                << IP_LIST.size() << "IPs tracked";

        pingAll();   // run immediately without waiting first interval
    }

private slots:

    void pingAll()
    {
        for (const QString& ip : IP_LIST)
        {
            // Skip if previous ping process still running for this IP
            if (m_process[ip] && m_process[ip]->state() != QProcess::NotRunning)
                continue;

            // Clean up old process
            if (m_process[ip])
            {
                m_process[ip]->deleteLater();
                m_process[ip] = nullptr;
            }

            QProcess* proc = new QProcess(this);
            m_process[ip]  = proc;

            // Store IP in process property so we can retrieve it in slot
            proc->setProperty("ip", ip);

            // ping -c 1 -W 2 <ip>
            QStringList args;
            args << "-c" << QString::number(PING_COUNT)
                 << "-W" << QString::number(PING_TIMEOUT_SEC)
                 << ip;

            connect(proc,
                    QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
                    this,
                    &IPMonitor::onPingFinished);

            proc->start("ping", args);
        }
    }

    void onPingFinished(int exitCode, QProcess::ExitStatus /*exitStatus*/)
    {
        QProcess* proc = qobject_cast<QProcess*>(sender());
        if (!proc)
            return;

        QString ip      = proc->property("ip").toString();
        bool reachable  = (exitCode == 0);   // ping returns 0 on success

        bool wasUp = m_status[ip];

        // ── State change: UP → DOWN ───────────────────────────────────────────
        if (wasUp && !reachable)
        {
            m_status[ip] = false;
            qWarning() << "[" << timestamp() << "] DOWN:" << ip;
            fireNotify(ip, "DOWN");
        }
        // ── State change: DOWN → UP ───────────────────────────────────────────
        else if (!wasUp && reachable)
        {
            m_status[ip] = true;
            qInfo() << "[" << timestamp() << "] UP  :" << ip;
            fireNotify(ip, "UP");
        }
        else
        {
            // No state change — silent (remove this line if you want verbose log)
            qDebug() << "[" << timestamp() << "]"
                     << (reachable ? "OK  " : "FAIL") << ":" << ip;
        }

        proc->deleteLater();
        m_process[ip] = nullptr;
    }

private:

    // ── Run notify.sh <ip> <status> ──────────────────────────────────────────
    void fireNotify(const QString& ip, const QString& status)
    {
        QProcess* notifier = new QProcess(this);

        QStringList args;
        args << ip << status;

        notifier->start(NOTIFY_SCRIPT, args);

        // Fire-and-forget — clean up when done
        connect(notifier,
                QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
                notifier,
                &QProcess::deleteLater);

        qInfo() << "[" << timestamp() << "] notify.sh" << ip << status;
    }

    QString timestamp() const
    {
        return QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    }

    QTimer*                  m_timer;
    QMap<QString, bool>      m_status;    // ip → reachable?
    QMap<QString, QProcess*> m_process;   // ip → active ping process
};

// ── Entry point ───────────────────────────────────────────────────────────────

#include "main.moc"

int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);

    IPMonitor monitor;
    monitor.start();

    return app.exec();
}
