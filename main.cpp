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

#include <QApplication>
#include <QCommandLineParser>


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setAttribute(Qt::AA_EnableHighDpiScaling);
    app.setQuitOnLastWindowClosed(true);

    // Set application info
    app.setOrganizationName("CoreBox");
    app.setApplicationName("CoreInfo");

    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addVersionOption();

    const QString files = "[FILE1, FILE2,...]";
    parser.addPositionalArgument("files", files, files);

    parser.process(app);

    QStringList args = parser.positionalArguments();

    QStringList paths;
    foreach (QString arg, args) {
      QFileInfo info(arg);
      paths.push_back(info.absoluteFilePath());
    }
    coreinfo w;
    if (paths.count()) {
        w.openFiles(paths);
    }
    w.show();

    return app.exec();
}
