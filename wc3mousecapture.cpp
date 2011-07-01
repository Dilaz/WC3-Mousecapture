#include "wc3mousecapture.h"

WC3MouseCapture::WC3MouseCapture(QWidget *parent)
: QWidget(parent), m_icon(QIcon(ICON_NAME), this), m_timer(this), m_menu(this),
m_exitAction(EXIT_ACTION_TITLE, this), m_isDisabled(false) {
	// Set up trayicon
	m_icon.show();

	// Hide widget
	//QTimer::singleShot(0, this, SLOT(hide()));

	// Set up menu
	m_menu.addAction(&m_exitAction);
	m_icon.setContextMenu(&m_menu);

	// Save default mouse rect
	GetClipCursor(&m_defaultRect);

	// Connect signals & slots
	QObject::connect(&m_timer, SIGNAL(timeout()), this, SLOT(checkActiveWindow()));
	QObject::connect(this, SIGNAL(wc3Activated(HWND&)), this, SLOT(captureMouse(HWND&)));
	QObject::connect(this, SIGNAL(wc3Deactivated()), this, SLOT(endCapture()));
	QObject::connect(&m_exitAction, SIGNAL(activated()), this, SLOT(close()));

	// Start the timer
	m_timer.start(DEFAULT_TIMEOUT);

	// Register hotkey
	RegisterHotKey(QWidget::winId(), NULL, NULL, DEFAULT_HOTKEY);
}

WC3MouseCapture::~WC3MouseCapture() {
	// Stop the timer
	m_timer.stop();

	// Reset mouse rect
	endCapture();

	// Delete context menu
	m_menu.deleteLater();
}

bool WC3MouseCapture::winEvent(MSG *message, long *result) {
	if (message->message == WM_HOTKEY) {
		endCapture();
		m_isDisabled = true;
		return false;
	}

	return QWidget::winEvent(message, result);
}

void WC3MouseCapture::captureMouse(HWND &hWnd) {
	// Check if capture is disabled
	if (m_isDisabled) {
		return;
	}

	// Get WC3 window & client rect
	RECT windowRect, clientRect;
	GetWindowRect(hWnd, &windowRect);
	GetClientRect(hWnd, &clientRect);

	// Calculate border size
	int borderSize = (windowRect.right - windowRect.left - clientRect.right) / 2;

	// Calculate titlebar size
	int titlebarSize = windowRect.bottom - windowRect.top - clientRect.bottom - borderSize;

	// Calculate new coordinates
	windowRect.top += titlebarSize;
	windowRect.left += borderSize;
	windowRect.bottom -= borderSize;
	windowRect.right -= borderSize;
	
	// Clip cursor to wc3 window
	ClipCursor(&windowRect);
}

void WC3MouseCapture::endCapture() {
	// End disable
	m_isDisabled = false;

	// Restore mouse rect to default
	ClipCursor(&m_defaultRect);
}

void WC3MouseCapture::checkActiveWindow() {
	// Find WC3 window
	HWND hWnd = FindWindowA(NULL, reinterpret_cast<LPCSTR>(WC3_WINDOW_NAME));

	// Check if WC3 is open
	if (hWnd == NULL) {
		return;
	}

	// Check if WC3 window is active
	if (hWnd == GetForegroundWindow()) {
		emit wc3Activated(hWnd);
	}
	else {
		emit wc3Deactivated();
	}
}
