#ifndef __MIPS_Simulator__CacheView__
#define __MIPS_Simulator__CacheView__

#include <QObject>
#include <QtWidgets>
#include <string>
#include <iostream>
#include "cache.h"

class CacheView : public QWidget {
  Q_OBJECT
public:
    CacheView(int indexsize, int ways, int linesize) {
        ccsa = new QScrollArea;
        QGridLayout *ccLayout = new QGridLayout;
        QGroupBox *tmpGroup = new QGroupBox;
        QGridLayout *tmpLayout = new QGridLayout;
        _indexsize = indexsize;
        _linesize = linesize;
        _ways = ways;
        int cacheSize = indexsize * ways;
        cacheView = new QLabel*[cacheSize];
        for(int i = 0; i < indexsize; ++i) {
            for (int j = 0; j < ways; ++j) {
                std::string lb = std::to_string(i)+", "+std::to_string(j)+": ";
                std::string cv = "0 | 0 | 0 | 0 | ";
                for (int k = 0; k < linesize; ++k) {
                    cv += " 0 ";
                }
              //  std::cout<<"cacheview "<<i<<" "<<j<<" "<<cv<<std::endl;
                cacheView[i*ways+j] = new QLabel(cv.c_str());
                tmpLayout->addWidget(new QLabel(lb.c_str()), i*ways+j, 0);
                tmpLayout->addWidget(cacheView[i*ways+j], i*ways+j, 1, 1, 2);
            }
        }
        tmpGroup->setLayout(tmpLayout);
        tmpGroup->setMinimumWidth(250);
        ccsa->setWidget(tmpGroup);
        hitLb = new QLabel("0");
        missLb = new QLabel("0");
        ccLayout->addWidget(new QLabel("hit: "), 0, 0);
        ccLayout->addWidget(hitLb, 0, 1);
        ccLayout->addWidget(new QLabel("miss: "), 0, 2);
        ccLayout->addWidget(missLb, 0, 3);
        ccLayout->addWidget(new QLabel("index, way: "), 1, 0);
        ccLayout->addWidget(new QLabel("tag | valid | dirty | lru | data"), 1, 1, Qt::AlignHCenter);
        ccLayout->addWidget(ccsa, 2, 0, 1, 4);
        setLayout(ccLayout);
    }
    ~CacheView() {
        delete hitLb;
        delete missLb;
        for (int i = 0; i < _indexsize; ++i) {
            for (int j = 0; j < _ways; ++j) {
                delete cacheView[i*_ways+j];
            }
        }
        delete ccsa;
    }

 public slots:
    void lineUpdate(Cacheline** data, int idx, int way) {
        //std::cout<<"cache update "<<idx<<std::endl;
        std::string cv = "";
        cv += std::to_string(data[idx][way].tag) + " | " + std::to_string((int)data[idx][way].valid)
                + " | " + std::to_string((int)data[idx][way].dirty) + " | " + std::to_string(data[idx][way].lru)
                + " | ";
        for (int k = 0; k < _linesize; ++k) {
                cv += std::to_string((int)(data[idx][way].data[k])) + " ";
        }
        cacheView[idx*_ways+way]->setText(cv.c_str());
    }

    void hitUpdate(int hit) {
        hitLb->setText(std::to_string(hit).c_str());
    }

    void missUpdate(int miss) {
        missLb->setText(std::to_string(miss).c_str());
    }
private:
    QLabel *hitLb;
    QLabel *missLb;
    QScrollArea *ccsa;
    QLabel **cacheView;
    int _indexsize;
    int _linesize;
    int _ways;
};

#endif
