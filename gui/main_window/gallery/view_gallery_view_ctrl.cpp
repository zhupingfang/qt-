//
// Created by zhupi on 2024/8/11.
//

#include "view_gallery_view_ctrl.h"
ViewGalleryViewCtrl& ViewGalleryViewCtrl::GetInst()
{
	static ViewGalleryViewCtrl ins;
	return ins;
}
void ViewGalleryViewCtrl::Initial(SARibbonBar* ribbon)
{
	SARibbonCategory* categoryMain = ribbon->addCategoryPage(tr("view"));
	categoryMain->setObjectName(("view"));
	InitBase(*categoryMain);
}
void ViewGalleryViewCtrl::InitBase(SARibbonCategory& page)
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
