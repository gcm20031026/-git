#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QDialog>
#include "data/qmysqlite.h"

class QLabel;
class QLineEdit;
class QPushButton;

class LoginWindow : public QDialog
{
    Q_OBJECT

public:
    explicit LoginWindow(QMySqlite *database, QWidget *parent = nullptr);

    UserRecord currentUser() const;

private:
    void buildUi();
    void applyStyle();
    void setRegisterMode(bool enabled);
    void submit();

    QMySqlite *database = nullptr;
    bool registerMode = false;
    UserRecord loggedInUser;

    QLabel *titleLabel = nullptr;
    QLabel *subtitleLabel = nullptr;
    QLabel *messageLabel = nullptr;
    QLineEdit *usernameEdit = nullptr;
    QLineEdit *passwordEdit = nullptr;
    QLineEdit *confirmPasswordEdit = nullptr;
    QPushButton *submitButton = nullptr;
    QPushButton *modeButton = nullptr;
};

#endif // LOGINWINDOW_H
