#include "loginwindow.h"

#include <QApplication>
#include <QColor>
#include <QFrame>
#include <QGraphicsDropShadowEffect>
#include <QHBoxLayout>
#include <QIcon>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>

LoginWindow::LoginWindow(QMySqlite *database, QWidget *parent)
    : QDialog(parent), database(database)
{
    buildUi();
    applyStyle();
    setRegisterMode(false);
}

UserRecord LoginWindow::currentUser() const
{
    return loggedInUser;
}

void LoginWindow::buildUi()
{
    setWindowTitle(QString::fromUtf8(u8"用户登录"));
    setWindowIcon(QIcon(":/image/UI/icon.png"));
    setFixedSize(920, 560);
    setModal(true);

    QHBoxLayout *rootLayout = new QHBoxLayout(this);
    rootLayout->setContentsMargins(0, 0, 0, 0);
    rootLayout->setSpacing(0);

    QWidget *brandPanel = new QWidget(this);
    brandPanel->setObjectName("brandPanel");
    brandPanel->setFixedWidth(420);

    QVBoxLayout *brandLayout = new QVBoxLayout(brandPanel);
    brandLayout->setContentsMargins(44, 48, 44, 48);
    brandLayout->setSpacing(18);

    QLabel *badgeLabel = new QLabel(QString::fromUtf8(u8"SECURITY CENTER"), brandPanel);
    badgeLabel->setObjectName("badgeLabel");

    QLabel *brandTitle = new QLabel(QString::fromUtf8(u8"智能安防监控系统"), brandPanel);
    brandTitle->setObjectName("brandTitle");
    brandTitle->setWordWrap(true);

    QLabel *brandText = new QLabel(QString::fromUtf8(u8"视频预览、录像回放、检索配置统一管理"), brandPanel);
    brandText->setObjectName("brandText");
    brandText->setWordWrap(true);

    QLabel *statusCard = new QLabel(QString::fromUtf8(u8"数据库认证已启用\nSQLite 用户表自动维护\n密码加盐哈希存储"), brandPanel);
    statusCard->setObjectName("statusCard");
    statusCard->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    brandLayout->addWidget(badgeLabel);
    brandLayout->addSpacing(24);
    brandLayout->addWidget(brandTitle);
    brandLayout->addWidget(brandText);
    brandLayout->addStretch();
    brandLayout->addWidget(statusCard);

    QWidget *formPanel = new QWidget(this);
    formPanel->setObjectName("formPanel");
    QVBoxLayout *formOuterLayout = new QVBoxLayout(formPanel);
    formOuterLayout->setContentsMargins(66, 50, 66, 50);

    QFrame *formCard = new QFrame(formPanel);
    formCard->setObjectName("formCard");
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(formCard);
    shadow->setBlurRadius(32);
    shadow->setOffset(0, 16);
    shadow->setColor(QColor(11, 18, 32, 55));
    formCard->setGraphicsEffect(shadow);

    QVBoxLayout *formLayout = new QVBoxLayout(formCard);
    formLayout->setContentsMargins(42, 38, 42, 34);
    formLayout->setSpacing(18);

    titleLabel = new QLabel(formCard);
    titleLabel->setObjectName("titleLabel");

    subtitleLabel = new QLabel(formCard);
    subtitleLabel->setObjectName("subtitleLabel");
    subtitleLabel->setWordWrap(true);

    usernameEdit = new QLineEdit(formCard);
    usernameEdit->setPlaceholderText(QString::fromUtf8(u8"用户名"));
    usernameEdit->setClearButtonEnabled(true);

    passwordEdit = new QLineEdit(formCard);
    passwordEdit->setPlaceholderText(QString::fromUtf8(u8"密码"));
    passwordEdit->setEchoMode(QLineEdit::Password);

    confirmPasswordEdit = new QLineEdit(formCard);
    confirmPasswordEdit->setPlaceholderText(QString::fromUtf8(u8"确认密码"));
    confirmPasswordEdit->setEchoMode(QLineEdit::Password);

    messageLabel = new QLabel(formCard);
    messageLabel->setObjectName("messageLabel");
    messageLabel->setWordWrap(true);

    submitButton = new QPushButton(formCard);
    submitButton->setObjectName("submitButton");
    submitButton->setCursor(Qt::PointingHandCursor);

    modeButton = new QPushButton(formCard);
    modeButton->setObjectName("modeButton");
    modeButton->setCursor(Qt::PointingHandCursor);

    formLayout->addWidget(titleLabel);
    formLayout->addWidget(subtitleLabel);
    formLayout->addSpacing(8);
    formLayout->addWidget(usernameEdit);
    formLayout->addWidget(passwordEdit);
    formLayout->addWidget(confirmPasswordEdit);
    formLayout->addWidget(messageLabel);
    formLayout->addWidget(submitButton);
    formLayout->addWidget(modeButton);
    formLayout->addStretch();

    formOuterLayout->addWidget(formCard);

    rootLayout->addWidget(brandPanel);
    rootLayout->addWidget(formPanel);

    connect(submitButton, &QPushButton::clicked, this, &LoginWindow::submit);
    connect(modeButton, &QPushButton::clicked, this, [this]() {
        setRegisterMode(!registerMode);
    });
    connect(usernameEdit, &QLineEdit::returnPressed, this, &LoginWindow::submit);
    connect(passwordEdit, &QLineEdit::returnPressed, this, &LoginWindow::submit);
    connect(confirmPasswordEdit, &QLineEdit::returnPressed, this, &LoginWindow::submit);
}

void LoginWindow::applyStyle()
{
    setStyleSheet(
        "QDialog { background: #eef2f7; }"
        "#brandPanel { background: #111827; }"
        "#badgeLabel { color: #67e8f9; font-size: 12px; font-weight: 700; letter-spacing: 0px; }"
        "#brandTitle { color: #f8fafc; font-size: 34px; font-weight: 800; line-height: 120%; }"
        "#brandText { color: #cbd5e1; font-size: 16px; line-height: 150%; }"
        "#statusCard { color: #dbeafe; background: rgba(37, 99, 235, 0.20); border: 1px solid rgba(125, 211, 252, 0.25); border-radius: 8px; padding: 18px; font-size: 14px; line-height: 160%; }"
        "#formPanel { background: #eef2f7; }"
        "#formCard { background: #ffffff; border: 1px solid #e2e8f0; border-radius: 8px; }"
        "#titleLabel { color: #0f172a; font-size: 28px; font-weight: 800; }"
        "#subtitleLabel { color: #64748b; font-size: 14px; line-height: 150%; }"
        "QLineEdit { min-height: 46px; border: 1px solid #cbd5e1; border-radius: 8px; padding: 0 14px; color: #0f172a; background: #f8fafc; font-size: 15px; }"
        "QLineEdit:focus { border: 1px solid #2563eb; background: #ffffff; }"
        "#messageLabel { min-height: 22px; color: #dc2626; font-size: 13px; }"
        "#submitButton { min-height: 46px; border: none; border-radius: 8px; color: white; background: #2563eb; font-size: 16px; font-weight: 700; }"
        "#submitButton:hover { background: #1d4ed8; }"
        "#submitButton:pressed { background: #1e40af; }"
        "#modeButton { min-height: 40px; border: none; color: #2563eb; background: transparent; font-size: 14px; font-weight: 600; }"
        "#modeButton:hover { color: #1d4ed8; background: #eff6ff; border-radius: 8px; }"
    );
}

void LoginWindow::setRegisterMode(bool enabled)
{
    registerMode = enabled;
    messageLabel->clear();
    confirmPasswordEdit->setVisible(registerMode);
    confirmPasswordEdit->clear();

    if (registerMode)
    {
        setWindowTitle(QString::fromUtf8(u8"用户注册"));
        titleLabel->setText(QString::fromUtf8(u8"创建账号"));
        subtitleLabel->setText(QString::fromUtf8(u8"注册后即可进入监控系统，账号信息会保存到本地 SQLite 数据库。"));
        submitButton->setText(QString::fromUtf8(u8"注册并登录"));
        modeButton->setText(QString::fromUtf8(u8"已有账号，返回登录"));
    }
    else
    {
        setWindowTitle(QString::fromUtf8(u8"用户登录"));
        titleLabel->setText(QString::fromUtf8(u8"欢迎回来"));
        subtitleLabel->setText(QString::fromUtf8(u8"请输入账号密码，验证通过后进入监控主界面。"));
        submitButton->setText(QString::fromUtf8(u8"登录系统"));
        modeButton->setText(QString::fromUtf8(u8"没有账号？立即注册"));
    }
}

void LoginWindow::submit()
{
    if (!database)
    {
        messageLabel->setText(QString::fromUtf8(u8"数据库未初始化"));
        return;
    }

    const QString username = usernameEdit->text().trimmed();
    const QString password = passwordEdit->text();
    QString errorMessage;

    if (registerMode)
    {
        if (password != confirmPasswordEdit->text())
        {
            messageLabel->setText(QString::fromUtf8(u8"两次输入的密码不一致"));
            return;
        }

        if (!database->registerUser(username, password, &errorMessage))
        {
            messageLabel->setText(errorMessage);
            return;
        }
    }

    UserRecord user;
    if (!database->validateUser(username, password, &user, &errorMessage))
    {
        messageLabel->setText(errorMessage);
        return;
    }

    loggedInUser = user;
    accept();
}
