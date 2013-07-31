#include "chartxy.h"

ChartXY::ChartXY(QWidget *parent):
  QAbstractItemView(parent)
  {
  m_setting=new ChartXYSetting(this);
  m_render=new ChartXYRender(this,m_setting);

  //grabMouse(Qt::CrossCursor);
  }

QRect	ChartXY::visualRect(const QModelIndex &index) const
  {
  return(QRect());
  }

void ChartXY::scrollTo(const QModelIndex &index,ScrollHint hint)
  {
  update();
  }

QModelIndex ChartXY::indexAt(const QPoint &point) const
  {
  return(QModelIndex());
  }

ChartXYSetting &ChartXY::setting(void)
  {
  return(*m_setting);
  }

unsigned int ChartXY::version(void)
  {
  return(CHARTXY_VERSION);
  }

unsigned int ChartXY::revision(void)
  {
  return(CHARTXY_REVISION);
  }


QModelIndex ChartXY::moveCursor(CursorAction cursorAction,Qt::KeyboardModifiers modifiers)
  {
  return(QModelIndex());
  }

int ChartXY::horizontalOffset() const
  {
  return(0);
  }
int ChartXY::verticalOffset() const

  {
  return(0);
  }

bool ChartXY::isIndexHidden(const QModelIndex &index) const
  {
  return(false);
  }

void ChartXY::setSelection(const QRect &rect,QItemSelectionModel::SelectionFlags command)
  {
  update();
  }

QRegion ChartXY::visualRegionForSelection(const QItemSelection &selection) const
  {
  return(QRegion());
  }

void ChartXY::dataChanged(const QModelIndex &topLeft,const QModelIndex &bottomRight)
  {
  //qDebug() << "Data is changed " << endl;
  QAbstractItemView::dataChanged(topLeft, bottomRight);
  updateChart();
  }


void ChartXY::paintEvent(QPaintEvent *event)
  {
  m_render->paint();
  }

void ChartXY::resizeEvent(QResizeEvent *event)
  {
  updateGeometries();
  }

void ChartXY::scrollContentsBy(int dx, int dy)
  {
  }

void ChartXY::mousePressEvent(QMouseEvent *event)
  {
  if(event->button()==Qt::LeftButton)
    {
    setCursor(Qt::CrossCursor);
    }
  }

void ChartXY::mouseReleaseEvent(QMouseEvent *event)
  {
  setCursor(Qt::ArrowCursor);
  }

void ChartXY::updateChart(void)
  {
  m_setting->scale().calculateLimit(model(),m_setting->grid().horizontalTick().tickMajor()-1,m_setting->grid().verticalTick().tickMajor()-1);
  viewport()->update();
  }
