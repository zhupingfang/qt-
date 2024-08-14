//
// Created by zhupi on 2024/8/11.
//

#include "desktop_gallery_view_ctrl.h"
DesktopGalleryViewCtrl& DesktopGalleryViewCtrl::GetInst()
{
	static DesktopGalleryViewCtrl ins;
	return ins;
}

void DesktopGalleryViewCtrl::Initial(SARibbonBar* ribbon)
{
	SARibbonCategory* categoryMain = ribbon->addCategoryPage(tr("desktop"));
	//	SARibbonCategory* categoryMain = ribbon->addCategoryPage(tr("&Main"));
	categoryMain->setObjectName(("desktop"));
	InitBase(*categoryMain);
	InitFile(*categoryMain);
}


void DesktopGalleryViewCtrl::InitMain(SARibbonCategory& page)
{

}
void DesktopGalleryViewCtrl::InitBase(SARibbonCategory& page)
{
	SARibbonPannel* pannel1 = page.addPannel(("Cut"));
	actCut_ = new QAction(this);
	actCut_->setText("save");
	actCut_->setIcon(QIcon(":/icon/icon/save.svg"));
	actCut_->setObjectName("actCut_");
	actCut_->setShortcut(QKeySequence(QLatin1String("Ctrl+S")));
	pannel1->addSmallAction(actCut_);

	actCopy_ = new QAction(this);
	actCopy_->setText("Copy");
	actCopy_->setIcon(QIcon(":/icon/icon/save.svg"));
	actCopy_->setObjectName("actCopy_");
	actCopy_->setShortcut(QKeySequence(QLatin1String("Ctrl+c")));
	pannel1->addSmallAction(actCopy_);

	actUndo_ = new QAction(this);
	actUndo_->setText("Undo");
	actUndo_->setIcon(QIcon(":/icon/icon/save.svg"));
	actUndo_->setObjectName("actUndo_");
	actUndo_->setShortcut(QKeySequence(QLatin1String("Ctrl+S")));
	pannel1->addSmallAction(actUndo_);
	// actPast_
	actPast_ = new QAction(this);
	actPast_->setText("Past");
	actPast_->setIcon(QIcon(":/icon/icon/save.svg"));
	actPast_->setObjectName("actPast_");
	actPast_->setShortcut(QKeySequence(QLatin1String("Ctrl+S")));
	pannel1->addSmallAction(actPast_);

	actRedo_ = new QAction(this);
	actRedo_->setText("Redo");
	actRedo_->setIcon(QIcon(":/icon/icon/save.svg"));
	actRedo_->setObjectName("actRedo_");
	actRedo_->setShortcut(QKeySequence(QLatin1String("Ctrl+S")));
	pannel1->addSmallAction(actRedo_);

	actDelete_ = new QAction(this);
	actDelete_->setText("Delete");
	actDelete_->setIcon(QIcon(":/icon/icon/save.svg"));
	actDelete_->setObjectName("actDelete_");
	actDelete_->setShortcut(QKeySequence(QLatin1String("Ctrl+S")));
	pannel1->addSmallAction(actDelete_);
}
void DesktopGalleryViewCtrl::InitFile(SARibbonCategory& page)
{
	SARibbonPannel* pannel1 = page.addPannel(("New"));
	actCut_ = new QAction(this);
	actCut_->setText("save");
	actCut_->setIcon(QIcon(":/icon/icon/save.svg"));
	actCut_->setObjectName("actCut_");
	actCut_->setShortcut(QKeySequence(QLatin1String("Ctrl+S")));
	pannel1->addLargeAction(actCut_);

	actOpne_ = new QAction(this);
	actOpne_->setText("Opne");
	actOpne_->setIcon(QIcon(":/icon/icon/save.svg"));
	actOpne_->setObjectName("actOpne_");
	actOpne_->setShortcut(QKeySequence(QLatin1String("Ctrl+S")));
	pannel1->addLargeAction(actOpne_);

	actOpenExample_ = new QAction(this);
	actOpenExample_->setText("Opne\nExample");
	actOpenExample_->setIcon(QIcon(":/icon/icon/save.svg"));
	actOpenExample_->setObjectName("actOpenExample_");
	actOpenExample_->setShortcut(QKeySequence(QLatin1String("Ctrl+S")));
	pannel1->addLargeAction(actOpenExample_);

	actSave_ = new QAction(this);
	actSave_->setText("Save");
	actSave_->setIcon(QIcon(":/icon/icon/save.svg"));
	actSave_->setObjectName("actSave_");
	actSave_->setShortcut(QKeySequence(QLatin1String("Ctrl+S")));
	pannel1->addLargeAction(actSave_);

	actSaveAs_ = new QAction(this);
	actSaveAs_->setText("Save As");
	actSaveAs_->setIcon(QIcon(":/icon/icon/save.svg"));
	actSaveAs_->setObjectName("actSaveAs_");
	actSaveAs_->setShortcut(QKeySequence(QLatin1String("Ctrl+S")));
	pannel1->addLargeAction(actSaveAs_);

	// actClose_
	actClose_ = new QAction(this);
	actClose_->setText("Close");
	actClose_->setIcon(QIcon(":/icon/icon/save.svg"));
	actClose_->setObjectName("actClose_");
	actClose_->setShortcut(QKeySequence(QLatin1String("Ctrl")));
	pannel1->addLargeAction(actClose_);
}
