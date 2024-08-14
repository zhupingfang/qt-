//
// Created by zhupi on 2024/8/10.
//

#ifndef MAIN_WINDOW_SETTING_DIALOG_VIEW_CTRL_H
#define MAIN_WINDOW_SETTING_DIALOG_VIEW_CTRL_H

#include <QObject>
#include "main_windwo_setting_dialog.h"
#include "main_window.h"

class MainWindowSettingDialogViewCtrl : public QObject
{
	Q_OBJECT
public:
	static MainWindowSettingDialogViewCtrl &GetIns();

	void Initial(MainWindow * mainWindow);
private:
	explicit MainWindowSettingDialogViewCtrl();
	void InitConnect();

	// slot
	void ThemeComboBoxCurrentIndexChanged(int index);

private:
	MainWindow *mainWindow_ {nullptr};
	MainWindowSettingDialog *settingDialog_ {nullptr};

};

#endif  // MAIN_WINDOW_SETTING_DIALOG_VIEW_CTRL_H
