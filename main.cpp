/******************************************************************
 * project:	OSM Traffic
 *
 * (C)		Schmid Hubert 2024
 ******************************************************************/

/*
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see
 * <https://www.gnu.org/licenses/>.
 */

#include "mainwindow.h"

#include <QApplication>
#include <QCommandLineParser>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::setOrganizationName("Trafalgar");
    QCoreApplication::setApplicationName("QTrafalgar");
    QCommandLineParser parser;
    parser.setApplicationDescription(QCoreApplication::applicationName());
    parser.addHelpOption();

    // An string option with the file name of the network file (-f, --file)
    QCommandLineOption fileOption(QStringList() << "f" << "file",
        QCoreApplication::translate("QTrafalgar", "data network file"),
        QCoreApplication::translate("QTrafalgar", "data file"));
        parser.addOption(fileOption);

    parser.process(a);

    MainWindow w;
    QString tr_file;

    if(parser.isSet(fileOption))
    {
        tr_file  = parser.value(fileOption);
        w.loadFile(tr_file);
    }

    w.show();
    return a.exec();
}
