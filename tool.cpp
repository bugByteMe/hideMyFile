#include "tool.h"
#include <iostream>
#include <QTextCodec>

template <typename T>
QString getFilePath(QLabel *lbptr, T *pgptr){
    QString tmpstr = "";
    QFileDialog *dlg = new QFileDialog(pgptr);
    dlg->setDirectory(".");
    dlg->setNameFilter("File(*.*)");
    dlg->setFileMode(QFileDialog::ExistingFiles);
    dlg->setViewMode(QFileDialog::Detail);
    QStringList filenames;
    if(dlg->exec()){
        filenames = dlg->selectedFiles();
    }
    if(!filenames.empty()){
        tmpstr = filenames[0];
        lbptr->setText(tmpstr);
    }
    return tmpstr;
}

template <typename T>
QString getDir(QLabel *lbptr, T *pgptr){
    QString tmpstr = QFileDialog::getExistingDirectory(pgptr,"选择保存路径..","./");
    if(tmpstr != "") lbptr->setText(tmpstr);
    return tmpstr;
}

std::string readBinStr(std::string path, QProgressBar *bar, unsigned long long &dL, QLabel *lb){
    //Read
    lb->setText("读取数据...");
    unsigned char *ch;
    FILE *fi  = fopen(&(path[0]),"rb");
    fseek(fi,0,SEEK_END);
    unsigned long long len = ftell(fi);
    dL = len;
    ch = (unsigned char *)malloc(len*sizeof(unsigned char));
    fseek(fi,0,SEEK_SET);
    fread(ch, sizeof(unsigned char), len, fi);
    fclose(fi);
    //Convert to Bin
    lb->setText("转换数据...");
    std::string res;

    for(unsigned long long i=0; i<len; i++){
        bar->setValue((unsigned long long)(i*100LL/len/2));
        int tmp = ch[i];
        std::string s;
        int cnt = 0;
        while(tmp){
            cnt++;
            if((tmp&1) == 1){
                s += "1";
            }else{
                s += "0";
            }
            tmp>>=1;
        }
        cnt = 8-cnt;
        for(int j=0; j<cnt; j++){
            s += "0";
        }

        for(int j=0; j<8; j++){
            res += s[8-j-1];
        }
    }
    return res;
}

std::string strToBin(std::string orgStr){
    std::string hexStr, res, table[75];
    table[48]="0000"; table[49]="0001"; table[50]="0010"; table[51]="0011";
    table[52]="0100"; table[53]="0101"; table[54]="0110"; table[55]="0111";
    table[56]="1000"; table[57]="1001"; table[65]="1010"; table[66]="1011";
    table[67]="1100"; table[68]="1101"; table[69]="1110"; table[70]="1111";
    //toHex
    static const char *hex="0123456789ABCDEF";
    for(int i=0;i!=orgStr.size();i++){
        hexStr.push_back(hex[(orgStr[i]>>4)&0xf]);
        hexStr.push_back(hex[orgStr[i]&0xf]);
    }
    hexStr += "FF";
    //toBin
    for(int j=0; j<hexStr.length(); j++){
        res += table[int(hexStr[j])];
    }
    return res;
}

inline void modify(unsigned char &a, int b, int t){
    int k = a;
    if(b == 0){
        k &= ~(1<<(t-1));
    }else{
        k |= (1<<(t-1));
    }
    a = k;
}

void writeToPic(std::string picName, std::string filePath, std::string savePath, std::string binStr, unsigned long long dataLength, int tunnel, QProgressBar *bar, QLabel *lb){
    QImage img = QImage(QString::fromStdString(picName));
    //QImage img = QImage(":/pic/test.jpeg");
    img = img.convertToFormat(QImage::Format_RGB32);
    int height = img.height(), width = img.width();
    uchar *imgbit = img.bits();
    std::string tmp, fileName, fileNameBin;
    //get filename
    lb->setText("写入文件名...");
    for(int i=filePath.length()-1; i>=0; i--){
        if(filePath[i] == '/') break;
        tmp += filePath[i];
    }
    for(int i=tmp.length()-1; i>=0; i--){
        fileName += tmp[i];
    }
    //convert to bin
    fileNameBin = strToBin(fileName);
    QCoreApplication::processEvents();
    //write filename
    unsigned long long ptr = 0;
    for(int i=0; i<width; i++){
        //r
        modify(imgbit[0 + i*4+2], fileNameBin[ptr]-'0',1);
        ptr++;
        if(ptr == fileNameBin.length()) break;
        //g
        modify(imgbit[0 + i*4+1], fileNameBin[ptr]-'0',1);
        ptr++;
        if(ptr == fileNameBin.length()) break;
        //b
        modify(imgbit[0 + i*4+0], fileNameBin[ptr]-'0',1);
        ptr++;
        if(ptr == fileNameBin.length()) break;
    }
    //write datasize
    lb->setText("写入大小...");
    std::string datLenStr, datBinStr;
    std::string str;
    while(dataLength > 0){
        int a = dataLength%10;
        str += a + '0';
        dataLength /= 10;
    }
    for(int i=str.length()-1; i>=0; i--){
        datLenStr += str[i];
    }
    datLenStr += tunnel + '0';
    datBinStr = strToBin(datLenStr);
    ptr = 0;
    for(int i=0; i<width; i++){
        //r
        modify(imgbit[width*4 + i*4+2], datBinStr[ptr]-'0',1);
        ptr++;
        if(ptr == datBinStr.length()) break;
        //g
        modify(imgbit[width*4 + i*4+1], datBinStr[ptr]-'0',1);
        ptr++;
        if(ptr == datBinStr.length()) break;
        //b
        modify(imgbit[width*4 + i*4+0], datBinStr[ptr]-'0',1);
        ptr++;
        if(ptr == datBinStr.length()) break;
    }
    QCoreApplication::processEvents();
    //write data
    lb->setText("写入数据...");
    ptr = 0;
    unsigned long long length = binStr.length();
    for(int t=1; t<=tunnel; t++){
        bool flag = 0;
        for(int i=2; i<height; i++){
            //bool flag = 0;
            for(int j=0; j<width; j++){
                //r //3107520
                modify(imgbit[i*width*4 + j*4+2], binStr[ptr]-'0',t);
                ptr++;
                if(ptr == length){ flag = 1; break;}
                //g
                modify(imgbit[i*width*4 + j*4+1], binStr[ptr]-'0',t);
                ptr++;
                if(ptr == length){ flag = 1; break;}
                //b
                modify(imgbit[i*width*4 + j*4+0], binStr[ptr]-'0',t);
                ptr++;
                if(ptr == length){ flag = 1; break;}
                bar->setValue((unsigned long long)(50ULL+ptr*100ULL/length/2));
            }
            if(flag) break;
        }
        if(flag) break;
    }
    //save pic
    QImage img2;
    img2 = QImage(imgbit,width,height,QImage::Format_ARGB32);
    img2 = img2.convertToFormat(QImage::Format_RGB32);
    QCoreApplication::processEvents();
    img2.save(QString::fromStdString(savePath)+"/output.png");
}

QString getPicPath(QLabel *lbptr, Pagetwo *pgptr){
    QString tmpstr;
    QFileDialog *dlg = new QFileDialog(pgptr);
    dlg->setDirectory(".");
    dlg->setNameFilter("File(*.png)");
    dlg->setFileMode(QFileDialog::ExistingFiles);
    dlg->setViewMode(QFileDialog::Detail);
    QStringList filenames;
    if(dlg->exec()){
        filenames = dlg->selectedFiles();
    }
    if(!filenames.empty()){
        tmpstr = filenames[0];
        lbptr->setText(tmpstr);
    }
    return tmpstr;
}
std::string costumPicPath(PageOne * pgptr){
    QString tmpstr = "";
    QFileDialog *dlg = new QFileDialog(pgptr);
    dlg->setDirectory(".");
    dlg->setNameFilter("*.png *.jpg *.jpeg");
    dlg->setFileMode(QFileDialog::ExistingFiles);
    dlg->setViewMode(QFileDialog::Detail);
    QStringList filenames;
    if(dlg->exec()){
        filenames = dlg->selectedFiles();
    }
    if(!filenames.empty()){
        tmpstr = filenames[0];
    }else{
        return tmpstr.toStdString();
    }
    QImage img;
    img.load(tmpstr);
    QSize size = QSize(800,500);
    img = img.scaled(size, Qt::IgnoreAspectRatio);
    img.save("previewImg.png");
    return tmpstr.toLocal8Bit().toStdString();
}

bool getlast(int x, std::string &last, std::string &str){
    std::string hexTable = "0123456789ABCDEF";
    int lst = 0;
    if(x & 1 == 1){
        lst = 1;
    }
    str += hexTable[lst];
    if(last.length() != 8){
        last += hexTable[lst];
    }else{
        last = last.substr(1,7);
        last += hexTable[lst];
    }
    if(last == "11111111" && str.length() % 8 == 0) return true;
    return false;
}

int ZO(char ss){
    if(ss == '0') return 0;
    return 1;
}

std::string binToHex(std::string binStr){
    std::string hexTable = "0123456789ABCDEF";
    std::string res;
    for(int i=0; i<binStr.length()-8; i+=4){
        int tmp = 0;
        for(int j=3; j>=0; j--){
            int num = ZO(binStr[i+3-j]);
            if(num == 1) tmp |= (1<<j);
        }
        res += hexTable[tmp];
    }
    return res;
}

unsigned char ch2hex(char ch){
    static const char *hex="0123456789ABCDEF";
    for(unsigned char i=0;i!=16;++i)
        if(ch==hex[i])
            return i;
    return 0;
}

std::string solve(const char *src){
    int i=0;
    int cnt=0;
    char dest[500000];
    memset(dest,0,sizeof dest);
    unsigned char*d=(unsigned char*)dest;
    while(*src){
        if(i&1){
            d[cnt++]|=ch2hex(*src);
        }
        else{
            d[cnt]=ch2hex(*src)<<4;
        }
        src++;
        i++;
    }
    std::string out = dest;
    return out;
}

void getbit(int x, int t, std::string &str){
    if((x & (1<<(t-1))) == (1<<(t-1))){
        str += "1";
    }else{
        str += "0";
    }
}

void getFile(std::string picPath, std::string savePath, QProgressBar *bar, QLabel *lb){
    QImage img = QImage(QString::fromStdString(picPath));
    img = img.convertToFormat(QImage::Format_RGB32);
    int height = img.height(), width = img.width();
    uchar* imgbit = img.bits();
    //get filename
    std::string last, fileNameBin, fileNameHex, fileName;
    for(int j=0; j<width; j++){
        int r = imgbit[j*4+2];
        int g = imgbit[j*4+1];
        int b = imgbit[j*4];
        if(getlast(r,last,fileNameBin)){
            break;
        }
        if(getlast(g,last,fileNameBin)){
            break;
        }
        if(getlast(b,last,fileNameBin)){
            break;
        }
    }
    QCoreApplication::processEvents();
    fileNameHex = binToHex(fileNameBin);
    fileName = solve(&(fileNameHex[0]));
    //get size
    last = "";
    std::string sizeBin, sizeHex, sizeStr;
    unsigned long long size = 0;
    int tunnel;
    for(int j=0; j<width; j++){
        int r = imgbit[width*4+j*4+2];
        int g = imgbit[width*4+j*4+1];
        int b = imgbit[width*4+j*4];
        if(getlast(r,last,sizeBin)){
            break;
        }
        if(getlast(g,last,sizeBin)){
            break;
        }
        if(getlast(b,last,sizeBin)){
            break;
        }
    }
    QCoreApplication::processEvents();
    sizeHex = binToHex(sizeBin);
    sizeStr = solve(&(sizeHex[0]));
    tunnel = sizeStr[sizeStr.length()-1] - '0';
    sizeStr = sizeStr.substr(0,sizeStr.length()-1);
    for(int i=0; i<sizeStr.length(); i++){
        size *= 10;
        size += sizeStr[i] - '0';
    }
    size *= 8ULL;
    //get binstr
    lb->setText("读取数据...");
    std::string binStr;
    unsigned long long ptr = 0;
    for(int t=1; t<=tunnel; t++){
        bool flag = 0;
        for(int i=2; i<height; i++){
            //bool flag = 0;
            for(int j=0; j<width; j++){

                int r = imgbit[i*width*4+j*4+2];
                int g = imgbit[i*width*4+j*4+1];
                int b = imgbit[i*width*4+j*4];

                getbit(r,t,binStr);
                ptr++;
                if(ptr == size){
                    flag = 1;
                    break;
                }
                getbit(g,t,binStr);
                ptr++;
                if(ptr == size){
                    flag = 1;
                    break;
                }
                getbit(b,t,binStr);
                ptr++;
                if(ptr == size){
                    flag = 1;
                    break;
                }
                bar->setValue((unsigned long long)ptr*100ULL/size);
            }
            if(flag) break;
        }
        if(flag) break;
    }
    unsigned char *ch;
    ch = (unsigned char *)malloc(size/8ULL*sizeof(unsigned char));
    lb->setText("写入数据...");
    for(unsigned long long i=0; i<binStr.length(); i+=8ULL){
        int num = 0;
        for(int j=0; j<8; j++){
            if(binStr[i+j] == '1'){
                num |= (1<<(7-j));
            }
        }
        ch[i/8ULL] = num;
    }
    savePath += "/" + fileName;
    QTextCodec *code = QTextCodec::codecForName("GB2312");
    QString qstr = QString::fromStdString(savePath);
    std::string path = code->fromUnicode(qstr).data();
    FILE *fp = fopen(path.c_str(),"wb");
    fwrite(ch, sizeof(unsigned char), size/8ULL, fp);
    fclose(fp);
    lb->setText("完成！");
    bar->setValue(100);
}

template QString getFilePath(QLabel *, PageOne *);
//template QString getFilePath(QLabel *, Pagetwo *);

template QString getDir(QLabel *, PageOne *);
template QString getDir(QLabel *, Pagetwo *);
