#pragma once

#include <QCheckBox>
#include <algorithm>
#include <memory>
#include <set>
#include <vector>

#include "common/prefix.h"
#include "tools/cabana/streams/abstractstream.h"
#include "tools/replay/replay.h"

class ReplayStream : public AbstractStream {
  Q_OBJECT

public:
  ReplayStream(QObject *parent);
  void start() override;
  bool loadRoute(const QString &route, const QString &data_dir, uint32_t replay_flags = REPLAY_FLAG_NONE);
  bool eventFilter(const Event *event);
  void seekTo(double ts) override { replay->seekTo(std::max(double(0), ts), false); }
  bool liveStreaming() const override { return false; }
  inline QString routeName() const override { return replay->route()->name(); }
  inline QString carFingerprint() const override { return replay->carFingerprint().c_str(); }
  double minSeconds() const override { return replay->minSeconds(); }
  double maxSeconds() const { return replay->maxSeconds(); }
  inline QDateTime beginDateTime() const { return replay->routeDateTime(); }
  inline uint64_t beginMonoTime() const override { return replay->routeStartNanos(); }
  inline void setSpeed(float speed) override { replay->setSpeed(speed); }
  inline float getSpeed() const { return replay->getSpeed(); }
  inline Replay *getReplay() const { return replay.get(); }
  inline bool isPaused() const override { return replay->isPaused(); }
  void pause(bool pause) override;
  static AbstractOpenStreamWidget *widget(AbstractStream **stream);

private:
  void mergeSegments();
  std::unique_ptr<Replay> replay = nullptr;
  std::set<int> processed_segments;
  std::unique_ptr<OpenpilotPrefix> op_prefix;
};

class OpenReplayWidget : public AbstractOpenStreamWidget {
  Q_OBJECT

public:
  OpenReplayWidget(AbstractStream **stream);
  bool open() override;
  QString title() override { return tr("&Replay"); }

private:
  QLineEdit *route_edit;
  std::vector<QCheckBox *> cameras;
};
