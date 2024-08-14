//
// Created by zhupi on 2024/8/10.
//

#include <iostream>

#include "main_windwo_setting_dialog_view_ctrl.h"
#include "SARibbonGlobal.h"
MainWindowSettingDialogViewCtrl& MainWindowSettingDialogViewCtrl::GetIns()
{
	static MainWindowSettingDialogViewCtrl ins;
	return ins;
}

void MainWindowSettingDialogViewCtrl::Initial(MainWindow * mainWindow)
{
	mainWindow_ = mainWindow;
	if (settingDialog_ != nullptr) {
		delete settingDialog_;
		settingDialog_ = nullptr;
	}
	std::cout<< " ---------------------111111111111111111111---------- ";
	settingDialog_ = new MainWindowSettingDialog("setting", mainWindow);
	settingDialog_->Initial();
	InitConnect();
	settingDialog_->exec();
}

MainWindowSettingDialogViewCtrl::MainWindowSettingDialogViewCtrl() { }

void MainWindowSettingDialogViewCtrl::InitConnect()
{
	connect(settingDialog_, &MainWindowSettingDialog::CurrentIndexChangedSg,
	        this, &MainWindowSettingDialogViewCtrl::ThemeComboBoxCurrentIndexChanged);
}

void MainWindowSettingDialogViewCtrl::ThemeComboBoxCurrentIndexChanged(int index)
{
	if (mainWindow_ == nullptr) {
		// []
		return;
	}
	std::cout << " ------------------------------";
	HWWindowTheme t = static_cast< HWWindowTheme >(index);
	mainWindow_->setRibbonTheme(static_cast<SARibbonTheme>(t));
}