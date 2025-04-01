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

#include <QtCore/qloggingcategory.h>
Q_LOGGING_CATEGORY(trafalgar, "qtrafalgar")

static int debug_level = 3;

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
	QByteArray localMsg = msg.toLocal8Bit();

	switch (type)
	{
	case QtDebugMsg:
		if(debug_level >= 7)
			fprintf(stdout, "Message:  %s \n", localMsg.constData());
		break;

	case QtInfoMsg:
		if(debug_level >= 3)
			fprintf(stdout, "Info:     %s \n", localMsg.constData());
		break;

	case QtWarningMsg:
		if(debug_level >= 2)
			fprintf(stderr, " Warning: %s (%s:%u, %s)\n", localMsg.constData(),
				context.file, context.line, context.function);
		break;

	case QtCriticalMsg:
		if(debug_level >= 1)
			fprintf(stderr, " Error: : %s (%s:%u, %s)\n", localMsg.constData(),
				context.file, context.line, context.function);
		break;

	case QtFatalMsg:
		fprintf(stderr, "Fatal: %s (%s:%u, %s)\n", localMsg.constData(),
			context.file, context.line, context.function);
		abort();
	}
}


int main(int argc, char *argv[])
{
	qInstallMessageHandler(myMessageOutput);

	QApplication a(argc, argv);

	QCoreApplication::setOrganizationName("Trafalgar");
	QCoreApplication::setApplicationName("QTrafalgar");
	QCommandLineParser parser;
	parser.setApplicationDescription(QCoreApplication::applicationName());
	parser.addHelpOption();

	// A integer option with the verbose mode (-v, --verbose)
	QCommandLineOption verboseOption(QStringList() << "v" << "verbose",
				QCoreApplication::translate("tr_view", "Verbose mode"),
				QCoreApplication::translate("tr_view", "level"));
        parser.addOption(verboseOption);

	// An string option with the file name of the network file (-f, --file)
	QCommandLineOption fileOption(QStringList() << "f" << "file",
				QCoreApplication::translate("QTrafalgar", "data network file"),
				QCoreApplication::translate("QTrafalgar", "data file"));
	parser.addOption(fileOption);

	parser.process(a);

	MainWindow w;
	QString tr_file;

	if(parser.isSet(verboseOption))
	{
		int verbose = parser.value(verboseOption).toInt();
		if(verbose)
			debug_level = verbose;
		else
			fprintf(stderr, "ERROR: verbose option error '%s' (%i)\n",
		parser.value(verboseOption).toLatin1().constData(), debug_level);
	}

	if(parser.isSet(fileOption))
	{
		tr_file  = parser.value(fileOption);
		w.loadFile(tr_file);
	}

	w.show();
	return a.exec();
}
