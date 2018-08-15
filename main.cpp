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

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

	QStringList files;
    files << "/home/cosmos/Multimedia/Music/Classics/Ek.Pyar.Ka.Nagma.Hai-Lata.Mangeshkar.mp3";
    files << "/home/cosmos/Multimedia/Music/Classics/AAA.My.Name.Is.mp3";
    files << "/home/cosmos/Multimedia/Music/Classics/Ek.Main.Aur.Ek.Tu.mp3";
    files << "/home/cosmos/Multimedia/Music/Classics/Naam.Gum.Jayega.mp3";
    files << "/home/cosmos/Multimedia/Music/Classics/Yeh.Dil.Na.Hota.Bechara.ogg";

    coreinfo *w = new coreinfo( files );
    w->show();

    return a.exec();
}
