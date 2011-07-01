#ifndef WC3MOUSECAPTURE_H
#define WC3MOUSECAPTURE_H

#include <QWidget>
#include <QSystemTrayIcon>
#include <QMessageBox>
#include <QTimer>
#include <QMenu>

#include <windows.h>

// Constants
static const char WC3_WINDOW_NAME[] = "Warcraft III";
static const quint16 DEFAULT_TIMEOUT(250);
static const QString ICON_NAME("mouse.png");
static const QString EXIT_ACTION_TITLE("E&xit");
static const UINT DEFAULT_HOTKEY = VK_F6;

class WC3MouseCapture : public QWidget {
	Q_OBJECT

public:
	WC3MouseCapture(QWidget *parent = 0);
	~WC3MouseCapture();

private:
	QSystemTrayIcon m_icon;
	QTimer m_timer;
	QMenu m_menu;
	QAction m_exitAction;
	RECT m_defaultRect;
	bool m_isDisabled;

protected:
	bool winEvent(MSG *message, long *result);

private slots:
	void checkActiveWindow();
	void captureMouse(HWND &hWnd);
	void endCapture();

signals:
	void wc3Activated(HWND &hWnd);
	void wc3Deactivated();
};

#endif // WC3MOUSECAPTURE_H
