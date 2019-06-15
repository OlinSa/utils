#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <fstream>
#include <unistd.h>

typedef struct
{
    char *buf;
    int bufSize;
    char *cur;
    int pos;
} CACHE_CTX_T;

MainWindow::MainWindow(QWidget *parent, int bufSize) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    model = new QStandardItemModel(this);
    model->setItem(0,0, new QStandardItem("ID"));
    model->setItem(0,1, new QStandardItem("Pos"));
    model->setItem(0,2, new QStandardItem("IDC"));
    model->setItem(0,3, new QStandardItem("Type"));
    model->setItem(0,4, new QStandardItem("Len"));
    ui->tableView->setModel(model);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    //    ui->tableView->setColumnWidth(1, 10);

    //    model->item(0,0)->setForeground(QBrush(QColor(255, 0, 0)));
    //    model->item(0,0)->setBackground(QBrush(QColor(0, 255, 0)));


    if(NULL == (h264Wrap = new H264Wrap())){
        LOG_ERR("construct H264Wrap failed");
    }
    if(NULL == (buf = new unsigned char[bufSize])){
        LOG_ERR("malloc %d bytes failed", bufSize);
    }
    this->bufSize = bufSize;

    ui->lineEditFilePath->setText(QString::fromStdString("test.264"));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete h264Wrap;
    delete [] buf;
}

//                pos          alreadSeen      bufSize
// |  #############| ############| #############|
int MainWindow::showMediaInfo(const std::string &filename)
{
    unsigned long long fileOffset = 0;;

    int pos = 0;
    int alreadSeen = 0;
    int bufSize = this->bufSize;
    std::streamsize readCount;
    int nextFramePos;               //下一个nalu 偏移量

    NaluUnit nalu;
    NaluParam naluParam;
    int naluCount = 0;

    std::ifstream inFile(filename);
    if(!inFile){
        LOG_DEBUG("open %s failed", filename.c_str());
        return -1;
    }
    while(inFile.read(reinterpret_cast<char*>(&buf[0]) + alreadSeen, bufSize - alreadSeen))  {
        readCount = inFile.gcount();
        alreadSeen += readCount;
        //解析nalu
        while ((nextFramePos = h264Wrap->GetNaluFromBuffer(nalu, buf + pos, alreadSeen - pos)) > 0)
        {
            naluCount++;

            if(h264Wrap->DecodeNaluParams(naluParam, nalu.data, nalu.size)){
                insertItem(nalu, naluParam, fileOffset, naluCount, naluCount, QBrush());
                h264Wrap->FreeNaluParams(naluParam);
            }else {
                LOG_ERR("decode nalu params failed");
            }
            fileOffset += nextFramePos;
            pos += nextFramePos;
            LOG_DEBUG("%d nalu: startcode:%d,type:%d, data:%p, dataSize:%d,nextFramePos:%d,pos:%d,left:%d",
                      naluCount, nalu.startCode, nalu.type, nalu.data, nalu.size, nextFramePos, pos, alreadSeen-pos);

        }
        if (pos > 0)
        {
            memmove(buf, buf + pos, alreadSeen - pos); //memcpy会有内存重叠问题
            alreadSeen -= pos;
            pos = 0;
        }
        else
        {
            pos = 0;
            alreadSeen = 0;
        }
    }
    inFile.close();
    LOG_DEBUG("END");
}

void MainWindow::on_pushButtonStart_clicked()
{
    showMediaInfo(ui->lineEditFilePath->text().toStdString());
}


int MainWindow::insertItem(const NaluUnit &nalu, const NaluParam &naluParam, unsigned long long pos, int id, int row, QBrush brush)
{

    QList<QStandardItem*> list;
    //插入索引
    list<<new QStandardItem(QString::number(id));
    list<<new QStandardItem(QString::number(pos));

    QString idc;

    //nalu优先级
    switch (naluParam.header.nal_ref_idc) {
    case NALU_PRIORITY_DISPOSABLE:
    {
        idc = "DISPOSABLE";
    }break;
    case NALU_PRIRITY_LOW:
    {
        idc = "LOW";
    }break;
    case NALU_PRIORITY_HIGH:
    {
        idc = "HIGH";
    }break;
    case NALU_PRIORITY_HIGHEST:
    {
        idc = "HIGHEST";
    }break;
    default:
    {
        idc = "UNKNOW";
    }
    }

    list<<new QStandardItem(idc);

    //nalu类型
    QString type;
    switch (nalu.type) {
    case NALU_TYPE_SLICE:{
        type = "SLICE";
    }break;
    case NALU_TYPE_DPA:{
        type = "DPA";
    }break;
    case NALU_TYPE_DPB:{
        type = "DPB";
    }break;
    case NALU_TYPE_DPC:{
        type = "DPC";
    }break;
    case NALU_TYPE_IDR:{
        type = "IDR";
    }break;
    case NALU_TYPE_SEI:{
        type = "SEI";
    }break;
    case NALU_TYPE_SPS:{
        type = "SPS";
    }break;
    case NALU_TYPE_PPS:{
        type = "PPS";
    }break;
    case NALU_TYPE_AUD:{
        type = "AUD";
    }break;
    case NALU_TYPE_EOSEQ:{
        type = "EOSEQ";
    }break;
    case NALU_TYPE_EOSTREAM:{
        type = "EOSTREAM";
    }break;
    case NALU_TYPE_FILL:{
        type = "DPA";
    }break;
    default:{
        type = "UNKOWN";
    }
    }
    list<<new QStandardItem(type);

    //nalu size
    list<<new QStandardItem(QString::number(nalu.size));
    model->insertRow(row, list);
    for(int i = 0; i < 5; i++) {
        model->item(row, i)->setBackground(brush);
    }
    return 0;
}
