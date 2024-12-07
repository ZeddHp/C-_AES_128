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

    // ------------------------- Text Encryption/Decryption Tabs -------------------------
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

    // ------------------------- File Encryption/Decryption Tabs -------------------------
    QWidget *encryptFileTab = new QWidget();
    QVBoxLayout *encryptFileLayout = new QVBoxLayout(encryptFileTab);

    QLabel *encryptFileKeyInfo = new QLabel("Key [0/16]:");
    QLabel *encryptFileIVInfo = new QLabel("IV [0/16]:");

    QLineEdit *encryptFileInput = new QLineEdit();
    QLineEdit *encryptFileOutput = new QLineEdit();
    QLineEdit *encryptFileKey = new QLineEdit();
    QLineEdit *encryptFileIV = new QLineEdit();
    encryptFileInput->setStyleSheet(editStyle);
    encryptFileOutput->setStyleSheet(editStyle);
    encryptFileKey->setStyleSheet(editStyle);
    encryptFileIV->setStyleSheet(editStyle);

    QPushButton *browseEncryptInput = new QPushButton("Browse Input File");
    QPushButton *browseEncryptOutput = new QPushButton("Browse Output File");
    QPushButton *generateEncryptFileKeyIV = new QPushButton("Generate Key/IV");
    QPushButton *encryptFileButton = new QPushButton("Encrypt File");
    browseEncryptInput->setStyleSheet(buttonStyle);
    browseEncryptOutput->setStyleSheet(buttonStyle);
    generateEncryptFileKeyIV->setStyleSheet(buttonStyle);
    encryptFileButton->setStyleSheet(buttonStyle);

    encryptFileLayout->addWidget(new QLabel("Input File:"));
    encryptFileLayout->addWidget(encryptFileInput);
    encryptFileLayout->addWidget(browseEncryptInput);

    encryptFileLayout->addWidget(new QLabel("Output File:"));
    encryptFileLayout->addWidget(encryptFileOutput);
    encryptFileLayout->addWidget(browseEncryptOutput);

    encryptFileLayout->addWidget(encryptFileKeyInfo);
    encryptFileLayout->addWidget(encryptFileKey);
    encryptFileLayout->addWidget(encryptFileIVInfo);
    encryptFileLayout->addWidget(encryptFileIV);
    encryptFileLayout->addWidget(generateEncryptFileKeyIV);
    encryptFileLayout->addWidget(encryptFileButton);

    QWidget *decryptFileTab = new QWidget();
    QVBoxLayout *decryptFileLayout = new QVBoxLayout(decryptFileTab);

    QLabel *decryptFileKeyInfo = new QLabel("Key [0/16]:");
    QLabel *decryptFileIVInfo = new QLabel("IV [0/16]:");

    QLineEdit *decryptFileInput = new QLineEdit();
    QLineEdit *decryptFileOutput = new QLineEdit();
    QLineEdit *decryptFileKey = new QLineEdit();
    QLineEdit *decryptFileIV = new QLineEdit();
    decryptFileInput->setStyleSheet(editStyle);
    decryptFileOutput->setStyleSheet(editStyle);
    decryptFileKey->setStyleSheet(editStyle);
    decryptFileIV->setStyleSheet(editStyle);

    QPushButton *browseDecryptInput = new QPushButton("Browse Input File");
    QPushButton *browseDecryptOutput = new QPushButton("Browse Output File");
    QPushButton *generateDecryptFileKeyIV = new QPushButton("Generate Key/IV");
    QPushButton *decryptFileButton = new QPushButton("Decrypt File");
    browseDecryptInput->setStyleSheet(buttonStyle);
    browseDecryptOutput->setStyleSheet(buttonStyle);
    generateDecryptFileKeyIV->setStyleSheet(buttonStyle);
    decryptFileButton->setStyleSheet(buttonStyle);

    decryptFileLayout->addWidget(new QLabel("Input File:"));
    decryptFileLayout->addWidget(decryptFileInput);
    decryptFileLayout->addWidget(browseDecryptInput);

    decryptFileLayout->addWidget(new QLabel("Output File:"));
    decryptFileLayout->addWidget(decryptFileOutput);
    decryptFileLayout->addWidget(browseDecryptOutput);

    decryptFileLayout->addWidget(decryptFileKeyInfo);
    decryptFileLayout->addWidget(decryptFileKey);
    decryptFileLayout->addWidget(decryptFileIVInfo);
    decryptFileLayout->addWidget(decryptFileIV);
    decryptFileLayout->addWidget(generateDecryptFileKeyIV);
    decryptFileLayout->addWidget(decryptFileButton);

    tabWidget->addTab(encryptFileTab, "Encrypt File");
    tabWidget->addTab(decryptFileTab, "Decrypt File");

    setCentralWidget(tabWidget);

    // ------------------------- Connections for Text Encryption/Decryption -------------------------
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

    // ------------------------- Connections for File Encryption/Decryption -------------------------
    // Update Key/IV labels as user types
    connect(encryptFileKey, &QLineEdit::textChanged, [encryptFileKeyInfo, encryptFileKey](){
        encryptFileKeyInfo->setText(QString("Key [%1/16]:").arg(encryptFileKey->text().length()));
    });
    connect(encryptFileIV, &QLineEdit::textChanged, [encryptFileIVInfo, encryptFileIV](){
        encryptFileIVInfo->setText(QString("IV [%1/16]:").arg(encryptFileIV->text().length()));
    });

    connect(decryptFileKey, &QLineEdit::textChanged, [decryptFileKeyInfo, decryptFileKey](){
        decryptFileKeyInfo->setText(QString("Key [%1/16]:").arg(decryptFileKey->text().length()));
    });
    connect(decryptFileIV, &QLineEdit::textChanged, [decryptFileIVInfo, decryptFileIV](){
        decryptFileIVInfo->setText(QString("IV [%1/16]:").arg(decryptFileIV->text().length()));
    });

    // Generate random Key/IV
    connect(generateEncryptFileKeyIV, &QPushButton::clicked, [this, encryptFileKey, encryptFileIV](){
        encryptFileKey->setText(generateRandomString(16));
        encryptFileIV->setText(generateRandomString(16));
    });

    connect(generateDecryptFileKeyIV, &QPushButton::clicked, [this, decryptFileKey, decryptFileIV](){
        decryptFileKey->setText(generateRandomString(16));
        decryptFileIV->setText(generateRandomString(16));
    });

    // Browse buttons for File Encrypt
    connect(browseEncryptInput, &QPushButton::clicked, [this, encryptFileInput](){
        QString filePath = QFileDialog::getOpenFileName(this, "Select Input File");
        if(!filePath.isEmpty()){
            encryptFileInput->setText(filePath);
        }
    });

    connect(browseEncryptOutput, &QPushButton::clicked, [this, encryptFileOutput](){
        QString filePath = QFileDialog::getSaveFileName(this, "Select Output File");
        if(!filePath.isEmpty()){
            encryptFileOutput->setText(filePath);
        }
    });

    // Browse buttons for File Decrypt
    connect(browseDecryptInput, &QPushButton::clicked, [this, decryptFileInput](){
        QString filePath = QFileDialog::getOpenFileName(this, "Select Input File");
        if(!filePath.isEmpty()){
            decryptFileInput->setText(filePath);
        }
    });

    connect(browseDecryptOutput, &QPushButton::clicked, [this, decryptFileOutput](){
        QString filePath = QFileDialog::getSaveFileName(this, "Select Output File");
        if(!filePath.isEmpty()){
            decryptFileOutput->setText(filePath);
        }
    });

    // Encrypt File
    connect(encryptFileButton, &QPushButton::clicked, [this, encryptFileInput, encryptFileOutput, encryptFileKey, encryptFileIV](){
        QString inPath = encryptFileInput->text();
        QString outPath = encryptFileOutput->text();
        QString key = encryptFileKey->text();
        QString iv = encryptFileIV->text();

        if(inPath.isEmpty() || outPath.isEmpty() || key.length() != 16 || iv.length() != 16) {
            QMessageBox::warning(this, "Warning", "Please provide valid input/output files and a 16-byte key and IV.");
            return;
        }

        QFile inputFile(inPath);
        if(!inputFile.open(QIODevice::ReadOnly)) {
            QMessageBox::critical(this, "Error", "Failed to open input file.");
            return;
        }

        QByteArray inputData = inputFile.readAll();
        inputFile.close();

        AES aes(key, iv);
        QString encryptedData = aes.encrypt(QString::fromLatin1(inputData.toBase64()));

        QFile outputFile(outPath);
        if(!outputFile.open(QIODevice::WriteOnly)) {
            QMessageBox::critical(this, "Error", "Failed to open output file.");
            return;
        }
        // Store encrypted text as is, or could convert to base64 if needed:
        outputFile.write(encryptedData.toUtf8());
        outputFile.close();

        QMessageBox::information(this, "Success", "File encrypted successfully!");
    });

    // Decrypt File
    connect(decryptFileButton, &QPushButton::clicked, [this, decryptFileInput, decryptFileOutput, decryptFileKey, decryptFileIV](){
        QString inPath = decryptFileInput->text();
        QString outPath = decryptFileOutput->text();
        QString key = decryptFileKey->text();
        QString iv = decryptFileIV->text();

        if(inPath.isEmpty() || outPath.isEmpty() || key.length() != 16 || iv.length() != 16) {
            QMessageBox::warning(this, "Warning", "Please provide valid input/output files and a 16-byte key and IV.");
            return;
        }

        QFile inputFile(inPath);
        if(!inputFile.open(QIODevice::ReadOnly)) {
            QMessageBox::critical(this, "Error", "Failed to open input file.");
            return;
        }

        QByteArray encryptedData = inputFile.readAll();
        inputFile.close();

        AES aes(key, iv);
        QString decryptedText = aes.decrypt(QString::fromUtf8(encryptedData));
        // If we encoded original data with base64 before encryption:
        QByteArray decodedData = QByteArray::fromBase64(decryptedText.toLatin1());

        QFile outputFile(outPath);
        if(!outputFile.open(QIODevice::WriteOnly)) {
            QMessageBox::critical(this, "Error", "Failed to open output file.");
            return;
        }
        outputFile.write(decodedData);
        outputFile.close();

        QMessageBox::information(this, "Success", "File decrypted successfully!");
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
    AES aes(key);
    QString encryptedText = aes.encrypt(plainText);
    ui->textEditOutput->setText(encryptedText);
}

void MainWindow::on_pushButtonDecrypt_clicked() {
    QString key = ui->lineEditKey->text();
    QString cipherText = ui->textEditOutput->toPlainText();
    AES aes(key);
    QString decryptedText = aes.decrypt(cipherText);
    ui->lineEditInput->setText(decryptedText);
}
