//
// Created by zhupi on 2024/8/10.
//

#ifndef MAIN_WINDOW_SETTING_DIALOG_H
#define MAIN_WINDOW_SETTING_DIALOG_H

#include <QDialog>
#include <QComboBox>
#include "gui_commit.h"

class MainWindowSettingDialog : public QDialog
{
	Q_OBJECT
public:
	MainWindowSettingDialog(const QString &title, QWidget *parent);
	void Initial();
	void SetCurrnetTheme(const QString &theme);

signals:
	    void CurrentIndexChangedSg(int index);
private:
	void InitWidget();

private:
	QComboBox* mComboboxRibbonTheme_ {nullptr};
};
#endif  // MAIN_WINDOW_SETTING_DIALOG_H
