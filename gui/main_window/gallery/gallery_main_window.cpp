//
// Created by zhupi on 2024/8/10.
//

#include "gallery_main_window.h"
#include "SARibbonTabBar.h"

void GalleryMainWindow::Initial(SARibbonBar* ribbon)
{
	SARibbonCategory* categoryMain = ribbon->addCategoryPage(tr("Main"));
//	SARibbonCategory* categoryMain = ribbon->addCategoryPage(tr("&Main"));
	categoryMain->setObjectName(("categoryMain"));
	InitMain(*categoryMain);
}
void GalleryMainWindow::InitMain(SARibbonCategory& page)
{
	SARibbonPannel* pannel1 = page.addPannel(("Panel 1"));
	QAction* actSave = new QAction(this);
	actSave->setText("save");
	actSave->setIcon(QIcon(":/icon/icon/save.svg"));
	actSave->setObjectName("actSave");
	actSave->setShortcut(QKeySequence(QLatin1String("Ctrl+S")));
	pannel1->addLargeAction(actSave);

	actSave = new QAction(this);
	actSave->setText("save");
	actSave->setIcon(QIcon(":/icon/icon/save.svg"));
	actSave->setObjectName("actSave");
	actSave->setShortcut(QKeySequence(QLatin1String("Ctrl+S")));
	pannel1->addMediumAction(actSave);

	actSave = new QAction(this);
	actSave->setText("save");
	actSave->setIcon(QIcon(":/icon/icon/save.svg"));
	actSave->setObjectName("actSave");
	actSave->setShortcut(QKeySequence(QLatin1String("Ctrl+S")));
	pannel1->addMediumAction(actSave);
}
GalleryMainWindow& GalleryMainWindow::GetInst()
{
	static GalleryMainWindow ins;
	return ins;
}
GalleryMainWindow::GalleryMainWindow(){ }
