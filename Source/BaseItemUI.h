/*
  ==============================================================================

    BaseItemUI.h
    Created: 28 Oct 2016 8:04:09pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef BASEITEMUI_H_INCLUDED
#define BASEITEMUI_H_INCLUDED


#include "BaseItemMinimalUI.h"
#include "AssetManager.h"
#include "BoolImageToggleUI.h"
#include "StringParameterUI.h"

template<class T>
class BaseItemUI :
	public BaseItemMinimalUI<T>,
	public ButtonListener
{
public:
	enum ResizeMode { NONE, VERTICAL, HORIZONTAL, ALL };

	BaseItemUI<T>(T * _item,ResizeMode resizeMode = NONE, bool canBeDragged = false);
	virtual ~BaseItemUI<T>();

	ResizeMode resizeMode;
	bool canBeDragged;

	ScopedPointer<ResizableCornerComponent> cornerResizer;
	ScopedPointer<ResizableEdgeComponent> edgeResizer;
	Component * resizer;

	//LAYOUT

	//grabber
	const int grabberHeight = 10;
	Point<float> posAtMouseDown;


	//header
	int headerHeight;
	int headerGap;

	ScopedPointer<StringParameterUI> nameUI;
	ScopedPointer<BoolImageToggleUI> enabledBT;
	ScopedPointer<ImageButton> removeBT;

	void setContentSize(int contentWidth, int contentHeight);

	//minimode
	virtual void updateMiniModeUI();

	virtual void resized() override;
	virtual void resizedInternalHeader(Rectangle<int> &) {}
	virtual void resizedInternalContent(Rectangle<int> &) {}
	void buttonClicked(Button *b) override;


	void mouseDown(const MouseEvent &e) override;
	void mouseDrag(const MouseEvent &e) override;
	void mouseDoubleClick(const MouseEvent &e) override;

	void controllableFeedbackUpdateInternal(Controllable *) override;

	class Grabber : public Component
	{
	public:
		Grabber() {}
		~Grabber() {}
		void paint(Graphics &g) override;

		JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Grabber)
	};
	
	ScopedPointer<Grabber> grabber;

	class ItemUIListener
	{
	public:
		virtual ~ItemUIListener() {}
		virtual void itemUIGrabbed(BaseItemUI<T> *) {}
		virtual void itemUIMiniModeChanged(BaseItemUI<T> *) {}
	};

	ListenerList<ItemUIListener> itemUIListeners;
	void addItemUIListener(ItemUIListener* newListener) { itemUIListeners.add(newListener); }
	void removeItemUIListener(ItemUIListener* listener) { itemUIListeners.remove(listener); }

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BaseItemUI<T>)


};



template<class T>
BaseItemUI<T>::BaseItemUI(T * _item, ResizeMode _resizeMode, bool _canBeDragged) :
	BaseItemMinimalUI<T>(_item),
	resizeMode(_resizeMode),
	canBeDragged(_canBeDragged),
	resizer(nullptr),
	headerHeight(16), headerGap(5)
{
	
	nameUI = this->getBaseItem()->nameParam->createStringParameterUI();
	this->addAndMakeVisible(nameUI);

	if (this->getBaseItem()->canBeDisabled)
	{
		enabledBT = this->getBaseItem()->enabled->createImageToggle(AssetManager::getInstance()->getPowerBT());
		this->addAndMakeVisible(enabledBT);
	}
	
	removeBT = AssetManager::getInstance()->getRemoveBT();
	addAndMakeVisible(removeBT);
	removeBT->addListener(this);

	BaseItemMinimalUI<T>::setHighlightOnMouseOver(true);


	if (canBeDragged)
	{
		grabber = new Grabber();
		grabber->setAlwaysOnTop(true);
		addAndMakeVisible(grabber);
	}

	switch (resizeMode)
	{
	case NONE:
		break;
	case VERTICAL:
		edgeResizer = new ResizableEdgeComponent(this, nullptr, ResizableEdgeComponent::bottomEdge);
		edgeResizer->setAlwaysOnTop(true);
		addAndMakeVisible(edgeResizer);
		//resizer = edgeResizer;
		//setContentSize(getWidth(),(int)item->listUISize->floatValue());
		break;

	case HORIZONTAL:
		edgeResizer = new ResizableEdgeComponent(this, nullptr, ResizableEdgeComponent::rightEdge);
		edgeResizer->setAlwaysOnTop(true);
		addAndMakeVisible(edgeResizer);
		//resizer = edgeResizer;
		//setContentSize((int)item->listUISize->floatValue(),getHeight());
		break;

	case ALL:
		cornerResizer = new ResizableCornerComponent(this, nullptr);
		cornerResizer->setAlwaysOnTop(true);
		addAndMakeVisible(cornerResizer);
		//resizer = cornerResizer;
		//setContentSize((int)item->viewUISize->getPoint().x, (int)item->viewUISize->getPoint().y);
		break;
	}

	updateMiniModeUI();
}

template<class T>
BaseItemUI<T>::~BaseItemUI()
{
	removeBT->removeListener(this);
}

template<class T>
void BaseItemUI<T>::setContentSize(int contentWidth, int contentHeight)
{
	int targetHeight = headerHeight + headerGap + contentHeight;
	int targetWidth = contentWidth;
	if (resizer != nullptr)
	{
		targetHeight += resizer->getHeight();
	}

	setSize(targetWidth, targetHeight);
}

template<class T>
void BaseItemUI<T>::updateMiniModeUI()
{
	if (item->miniMode->boolValue())
	{
		if(resizer != nullptr) removeChildComponent(resizer);
		setSize(getWidth(), grabberHeight + headerHeight + 10);
	} else
	{
		if(resizer != nullptr) addAndMakeVisible(resizer);

		int targetHeight = 0;
		int targetWidth = getWidth();
		if (resizeMode == ALL)
		{
			targetWidth = (int)item->viewUISize->getPoint().x;
			targetHeight = (int)item->viewUISize->getPoint().y;
		}
		else if (resizeMode == VERTICAL) targetHeight = (int)item->listUISize->floatValue();
		else if (resizeMode == HORIZONTAL) targetWidth = (int)item->listUISize->floatValue();

		setContentSize(targetWidth, targetHeight);
	}

	itemUIListeners.call(&ItemUIListener::itemUIMiniModeChanged, this);
}

template<class T>
void BaseItemUI<T>::resized()
{

	//Header
	Rectangle<int> r = this->getLocalBounds().reduced(2);
	
	//Grabber
	if (canBeDragged)
	{
		//Grabber
		grabber->setBounds(r.removeFromTop(grabberHeight));
		grabber->repaint();
	}
	
	Rectangle<int> h = r.removeFromTop(headerHeight);
	
	if(enabledBT != nullptr)
	{
		enabledBT->setBounds(h.removeFromLeft(h.getHeight()));
		h.removeFromLeft(2);
	}

	removeBT->setBounds(h.removeFromRight(h.getHeight()));
	h.removeFromRight(2);

	resizedInternalHeader(h);
	nameUI->setBounds(h);

	r.removeFromTop(2);

	if (!item->miniMode->boolValue())
	{
		switch (resizeMode)
		{
		case NONE:
			break;

		case VERTICAL:
			edgeResizer->setBounds(r.removeFromBottom(10));
			item->listUISize->setValue((float)r.getHeight());
			break;

		case HORIZONTAL:
			edgeResizer->setBounds(r.removeFromRight(10));
			item->listUISize->setValue((float)r.getWidth());
			break;

		case ALL:
			cornerResizer->setBounds(r.removeFromBottom(10).withLeft(getWidth() - 10));
			item->viewUISize->setPoint((float)r.getWidth(), (float)r.getHeight());
			break;

		}

		resizedInternalContent(r);
	}
}

template<class T>
void BaseItemUI<T>::buttonClicked(Button * b)
{
	if (b == removeBT)
	{
		this->getBaseItem()->remove();
		return;
	}
}


template<class T>
void BaseItemUI<T>::mouseDown(const MouseEvent & e)
{
	BaseItemMinimalUI::mouseDown(e);

	if (e.mods.isLeftButtonDown())
	{
		if (canBeDragged && e.eventComponent == grabber) posAtMouseDown = item->viewUIPosition->getPoint();
	}
}


template<class T>
void BaseItemUI<T>::mouseDrag(const MouseEvent & e)
{
	BaseItemMinimalUI::mouseDrag(e);
	
	if (canBeDragged && e.mods.isLeftButtonDown() && e.eventComponent == grabber)
	{
		Point<float> targetPos = posAtMouseDown + e.getOffsetFromDragStart().toFloat() / Point<float>((float)getParentComponent()->getWidth(), (float)getParentComponent()->getHeight());
		item->viewUIPosition->setPoint(targetPos);
	}

}

template<class T>
void BaseItemUI<T>::mouseDoubleClick(const MouseEvent & e)
{
	if (e.eventComponent == grabber) item->miniMode->setValue(!item->miniMode->boolValue());

}

template<class T>
void BaseItemUI<T>::controllableFeedbackUpdateInternal(Controllable * c)
{
	if (c == item->miniMode) updateMiniModeUI();
	else if (canBeDragged && c == item->viewUIPosition) itemUIListeners.call(&ItemUIListener::itemUIGrabbed, this);
}


template<class T>
void BaseItemUI<T>::Grabber::paint(Graphics & g)
{
	Rectangle<int> r = getLocalBounds();
	g.setColour(BG_COLOR.brighter(.3f));
	const int numLines = 3;
	for (int i = 0; i < numLines; i++)
	{
		float th = (i + 1)*(float)getHeight() / ((float)numLines + 1);
		g.drawLine(0, th, (float)getWidth(), th, 1);
	}
}




#endif  // BASEITEMUI_H_INCLUDED
