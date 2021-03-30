#include "launchermanager.h"

LauncherManager *LauncherManager::m_launcherManager = nullptr;

LauncherManager *LauncherManager::getInstance()
{
    if(m_launcherManager == nullptr){
        m_launcherManager = new LauncherManager();
    }
    return m_launcherManager;
}

LauncherManager::~LauncherManager()
{
    delete m_mainWindow;
    delete m_gameProcess;
    delete m_logger;
    delete m_networkManager;
}

void LauncherManager::startGame()
{
    m_programPath = "./path/to/ue4.exe";

    m_gameProcess = new QProcess();

    connect(m_gameProcess, &QProcess::stateChanged, this, &LauncherManager::gameStateChanged);
    connect(m_gameProcess, &QProcess::errorOccurred, this, &LauncherManager::gameError);

    defineGamesOptions();

    qDebug() << m_programPath;
    qDebug() << "Args : " ;
    qDebug() << m_programArgs;

    m_gameProcess->start(m_programPath, m_programArgs);

    changeWindowsVisibility(false);
}

void LauncherManager::defineGamesOptions()
{
    if(m_playerName != NULL) m_programArgs << "-name" << m_playerName;
    if(m_userAccessToken != NULL) m_programArgs << "-token" << m_userAccessToken;
    if(m_startAsAdmin) m_programArgs << "-admin";
}

LauncherManager::LauncherManager(QObject *parent) : QObject(parent)
{
    m_mainWindow = MainWindow::getInstance(nullptr);
    m_networkManager = NetworkManager::getInstance();
    m_logger = Logger::getInstance();

    connect(m_mainWindow, &MainWindow::login, m_networkManager, &NetworkManager::postCredentials);
    connect(m_mainWindow, &MainWindow::getPlayerName, this, &LauncherManager::setupPlayer);
    connect(m_mainWindow, &MainWindow::commandCommit, this, &LauncherManager::parseCommand);
    connect(m_mainWindow, &MainWindow::startGame, this, &LauncherManager::startGame);
    connect(m_networkManager, &NetworkManager::loginQueryAnswer, this, &LauncherManager::loginCheck);
}

void LauncherManager::changeWindowsVisibility(bool visibility)
{
    m_mainWindow->setVisible(visibility);
    m_logger->setVisible(visibility);
}

void LauncherManager::parseCommand(QString cmd)
{
    AdminCommand *admin = new AdminCommand();
    ForceExitCommand *fExit = new ForceExitCommand();
    LogCommand *log = new LogCommand();

    connect(admin, &AdminCommand::executeCommand, this, &LauncherManager::executeCommand);
    connect(fExit, &ForceExitCommand::executeCommand, this, &LauncherManager::executeCommand);
    connect(log, &LogCommand::executeCommand, this, &LauncherManager::executeCommand);

    admin->setNext(fExit);
    fExit->setNext(log);

    admin->message(cmd);
}

void LauncherManager::gameStateChanged()
{

}

void LauncherManager::gameError()
{

}

void LauncherManager::executeCommand(Command::COMMAND cmd)
{
    switch(cmd)
    {
        case Command::ADMIN:
            if(m_isAdmin)
            {
                m_startAsAdmin = true;
            }
            else
            {
                m_startAsAdmin = false;
                QMessageBox *msgBox = new QMessageBox();
                m_isAdmin ? msgBox->setText("Admin mode !"): msgBox->setText("Nice Try ..!");
                msgBox->exec();
            }
        break;

        default:
        break;
    }
}

void LauncherManager::loginCheck(QString token)
{
    if(token != NULL)
    {
        m_userAccessToken = token;
        m_mainWindow->changeStartGameBtn(true);
    }
    else
    {
        // Wrong username or Password
        qDebug() << "Wrong username";
    }
}

void LauncherManager::setupPlayer(QString id)
{
    m_playerName = id;
}
