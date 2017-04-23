/*
  ==============================================================================

    StateMachineView.h
    Created: 28 Oct 2016 8:27:44pm
    Author:  bkupe

  ==============================================================================
*/

#ifndef STATEMACHINEVIEW_H_INCLUDED
#define STATEMACHINEVIEW_H_INCLUDED

#pragma warning(disable:4244 4100 4305)

#include "StateManager.h"
#include "StateViewUI.h"
#include "BaseManagerShapeShifterUI.h"
#include "Style.h"
#include "StateTransitionManagerUI.h"

class StateMachineView :
	public BaseManagerShapeShifterViewUI<StateManager,State,StateViewUI>,
	public StateViewUI::Listener
{
public:
	StateMachineView(StateManager * _manager);
	~StateMachineView();
	
	StateManager * manager;
	
	//Transitions
	ScopedPointer<StateTransitionManagerUI> stmUI;
	bool transitionCreationMode;
	StateViewUI * transitionCreationSourceUI;

	void mouseMove(const MouseEvent &e) override;
	bool keyPressed(const KeyPress &e) override;

	void paint(Graphics &g) override;
	void resized() override;

	//Transition
	void startCreateTransition(StateViewUI * sourceUI);
	void finishCreateTransition(StateViewUI * destUI);

	void addItemUIInternal(StateViewUI * s) override;
	void removeItemUIInternal(StateViewUI * s) override;

	//From StateViewUI listener
	void askCreateTransitionFromUI(StateViewUI * se) override;
	void askFinishTransitionFromUI(StateViewUI * se) override;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(StateMachineView)
};



#endif  // STATEMACHINEVIEW_H_INCLUDED
