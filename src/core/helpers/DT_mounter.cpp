﻿#include "DT_mounter.h"
#include "messagebox.h"
#include "QApplicationSettings.h"
#include <QProcess>

void DT_mounter::mountImage(QString path)
{
#ifdef Q_WS_WIN //file association for windows
	QApplicationSettings* settings = QApplicationSettings::getInstance();
	QString exe = settings->valueString("DT", "Executable");

	if(exe.isEmpty())
	{
		QApplicationSettings::FreeInstance();
		CustomMessageBox::warning(nullptr, "DT Mounter", qApp->translate("QTorrentDisplayModel","DT_PATH_NOT_SET"));
		return;
	}

	bool useCustomCmd = settings->valueBool("DT", "UseCustomCommand");
	int driveNum = settings->valueInt("DT", "Drive");
	QString command = useCustomCmd ?  settings->valueString("DT", "CustomtCommand") : settings->valueString("DT", "DefaultCommand");
	QProcess* dt = new QProcess;
	QStringList args;
	dt->setNativeArguments(command.arg(QString::number(driveNum)).arg(path));
	dt->start(exe, args);
	QApplicationSettings::FreeInstance();

	if(!dt->waitForStarted(5000))
	{
		CustomMessageBox::warning(nullptr, "DT Mounter", qApp->translate("QTorrentDisplayModel", "LAUNCH_ERROR") + exe);
		return;
	}

	dt->waitForFinished();
	delete dt;
#endif
}