/*
CoreInfo give's a file's detail informations.

CoreInfo is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; version 2
of the License.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, see {http://www.gnu.org/licenses/}. */

#include "coreinfo.h"

#include "configtreetext.h"

#include <QDir>
#include <QMimeData>
#include <QTextBrowser>
#include <QTreeWidget>
#include <QTimer>
#include <QProgressDialog>
#include <QVBoxLayout>


coreinfo::coreinfo( QStringList fileNames, QWidget *parent): QTreeWidget(parent)
{

    C = new Core();
    openFiles( fileNames );

//    QVBoxLayout * mainLayout = new QVBoxLayout();
//    viewWidget = new QWidget();
//    setWindowTitle("CoreInfo");
//    mainLayout->setContentsMargins(0,0,0,0);
//    mainLayout->addWidget(viewWidget);
//    setLayout(mainLayout);
//    this->show();
}

coreinfo::~coreinfo()
{

}

void coreinfo::openFiles(QStringList fileNames)
{
    //Configuring
    if(fileNames.isEmpty())
        return;
    for(int i=0;i<fileNames.size();i++) {
        fileNames[i] = QDir::toNativeSeparators(fileNames[i]);
    }
    C->Menu_File_Open_Files_Begin(true, false);
    for (int Pos=0; Pos<fileNames.size(); Pos++){
        C->Menu_File_Open_Files_Continue(QString2wstring(fileNames[Pos]));

    }

    openTimerInit();

    refreshDisplay();
}

void coreinfo::openTimerInit ()
{
    progressDialog=new QProgressDialog("Opening files...","Abort Opening", 0, 10000, this);
    progressDialog->setWindowModality(Qt::WindowModal);
    progressDialog->setMinimumDuration(0);
    progressDialog->setWindowTitle("MediaInfo");

    if (timer==nullptr)
    {
        timer=new QTimer();
        connect(timer, SIGNAL(timeout()), this, SLOT(updateProgressBar()));
        timer->start(100);
    }
}

void coreinfo::updateProgressBar ()
{
    if (progressDialog==nullptr)
        return;

    progressDialog->setValue((int)C->State_Get());

    if (C->State_Get()==10000 || progressDialog->wasCanceled())
    {
        progressDialog->hide();
        timer->stop();

        delete progressDialog; progressDialog=nullptr;
        delete timer; timer=nullptr;

        //Showing
        refreshDisplay();
    }
}

void coreinfo::refreshDisplay()
{

    C->Menu_View_Tree();
    loadTreeView(true);

    // Show info in QTextBrowser
//    QFont font("Mono");
//    font.setStyleHint(QFont::TypeWriter);
//    C->Menu_View_Text();
//    viewWidget = new QTextBrowser();
//    ((QTextBrowser*)viewWidget)->setFont(font);
//    if(ConfigTreeText::getIndex()==0)
//        ((QTextBrowser*)viewWidget)->setText(wstring2QString(C->Inform_Get()));
//    else {
//        for (size_t FilePos=0; FilePos<C->Count_Get(); FilePos++) {
//            for (int streamKind=0;streamKind<4;streamKind++) {
//                if(!ConfigTreeText::getConfigTreeText()->getFields(streamKind).isEmpty())
//                    ((QTextBrowser*)viewWidget)->append("\n"+wstring2QString(C->Get(FilePos, (stream_t)streamKind, 0, __T("StreamKind/String"), Info_Text)));
//                for (size_t streamPos=Stream_General; streamPos<C->Count_Get(FilePos, (stream_t)streamKind); streamPos++)
//                {
//                    foreach(QString field, ConfigTreeText::getConfigTreeText()->getFields(streamKind)) {
//                        QString A=wstring2QString(C->Get(FilePos, (stream_t)streamKind, streamPos, QString2wstring(field)));
//                        QString B=wstring2QString(C->Get(FilePos, (stream_t)streamKind, streamPos, QString2wstring(field), Info_Name_Text));
//                        if (B.isEmpty())
//                            B=wstring2QString(C->Get(FilePos, (stream_t)streamKind, streamPos, QString2wstring(field), Info_Name));
//                        ((QTextBrowser*)viewWidget)->append(B+" : "+A);
//                    }
//                }
//            }
//        }
//    }

}

QDir coreinfo::getCommonDir(Core*C)
{
    QList<QStringList> list;
    QStringList dirName;
    unsigned fileCount = (unsigned)C->Count_Get();
    if(fileCount==0)
        return QDir::home();
    for(unsigned filePos=0;filePos<fileCount;filePos++)
        list.append(QDir::toNativeSeparators(wstring2QString(C->Get(filePos,Stream_General, 0, __T("CompleteName")))).split(QDir::separator ()));
    if(!list.isEmpty())
        for(int i=0;i<list[0].size()-1;++i) {
            int j;
            for(j=1;(j<list.size())&&(list[j].size()>i);++j) {
                if(list[j][i]!=list[0][i])
                    break;
            }
            if((j<list.size())&&(list[j].size()>=i))
                break;
            else
                dirName.append(list[0][i]);
        }
    QDir dir(dirName.join(QDir::separator ()));
    return dir;
}

QString coreinfo::shortName(QDir d, QString name)
{
    //Elminating unuseful info from filenames
    return d.relativeFilePath(name);
}


void coreinfo::loadTreeView(bool completeDisplay)
{
    setColumnCount(2);
    QStringList headers = QStringList("key");
    headers.append("value");
    setHeaderLabels(headers);
    unsigned fileCount = (unsigned)C->Count_Get();

    QDir dir = getCommonDir(C);
    for (size_t filePos=0; filePos<fileCount; filePos++) {
        //Pour chaque fichier
        QTreeWidgetItem* treeItem = new QTreeWidgetItem(this,QStringList(shortName(dir,wstring2QString(C->Get(filePos, Stream_General, 0, __T("CompleteName"))))));
        addTopLevelItem(treeItem);

        for (int streamKind=(int)Stream_General; streamKind<(int)Stream_Max; streamKind++)
        {
            //Pour chaque type de flux
            QString StreamKindText=wstring2QString(C->Get(filePos, (stream_t)streamKind, 0, __T("StreamKind/String"), Info_Text));
            size_t StreamsCount=C->Count_Get(filePos, (stream_t)streamKind);
            for (size_t streamPos=Stream_General; streamPos<StreamsCount; streamPos++)
            {
                //Pour chaque stream
                QString A=StreamKindText;
                QString B=wstring2QString(C->Get(filePos, (stream_t)streamKind, streamPos, __T("StreamKindPos"), Info_Text));
                if (!B.isEmpty())
                {
                    A+=" #"+B;
                }
                QTreeWidgetItem* node = new QTreeWidgetItem(treeItem,QStringList(A));
                treeItem->addChild(node);

                if(ConfigTreeText::getIndex()==0) {
                    size_t ChampsCount=C->Count_Get(filePos, (stream_t)streamKind, streamPos);
                    for (size_t Champ_Pos=0; Champ_Pos<ChampsCount; Champ_Pos++)
                    {
                        if ((completeDisplay || C->Get(filePos, (stream_t)streamKind, streamPos, Champ_Pos, Info_Options)[InfoOption_ShowInInform]==__T('Y')) && C->Get(filePos, (stream_t)streamKind, streamPos, Champ_Pos, Info_Text)!=__T(""))
                        {
                            QString A=wstring2QString(C->Get(filePos, (stream_t)streamKind, streamPos, Champ_Pos, Info_Text));
                            A+=wstring2QString(C->Get(filePos, (stream_t)streamKind, streamPos, Champ_Pos, Info_Measure_Text));

                            QString D=wstring2QString(C->Get(filePos, (stream_t)streamKind, streamPos, Champ_Pos, Info_Name_Text));
                            if (D.isEmpty())
                                D=wstring2QString(C->Get(filePos, (stream_t)streamKind, streamPos, Champ_Pos, Info_Name)); //Texte n'existe pas

                            QStringList sl = QStringList(D);
                            sl.append(A);
                            node->addChild(new QTreeWidgetItem(node,sl));
                        }
                    }
                } else {
                    foreach(QString field, ConfigTreeText::getConfigTreeText()->getFields(streamKind)) {
                        QString A=wstring2QString(C->Get(filePos, (stream_t)streamKind, streamPos, QString2wstring(field), Info_Text));
                        A+=wstring2QString(C->Get(filePos, (stream_t)streamKind, streamPos, QString2wstring(field), Info_Measure_Text));
                        QString B=wstring2QString(C->Get(filePos, (stream_t)streamKind, streamPos, QString2wstring(field), Info_Name_Text));
                        if (B.isEmpty())
                            B=wstring2QString(C->Get(filePos, (stream_t)streamKind, streamPos, QString2wstring(field), Info_Name));
                        QStringList sl = QStringList(B);
                        sl.append(A);
                        node->addChild(new QTreeWidgetItem(node,sl));
                    }
                }
            }
        }
    }
    if(C->Count_Get()<=1)
        expandAll();
    resizeColumnToContents(0);
}
