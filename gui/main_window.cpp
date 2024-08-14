#include "main_window.h"
#if !SARIBBON_USE_3RDPARTY_FRAMELESSHELPER
#include "SAFramelessHelper.h"
#endif
#include "SARibbonApplicationButton.h"
#include "SARibbonBar.h"
#include "SARibbonButtonGroupWidget.h"
#include "SARibbonCategory.h"
#include "SARibbonCustomizeWidget.h"
#include "SARibbonGallery.h"
#include "SARibbonMenu.h"
#include "SARibbonQuickAccessBar.h"
#include "SARibbonSystemButtonBar.h"
#include <QAbstractButton>
#include <QAction>
#include <QApplication>
#include <QButtonGroup>
#include <QCalendarWidget>
#include <QElapsedTimer>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QStatusBar>
#include <QTextStream>
#include <QXmlStreamWriter>
#include <QShortcut>
#include <QDockWidget>

//
#include "main_windwo_setting_dialog_view_ctrl.h"
#include "gallery_main_window.h"
#include "desktop_gallery_view_ctrl.h"


MainWindow::MainWindow(QWidget* par)
    : SARibbonMainWindow(par), mWidgetForCustomize(nullptr), mMenuApplicationBtn(nullptr)
{
	setWindowTitle(("ribbon mainwindow test"));
	Initial();
	setMinimumWidth(500);
	setWindowIcon(QIcon(":/icon/icon/SA.svg"));
	showMaximized();
}

void MainWindow::createRibbonApplicationButton()
{
	SARibbonBar* ribbon = ribbonBar();
	if (!ribbon) {
		return;
	}
	QAbstractButton* btn = ribbon->applicationButton();
	if (!btn) {
		// cn: SARibbonBar默认就会创建一个SARibbonApplicationButton，因此，在正常情况下，这个位置不会进入
		// en: SARibbonBar creates a SARibbonApplicationButton by default. Therefore, under normal circumstances, this location will not enter
		btn = new SARibbonApplicationButton(this);
		ribbon->setApplicationButton(btn);
	}
	ribbon->applicationButton()->setText(("  &File  "));  // 文字两边留有间距，好看一点
	// cn: SARibbonMenu和QMenu的操作是一样的
	// en: The operations of SARibbonMenu and QMenu are the same
	if (!mMenuApplicationBtn) {
		mMenuApplicationBtn = new SARibbonMenu(this);
		mMenuApplicationBtn->addAction(createAction("test1", ":/icon/icon/action.svg"));
		mMenuApplicationBtn->addAction(createAction("test2", ":/icon/icon/action2.svg"));
		mMenuApplicationBtn->addAction(createAction("test3", ":/icon/icon/action3.svg"));
		mMenuApplicationBtn->addAction(createAction("test4", ":/icon/icon/action4.svg"));
	}
	SARibbonApplicationButton* appBtn = qobject_cast< SARibbonApplicationButton* >(btn);
	if (!appBtn) {
		return;
	}
	appBtn->setMenu(mMenuApplicationBtn);
}

QAction* MainWindow::createAction(const QString& text, const QString& iconurl)
{
	QAction* act = new QAction(this);
	act->setText(text);
	act->setIcon(QIcon(iconurl));
	act->setObjectName(text);
	return act;
}

void MainWindow::AddQDockWidget()
{
    workbench();
    // 创建第一个侧边栏
    QDockWidget *leftDock1 = new QDockWidget("Left Dock 1", this);
    this->addDockWidget(Qt::LeftDockWidgetArea, leftDock1);

	// []
	QWidget *leftDockWidget1 = new QWidget(leftDock1);
	leftDock1->setWidget(leftDockWidget1);

    // 创建第二个侧边栏
    QDockWidget *leftDock2 = new QDockWidget("Left Dock 2", this);
    QWidget *leftDockWidget2 = new QWidget(leftDock2);
    leftDock2->setWidget(leftDockWidget2);
    this->addDockWidget(Qt::LeftDockWidgetArea, leftDock2);

    // 确保两个侧边栏竖直排列
    this->splitDockWidget(leftDock1, leftDock2, Qt::Vertical);
}

void MainWindow::workbench()
{
    // 创建中央窗口
       QWidget *centralWidget = new QWidget(this);
       QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
       centralWidget->setLayout(mainLayout);

       QHBoxLayout *hbox = new QHBoxLayout;
       QWidget *workArea = new QWidget(centralWidget);
       this->setCentralWidget(centralWidget);
}


void MainWindow::Initial()
{
	setStatusBar(new QStatusBar());
	AddQDockWidget();
	createQuickAccessBar();
	createCategoryMain();
	createRibbonApplicationButton();
}


void MainWindow::createQuickAccessBar()
{
	SARibbonQuickAccessBar* quickAccessBar = ribbonBar()->quickAccessBar();
	QAction* actionCustomizeAndSaveWithApply = createAction("customize and save with apply", ":/icon/icon/customize.svg");
	quickAccessBar->addAction(actionCustomizeAndSaveWithApply);
	connect(actionCustomizeAndSaveWithApply, &QAction::triggered, [&] (bool checked)->void {
		MainWindowSettingDialogViewCtrl::GetIns().Initial(this);
	});

}

void MainWindow::createCategoryMain()
{
	//    AddQDockWidget();
	SARibbonBar* ribbon = ribbonBar();
	//! 通过setContentsMargins设置ribbon四周的间距
	ribbon->setContentsMargins(5, 0, 5, 0);
	GalleryMainWindow::GetInst().Initial(ribbon);
	DesktopGalleryViewCtrl::GetInst().Initial(ribbon);
}

