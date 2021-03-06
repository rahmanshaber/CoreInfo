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
#include "ui_coreinfo.h"


coreinfo::coreinfo(QWidget *parent) :QWidget(parent),ui(new Ui::coreinfo)
{
    ui->setupUi(this);

    // set stylesheet from style.qrc
    setStyleSheet(CPrime::ThemeFunc::getStyleSheetFileContent(CPrime::StyleTypeName::DialogStyle));

    C = new Core();

    // For testing
    //openFiles(QStringList() << "File Path" );
}

coreinfo::~coreinfo()
{
    delete ui;
}

void coreinfo::openFiles(QStringList fileNames)
{
    //Configuring
    if (fileNames.count() == 0) {
        qDebug() << "No file found.\nPlease enter some valid file path.";
        return;
    }
    for (int i = 0; i < fileNames.count(); i++) {
        fileNames[i] = QDir::toNativeSeparators(fileNames[i]);
    }

    this->setWindowTitle(QFileInfo(fileNames[0]).fileName() + " - CoreInfo");

    // For opening multiple files
//    C->Menu_File_Open_Files_Begin(true, false);
//    for (int Pos=0; Pos<fileNames.size(); Pos++){
//        C->Menu_File_Open_Files_Continue(QString2wstring(fileNames[Pos]));
//    }

    // For opening single file
    C->Menu_File_Open_File( QString2wstring( fileNames[ 0 ] ) );

    refreshDisplay();
}

void coreinfo::refreshDisplay()
{
    // Show info in QTreeWidget
    C->Menu_View_Tree();
    ui->mainLayout->addWidget(showTreeView(true));

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
//        }
}

QDir coreinfo::getCommonDir(Core *C) // Get the common directory among all files
{
    QList<QStringList> list;
    QStringList dirName;
    unsigned fileCount = static_cast<unsigned>(C->Count_Get());
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

QTreeWidget *coreinfo::showTreeView(bool completeDisplay)
{
    QTreeWidget* treeWidget = new QTreeWidget();
    //treeWidget->setHeaderHidden(true);
    treeWidget->setColumnCount(2);
    QStringList headers = QStringList("key");
    headers.append("value");
    treeWidget->setHeaderLabels(headers);
    unsigned fileCount = static_cast<unsigned>(C->Count_Get());

    //QDir dir = getCommonDir(C);

    for (size_t filePos=0; filePos<fileCount; filePos++) {

        // For each file
        //QTreeWidgetItem* treeItem = new QTreeWidgetItem(treeWidget,QStringList(shortName(dir,wstring2QString(C->Get(filePos, Stream_General, 0, __T("CompleteName"))))));
        //treeWidget->addTopLevelItem(treeItem);

        for (int streamKind=static_cast<int>(Stream_General); streamKind< static_cast<int>(Stream_Max); streamKind++)
        {
            // For each type of flow
            QString StreamKindText=wstring2QString(C->Get(filePos, static_cast<stream_t>(streamKind), 0, __T("StreamKind/String"), Info_Text));
            size_t StreamsCount=C->Count_Get(filePos,static_cast<stream_t>(streamKind));
            for (size_t streamPos=Stream_General; streamPos<StreamsCount; streamPos++)
            {
                // For each stream
                QString A=StreamKindText;
                QString B=wstring2QString(C->Get(filePos,static_cast<stream_t>(streamKind), streamPos, __T("StreamKindPos"), Info_Text));
                if (!B.isEmpty())
                {
                    A+=" #"+B;
                }

                // For each file
                //QTreeWidgetItem* node = new QTreeWidgetItem(treeItem,QStringList(A));
                //treeItem->addChild(node);

                // For one file
                QTreeWidgetItem* node = new QTreeWidgetItem(treeWidget,QStringList(A));
                treeWidget->addTopLevelItem(node);

                if(ConfigTreeText::getIndex()==0) {
                    size_t ChampsCount=C->Count_Get(filePos, static_cast<stream_t>(streamKind), streamPos);
                    for (size_t Champ_Pos=0; Champ_Pos<ChampsCount; Champ_Pos++)
                    {
                        if ((completeDisplay || C->Get(filePos, static_cast<stream_t>(streamKind), streamPos, Champ_Pos, Info_Options)[InfoOption_ShowInInform]
                             ==__T('Y')) && C->Get(filePos,static_cast<stream_t>(streamKind), streamPos, Champ_Pos, Info_Text)!=__T(""))
                        {
                            QString A=wstring2QString(C->Get(filePos,static_cast<stream_t>(streamKind), streamPos, Champ_Pos, Info_Text));
                            A+=wstring2QString(C->Get(filePos,static_cast<stream_t>(streamKind), streamPos, Champ_Pos, Info_Measure_Text));

                            QString D=wstring2QString(C->Get(filePos,static_cast<stream_t>(streamKind), streamPos, Champ_Pos, Info_Name_Text));
                            if (D.isEmpty())
                                D=wstring2QString(C->Get(filePos, static_cast<stream_t>(streamKind), streamPos, Champ_Pos, Info_Name)); //Texte n'existe pas

                            QStringList sl = QStringList(D);
                            sl.append(A);
                            node->addChild(new QTreeWidgetItem(node,sl));
                        }
                    }
                } else {
                    foreach(QString field, ConfigTreeText::getConfigTreeText()->getFields(streamKind)) {
                        QString A=wstring2QString(C->Get(filePos,static_cast<stream_t>(streamKind), streamPos, QString2wstring(field), Info_Text));
                        A+=wstring2QString(C->Get(filePos,static_cast<stream_t>(streamKind), streamPos, QString2wstring(field), Info_Measure_Text));
                        QString B=wstring2QString(C->Get(filePos,static_cast<stream_t>(streamKind), streamPos, QString2wstring(field), Info_Name_Text));
                        if (B.isEmpty())
                            B=wstring2QString(C->Get(filePos,static_cast<stream_t>(streamKind), streamPos, QString2wstring(field), Info_Name));
                        QStringList sl = QStringList(B);
                        sl.append(A);
                        node->addChild(new QTreeWidgetItem(node,sl));
                    }
                }
            }
        }
    }
    if(C->Count_Get()<=1)
        treeWidget->expandAll();
    treeWidget->resizeColumnToContents(0);
    return treeWidget;
}

QString coreinfo::shortName(QDir d, QString name)
{
    //Elminating unuseful info from filenames
    return d.relativeFilePath(name);
}
