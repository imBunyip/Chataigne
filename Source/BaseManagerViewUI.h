/*
  ==============================================================================

	BaseManagerViewUI.h
	Created: 23 Apr 2017 2:48:02pm
	Author:  Ben

  ==============================================================================
*/

#ifndef BASEMANAGERVIEWUI_H_INCLUDED
#define BASEMANAGERVIEWUI_H_INCLUDED

#include "BaseManagerUI.h"
#include "BaseItemUI.h"

template<class M, class T, class U>

class BaseManagerViewUI :
	public BaseManagerUI<M, T, U>,
	public BaseItemUI<T>::ItemUIListener
{
public:
	BaseManagerViewUI<M, T, U>(const String &contentName, M * _manager);
	virtual ~BaseManagerViewUI();

	Point<int> viewOffset; //in pixels, viewOffset of 0 means zeroPos is at the center of the window
						   //interaction
	Point<int> initViewOffset;

	virtual void mouseDown(const MouseEvent &e) override;
	virtual void mouseDrag(const MouseEvent &e) override;
	virtual void mouseUp(const MouseEvent &e) override;
	virtual bool keyPressed(const KeyPress &e) override;

	virtual void paint(Graphics &g) override;
	virtual void resized() override;

	virtual void updateViewUIPosition(U * se);

	virtual void addItemFromMenu(bool isFromAddButton, Point<int> mouseDownPos) override;

	Point<int> getSize();
	Point<int> getViewMousePosition();
	Point<int> getViewPos(const Point<int> &originalPos);
	Point<float> getItemsCenter();

	virtual void homeView();
	virtual void frameView();

	virtual void addItemUIInternal(U * se) override;
	virtual void removeItemUIInternal(U * se) override;

	virtual void itemUIGrabbed(BaseItemUI<T> * se) override;
};



template<class M, class T, class U>
BaseManagerViewUI<M, T, U>::BaseManagerViewUI(const String & contentName, M * _manager) :
	BaseManagerUI(contentName,_manager,false)
{
	bgColor = BG_COLOR.darker(.3f);
}


template<class M, class T, class U>
BaseManagerViewUI<M, T, U>::~BaseManagerViewUI()
{
}

template<class M, class T, class U>
void BaseManagerViewUI<M, T, U>::mouseDown(const MouseEvent & e)
{
	BaseManagerUI::mouseDown(e);
	if ((e.mods.isLeftButtonDown() && e.mods.isAltDown()) || e.mods.isMiddleButtonDown())
	{
		setMouseCursor(MouseCursor::UpDownLeftRightResizeCursor);
		updateMouseCursor();
		initViewOffset = Point<int>(viewOffset.x, viewOffset.y);
	}
}

template<class M, class T, class U>
void BaseManagerViewUI<M, T, U>::mouseDrag(const MouseEvent & e)
{
	BaseManagerUI::mouseDrag(e);
	if ((e.mods.isLeftButtonDown() && e.mods.isAltDown()) || e.mods.isMiddleButtonDown())
	{
		viewOffset = initViewOffset + e.getOffsetFromDragStart();
		resized();
		repaint();
	}
}

template<class M, class T, class U>
void BaseManagerViewUI<M, T, U>::mouseUp(const MouseEvent & e)
{
	BaseManagerUI::mouseUp(e);
	setMouseCursor(MouseCursor::NormalCursor);
	updateMouseCursor();
}

template<class M, class T, class U>
bool BaseManagerViewUI<M, T, U>::keyPressed(const KeyPress & e)
{
	if (BaseManagerUI::keyPressed(e)) return true;

	if (e.getKeyCode() == KeyPress::createFromDescription("f").getKeyCode())
	{
		frameView();
		return true;
	} else if (e.getKeyCode() == KeyPress::createFromDescription("h").getKeyCode())
	{
		homeView();
		return true;
	}

	return false;
}

template<class M, class T, class U>
void BaseManagerViewUI<M, T, U>::paint(Graphics & g)
{
	int checkerSize = 32;
	int checkerTX = -checkerSize * 2 + ((getWidth() / 2 + viewOffset.x) % (checkerSize * 2));
	int checkerTY = -checkerSize * 2 + ((getHeight() / 2 + viewOffset.y) % (checkerSize * 2));
	Rectangle<int> checkerBounds(checkerTX, checkerTY, getWidth() + checkerSize * 4, getHeight() + checkerSize * 4);
	g.fillCheckerBoard(checkerBounds, checkerSize, checkerSize, BG_COLOR.darker(.3f), BG_COLOR.darker(.2f));

	g.setColour(BG_COLOR.darker(.05f));
	Point<int> center = getSize() / 2;
	center += viewOffset;
	g.drawLine(center.x, 0, center.x, getHeight(), 2);
	g.drawLine(0, center.y, getWidth(), center.y, 2);

}

template<class M, class T, class U>
void BaseManagerViewUI<M, T, U>::resized()
{
	Rectangle<int> r = getLocalBounds();
	addItemBT->setBounds(r.withSize(24, 24).withX(r.getWidth() - 24));
	for (auto &tui : itemsUI)
	{
		updateViewUIPosition(tui);
	}
}

template<class M, class T, class U>
void BaseManagerViewUI<M, T, U>::updateViewUIPosition(U * se)
{
	Point<int> pe = getSize()*se->item->viewUIPosition->getPoint();
	pe += getSize() / 2; //position at center of window
	pe += viewOffset;
	se->setTopLeftPosition(pe.x, pe.y);
}

template<class M, class T, class U>
void BaseManagerViewUI<M, T, U>::addItemFromMenu(bool isFromAddButton, Point<int> mouseDownPos)
{
	manager->addItem(getViewPos(mouseDownPos).toFloat() / getSize());
}

template<class M, class T, class U>
Point<int> BaseManagerViewUI<M, T, U>::getSize()
{
	return Point<int>(getWidth(), getHeight());
}

template<class M, class T, class U>
Point<int> BaseManagerViewUI<M, T, U>::getViewMousePosition()
{
	return getViewPos(getMouseXYRelative());
}

template<class M, class T, class U>
Point<int> BaseManagerViewUI<M, T, U>::getViewPos(const Point<int>& originalPos)
{
	return originalPos - getSize() / 2 - viewOffset;
}

template<class M, class T, class U>
Point<float> BaseManagerViewUI<M, T, U>::getItemsCenter()
{
	Point<float> average;
	for (auto &se : itemsUI)
	{
		average += se->item->viewUIPosition->getPoint();
	}
	average /= itemsUI.size();
	return average;
}

template<class M, class T, class U>
void BaseManagerViewUI<M, T, U>::homeView()
{
	viewOffset = Point<int>();
	resized();
	repaint();
}

template<class M, class T, class U>
void BaseManagerViewUI<M, T, U>::frameView()
{
	viewOffset = (-getItemsCenter()*getSize()).toInt();
	resized();
	repaint();
}

template<class M, class T, class U>
void BaseManagerViewUI<M, T, U>::addItemUIInternal(U * se)
{
	updateViewUIPosition(se);
	se->addItemUIListener(this);
}


template<class M, class T, class U>
void BaseManagerViewUI<M, T, U>::removeItemUIInternal(U * se)
{
	se->removeItemUIListener(this);
}


template<class M, class T, class U>
void BaseManagerViewUI<M, T, U>::itemUIGrabbed(BaseItemUI<T> * se)
{
	updateViewUIPosition(static_cast<U *>(se));
}




#endif  // BASEMANAGERVIEWUI_H_INCLUDED