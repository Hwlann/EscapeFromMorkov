#ifndef LAUNCHERMANAGER_H
#define LAUNCHERMANAGER_H

#include <QObject>
#include <QProcess>
#include <QDebug>
#include <QSoundEffect>
#include <QSound>

#include "Launcher/mainwindow.h"
#include "Network/networkmanager.h"

#include "Command/admincommand.h"
#include "Command/forceexitcommand.h"
#include "Command/logcommand.h"
#include "Command/forcepath.h"

#define  CMD_SHOW_LOGS "show logs"
#define  ADMIN_TOKEN   "46e9e734-915e-11eb-8c90-0242ac110007"

class LauncherManager : public QObject
{
    Q_OBJECT
public:
    static LauncherManager *getInstance();
    ~LauncherManager();

    void startGame();
    void defineGamesOptions();    
    void setSounds();

public slots:
    void musicManager();

private:
    explicit LauncherManager(QObject *parent = nullptr);
    static LauncherManager *m_launcherManager;

    MainWindow *m_mainWindow = nullptr;
    QProcess *m_gameProcess = nullptr;
    NetworkManager *m_networkManager = nullptr;
    Logger *m_logger = nullptr;

    QString m_programPath;
    QStringList m_programArgs;
    void changeWindowsVisibility(bool visibility);

    bool m_startAsAdmin = false;
    bool m_isAdmin = false;

    QString m_playerName = NULL;
    QString m_userAccessToken = NULL;

    QString m_musicPath = ":/sounds/Portal-SelfEsteemFund.wav";
    QSoundEffect *m_music;
    bool m_mute = false;

signals:

private slots:
    void parseCommand(QString cmd);
    void gameStateChanged();
    void gameError();
    void executeCommand(Command::COMMAND cmd, QString command);
    void loginCheck(QString token);
    void setupPlayer(QString id);
};

#endif // LAUNCHERMANAGER_H
