//
// Created by zhupi on 2024/8/11.
//

#ifndef DESKTOP_GALLERY_VIEW_CTRL_H
#define DESKTOP_GALLERY_VIEW_CTRL_H

#include <QObject>
#include "SARibbonBar.h"

class DesktopGalleryViewCtrl : QObject
{
	Q_OBJECT
public:
	static DesktopGalleryViewCtrl &GetInst();
	~DesktopGalleryViewCtrl() {}
	void Initial(SARibbonBar* ribbon);
private:
	DesktopGalleryViewCtrl() {}
	void InitMain(SARibbonCategory& page);
	void InitBase(SARibbonCategory& page);
	void InitFile(SARibbonCategory& page);


private:
	QAction* actCut_ {nullptr};
	QAction* actCopy_ {nullptr};
	QAction* actPast_ {nullptr};
	QAction* actUndo_ {nullptr};
	QAction* actRedo_ {nullptr};
	QAction* actDelete_ {nullptr};

	//
	QAction* actNew_ {};
	QAction* actOpne_ {};
	QAction* actOpenExample_ {};
	QAction* actSave_ {};
	QAction* actSaveAs_ {};
	QAction* actClose_ {};

	//
	QAction* actImport_ {};
	QAction* actExport_ {};

	//
	QAction* actUnit_ {};
	QAction* actOpenScraipt_ {};
	QAction* actSaveScript_ {};
	QAction* actVariable_ {};
	QAction* actMaterial_ {};

};

#endif  // DESKTOP_GALLERY_VIEW_CTRL_H
