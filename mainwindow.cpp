#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "aes.h"
#include <QFileDialog>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QTabWidget>
#include <QRandomGenerator>
#include <QFile>
#include <QMessageBox>
#include <QByteArray>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    // Styling
    QString buttonStyle = "QPushButton { background-color: #4CAF50; color: white; border-radius: 10px; padding: 6px; }"
                          "QPushButton:hover { background-color: #45a049; }";
    QString editStyle = "QLineEdit { border: 2px solid #ccc; border-radius: 4px; padding: 0 8px; }"
                        "QTextEdit { border: 2px solid #ccc; border-radius: 4px; }";

    // Setup tabs
    QTabWidget *tabWidget = new QTabWidget(this);
    QWidget *encryptTab = new QWidget();
    QWidget *decryptTab = new QWidget();

    QVBoxLayout *encryptLayout = new QVBoxLayout(encryptTab);
    QVBoxLayout *decryptLayout = new QVBoxLayout(decryptTab);

    QLabel *encryptKeyInfo = new QLabel("Key [0/16]:");
    QLabel *encryptIVInfo = new QLabel("IV [0/16]:");  // IV Label for encryption
    QLabel *decryptKeyInfo = new QLabel("Key [0/16]:");
    QLabel *decryptIVInfo = new QLabel("IV [0/16]:");  // IV Label for decryption

    QLineEdit *encryptInput = new QLineEdit();
    QLineEdit *encryptKey = new QLineEdit();
    QLineEdit *encryptIV = new QLineEdit();
    encryptInput->setStyleSheet(editStyle);
    encryptKey->setStyleSheet(editStyle);
    encryptIV->setStyleSheet(editStyle);

    QPushButton *generateEncryptKeyIV = new QPushButton("Generate Key/IV");
    QPushButton *encryptButton = new QPushButton("Encrypt Text");
    generateEncryptKeyIV->setStyleSheet(buttonStyle);
    encryptButton->setStyleSheet(buttonStyle);

    QTextEdit *encryptOutput = new QTextEdit();
    encryptOutput->setStyleSheet(editStyle);

    encryptLayout->addWidget(new QLabel("Plaintext:"));
    encryptLayout->addWidget(encryptInput);
    encryptLayout->addWidget(encryptKeyInfo);
    encryptLayout->addWidget(encryptKey);
    encryptLayout->addWidget(encryptIVInfo);
    encryptLayout->addWidget(encryptIV);
    encryptLayout->addWidget(generateEncryptKeyIV);
    encryptLayout->addWidget(encryptButton);
    encryptLayout->addWidget(new QLabel("Encrypted Text:"));
    encryptLayout->addWidget(encryptOutput);

    QLineEdit *decryptInput = new QLineEdit();
    QLineEdit *decryptKey = new QLineEdit();
    QLineEdit *decryptIV = new QLineEdit();
    decryptInput->setStyleSheet(editStyle);
    decryptKey->setStyleSheet(editStyle);
    decryptIV->setStyleSheet(editStyle);

    QPushButton *generateDecryptKeyIV = new QPushButton("Generate Key/IV");
    QPushButton *decryptButton = new QPushButton("Decrypt Text");
    generateDecryptKeyIV->setStyleSheet(buttonStyle);
    decryptButton->setStyleSheet(buttonStyle);

    QTextEdit *decryptOutput = new QTextEdit();
    decryptOutput->setStyleSheet(editStyle);

    decryptLayout->addWidget(new QLabel("Encrypted Text:"));
    decryptLayout->addWidget(decryptInput);
    decryptLayout->addWidget(decryptKeyInfo);
    decryptLayout->addWidget(decryptKey);
    decryptLayout->addWidget(decryptIVInfo);
    decryptLayout->addWidget(decryptIV);
    decryptLayout->addWidget(generateDecryptKeyIV);
    decryptLayout->addWidget(decryptButton);
    decryptLayout->addWidget(new QLabel("Decrypted Text:"));
    decryptLayout->addWidget(decryptOutput);

    tabWidget->addTab(encryptTab, "Encrypt");
    tabWidget->addTab(decryptTab, "Decrypt");
    setCentralWidget(tabWidget);

    connect(encryptButton, &QPushButton::clicked, [this, encryptInput, encryptKey, encryptIV, encryptOutput](){
        AES aes(encryptKey->text(), encryptIV->text());
        encryptOutput->setText(aes.encrypt(encryptInput->text()));
    });

    connect(decryptButton, &QPushButton::clicked, [this, decryptInput, decryptKey, decryptIV, decryptOutput](){
        AES aes(decryptKey->text(), decryptIV->text());
        decryptOutput->setText(aes.decrypt(decryptInput->text()));
    });

    connect(encryptKey, &QLineEdit::textChanged, [encryptKeyInfo, encryptKey](){
        encryptKeyInfo->setText(QString("Key [%1/16]:").arg(encryptKey->text().length()));
    });
    connect(encryptIV, &QLineEdit::textChanged, [encryptIVInfo, encryptIV](){
        encryptIVInfo->setText(QString("IV [%1/16]:").arg(encryptIV->text().length()));
    });

    connect(decryptKey, &QLineEdit::textChanged, [decryptKeyInfo, decryptKey](){
        decryptKeyInfo->setText(QString("Key [%1/16]:").arg(decryptKey->text().length()));
    });
    connect(decryptIV, &QLineEdit::textChanged, [decryptIVInfo, decryptIV](){
        decryptIVInfo->setText(QString("IV [%1/16]:").arg(decryptIV->text().length()));
    });

    connect(generateEncryptKeyIV, &QPushButton::clicked, [this, encryptKey, encryptIV](){
        encryptKey->setText(generateRandomString(16));
        encryptIV->setText(generateRandomString(16));
    });

    connect(generateDecryptKeyIV, &QPushButton::clicked, [this, decryptKey, decryptIV](){
        decryptKey->setText(generateRandomString(16));
        decryptIV->setText(generateRandomString(16));
    });
}

QString MainWindow::generateRandomString(int length) const {
    const QString possibleCharacters("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
    QString randomString;
    for(int i = 0; i < length; ++i) {
        int index = QRandomGenerator::global()->bounded(possibleCharacters.length());
        randomString.append(possibleCharacters.at(index));
    }
    return randomString;
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_pushButtonEncrypt_clicked() {
    QString key = ui->lineEditKey->text();
    QString plainText = ui->lineEditInput->text();
    //QString iv = ui->lineEditIV->text();
    AES aes(key);
    QString encryptedText = aes.encrypt(plainText);
    ui->textEditOutput->setText(encryptedText);
}

void MainWindow::on_pushButtonDecrypt_clicked() {
    QString key = ui->lineEditKey->text();
    QString cipherText = ui->textEditOutput->toPlainText();
    //QString iv = ui->lineEditIV->text();
    AES aes(key);
    QString decryptedText = aes.decrypt(cipherText);
    ui->lineEditInput->setText(decryptedText);
}
