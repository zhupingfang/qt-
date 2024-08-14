//
// Created by zhupi on 2024/8/11.
//

#ifndef VIEW_GALLERY_VIEW_CTRL_H
#define VIEW_GALLERY_VIEW_CTRL_H

#include <QObject>
#include "SARibbonBar.h"

class ViewGalleryViewCtrl : public QObject
{
	Q_OBJECT
public:
	static ViewGalleryViewCtrl &GetInst();
	~ViewGalleryViewCtrl() {}
	void Initial(SARibbonBar* ribbon);
private:
	ViewGalleryViewCtrl() {}
	void InitBase(SARibbonCategory& page);

private:
	QAction* actCut_ {nullptr};
	QAction* actCopy_ {nullptr};
	QAction* actPast_ {nullptr};
	QAction* actUndo_ {nullptr};
	QAction* actRedo_ {nullptr};
	QAction* actDelete_ {nullptr};
};

#endif  // VIEW_GALLERY_VIEW_CTRL_H
