#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H
#include "SARibbonMainWindow.h"
class SARibbonCategory;
class SARibbonContextCategory;
class SARibbonCustomizeWidget;
class SARibbonActionsManager;
class SARibbonQuickAccessBar;
class SARibbonButtonGroupWidget;
class SARibbonPannel;
class QTextEdit;
class QComboBox;
class QCloseEvent;
class QLineEdit;

#include <iostream>

#include <QPlainTextEdit>
#include <QKeyEvent>
#include <QDebug>

class CommandPrompt : public QPlainTextEdit {
    Q_OBJECT

public:
    CommandPrompt(QWidget *parent = nullptr)
          : QPlainTextEdit(parent), prompt(">>> ")
    {
        insertPrompt();
        setReadOnly(false);
    }

protected:
    void keyPressEvent(QKeyEvent *event) override
    {
        if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
            QTextCursor cursor = textCursor();
            cursor.movePosition(QTextCursor::End, QTextCursor::MoveAnchor);
            setTextCursor(cursor);

            // 获取当前行的文本
            cursor.select(QTextCursor::LineUnderCursor);
            QString command = cursor.selectedText().mid(prompt.length());

            // 在这里可以处理命令
            qDebug() << "Command entered:" << command;

            // 插入新的提示符
            insertPlainText("\n");
            insertPrompt();

            return;
        }

        QPlainTextEdit::keyPressEvent(event);
    }

private:
    QString prompt;
    void insertPrompt()
    {
        insertPlainText(prompt);
        QTextCursor cursor = textCursor();
        cursor.movePosition(QTextCursor::End, QTextCursor::MoveAnchor);
        setTextCursor(cursor);
    }
};

class MainWindow : public SARibbonMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget* par = nullptr);
	void Initial();


private:
	// 创建ribbon-application菜单示例
	void createRibbonApplicationButton();
	QAction* createAction(const QString& text, const QString& iconurl);
	// 创建顶部的工具按钮
	void createQuickAccessBar();
	// 创建main菜单示例
	void createCategoryMain();
	// []
	void AddQDockWidget();
	void workbench();

private:
	SARibbonCustomizeWidget* mWidgetForCustomize;
	QMenu* mMenuApplicationBtn;

    //
    QAction* mActionVisibleAll { nullptr };    ///< 控制所有action是否可见的开关
    bool mHasApplyCustomizeXmlFile { false };  /// 程序启动后是否应用上次修改
};

#endif  // MAIN_WINDOW_H
