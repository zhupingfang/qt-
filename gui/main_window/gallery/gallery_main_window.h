//
// Created by zhupi on 2024/8/10.
//

#ifndef GALLERY_MAIN_WINDOW_H
#define GALLERY_MAIN_WINDOW_H

#include <QObject>
#include "SARibbonBar.h"
class GalleryMainWindow : public QObject
{
	Q_OBJECT
public:
	static GalleryMainWindow &GetInst();
	~GalleryMainWindow() {}
	void Initial(SARibbonBar* ribbon);
private:
	GalleryMainWindow();
	void InitMain(SARibbonCategory& page);
};

#endif  // GALLERY_MAIN_WINDOW_H
