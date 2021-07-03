#pragma once

#include <QCheckBox>
#include <QDockWidget>
#include <QSlider>

#include "obs.h"
#include <../UI/obs-frontend-api/obs-frontend-api.h>

#include "media-control.hpp"
#include "obs.hpp"
#include "qt-display.hpp"
#include "volume-meter.hpp"


typedef std::function<bool(QObject *, QEvent *)> EventFilterFunc;

class OBSEventFilter : public QObject {
	Q_OBJECT
public:
	OBSEventFilter(EventFilterFunc filter_) : filter(filter_) {}

protected:
	bool eventFilter(QObject *obj, QEvent *event)
	{
		return filter(obj, event);
	}

public:
	EventFilterFunc filter;
};

class LockedCheckBox : public QCheckBox {
	Q_OBJECT

public:
	LockedCheckBox();
	explicit LockedCheckBox(QWidget *parent);
};

class MuteCheckBox : public QCheckBox {
	Q_OBJECT
};

class SliderIgnoreScroll : public QSlider {
	Q_OBJECT

public:
	SliderIgnoreScroll(QWidget *parent = nullptr);
	SliderIgnoreScroll(Qt::Orientation orientation,
			   QWidget *parent = nullptr);

protected:
	virtual void wheelEvent(QWheelEvent *event) override;
};


class SourceDock : public QDockWidget {
	Q_OBJECT





private:
	OBSSource source;
	std::unique_ptr<OBSEventFilter> eventFilter;

	
	OBSQTDisplay *preview;
	VolumeMeter *volMeter;
	obs_volmeter_t *obs_volmeter;
	LockedCheckBox * locked;
	SliderIgnoreScroll * slider;
	MuteCheckBox * mute;
	MediaControl * mediaControl;

	static void DrawPreview(void *data, uint32_t cx, uint32_t cy);
	
	static void OBSVolumeLevel(void *data,
				   const float magnitude[MAX_AUDIO_CHANNELS],
				   const float peak[MAX_AUDIO_CHANNELS],
				   const float inputPeak[MAX_AUDIO_CHANNELS]);
	static void OBSVolume(void *data, calldata_t *calldata);
	static void OBSMute(void *data, calldata_t *calldata);

	bool GetSourceRelativeXY(int mouseX, int mouseY, int &x, int &y);

	bool HandleMouseClickEvent(QMouseEvent *event);
	bool HandleMouseMoveEvent(QMouseEvent *event);
	bool HandleMouseWheelEvent(QWheelEvent *event);
	bool HandleFocusEvent(QFocusEvent *event);
	bool HandleKeyEvent(QKeyEvent *event);

	OBSEventFilter *BuildEventFilter();

private slots:
	void LockVolumeControl(bool lock);
	void MuteVolumeControl(bool mute);
	void SliderChanged(int vol);
	void SetOutputVolume(double volume);
	void SetMute(bool muted);
public:
	SourceDock(OBSSource source, QWidget *parent = nullptr);
	~SourceDock();
	void Rename(std::string new_name);
};

