//
// Created by zhupi on 2024/8/10.
//

#include "main_windwo_setting_dialog.h"

namespace {
const int RETURN_ERROR = -1;
const int MINI_WIDGET = 640;
const int MINI_HEIGTH = 480;
}

MainWindowSettingDialog::MainWindowSettingDialog(const QString& title, QWidget* parent)
	: QDialog(parent)
{
	this->setWindowTitle(title);
	this->setMinimumSize(MINI_WIDGET, MINI_HEIGTH);
}

void MainWindowSettingDialog::Initial()
{
	InitWidget();
}


void MainWindowSettingDialog::InitWidget()
{
	mComboboxRibbonTheme_ = new QComboBox(this);
	mComboboxRibbonTheme_->setWindowTitle(tr("RibbonTheme"));
	mComboboxRibbonTheme_->setObjectName("RibbonTheme");
	mComboboxRibbonTheme_->addItem("Theme Win7", static_cast< int >(HWWindowTheme::RibbonThemeWindows7));
	mComboboxRibbonTheme_->addItem("Theme Office2013", static_cast< int >(HWWindowTheme::RibbonThemeOffice2013));
	mComboboxRibbonTheme_->addItem("Theme Office2016 Blue", static_cast< int >(HWWindowTheme::RibbonThemeOffice2016Blue));
	mComboboxRibbonTheme_->addItem("Theme Office2021 Blue", static_cast< int >(HWWindowTheme::RibbonThemeOffice2021Blue));
	mComboboxRibbonTheme_->addItem("Theme Dark", static_cast< int >(HWWindowTheme::RibbonThemeDark));
	mComboboxRibbonTheme_->addItem("Theme Dark2", static_cast< int >(HWWindowTheme::RibbonThemeDark2));
	connect(mComboboxRibbonTheme_, QOverload< int >::of(&QComboBox::currentIndexChanged), this,&MainWindowSettingDialog::CurrentIndexChangedSg);
}
void MainWindowSettingDialog::SetCurrnetTheme(const QString& theme)
{
	if (mComboboxRibbonTheme_ == nullptr) {
		// []
		return;
	}
	int ret = mComboboxRibbonTheme_->findData(theme);
	if (ret != RETURN_ERROR) {
		mComboboxRibbonTheme_->setCurrentText(theme);
	}
}
