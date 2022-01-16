#ifndef TOOL_H
#define TOOL_H

#include <QString>
#include <QLabel>
#include <pageone.h>
#include <pagetwo.h>
#include <QFileDialog>
#include <string>
#include <QProgressBar>
#include <QCoreApplication>
#include <QLabel>

template <typename T>
QString getFilePath(QLabel *lbptr, T *pgptr);

template <typename T>
QString getDir(QLabel *lbptr, T *pgptr);

std::string readBinStr(std::string, QProgressBar *, unsigned long long &, QLabel *);

std::string strToBin(std::string);

inline void modify(unsigned char &, int, int);

void writeToPic(std::string, std::string, std::string, std::string, unsigned long long, int, QProgressBar *, QLabel *);

QString getPicPath(QLabel *, Pagetwo *);

std::string costumPicPath(PageOne *);

bool getlast(int, std::string &, std::string &);

void getFile(std::string, std::string, QProgressBar *, QLabel *);

std::string solve(const char *);

unsigned char ch2hex(char);

int ZO(char);

std::string binToHex(std::string);

void getbit(int, int, std::string &);
#endif // TOOL_H
