#ifndef __MIPS_Simulator__MemoryView__
#define __MIPS_Simulator__MemoryView__

#include <QObject>
#include <QtWidgets>
#include <string>
#include <iostream>

class MemoryView : public QWidget {
    Q_OBJECT
public:
    MemoryView(int size) {
        _size = size;
        QGridLayout *mmLayout = new QGridLayout;
        mmsa = new QScrollArea;
        QGroupBox *tmpGroup2 = new QGroupBox;
        QGridLayout *tmpLayout2 = new QGridLayout;
        memView = new QLabel*[_size];
        for(int i = 0; i < _size; ++i) {
            if (i%4 == 0) {
                 std::string lb = std::to_string(i)+":";
                 QPushButton *bkPB = new QPushButton(lb.c_str());
                 bkPB->setCheckable(true);
                 bkPB->setMaximumWidth(60);
                 connect(bkPB, SIGNAL(clicked()), this, SLOT(breakPoint()));
                 tmpLayout2->addWidget(bkPB, i/4, 0);
            }
            memView[i] = new QLabel(tr("0"));
            tmpLayout2->addWidget(memView[i], i/4, 1+i%4);
        }

        tmpGroup2->setLayout(tmpLayout2);
        tmpGroup2->setMinimumWidth(220);
        mmsa->setWidget(tmpGroup2);
        decRB = new QRadioButton("dec");
        decRB->setChecked(true);
        connect(decRB, SIGNAL(toggled(bool)), this, SLOT(format()));
        QRadioButton *hexRB = new QRadioButton("hex");
        QGroupBox *rbGroup = new QGroupBox;
        QHBoxLayout *rbLayout = new QHBoxLayout;
        rbLayout->addWidget(decRB);
        rbLayout->addWidget(hexRB);
        rbGroup->setLayout(rbLayout);

        mmLayout->addWidget(new QLabel("index:"), 0, 0);
        mmLayout->addWidget(new QLabel("data"), 0, 1, 1, 3);
        mmLayout->addWidget(rbGroup, 0, 4);
        mmLayout->addWidget(mmsa, 1, 0, 1, 5);
        setLayout(mmLayout);
        //setMinimumWidth(400);
    }
    ~MemoryView() {
        for (int i = 0; i < _size; ++i) {
            delete memView[i];
        }
        delete[] memView;
        delete mmsa;
    }

public slots:
    void update(uint8_t *data, uint32_t add, int len) {
        for (int i = add; i<add+len; ++i) {
            char buf[50];
            if (decRB->isChecked()) {
                sprintf(buf, "%d", (int)data[i]);
            } else {
                sprintf(buf, "%x", (int)data[i]);
            }
            memView[i]->setText(buf);
        }
    }
    void format() {
       if (decRB->isChecked()) {
           for (int i = 0; i < _size; ++i) {
               int data = std::stoi(memView[i]->text().toStdString(), nullptr, 16);
               char buf[50];
               sprintf(buf, "%d", data);
               memView[i]->setText(buf);
           }
       } else {
           for (int i = 0; i < _size; ++i) {
               int data = std::stoi(memView[i]->text().toStdString(), nullptr, 10);
               char buf[50];
               sprintf(buf, "%x", data);
               memView[i]->setText(buf);
           }
       }
    }

signals:
    void addBreakPoint(uint32_t pc);
    void clearBreakPoint(uint32_t pc);
private slots:
    void breakPoint() {
         QPushButton *s = (QPushButton*)QObject::sender();
         uint32_t pc = std::stoi(s->text().toStdString());
         if (s->isChecked()) {
            emit addBreakPoint(pc);
         } else {
           emit clearBreakPoint(pc);
         }
    }

private:
    QScrollArea *mmsa;
    QLabel **memView;
    QRadioButton *decRB;
    int _size;
};

#endif
