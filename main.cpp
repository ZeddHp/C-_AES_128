#include "mainwindow.h"
#include "aes.h"
#include <QApplication>
#include <QLocale>
#include <QTranslator>

/*void testAES() {
    QString key = "thisis16bitkey!!"; // 16 bytes key for AES-128
    QString plaintext = "Hello, World!123"; // 16 bytes plaintext
    QString encrypted = encrypt(plaintext, key);
    QString decrypted = decrypt(encrypted, key);

    qDebug() << "Plaintext: " << plaintext;
    qDebug() << "Encrypted: " << encrypted;
    qDebug() << "Decrypted: " << decrypted;
}*/

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QString key = "your16charlength";
    QString plaintext = "Hello, World!123";
    QString iv = "1234567890123456";

    //AES aes(key);
    //NEW
    AES aes(key, iv);
    QString encryptedText = aes.encrypt(plaintext);
    QString decryptedText = aes.decrypt(encryptedText);

    MainWindow w;
    w.show();
    return a.exec();
}
